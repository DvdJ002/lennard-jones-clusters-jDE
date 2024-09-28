#include "exportmanager.h"

#include <QJsonArray>

ExportManager::ExportManager(QObject *parent) : QObject{parent}
{}

/*------------------- JDE FUNCTIONS -------------------*/
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

/*------------------- GRAPH FUNCTIONS -------------------*/

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

QVector<QLineSeries*>* ExportManager::convertJSONToSeriesList(const QJsonObject &importedJSON){
    QVector<QLineSeries*> *seriesVec = new QVector<QLineSeries*>();  // Output vector
        QJsonArray seriesArray = importedJSON["seriesList"].toArray();

    // Iterate over all series
    for (const QJsonValue &seriesValue : seriesArray) {
        // Object for all the JSON data
        QLineSeries *lineSeries = new QLineSeries();

        QJsonObject seriesObject = seriesValue.toObject();
        // Set the name and points to the series
        QString seriesName = seriesObject["name"].toString();
        lineSeries->setName(seriesName);
        QJsonArray pointsArray = seriesObject["points"].toArray();

        // Iterate over each point and add it to QLineSeries
        for (const QJsonValue &pointValue : pointsArray) {
            QJsonObject pointObject = pointValue.toObject();
            lineSeries->append(
                pointObject["x"].toDouble(), pointObject["y"].toDouble()
            );
        }

        // Add the series to output vector
        seriesVec->append(lineSeries);
    }

    return seriesVec;
}

// Returns empty array if error
QVector<QLineSeries*>* ExportManager::importGraphValues(const QString &filePath){
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return new QVector<QLineSeries*>();

    QByteArray fileData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(fileData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        return new QVector<QLineSeries*>();

    return convertJSONToSeriesList(doc.object());
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
