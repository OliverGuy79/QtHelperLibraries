#ifndef FIRESTOREWRAPPER_H
#define FIRESTOREWRAPPER_H

#include <QObject>
#include <QJsonDocument>
#include "restapimanager.h"
#include "FirebaseWrapper_global.h"



enum class FirestoreRequestTypes{

     GET_DOCUMENT,
     CREATE_DOCUMENT,
     GET_MULTIPLE_DOCUMENT
  };

class FIREBASEWRAPPER_EXPORT  FireStoreWrapper : public QObject
{
    Q_OBJECT
public:
    explicit FireStoreWrapper(QObject *parent = nullptr);
    const QString &getFirebaseProjectId() const;

    const QString &getId_token() const;


    const QString &getLastQuery() const;
    void setLastQuery(const QString &newLastQuery);

public slots:
    void getDocument(QString documentPath);
    void createDocument(QString collectionId, QString documentId, QVariantMap temp);
    void deleteDocument(QString documentName);
    void listDocuments();
    void runQuery(QString query);
    void getMultipleDocuments(QList<QString> documentsNames);
    void forwardResponse(QVariant response);
    void setIdToken(const QString &newId_token);
    void setFirebaseProjectId(const QString &newFirebaseProjectId);

    QVariantMap buildQuery(QList<QString> fieldList, QString collectionId, QString filterField, QString filterOperator, QVariant filteringValue, QString orderingField, QString orderDIrection);
signals:

    void firebaseProjectIdChanged();

    void id_tokenChanged();

    void lastQueryChanged();

    void firestorDataReady2(QVariant data, QString queryType);
    void firestorDataReady(QVariant data, FirestoreRequestTypes queryType);

private:
    QSharedPointer<RestApiManager> restApiHandler;
    QString firebaseProjectId;
    QString id_token;
    QString lastQuery;
    FirestoreRequestTypes requestType;
    Q_PROPERTY(QString firebaseProjectId READ getFirebaseProjectId WRITE setFirebaseProjectId NOTIFY firebaseProjectIdChanged)
    Q_PROPERTY(QString id_token READ getId_token WRITE setIdToken NOTIFY id_tokenChanged)

    Q_PROPERTY(QString lastQuery READ getLastQuery WRITE setLastQuery NOTIFY lastQueryChanged)

};

#endif // FIRESTOREWRAPPER_H
