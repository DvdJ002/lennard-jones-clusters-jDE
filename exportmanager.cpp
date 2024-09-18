#include "exportmanager.h"


ExportManager::ExportManager(QObject *parent) : QObject{parent}
{}

QJsonObject ExportManager::getJSONFromArguments(
    int N, int seed, int nfesLmt, int runtimeLmt, int Np, double target,  bool optimizeZeroes
){
    QJsonObject arguments;
    arguments["N"] = N;
    arguments["seed"] = seed;
    arguments["nfesLmt"] = nfesLmt;
    arguments["runtimeLmt"] = runtimeLmt;
    arguments["Np"] = Np;
    arguments["target"] = target;
    arguments["optimizeZeroes"] = optimizeZeroes;
    return arguments;
}

bool ExportManager::exportJdeArguments(const QString &filePath, const QJsonObject &arguments) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QJsonDocument doc(arguments);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

QJsonObject ExportManager::importJdeArguments(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return QJsonObject();

    QByteArray fileData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(fileData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        return QJsonObject();  // Handle JSON parsing error

    return doc.object();
}

bool ExportManager::automaticJdeExport(
    const QString &filePath, const QJsonObject &arguments, const QString &results, const QDateTime &timeStamp)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    std::string outputString = "";
    // TODO: Timestamp currently symbolic, need to acquire it from thread
    if (!timeStamp.isNull()) {
        //file.write("[" + timeStamp.toString("yyyy-MM-dd HH:mm:ss").toUtf8() + "]\n");
        outputString += "[" + timeStamp.toString("yyyy-MM-dd HH:mm:ss").toUtf8() + "]\n";
    }
    // TODO: Possibly write arguments in human form?
    if (!arguments.isEmpty()){
        QJsonDocument doc(arguments);
        //file.write(doc.toJson(QJsonDocument::Indented));
        //file.write("\n");
        outputString += doc.toJson(QJsonDocument::Indented).toStdString() + "\n";
    }
    if (!results.isEmpty()){
        //file.write(results.toUtf8());
        outputString += results.toUtf8();
        outputString += "\n---------------------------------------\n";
    }

    file.write(QByteArray::fromStdString(outputString));
    file.close();

    return true;
}

