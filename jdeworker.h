#ifndef JDEWORKER_H
#define JDEWORKER_H

#include <vector>
#include <random>
#include <QObject>
#include "x.h"

class JDEWorker : public QObject
{
    Q_OBJECT
public:
    explicit JDEWorker(QObject *parent = nullptr);
    // Algorithm arguments and flags
    unsigned int N, seed, nfesLmt, runtimeLmt, Np; double target; bool optimizeZeroes;
    // Execution variable
    bool runInstance;
    void setArguments(unsigned int N, unsigned int seed, unsigned int nfesLmt, unsigned int runtimeLmt,
                      unsigned int Np, double target, bool optimizeZeroes);
    void terminateExecution();

signals:
    void fitnessUpdated(double fitness, double elapsed);  // Signal to notify UI of the new best fitness
    void sendAlgorithmResults(std::string results);
    void taskFinished();

public slots:
    void startAlgorithm(X& best, std::vector<X>& population, unsigned int& nfes, std::mt19937& generator);
    void initializeAlgorithm();
};
#endif // JDEWORKER_H
