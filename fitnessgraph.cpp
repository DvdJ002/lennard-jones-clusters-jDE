#include "fitnessgraph.h"

FitnessGraph::FitnessGraph(QWidget *parent) : QWidget(parent),
    chart(new QChart), targetLine(new QLineSeries), axisX(new QValueAxis), zoomedIn(false),
    axisY(new QValueAxis), seriesList(new QVector<QLineSeries*>()), colorPicker(new ColorPicker(6))
{
    // Create a chart view
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set up the line series
    chart->setTheme(QChart::ChartThemeHighContrast);
    targetLine->setName("Target");
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

    // Horizontal line marks the target energy
    targetLine->attachAxis(axisX);
    targetLine->attachAxis(axisY);
    QPen pen(Qt::red);
    pen.setWidthF(0.03);
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

    // Set the colors that the lines will be assigned randomly
    colorPicker->addColor(Qt::black);
    colorPicker->addColor(Qt::blue);
    colorPicker->addColor(Qt::green);
    colorPicker->addColor(Qt::darkRed);
    colorPicker->addColor(Qt::magenta);
    colorPicker->addColor(Qt::darkGray);
}


// Time elapsed is in seconds
void FitnessGraph::updateGraph(double newFitness, double timeElapsed){
    // Append energy point to the last element in the vector
    seriesList->at(seriesList->size()-1)->append(timeElapsed, newFitness);

    // Extend X axis if out of range
    if (timeElapsed > axisX->max()) {
        axisX->setRange(timeElapsed - X_RANGE_INIT, timeElapsed);
    }

    // Draw target line if necessary
    if (displayTargetValue) {
        targetLine->append(axisX->min(), targetValue);
        targetLine->append(axisX->max(), targetValue);
    }

}

// Adds a different colored line - chosen by colorPicker
void FitnessGraph::addLineToChart(){
    QLineSeries *newSeries = new QLineSeries();
    QPen pen(colorPicker->randomColor());
    newSeries->setName("Energy line " + QString::number(seriesList->size() + 1));
    newSeries->setPen(pen);

    // Add new series to chart and vector
    chart->addSeries(newSeries);
    newSeries->attachAxis(axisX); newSeries->attachAxis(axisY);
    seriesList->push_back(newSeries);
}

void FitnessGraph::importSeries(QVector<QLineSeries*>* seriesVec){
    // Clear previous series - avoid duplicate line numbers
    clearAllSeries();
    for (QLineSeries* series : *seriesVec){
        QPen pen(colorPicker->randomColor());
        series->setPen(pen);
        chart->addSeries(series);
        series->attachAxis(axisX); series->attachAxis(axisY);
        seriesList->push_back(series);
    }
}

// TODO: Clear all series in vector and reset X range
void FitnessGraph::resetGraph() {
    clearAllSeries();
    targetLine->clear();
    axisX->setRange(0, X_RANGE_INIT);
    setYRange(Y_RANGE_LOW_INIT, Y_RANGE_HIGH_INIT);
}

// ALWAYS CALL BEFORE NEW ALGORITHM INSTANCE IS LAUNCHED
void FitnessGraph::newRunInstance(){
    if (clearLineWhenStart){
        clearAllSeries();
        targetLine->clear();
        addLineToChart();
    } else {
        addLineToChart();
    }
    // Don't allow multiple target lines
    targetLine->clear();
    axisX->setRange(0, X_RANGE_INIT);
}

void FitnessGraph::clearAllSeries(){
    for (QLineSeries* series : *seriesList) {
        delete series;
    }
    seriesList->clear();
}

void FitnessGraph::setTargetEnergy(double target){
    this->targetValue = target;
}

void FitnessGraph::setYRange(int yLow, int YHigh){
    Y_RANGE_LOW = yLow;
    Y_RANGE_HIGH = YHigh;
    axisY->setRange(Y_RANGE_LOW, Y_RANGE_HIGH);
}

// Toggles Y range between [target-1, target+1] and default
void FitnessGraph::toggleZoom(){
    zoomedIn = !zoomedIn;
    zoomedIn ? axisY->setRange(targetValue - 0.35, targetValue + 0.75)
         : this->setYRange(Y_RANGE_LOW, Y_RANGE_HIGH);
}


// Sets user-defined preferences, func called by mainwindow, acquired by settings manager
void FitnessGraph::setPreferences(
    bool axisTitle, bool displayTarget, int yRangeLow, int yRangeHigh, bool clearLine)
{
    if (axisTitle){
        axisX->setTitleText(""); axisY->setTitleText("");
    } else {
        axisX->setTitleText("Runtime"); axisY->setTitleText("Best energy");
    }
    this->displayTargetValue = displayTarget;
    this->setYRange(yRangeLow, yRangeHigh);
    this->clearLineWhenStart = clearLine;
}

QVector<QLineSeries*>* FitnessGraph::getLinesForExport(){
    return seriesList;
}

FitnessGraph::~FitnessGraph()
{
    delete targetLine;
    delete axisX;
    delete axisY;

    delete seriesList;
    delete colorPicker;
}

