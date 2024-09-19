#include "fitnessgraph.h"

FitnessGraph::FitnessGraph(QWidget *parent) : QWidget(parent),
    chart(new QChart), series(new QLineSeries), targetLine(new QLineSeries),
    axisX(new QValueAxis), axisY(new QValueAxis)
{
    // Create a chart view
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set up the line series
    chart->setTheme(QChart::ChartThemeHighContrast);
    series->setName("Current energy");
    targetLine->setName("Target");
    chart->addSeries(series);
    chart->addSeries(targetLine);
    chart->setDropShadowEnabled(true);


    // Set up X and Y axes
    axisX->setTitleText("Runtime");
    axisX->setLabelFormat("%i");
    axisX->setRange(0, X_RANGE_INIT);  // Initial X range

    axisY->setTitleText("Best energy");
    axisY->setLabelFormat("%.2f");
    axisY->setRange(Y_RANGE_LOW, Y_RANGE_HIGH);  // Initial Y range

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Horizontal line marks the target energy
    targetLine->attachAxis(axisX);
    targetLine->attachAxis(axisY);
    QPen pen(Qt::red);
    pen.setWidthF(0.015);
    targetLine->setPen(pen);

    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setWeight(QFont::Bold);
    chart->setTitle("jDE Output");
    chart->setTitleFont(titleFont);

    // Set layout so the chart view is displayed properly
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}

// Time elapsed is in seconds
void FitnessGraph::updateGraph(double newFitness, double timeElapsed){
    series->append(timeElapsed, newFitness);
    targetLine->append(axisX->min(), targetValue);
    targetLine->append(axisX->max(), targetValue);

    // Extend X axis if out of range
    if (timeElapsed > axisX->max()) {
        axisX->setRange(timeElapsed - X_RANGE_INIT, timeElapsed);
    }
}

void FitnessGraph::resetGraph() {
    series->clear();
    targetLine->clear();
    axisX->setRange(0, X_RANGE_INIT);
}

void FitnessGraph::setTargetEnergy(double target){
    this->targetValue = target;
}
