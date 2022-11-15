#include "firestorewrapper.h"
#include <QDateTime>

FireStoreWrapper::FireStoreWrapper(QObject *parent)
    : QObject{parent}
{
    restApiHandler.reset(new RestApiManager());
    connect(restApiHandler.data(), &RestApiManager::responseReady, this, &FireStoreWrapper::forwardResponse);
}

void FireStoreWrapper::getDocument(QString documentPath)
{
   QString requestUrl = QString ("https://firestore.googleapis.com/v1/projects/%1/databases/(default)/documents/%2").arg(this->firebaseProjectId, documentPath);

   QVariantMap headers;
   headers["Authorization"] = "Bearer "+this->id_token;
   restApiHandler->get(requestUrl, QVariantMap(), headers);

   lastQuery = "GET_DOCUMENT";
   requestType = FirestoreRequestTypes::GET_DOCUMENT;


}


QVariantMap FireStoreWrapper::buildQuery(QList<QString> fieldList, QString collectionId, QString filterField, QString filterOperator, QVariant filteringValue,  QString orderingField, QString orderDirection){

    QStringList filterOperatorsList = {"OPERATOR_UNSPECIFIED",
                                       "LESS_THAN",
                                       "LESS_THAN_OR_EQUAL",
                                       "GREATER_THAN",
                                       "GREATER_THAN_OR_EQUAL",
                                       "EQUAL",
                                       "NOT_EQUAL",
                                       "ARRAY_CONTAINS",
                                       "IN",
                                       "ARRAY_CONTAINS_ANY",
                                       "NOT_IN"
                                      };

    if (!filterOperatorsList.contains(filterOperator)){
        QVariantMap returnValue;
        returnValue["Error"] = "Query Invalid!";
        return returnValue;
    }




    QStringList orderDirectionList = {"ASCENDING",
                                       "DESCENDING"};

    if (!orderDirectionList.contains(orderDirection)){
        QVariantMap returnValue;
        returnValue["Error"] = "Query Invalid!";
        return returnValue;
    }



    QVariantList fields;

    QList<QString>::iterator i;
    for (i = fieldList.begin(); i != fieldList.end(); ++i){

        QVariantMap fieldReference;
        fieldReference["fieldPath"] = *i;
        fields.append(fieldReference) ;
    }

    //Projection
    QVariantMap projection;
    projection["fields"] = fields;


    //COllectionSelection
    QVariantMap collectionSelector;
    collectionSelector["collectionId"] = collectionId;
    collectionSelector["allDescendants"] = true;

    QVariantList collectionSelectorList;
    collectionSelectorList.append(collectionSelector);


    //Filter
    QVariantMap filterFieldReference;
    filterFieldReference["fieldPath"] = filterField;

    QVariantMap fieldFilter;
    fieldFilter["field"] = filterFieldReference;
    fieldFilter["op"] = filterOperator;
    fieldFilter["value"] = filteringValue;

    QVariantMap filter;
    filter["fieldFilter"] = fieldFilter;

    //Order
    QVariantMap orderFieldReference;
    orderFieldReference["fieldPath"] = orderingField;

    QVariantMap order;
    order["field"] = orderFieldReference;
    order["direction"] = orderDirection;


    QVariantMap query;
    query["select"] = projection;
    query["from"] = collectionSelectorList;
    query["where"] = filter;
    query["orderBy"] = order;
    query["select"] = projection;
    query["select"] = projection;
    query["select"] = projection;

    return query;



}

const QString &FireStoreWrapper::getLastQuery() const
{
    return lastQuery;
}

void FireStoreWrapper::setLastQuery(const QString &newLastQuery)
{
    if (lastQuery == newLastQuery)
        return;
    lastQuery = newLastQuery;
    emit lastQueryChanged();
}

void FireStoreWrapper::createDocument(QString collectionId, QString documentId, QVariantMap document)
{

    QString requestUrl = QString ("https://firestore.googleapis.com/v1/projects/%1/databases/(default)/documents/%2").arg(this->firebaseProjectId, collectionId);

    QVariantMap body;
    body["fields"] = document;
    body["createTime"] = QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddTHH:mm:ssZ");
    body["updateTime"] = QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddTHH:mm:ssZ");


    QVariantMap parameters;
    if (documentId == "")
        parameters["documentId"] = documentId;

    QVariantMap headers;
    headers["Authorization"] = "Bearer "+this->id_token;
    restApiHandler->post(requestUrl, body, parameters, headers);
    lastQuery = "CREATE_DOCUMENT";
    requestType = FirestoreRequestTypes::CREATE_DOCUMENT;

}

void FireStoreWrapper::deleteDocument(QString documentName)
{

}

void FireStoreWrapper::listDocuments()
{

}

void FireStoreWrapper::runQuery(QString query)
{

}



void FireStoreWrapper::getMultipleDocuments(QList<QString> documentsNames)
{
    QString requestUrl = QString ("https://firestore.googleapis.com/v1/projects/%1/databases/(default)/documents/").arg(this->firebaseProjectId);
    QVariantMap body;
    body["documents"] = documentsNames;

    QVariantMap headers;
    headers["Authorization"] = "Bearer "+this->id_token;
    restApiHandler->post(requestUrl, body, QVariantMap(), headers);
    lastQuery = "GET_MULTIPLE_DOCUMENT";
    requestType = FirestoreRequestTypes::GET_MULTIPLE_DOCUMENT;

}

void FireStoreWrapper::
forwardResponse(QVariant response){


    emit firestorDataReady(response, requestType);

}








const QString &FireStoreWrapper::getId_token() const
{
    return id_token;
}

void FireStoreWrapper::setIdToken(const QString &newId_token)
{
    if (id_token == newId_token)
        return;
    id_token = newId_token;
    emit id_tokenChanged();
}

const QString &FireStoreWrapper::getFirebaseProjectId() const
{
    return firebaseProjectId;
}

void FireStoreWrapper::setFirebaseProjectId(const QString &newFirebaseProjectId)
{
    if (firebaseProjectId == newFirebaseProjectId)
        return;
    firebaseProjectId = newFirebaseProjectId;
    emit firebaseProjectIdChanged();
}
