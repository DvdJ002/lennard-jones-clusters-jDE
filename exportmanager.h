#ifndef EXPORTMANAGER_H
#define EXPORTMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QJsonDocument>

class ExportManager : public QObject
{
    Q_OBJECT
public:
    explicit ExportManager(QObject *parent = nullptr);

    QJsonObject getJSONFromArguments(
        int N, int seed, int nfesLmt, int runtimeLmt, int Np, double target, bool optimizeZeroes
    );

    QJsonObject importJdeArguments(const QString &filePath);
    bool exportJdeArguments(const QString &filePath, const QJsonObject &arguments);
    bool exportAll(const QString &filePath, const QJsonObject &arguments, const QString &results);

signals:
};

#endif // EXPORTMANAGER_H
