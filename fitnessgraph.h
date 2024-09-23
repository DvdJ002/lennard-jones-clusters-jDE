#ifndef FITNESSGRAPH_H
#define FITNESSGRAPH_H

#include <QObject>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QWidget>
#include <QVBoxLayout>
#include "ColorPicker.h"

class FitnessGraph : public QWidget
{
    Q_OBJECT
public:
    explicit FitnessGraph(QWidget *parent = nullptr);
    void updateGraph(double newFitness, double timeElapsed);
    void addLineToChart();
    void resetGraph();
    void newRunInstance();
    void clearAllSeries();
    void setPreferences(bool darkMode, bool displayTarget, int yRangeBottom, int yRangeTop, bool clearLine);
    void setTargetEnergy(double target);
    void setYRange(int yLow, int yHigh);
    ~FitnessGraph();

private:
    QChart *chart;
    QLineSeries *targetLine;
    QValueAxis *axisX, *axisY;
    QVector<QLineSeries*> *seriesList;
    ColorPicker* colorPicker;
    double targetValue = 0.0;
    bool displayTargetValue, clearLineWhenStart;

    // Range values
    const int X_RANGE_INIT = 6.0;
    int Y_RANGE_LOW = -29.0;
    int Y_RANGE_HIGH = 1.0;
};

#endif // FITNESSGRAPH_H
