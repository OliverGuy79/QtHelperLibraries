#ifndef FIREBASEAUTHWRAPPER_H
#define FIREBASEAUTHWRAPPER_H

#include <QObject>
#include "restapimanager.h"
#include "googleauth.h"


class FIREBASEWRAPPER_EXPORT FirebaseAuthWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString emailIdToken READ getEmailIdToken WRITE setEmailIdToken NOTIFY emailIdTokenChanged)
    Q_PROPERTY(QString emailRefreshToken READ getEmailRefreshToken WRITE setEmailRefreshToken NOTIFY emailRefreshTokenChanged)
    Q_PROPERTY(QString apiKey READ getApiKey WRITE setApiKey NOTIFY apiKeyChanged)
    Q_PROPERTY(QVariant googleUserInfo READ getGoogleUserInfo NOTIFY googleUserInfoChanged)
    QML_ELEMENT

public:
    explicit FirebaseAuthWrapper(QObject *parent = nullptr);
    ~FirebaseAuthWrapper();


    const QVariant &getGoogleUserInfo() const;
    void setGoogleUserInfo();
    const QString &getEmailIdToken() const;
    void setEmailIdToken(const QString &newEmailIdToken);


public slots:

    void signUpWithEmail(const QString & email, const QString & password);
    void signInWithEmail(const QString & email, const QString & password);
    void signInWithRefreshToken(const QString & email, const QString & password);
    void parseAuthResponse(QVariant response);
    void setApiKey(const QString &newApiKey);
    const QString &getEmailRefreshToken() const;
    void setEmailRefreshToken(const QString &newEmailRefreshToken);

    QString getApiKey();
    void signInWithOauthAccessToken();
    void generateOauthToken();
    void checkRestApiState();
    void signInWithGoogle();
    void setupGoogleAuth(QString clientId, QString clientCode);
    QString getGoogleToken();
signals:
    void signUpCompleted();
    void signInCompleted();
    void signUpFailed();
    void signInFailed();

    void emailIdTokenChanged();
    void apiKeyChanged();

    void emailRefreshTokenChanged();
    void signedIn(QVariant response);
    void googleUserInfoChanged();

private:

    QString apiKey ;
    QString emailSignUpEndpoint;
    QString emailSignInEndpoint;
    QString refreshTokenEndpoint;
    QString oauthEndpoint;
    QString emailIdToken;
    QString emailRefreshToken;
    QSharedPointer<RestApiManager> restApi;
    QSharedPointer<GoogleAuth> googleAuth;
    QVariant googleUserInfo;




};

#endif // FIREBASEAUTHWRAPPER_H
