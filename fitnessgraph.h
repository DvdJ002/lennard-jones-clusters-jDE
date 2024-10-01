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
    void importSeries(QVector<QLineSeries*>* seriesVec);
    void resetGraph();
    void newRunInstance();
    void clearAllSeries();
    void setPreferences(bool darkMode, bool displayTarget, int yRangeBottom, int yRangeTop, bool clearLine);
    void setTargetEnergy(double target);
    void setYRange(int yLow, int yHigh);
    void toggleZoom();
    QVector<QLineSeries*>* getLinesForExport();
    ~FitnessGraph();

    const int X_RANGE_INIT = 6.0;
    const int Y_RANGE_LOW_INIT =  -29.0;
    const int Y_RANGE_HIGH_INIT =  1.0;

private:
    QChart *chart;
    QLineSeries *targetLine;
    QValueAxis *axisX, *axisY;
    QVector<QLineSeries*> *seriesList;
    ColorPicker* colorPicker;
    double targetValue = 0.0;
    bool displayTargetValue, clearLineWhenStart, zoomedIn;

    // Range values
    int Y_RANGE_LOW = -29.0;
    int Y_RANGE_HIGH = 1.0;
};

#endif // FITNESSGRAPH_H
