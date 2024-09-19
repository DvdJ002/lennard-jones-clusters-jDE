#include "fitnessgraph.h"

FitnessGraph::FitnessGraph(QWidget *parent)
    :
    QWidget(parent),
    chart(new QChart),
    series(new QLineSeries),
    axisX(new QValueAxis),
    axisY(new QValueAxis)
{
    // Create a chart view
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set up the line series
    chart->setTheme(QChart::ChartThemeLight);
    chart->addSeries(series);
    chart->setDropShadowEnabled(true);

    // Set up X and Y axes
    axisX->setTitleText("Runtime");
    axisX->setLabelFormat("%i");
    axisX->setRange(0, 10);  // Initial X-axis range

    axisY->setTitleText("Best energy");
    axisY->setLabelFormat("%.2f");
    axisY->setRange(5, -30);  // Initial Y-axis range

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QFont titleFont;
    titleFont.setPointSize(15);
    titleFont.setWeight(QFont::Bold);
    chart->setTitle("Live jDE Output");
    chart->setTitleFont(titleFont);

    // Set layout so the chart view is displayed properly
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}

// Time elapsed in seconds
void FitnessGraph::updateGraph(double newFitness, double timeElapsed){
    series->append(timeElapsed, newFitness);
    std::cout << "Appended ";
    std::cout << timeElapsed;
    if (timeElapsed > axisX->max()) {
        axisX->setRange(timeElapsed - 10.0, timeElapsed);
    }
}
