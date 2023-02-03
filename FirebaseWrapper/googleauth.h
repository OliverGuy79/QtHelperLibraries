#ifndef GOOGLEAUTH_H
#define GOOGLEAUTH_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QOAuth2AuthorizationCodeFlow>

#include "FirebaseWrapper_global.h"

class FIREBASEWRAPPER_EXPORT GoogleAuth : public QObject
{

    Q_OBJECT
public:
    GoogleAuth(const QString &clientIdentifier, const QString &clientCode, QObject *parent = nullptr);


    QNetworkReply *requestHotThreads();
    bool isPermanent() const;
    void setPermanent(bool value);

    const QString &getClientId() const;
    void setClientId(const QString &newClientId);

    const QString &getClientCode() const;
    void setClientCode(const QString &newClientCode);


public slots:
    void grant();

    void NotifyAuthentication();
    void requestUserInfo();
    void authenticationDataAvailable();
    QString getToken();
    void setupOauth();
    QVariant getUserInfo();
    void resetOauth(const QString &newClientCode, const QString &newClientId);
    QString getIdToken();
signals:
    void authenticated();
    void subscribed(const QUrl &url);

    void clientIdChanged();

    void clientCodeChanged();

    void userInfoChanged();

private:
    QOAuth2AuthorizationCodeFlow oauth2;
    QSharedPointer<QNetworkReply> replyOauth;
    bool permanent = false;
    QUrl googleUserinfoUrl = QUrl("https://www.googleapis.com/oauth2/v3/userinfo");

    QString clientId;
    QString clientCode;

    QVariant userInfo;
    Q_PROPERTY(QString clientId READ getClientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString clientCode READ getClientCode WRITE setClientCode NOTIFY clientCodeChanged)
    Q_PROPERTY(QVariant userInfo READ getUserInfo NOTIFY userInfoChanged)
};

#endif // GOOGLEAUTH_H
