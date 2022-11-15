#ifndef RESTAPIMANAGER_H
#define RESTAPIMANAGER_H

#include "CuteSimpleRestClientLib_global.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QVariantMap>
#include <qqml.h>
#include <QFile>
#include <QtQml/qqmlregistration.h>

class CUTESIMPLERESTCLIENTLIB_EXPORT RestApiManager:  public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant latestResponse READ getLatestResponse WRITE setLatestResponse NOTIFY latestResponseChanged)
    QML_ELEMENT


public:
    explicit RestApiManager(QObject *parent = nullptr) ;
    ~RestApiManager() ;



    const QVariant &getLatestResponse() const;

    void setLatestResponse(const QVariant &newLatestResponse);

    const QVariant &getResponseStatusCode() const;

public slots:
    void get(QString endpoint, QVariantMap parameters = QVariantMap(), QVariantMap headers = QVariantMap());
    void post(QString endpoint,  QVariantMap body= QVariantMap(), QVariantMap parameters= QVariantMap(), QVariantMap headers= QVariantMap());
    void put(QString endpoint,  QVariantMap body= QVariantMap(), QVariantMap parameters= QVariantMap(), QVariantMap headers= QVariantMap());
    QVariant networkReplyReadRead();
    void del(QString endpoint, QVariantMap parameters= QVariantMap(), QVariantMap headers= QVariantMap());
    void uploadIamge(QString endpoint, QString FilePath, QVariantMap body= QVariantMap(), QVariantMap parameters= QVariantMap(), QVariantMap headers= QVariantMap());
    void uploadAudio(QString endpoint, QString FilePath, QVariantMap body= QVariantMap(), QVariantMap parameters= QVariantMap(), QVariantMap headers= QVariantMap());
    void handleFailedRequest(QNetworkReply::NetworkError code);
    void checkRequestStatus();
signals:
    void latestResponseChanged();
    void responseReady(QVariant response);

private:

    QSharedPointer<QFile>fileToSend;
    QSharedPointer<QNetworkAccessManager> m_networkManager;
    QSharedPointer<QNetworkReply> m_networkReply;
    QSharedPointer<QHttpMultiPart> multiPart;
    QVariantMap header;
    QVariant responseStatusCode  = 0;
    QVariant m_latestResponse;

    QList<QNetworkRequest> pendingQuerys;
    QList<QJsonDocument> pendingQuerysBody;
    QList<QSharedPointer<QHttpMultiPart>> pendingQuerysMultipartHttp;
    QList<QString> pendingQuerysMethod;

};

#endif // RESTAPIMANAGER_H
