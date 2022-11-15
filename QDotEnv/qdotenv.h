#ifndef QDOTENV_H
#define QDOTENV_H

#include "QDotEnv_global.h"

#include <QObject>
#include <QSharedPointer>
#include <QVariantMap>
#include <qqml.h>
#include <QFile>
#include <QtQml/qqmlregistration.h>

class QDOTENV_EXPORT QDotEnv: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap envDict READ getEnvMap CONSTANT)
    QML_ELEMENT

public:
    QDotEnv(QObject *parent = nullptr) ;

    const QVariantMap &getEnvMap() const;
    QVariant &operator[](const QString &key);

public slots:
    void load_dotenv();
    QVariant getenv(QString key, QVariant defaultValue = "");

private:
    QVariantMap envDict;

};

#endif // QDOTENV_H
