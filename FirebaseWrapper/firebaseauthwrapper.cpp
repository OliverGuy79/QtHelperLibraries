#include "firebaseauthwrapper.h"
#include <QJsonDocument>


FirebaseAuthWrapper::FirebaseAuthWrapper(QObject *parent)
    : QObject{parent}
{
    restApi.reset(new RestApiManager());
    connect(restApi.data(), &RestApiManager::responseReady, this, &FirebaseAuthWrapper::parseAuthResponse);

}

FirebaseAuthWrapper::~FirebaseAuthWrapper()
{

}

void FirebaseAuthWrapper::setupGoogleAuth(QString clientId, QString clientCode){

    googleAuth.reset(new GoogleAuth(clientId, clientCode, this));
    connect(googleAuth.data(), &GoogleAuth::authenticated, this, &FirebaseAuthWrapper::signInWithGoogleOauthIdToken);
    connect(googleAuth.data(), &GoogleAuth::userInfoChanged, this, &FirebaseAuthWrapper::setGoogleUserInfo);

}

QString FirebaseAuthWrapper::getGoogleToken()
{
    return googleAuth->getToken();
}

const QVariant &FirebaseAuthWrapper::getGoogleUserInfo() const
{
    return googleUserInfo;
}

void FirebaseAuthWrapper::setGoogleUserInfo()
{
    googleUserInfo = this->googleAuth->getUserInfo();
    emit googleUserInfoChanged();
}



void FirebaseAuthWrapper::signUpWithEmail(const QString &email, const QString &password)
{
    QVariantMap body;
    body["email"] = email;
    body["password"] = password;
    body["returnSecureToken"] = true;

    QVariantMap headers;
    body["email"] = email;


    restApi->post(emailSignUpEndpoint,body, QVariantMap(), headers);
}

void FirebaseAuthWrapper::signInWithEmail(const QString &email, const QString &password)
{

    QVariantMap body;
    body["email"] = email;
    body["password"] = password;
    body["returnSecureToken"] = true;

    QVariantMap headers;
    body["email"] = email;


    restApi->post(emailSignInEndpoint,body, QVariantMap(), headers);
}

void FirebaseAuthWrapper::signInWithRefreshToken(const QString &email, const QString &password)
{
    QVariantMap body;
    body["email"] = email;
    body["password"] = password;
    body["returnSecureToken"] = true;

    QVariantMap headers;
    body["email"] = email;


    restApi->post(refreshTokenEndpoint,body, QVariantMap(), headers);
}

void FirebaseAuthWrapper::signInWithGoogle(){

    this->googleAuth->grant();


}


void FirebaseAuthWrapper::printSignInResponse(){
//                qDebug()<<restApi->getLatestResponse();
}

void FirebaseAuthWrapper::generateOauthToken(){

    this->googleAuth->grant();
}


void FirebaseAuthWrapper::signInWithGoogleOauthIdToken()
{

    //qDebug()<<"\n\nId Token: "<<googleAuth->getIdToken()<<"\n\n";

    QVariantMap body;
    body["postBody"] = "id_token="+googleAuth->getIdToken()+"&providerId=google.com";
    body["requestUri"] = "http://localhost";
    body["returnIdpCredential"] = true;
    body["returnSecureToken"] = true;
    QVariantMap headers;
    headers["Content-Type"] ="application/json";

    restApi->post(googleOauthSignInEndpoint,body, QVariantMap(), headers);
}


void FirebaseAuthWrapper::checkRestApiState(){

    restApi->checkRequestStatus();
}

void FirebaseAuthWrapper::parseAuthResponse(QVariant response)
{
    QVariant statusCode = restApi->getResponseStatusCode();

    if(statusCode.isValid() && statusCode.toInt() == 200){

        emit signedIn(response);

    }
        QJsonDocument jsonResponse = QJsonDocument::fromVariant(response);
        qDebug()<<jsonResponse;

}

void FirebaseAuthWrapper::setApiKey(const QString &newApiKey)
{
    apiKey = newApiKey;
    this->emailSignUpEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + apiKey;
    this->emailSignInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + apiKey;
    this->refreshTokenEndpoint = "https://securetoken.googleapis.com/v1/token?key=" + apiKey;
    this->googleOauthSignInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithIdp?key=" + apiKey;


}

const QString &FirebaseAuthWrapper::getEmailRefreshToken() const
{
    return emailRefreshToken;
}

void FirebaseAuthWrapper::setEmailRefreshToken(const QString &newEmailRefreshToken)
{
    if (emailRefreshToken == newEmailRefreshToken)
        return;
    emailRefreshToken = newEmailRefreshToken;
    emit emailRefreshTokenChanged();
}

QString FirebaseAuthWrapper::getApiKey()
{
    return this->apiKey;
}

const QString &FirebaseAuthWrapper::getEmailIdToken() const
{
    return emailIdToken;
}

void FirebaseAuthWrapper::setEmailIdToken(const QString &newEmailIdToken)
{
    if (emailIdToken == newEmailIdToken)
        return;
    emailIdToken = newEmailIdToken;
    emit emailIdTokenChanged();
}
