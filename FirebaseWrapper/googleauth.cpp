#include "googleauth.h"
#include <QtNetworkAuth>
#include <QDebug>
#include <QDesktopServices>


GoogleAuth::GoogleAuth(const QString &clientIdentifier, const QString &clientCode, QObject *parent) :
    QObject(parent)
{
    this->clientId = clientIdentifier;
    this->clientCode = clientCode;
    setupOauth();
}

void GoogleAuth::setupOauth(){

    auto replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    this->oauth2.setReplyHandler(replyHandler);
    this->oauth2.setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/v2/auth"));
    this->oauth2.setAccessTokenUrl(QUrl("https://oauth2.googleapis.com/token"));
    this->oauth2.setScope("profile email");
    this->oauth2.setClientIdentifier(this->clientId);
    this->oauth2.setClientIdentifierSharedKey(this->clientCode);



    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged, [=](
            QAbstractOAuth::Status status) {
        if (status == QAbstractOAuth::Status::Granted)
        {
            const QString token = this->oauth2.token();
            const QString refreshTokenthis=this->oauth2.refreshToken();
            qDebug()<<oauth2.token();
            emit authenticated();
        }
        else{
            qDebug()<<oauth2.state();
        }
    });
    this->oauth2.setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
        if (stage == QAbstractOAuth::Stage::RequestingAuthorization && isPermanent())
            parameters->insert("duration", "permanent");
    });



    this->oauth2.setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
        if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
            QByteArray code = parameters->value("code").toByteArray();
            parameters->replace("code", QUrl::fromPercentEncoding(code));

        }
    });

    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    connect(this, &GoogleAuth::authenticated, this, &GoogleAuth::NotifyAuthentication);

}

QVariant GoogleAuth::getUserInfo()
{
    return this->userInfo;
}

void GoogleAuth::NotifyAuthentication(){

    this->requestUserInfo();
}


bool GoogleAuth::isPermanent() const
{
    return this->permanent;
}

void GoogleAuth::setPermanent(bool value)
{
    this->permanent = value;
}

void GoogleAuth::grant()
{
    this->oauth2.grant();
}

QString GoogleAuth::getToken()
{
    QString bearer = this->oauth2.token();
    //qDebug()<<bearer;
    return bearer;
}

const QString &GoogleAuth::getClientCode() const
{
    return this->clientCode;
}

void GoogleAuth::setClientCode(const QString &newClientCode)
{
    if (this->clientCode == newClientCode)
        return;
    this->clientCode = newClientCode;
    emit clientCodeChanged();
}

void GoogleAuth::resetOauth(const QString &newClientCode, const QString &newClientId)
{
    if (this->clientCode == newClientCode||this->clientId == newClientId)
        return;
    this->clientCode = newClientCode;
    this->clientId = newClientId;
    setupOauth();

}


const QString &GoogleAuth::getClientId() const
{
    return this->clientId;
}

void GoogleAuth::setClientId(const QString &newClientId)
{
    if (this->clientId == newClientId)
        return;
    this->clientId = newClientId;
    emit clientIdChanged();
}


void GoogleAuth::authenticationDataAvailable(){

    const auto json = this->replyOauth->readAll();
    this->userInfo = QJsonDocument::fromJson(json).toVariant();
    emit userInfoChanged();

}

void GoogleAuth::requestUserInfo()
{
    this->replyOauth.reset(this->oauth2.get(this->googleUserinfoUrl));
    connect(this->replyOauth.data(), &QNetworkReply::readyRead, this, &GoogleAuth::authenticationDataAvailable);
}


