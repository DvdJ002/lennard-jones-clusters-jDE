#ifndef FITNESSGRAPH_H
#define FITNESSGRAPH_H

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
    void setTargetEnergy(double target);
    void updateGraph(double newFitness, double timeElapsed);
    void resetGraph();
    void setPreferences(bool darkMode, bool displayTarget, int yRangeBottom, int yRangeTop, bool clearLine);

private:
    QChart *chart;
    QLineSeries *series, *targetLine;
    QValueAxis *axisX;
    QValueAxis *axisY;
    double targetValue = 0.0;
    bool displayTargetValue, clearLineWhenStart;

    // Range values
    const int X_RANGE_INIT = 6.0;
    int Y_RANGE_LOW = -29.0;
    int Y_RANGE_HIGH = 1.0;
};

#endif // FITNESSGRAPH_H
