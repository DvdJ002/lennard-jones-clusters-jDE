#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "autoexportdialog.h"
#include "jdeworker.h"
#include "jdestatsdialog.h"

#include <QThread>
#include <cctype>
#include <climits>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), exportManager(new ExportManager()),
    settingsManager(new SettingsManager), mainFitnessGraph(new FitnessGraph())
{
    ui->setupUi(this);

    // Disables resizing
    this->setFixedSize(this->size());
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setup_ui_elements();
    settingsManager->recordAppOpened();
    settingsManager->recordLastUsage(QDateTime::currentDateTime());
    applyGraphSettings();
}


void MainWindow::setup_ui_elements(){
    this->stackedWidget = ui->stackedWidget;
    stackedWidget->setCurrentIndex(0);

    ui->label_livedata->setStyleSheet("border: 1px solid black;");
    ui->label_end_results->setStyleSheet("border: 1px solid black;");

    // Menu and actions
    QString style = "QMenuBar::item:selected { background: white; } QMenuBar::item:pressed {  background: white; }";
    ui->menuBar->setStyleSheet(style);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::quit_app);
    connect(ui->actionImport_jDE_arguments, &QAction::triggered, this, &MainWindow::action_import_jde_arguments);
    connect(ui->actionExport_jDE_arguments, &QAction::triggered, this, &MainWindow::action_export_jde_arguments);
    connect(ui->actionAutomatic_jDE_export, &QAction::triggered, this, &MainWindow::action_jde_automatic_export);
    connect(ui->actionExport_maingraph_values, &QAction::triggered, this, &MainWindow::action_export_jde_graph);
    connect(ui->actionImport_maingraph_values, &QAction::triggered, this, &MainWindow::action_import_jde_graph);
    connect(ui->actionjDE_Stats, &QAction::triggered, this, &MainWindow::action_show_jde_stats);


    // Input data form styling
    QFont font;
    font.setPointSize(12);
    font.setBold(false);
    font.setItalic(false);
    font.setWeight(QFont::DemiBold);
    ui->edit_N->setFont(font);
    ui->edit_seed->setFont(font);
    ui->edit_target->setFont(font);
    ui->edit_runtimelmt->setFont(font);
    ui->edit_np->setFont(font);

    ui->graphLayout->addWidget(mainFitnessGraph);

    // Fill elements with graph settings
    QVector<int> rangeVec = settingsManager->getGraphSettingsYRange();
    QMap preferenceMap = settingsManager->getGraphSettingsCheckbox();
    ui->checkBox_axistitle->setChecked(preferenceMap[settingsManager->G_S_AXIS_TITLE_KEY]);
    ui->checkBox_targetline->setChecked(preferenceMap[settingsManager->G_S_DISPLAY_TARGET_KEY]);
    ui->checkBox_clearline->setChecked(preferenceMap[settingsManager->G_S_CLEAR_LINE_KEY]);
    ui->edit_yrange->setText(QString::number(rangeVec[0]) + ":" + QString::number(rangeVec[1]));
}


/* ---------------------BUTTON SLOTS--------------------- */

// Slots for switching between screens
void MainWindow::on_button_mainscreen_clicked() { stackedWidget->setCurrentIndex(0); }
void MainWindow::on_button_screen2_clicked() { stackedWidget->setCurrentIndex(1); }
void MainWindow::on_button_info_clicked() { stackedWidget->setCurrentIndex(2); }

void MainWindow::on_button_resetgraph_clicked() {
    if (is_jde_running()) {
        prompt_warning_message("Invalid action", "Cannot reset graph while jDE instance is running");
        return;
    }
    mainFitnessGraph->resetGraph();
}

void MainWindow::on_button_start_clicked() {
    // DO NOT START ANOTHER THREAD IF PREV INSTANCE IS RUNNING
    if (is_jde_running()) {
        jdeWorker->terminateExecution();
        return;
    }

    if (!is_input_valid()){
        prompt_warning_message("Invalid arguments!", "The arguments contain blank fields or non-number characters");
        return;
    }

    // Create a jde worker object and move it to a separate thread
    jdeWorker = new JDEWorker();
    jdeWorkerThread = new QThread();
    jdeWorker->moveToThread(jdeWorkerThread);

    connect(jdeWorkerThread, &QThread::started, jdeWorker, &JDEWorker::initializeAlgorithm);
    // Signal to update new best fitness in UI
    connect(jdeWorker, &JDEWorker::fitnessUpdated, this, &MainWindow::update_best_fitness);
    connect(jdeWorker, &JDEWorker::sendAlgorithmResults, this, &MainWindow::display_algo_results);

    // Quit and clean up thread and worker when task is finished
    connect(jdeWorker, &JDEWorker::taskFinished, jdeWorkerThread, &QThread::quit);
    connect(jdeWorkerThread, &QThread::finished, jdeWorker, &QObject::deleteLater);
    connect(jdeWorkerThread, &QThread::finished, jdeWorkerThread, &QObject::deleteLater);

    start_worker();
}

void MainWindow::on_button_savegraphsettings_clicked()
{
    if (is_jde_running()) {
        prompt_warning_message("Invalid action", "Cannot save graph settings while jDE instance is running");
        return;
    }

    settingsManager->setGraphSettings(
        ui->checkBox_axistitle->isChecked(), ui->checkBox_targetline->isChecked(),
        ui->edit_yrange->toPlainText(), ui->checkBox_clearline->isChecked()
    );

    // Graph preferences acquired from settings manager
    applyGraphSettings();
    prompt_info_message("Graph preferences saved");

    settingsManager->recordGraphSettingsSave();
}


// Resets the Y range setting on graph to -29.0:1.0
void MainWindow::on_button_resetyrange_clicked()
{
    mainFitnessGraph->setYRange(mainFitnessGraph->Y_RANGE_LOW_INIT, mainFitnessGraph->Y_RANGE_HIGH_INIT);
    QString formattedString =
        QString::number(mainFitnessGraph->Y_RANGE_LOW_INIT) + ":" +
        QString::number(mainFitnessGraph->Y_RANGE_HIGH_INIT);
    settingsManager->setGraphSettingsYRange(formattedString);
    ui->edit_yrange->setText(formattedString);
}


/* ---------------------MENU ACTION SLOTS--------------------- */

void MainWindow::quit_app(){
    qApp->exit();
}

void MainWindow::action_import_jde_arguments(){
        QString filePath = QFileDialog::getOpenFileName(this, tr("Import arguments"), "", tr("JSON Files (*.json)"));
        if (filePath.isEmpty()) {
            return;
        }

        QJsonObject arguments = exportManager->importJdeArguments(filePath);
        if (arguments.isEmpty()){
            prompt_warning_message("Import Error", "Failed to import jDE arguments");
            return;
        }

        QJsonValue nValue = arguments.value("N");
        QJsonValue npValue = arguments.value("Np");
        QJsonValue nfesLmtValue = arguments.value("nfesLmt");
        QJsonValue optimizeZeroesValue = arguments.value("optimizeZeroes");
        QJsonValue runtimeLmtValue = arguments.value("runtimeLmt");
        QJsonValue seedValue = arguments.value("seed");
        QJsonValue targetValue = arguments.value("target");

        if (optimizeZeroesValue.isUndefined() || runtimeLmtValue.isUndefined() || seedValue.isUndefined() ||
            nValue.isUndefined() || npValue.isUndefined() || nfesLmtValue.isUndefined() || targetValue.isUndefined())
        {
            prompt_warning_message("Import Error", "Failed to find JSON values");
            return;
        }

        ui->edit_N->setText(QString::number(nValue.toInteger()));
        ui->edit_np->setText(QString::number(npValue.toInteger()));
        ui->edit_runtimelmt->setText(QString::number(runtimeLmtValue.toInteger()));
        ui->edit_seed->setText(QString::number(seedValue.toInteger()));
        ui->edit_target->setText(QString::number(targetValue.toDouble()));
        optimizeZeroesValue.toBool() ?
            ui->checkBox_opt->setCheckState(Qt::Checked) : ui->checkBox_opt->setCheckState(Qt::Unchecked);

        settingsManager->recordArgumentsImport();

}

void MainWindow::action_export_jde_arguments(){
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export arguments"), "", tr("JSON Files (*.json)"));
    if (filePath.isEmpty())
        return;
    QJsonObject arguments = exportManager->getJSONFromArguments(
        ui->edit_N->toPlainText().toInt(), ui->edit_seed->toPlainText().toInt(), -1,
        ui->edit_runtimelmt->toPlainText().toInt(), ui->edit_np->toPlainText().toInt(),
        ui->edit_target->toPlainText().toDouble(), (ui->checkBox_opt->checkState() == Qt::Checked)
    );

    QFileInfo fileInfo(filePath);
    if (!exportManager->exportJdeArguments(filePath, arguments)){
        prompt_warning_message("Export Error", "Failed to export jDE arguments to" + fileInfo.fileName().toStdString());
        return;
    }
    prompt_info_message("jDE arguments exported to " + fileInfo.fileName().toStdString());

    settingsManager->recordArgumentsExport();
}

void MainWindow::action_jde_automatic_export(){
    AutoExportDialog dialog(this, settingsManager);
    if (dialog.exec() == QDialog::Accepted){
        prompt_info_message("Automatic export preferences saved");
    }
}

void MainWindow::action_export_jde_graph(){
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export graph values"), "", tr("JSON Files (*.json)"));
    if (filePath.isEmpty())
        return;

    QFileInfo fileInfo(filePath);
    if (!exportManager->exportGraphValues(filePath, mainFitnessGraph->getLinesForExport())){
        prompt_warning_message("Export Error", "Failed to export graph values to" + fileInfo.fileName().toStdString());
        return;
    }
    prompt_info_message("Graph values exported to " + fileInfo.fileName().toStdString());

    settingsManager->recordGraphExport();
}

void MainWindow::action_import_jde_graph(){
    QString filePath = QFileDialog::getOpenFileName(this, tr("Import graph values"), "", tr("JSON Files (*.json)"));
    if (filePath.isEmpty()) {
        return;
    }

    QVector<QLineSeries*> *seriesVec = exportManager->importGraphValues(filePath);
    if (seriesVec->isEmpty()){
        prompt_warning_message("Import Error", "Failed to open file or invalid JSON format");
        return;
    }
    mainFitnessGraph->importSeries(seriesVec);

    settingsManager->recordGraphImport();
}

void MainWindow::action_show_jde_stats() {
    // Open the statistics dialog
    JDEStatsDialog statsDialog(this, settingsManager);
    statsDialog.exec();
}

/* ---------------------jDE ALGORITHM--------------------- */

void MainWindow::start_worker(){
    ui->label_livedata->setText("");
    ui->button_start->setText("Stop");
    settingsManager->recordAlgorithmRun();

    // Reset graph values before new instance starts
    mainFitnessGraph->setTargetEnergy(ui->edit_target->toPlainText().toDouble());
    mainFitnessGraph->newRunInstance();

    jdeWorker->setArguments(
        ui->edit_N->toPlainText().toInt(), ui->edit_seed->toPlainText().toInt(), -1,
        ui->edit_runtimelmt->toPlainText().toInt(), ui->edit_np->toPlainText().toInt(),
        ui->edit_target->toPlainText().toDouble(), ui->checkBox_opt
        );
    jdeWorkerThread->start();
}

// Received a new best energy (with timestamp) from the algorithm
void MainWindow::update_best_fitness(double bestFitness, double elapsed){
    ui->label_livedata->setText(
        QString::number(bestFitness) + " . . . . . . . (" + QString::number(elapsed) +
        + "s)\n" + ui->label_livedata->text()
    );

    // Update graph values
    mainFitnessGraph->updateGraph(bestFitness, elapsed);
}

// This function is called when the jde thread is finished
void MainWindow::display_algo_results(std::string results, double runtime){
    ui->label_end_results->setText("");
    ui->label_end_results->setText(QString::fromStdString(results));
    ui->button_start->setText("Start");

    jdeWorker = NULL; jdeWorkerThread = NULL;

    // Automatically export the algorithm output if needed
    if (settingsManager->automaticExportExist()){
        QMap preferenceMap = settingsManager->getAutomaticExportPreferences();
        QJsonObject arguments = exportManager->getJSONFromArguments(
            ui->edit_N->toPlainText().toInt(), ui->edit_seed->toPlainText().toInt(), -1,
            ui->edit_runtimelmt->toPlainText().toInt(), ui->edit_np->toPlainText().toInt(),
            ui->edit_target->toPlainText().toDouble(), (ui->checkBox_opt->checkState() == Qt::Checked)
        );

        exportManager->automaticJdeExport(
            settingsManager->getAutomaticExportFile(),
            preferenceMap[settingsManager->A_X_ARGUMENTS_KEY] ? arguments : QJsonObject(),
            preferenceMap[settingsManager->A_X_RESULTS_KEY] ? QString::fromStdString(results) : "",
            preferenceMap[settingsManager->A_X_TIMESTAMP_KEY] ? QDateTime::currentDateTime() : QDateTime());
    }

    // save runtime average of each result in settings
    settingsManager->recordAlgorithmStop();
    settingsManager->addToCumulativeRuntime(runtime);
}

bool MainWindow::is_jde_running(){
    return (jdeWorker || jdeWorkerThread);
}

bool MainWindow::is_input_valid() {
    // Get all fields to validate
    QString N = ui->edit_N->toPlainText();
    QString seed = ui->edit_seed->toPlainText();
    QString runtimeLmt = ui->edit_runtimelmt->toPlainText();
    QString np = ui->edit_np->toPlainText();
    QString target = ui->edit_target->toPlainText();

    if (N.isEmpty() || seed.isEmpty() || runtimeLmt.isEmpty() || np.isEmpty() || target.isEmpty()) {
        return false;
    }

    // Ensure all fields are valid numbers
    bool validN, validSeed, validRuntimeLmt, validNp, validTarget;

    N.toInt(&validN);
    seed.toInt(&validSeed);
    runtimeLmt.toInt(&validRuntimeLmt);
    np.toInt(&validNp);
    target.toDouble(&validTarget);

    if (!validN || !validSeed || !validRuntimeLmt || !validNp || !validTarget) {
        return false;
    }
    return true;
}


/* --------------------- OTHER (MIGHT DECOUPLE LATER)--------------------- */

void MainWindow::prompt_warning_message(std::string warningText, std::string infoText){
    QMessageBox msgBox;
    settingsManager->recordActionFailed();
    msgBox.setInformativeText(QString::fromStdString(infoText));
    msgBox.setText(QString::fromStdString(warningText));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}

void MainWindow::prompt_info_message(std::string infoText){
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(infoText));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::applyGraphSettings(){
    // Set graph preferences acquired from settings manager
    QMap preferenceMap = settingsManager->getGraphSettingsCheckbox();
    QVector<int> yRangeVec = settingsManager->getGraphSettingsYRange();
    int yLow = mainFitnessGraph->Y_RANGE_LOW_INIT, yHigh = mainFitnessGraph->Y_RANGE_HIGH_INIT;
    if (!yRangeVec.isEmpty()){
        yLow = yRangeVec[0];
        yHigh = yRangeVec[1];
    }
    mainFitnessGraph->setPreferences(
        preferenceMap[settingsManager->G_S_AXIS_TITLE_KEY],
        preferenceMap[settingsManager->G_S_DISPLAY_TARGET_KEY],
        yLow, yHigh, preferenceMap[settingsManager->G_S_CLEAR_LINE_KEY]
    );
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stackedWidget;
    delete jdeWorker;
    delete jdeWorkerThread;

    delete exportManager;
    delete settingsManager;
}


