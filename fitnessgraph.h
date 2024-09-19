#ifndef FITNESSGRAPH_H
#define FITNESSGRAPH_H

#include <iostream>
#include <QObject>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QWidget>
#include <QVBoxLayout>

class FitnessGraph : public QWidget
{
    Q_OBJECT
public:
    explicit FitnessGraph(QWidget *parent = nullptr);

    void updateGraph(double newFitness, double timeElapsed);

private:
    QChart *chart;
    QLineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;

};

#endif // FITNESSGRAPH_H
