#ifndef EXPORTMANAGER_H
#define EXPORTMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QJsonDocument>
#include <QLineSeries>

class ExportManager : public QObject
{
    Q_OBJECT
public:
    explicit ExportManager(QObject *parent = nullptr);

    // jDE related functions
    QJsonObject getJSONFromArguments(int N, int seed, int nfesLmt, int runtimeLmt, int Np, double target, bool optimizeZeroes);
    QJsonObject importJdeArguments(const QString &filePath);
    bool exportJdeArguments(const QString &filePath, const QJsonObject &arguments);
    // If &arguments or &results is empty, don't write it to file
    bool automaticJdeExport(const QString &filePath, const QJsonObject &arguments, const QString &results, const QDateTime &timeStamp);

    // Graph export
    QJsonObject getJSONFromSeriesList(QVector<QLineSeries*> *seriesList);
    bool exportGraphValues(const QString &filePath, QVector<QLineSeries*> *seriesList);
signals:
};

#endif // EXPORTMANAGER_H
