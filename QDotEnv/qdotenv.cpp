#include "qdotenv.h"
#include <QtDebug>
QDotEnv::QDotEnv(QObject *parent): QObject(parent)
{
}

const QVariantMap &QDotEnv::getEnvMap() const
{
    return this->envDict;
}

void QDotEnv::load_dotenv()
{
    QFile file("://.env");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){

        qWarning()<<"Warning! .env File could not be loaded!";
        return;
    }

    while (!file.atEnd()) {
        QString data = file.readLine();
        QStringList keyValueList = data.trimmed().split('=');
        this->envDict[keyValueList[0]] = keyValueList[1];
    }

}

QVariant &QDotEnv::operator[](const QString &key){

    return this->envDict[key];
}


QVariant QDotEnv::getenv(QString key, QVariant defaultValue)
{

    if (!this->envDict.contains(key)){
        this->envDict[key] = defaultValue;
    }
    return this->envDict[key];

}
