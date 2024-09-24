#include "exportmanager.h"

#include <QJsonArray>

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

// Empty JSON object = error in importing
QJsonObject ExportManager::importJdeArguments(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return QJsonObject();

    QByteArray fileData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(fileData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        return QJsonObject();

    return doc.object();
}

bool ExportManager::automaticJdeExport(
    const QString &filePath, const QJsonObject &arguments, const QString &results, const QDateTime &timeStamp)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    std::string outputString = "";
    if (!timeStamp.isNull()) {
        outputString += "[" + timeStamp.toString("yyyy-MM-dd HH:mm:ss").toUtf8() + "]\n";
    }
    // TODO: Possibly write arguments in human form?
    if (!arguments.isEmpty()){
        QJsonDocument doc(arguments);
        outputString += doc.toJson(QJsonDocument::Indented).toStdString() + "\n";
    }
    if (!results.isEmpty()){
        outputString += results.toUtf8();
        outputString += "\n---------------------------------------\n";
    }

    file.write(QByteArray::fromStdString(outputString));
    file.close();

    return true;
}


QJsonObject ExportManager::getJSONFromSeriesList(QVector<QLineSeries*> *seriesList){
    QJsonObject rootObject;  // Holds the entire JSON structure
    QJsonArray seriesArray;

    for (int i = 0; i < seriesList->size(); ++i) {
        QLineSeries *series = seriesList->at(i);
        QJsonArray pointsArray;  // Holds points of the QLineSeries

        for (const QPointF &point : series->points()) {
            QJsonObject pointObject;
            pointObject["x"] = point.x();
            pointObject["y"] = point.y();
            pointsArray.append(pointObject);
        }

        QJsonObject seriesObject;
        seriesObject["name"] = series->name();  // If series has a name
        seriesObject["points"] = pointsArray;

        // Append JSON object to the series array
        seriesArray.append(seriesObject);
    }

    // Add series array to root object
    rootObject["seriesList"] = seriesArray;

    // Convert the QJsonObject into a QJsonDocument
    return rootObject;
}


bool ExportManager::exportGraphValues(const QString &filePath, QVector<QLineSeries*> *seriesList){
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;


    QJsonDocument doc(this->getJSONFromSeriesList(seriesList));
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}
