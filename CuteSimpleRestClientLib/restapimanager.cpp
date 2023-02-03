#include "restapimanager.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QHttpMultiPart>
#include <QFile>
#include <QDir>
#include <QFileInfo>




/**
 * @brief Construct a new Rest Api Manager:: Rest Api Manager object
 * 
 * @param parent 
 */
RestApiManager::RestApiManager(QObject *parent): QObject(parent)
{
    m_networkManager.reset(new QNetworkAccessManager(this));
}

RestApiManager::~RestApiManager()
{

}

const QVariant &RestApiManager::getLatestResponse() const
{
    return m_latestResponse;
}

void RestApiManager::handleFailedRequest(QNetworkReply::NetworkError code)
{
    qDebug()<<"Error Occured! Error Code: "<<code
             <<"\nFailed Request URL: "<< m_networkReply->request().url();
}


QVariant RestApiManager::networkReplyReadRead()
{
    responseStatusCode = m_networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QByteArray rawResponse = m_networkReply->readAll();
    qDebug()<<rawResponse;
    QVariant tempResponse;
    if (QJsonDocument::fromJson( rawResponse).isObject()){
        tempResponse = QJsonDocument::fromJson( rawResponse).object().toVariantMap();
    }
    else if (QJsonDocument::fromJson( rawResponse).isArray()){
        tempResponse = QJsonDocument::fromJson( rawResponse).array().toVariantList();
    }
    emit responseReady(tempResponse);
    m_latestResponse = tempResponse;
    emit latestResponseChanged();

    if ( !fileToSend.isNull()){
        fileToSend->close();
    }

    if (!pendingQuerys.isEmpty()){
        if (pendingQuerysMethod[0] == "get"){

            m_networkReply.reset(m_networkManager->get(pendingQuerys[0]));
            connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);
            pendingQuerys.removeFirst();
            pendingQuerysMethod.removeFirst();
        }
        else if (pendingQuerysMethod[0] == "delete"){

            m_networkReply.reset(m_networkManager->deleteResource(pendingQuerys[0]));
            connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);
            pendingQuerys.removeFirst();
            pendingQuerysMethod.removeFirst();
        }
        else if (pendingQuerysMethod[0] == "put"){

            m_networkReply.reset(m_networkManager->put(pendingQuerys[0], pendingQuerysBody[0].toJson()));
            connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);
            pendingQuerys.removeFirst();
            pendingQuerysMethod.removeFirst();
            pendingQuerysBody.removeFirst();
        }
        else if (pendingQuerysMethod[0] == "post"){

            m_networkReply.reset(m_networkManager->post(pendingQuerys[0], pendingQuerysBody[0].toJson()));
            connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);
            pendingQuerys.removeFirst();
            pendingQuerysMethod.removeFirst();
            pendingQuerysBody.removeFirst();
        }
        else if (pendingQuerysMethod[0] == "post_multipart"){

            m_networkReply.reset(m_networkManager->post(pendingQuerys[0], pendingQuerysMultipartHttp[0].data()));
            connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);
            pendingQuerys.removeFirst();
            pendingQuerysMethod.removeFirst();
            pendingQuerysMultipartHttp.removeFirst();
        }
    }

    return m_latestResponse;
}

/**
 * @brief Sends an HTTP get request using the given endpoint and parameters
 * 
 * @param endpoint 
 * @param parameters 
 * @param headers 
 */
void RestApiManager::get(QString endpoint, QVariantMap parameters, QVariantMap headers )
{



    QUrl urlEndpoint(endpoint);
    QUrlQuery query;

    //Set Query Parameters
    if (!parameters.isEmpty()){
        QMapIterator<QString, QVariant> i(parameters);
        while (i.hasNext()) {
            i.next();
            query.addQueryItem(i.key(), i.value().toString());
        }
    }
    if(!query.isEmpty())
        urlEndpoint.setQuery(query);
    QNetworkRequest request = QNetworkRequest(urlEndpoint);


    //Set  The Headers
    if (!headers.isEmpty()){
        QMapIterator<QString, QVariant> i(headers);
        while (i.hasNext()) {
            i.next();
            request.setRawHeader(i.key().toUtf8(), i.value().toString().toUtf8());
        }
    }

    if(!m_networkReply.isNull() && m_networkReply->isRunning()){

        pendingQuerys.append(request);
        pendingQuerysMethod.append("get");
        return;
    }
    m_networkReply.reset(m_networkManager->get(request));
    connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);
    connect(m_networkReply.data(), &QNetworkReply::errorOccurred, this, &RestApiManager::handleFailedRequest);


}

/**
 * @brief Sends an HTTP get request using the given endpoint , body and parameters
 * 
 * @param endpoint 
 * @param body 
 * @param parameters 
 * @param headers 
 */
void RestApiManager::post(QString endpoint, QVariantMap body, QVariantMap parameters, QVariantMap headers)
{
    QUrl urlEndpoint(endpoint);
    QUrlQuery query;

    //Set Query Parameters
    if (!parameters.isEmpty()){
        QMapIterator<QString, QVariant> i(parameters);
        while (i.hasNext()) {
            i.next();
            query.addQueryItem(i.key(), i.value().toString());
            qDebug()<<"Parameter:  "<<i.key().toUtf8();

        }
    }
    if(!query.isEmpty())
        urlEndpoint.setQuery(query);
    qDebug()<<"Endpoint:  "<<urlEndpoint;
    QNetworkRequest request = QNetworkRequest(urlEndpoint);


    //Set  The Headers
    if (!headers.isEmpty()){
        QMapIterator<QString, QVariant> i(headers);
        while (i.hasNext()) {
            i.next();
            request.setRawHeader(i.key().toUtf8(), i.value().toString().toUtf8());
            qDebug()<<"Header:  "<<i.key().toUtf8();
        }
    }


    QJsonDocument jsonBody = QJsonDocument::fromVariant(body);

    if(!m_networkReply.isNull() && m_networkReply->isRunning()){

        pendingQuerys.append(request);
        pendingQuerysBody.append(jsonBody);
        pendingQuerysMethod.append("post");
        return;
    }

    m_networkReply.reset(m_networkManager->post(request, jsonBody.toJson()));

    connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);



}

void RestApiManager::put(QString endpoint, QVariantMap body, QVariantMap parameters, QVariantMap headers)
{

    QUrl urlEndpoint(endpoint);
    QUrlQuery query;

    //Set Query Parameters
    if (!parameters.isEmpty()){
        QMapIterator<QString, QVariant> i(parameters);
        while (i.hasNext()) {
            i.next();
            query.addQueryItem(i.key(), i.value().toString());
        }
    }
    if(!query.isEmpty())
        urlEndpoint.setQuery(query);
    QNetworkRequest request = QNetworkRequest(urlEndpoint);


    //Set  The Headers
    if (!headers.isEmpty()){
        QMapIterator<QString, QVariant> i(headers);
        while (i.hasNext()) {
            i.next();
            request.setRawHeader(i.key().toUtf8(), i.value().toString().toUtf8());
        }
    }


    QJsonDocument jsonBody = QJsonDocument::fromVariant(body);

    if(!m_networkReply.isNull() && m_networkReply->isRunning()){

        pendingQuerys.append(request);
        pendingQuerysBody.append(jsonBody);
        pendingQuerysMethod.append("put");
        return;
    }
    m_networkReply.reset(m_networkManager->put(request, jsonBody.toJson()));
    connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);

}

void RestApiManager::del(QString endpoint, QVariantMap parameters, QVariantMap headers)
{

    QUrl urlEndpoint(endpoint);
    QUrlQuery query;

    //Set Query Parameters
    if (!parameters.isEmpty()){
        QMapIterator<QString, QVariant> i(parameters);
        while (i.hasNext()) {
            i.next();
            query.addQueryItem(i.key(), i.value().toString());
        }
    }
    if(!query.isEmpty())
        urlEndpoint.setQuery(query);
    QNetworkRequest request = QNetworkRequest(urlEndpoint);


    //Set  The Headers
    if (!headers.isEmpty()){
        QMapIterator<QString, QVariant> i(headers);
        while (i.hasNext()) {
            i.next();
            request.setRawHeader(i.key().toUtf8(), i.value().toString().toUtf8());
        }
    }

    if(!m_networkReply.isNull() && m_networkReply->isRunning()){

        pendingQuerys.append(request);
        pendingQuerysMethod.append("delete");
        return;
    }
    m_networkReply.reset(m_networkManager->deleteResource(request));
    connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);

}

void RestApiManager::uploadIamge(QString endpoint, QString FilePath, QVariantMap body, QVariantMap parameters, QVariantMap headers)
{


    QFileInfo fileInfo(FilePath);
    QString extension = fileInfo.suffix();
    QVariant imageContentType;

    if (extension.compare("jpg")||extension.compare("jpeg"))
        imageContentType = QVariant("image/jpeg");
    else if (extension.compare("png"))
        imageContentType = QVariant("image/png");
    else{
        qDebug()<<"Error. Unsupported data type";
        return;
    }

    multiPart.reset(new QHttpMultiPart(QHttpMultiPart::FormDataType));

    qDebug()<<"File path: " <<FilePath;
    FilePath.remove("file:///");
    qDebug()<<"Temp File path: " <<FilePath;

    fileToSend.reset(new QFile(FilePath));


    if(fileToSend->open(QIODevice::ReadOnly)){
        qDebug()<<"File Opened";
    }
    else{
        qDebug()<<"File Not Opened";
        return;
    }
    //    fileToSend->setParent(multiPart.data()); // we cannot delete the file now, so delete it with the multiPart

    QJsonDocument jsonBody = QJsonDocument::fromVariant(body);

    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("Application/json"));
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"imageMetadata\""));
    jsonPart.setBody(jsonBody.toJson());


    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, imageContentType);
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\""+fileInfo.fileName()+"\"; filename=\""+fileInfo.fileName()+"\""));
    imagePart.setBodyDevice(fileToSend.data());

    multiPart->append(jsonPart);
    multiPart->append(imagePart);

    QNetworkRequest request(endpoint);

    if(!m_networkReply.isNull() && m_networkReply->isRunning()){

        pendingQuerys.append(request);
        pendingQuerysMultipartHttp.append(multiPart);
        pendingQuerysMethod.append("post_multipart");
        return;
    }

    m_networkReply.reset( m_networkManager->post(request, multiPart.data()));
    connect(m_networkReply.data(), &QNetworkReply::readyRead, this, &RestApiManager::networkReplyReadRead);

}

void RestApiManager::uploadAudio(QString endpoint, QString FilePath, QVariantMap body, QVariantMap parameters, QVariantMap headers)
{
    // TODO Implement UploadAudio method
}

void RestApiManager::setLatestResponse(const QVariant &newLatestResponse)
{
    if (m_latestResponse == newLatestResponse)
        return;
    m_latestResponse = newLatestResponse;
    emit latestResponseChanged();
}


void RestApiManager::checkRequestStatus(){

    if (!m_networkReply.isNull()){
        if (m_networkReply->isRunning()){
            qDebug()<<"Request Still Running";
        }
        else{
            qDebug()<<"Request Not Running";
        }
    }
    else{
        qDebug()<<"Request Not Made Yet";
    }

}

void RestApiManager::setResponseStatusCode(const QVariant &newResponseStatusCode)
{
    if (responseStatusCode == newResponseStatusCode)
        return;
    responseStatusCode = newResponseStatusCode;
    emit responseStatusCodeChanged();
}

const QVariant &RestApiManager::getResponseStatusCode() const
{
    return responseStatusCode;
}


