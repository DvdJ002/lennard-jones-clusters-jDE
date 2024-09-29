#include "settingsmanager.h"

SettingsManager::SettingsManager() : settings(new QSettings("DvdJ002", "LennardJonesVisualization")) {
    initializeSettings();
}

SettingsManager::~SettingsManager() {
    delete settings;
}

void SettingsManager::initializeSettings(){
    if (!settings->contains(A_X_FILE_KEY)) {
        settings->setValue(A_X_FILE_KEY, "");
        settings->setValue(A_X_ARGUMENTS_KEY, false);
        settings->setValue(A_X_RESULTS_KEY, false);
        settings->setValue(A_X_TIMESTAMP_KEY, false);
    }
    if (!settings->contains(G_S_AXIS_TITLE_KEY)) {
        settings->setValue(G_S_AXIS_TITLE_KEY, false);
        settings->setValue(G_S_DISPLAY_TARGET_KEY, false);
        settings->setValue(G_S_Y_RANGE_KEY, "-29:1");
        settings->setValue(G_S_CLEAR_LINE_KEY, false);
    }
}

void SettingsManager::setValue(const QString &key, const QVariant &value) {
    settings->setValue(key, value);
}

// Default value is -1
QVariant SettingsManager::getValue(const QString &key) const {
    return settings->value(key, -1);
}

/*---------------- Automatic export specific functions ----------------*/

void SettingsManager::setAutomaticExportPreferences(
    bool saveArguments, bool saveResults, bool saveTimestamp)
{
    settings->setValue(A_X_ARGUMENTS_KEY, saveArguments);
    settings->setValue(A_X_RESULTS_KEY, saveResults);
    settings->setValue(A_X_TIMESTAMP_KEY, saveTimestamp);
}

// Returns map with format (SETTINGS_KEY, <bool value of preference>)
QMap<QString, bool> SettingsManager::getAutomaticExportPreferences(){
    QMap<QString, bool> map;
    map[A_X_ARGUMENTS_KEY] = settings->value(A_X_ARGUMENTS_KEY, -1).toBool();
    map[A_X_RESULTS_KEY] = settings->value(A_X_RESULTS_KEY, -1).toBool();
    map[A_X_TIMESTAMP_KEY] = settings->value(A_X_TIMESTAMP_KEY, -1).toBool();
    return map;
}

void SettingsManager::setAutomaticExportFile(const QString outputFile){
    settings->setValue(A_X_FILE_KEY, outputFile);
}

QString SettingsManager::getAutomaticExportFile(){
    return settings->value(A_X_FILE_KEY).toString();
}

bool SettingsManager::automaticExportExist(){
    return (settings->value(A_X_FILE_KEY).toString() != "");
}


/*---------------- Graph specific functions ----------------*/

void SettingsManager::setGraphSettings(
    const bool axisTitle, const bool displayTarget, const QString yRange, const bool clearLine)
{
    settings->setValue(G_S_AXIS_TITLE_KEY, axisTitle);
    settings->setValue(G_S_DISPLAY_TARGET_KEY, displayTarget);
    settings->setValue(G_S_Y_RANGE_KEY, yRange);
    settings->setValue(G_S_CLEAR_LINE_KEY, clearLine);
}

void SettingsManager::setGraphSettingsYRange(const QString yRange){
    settings->setValue(G_S_Y_RANGE_KEY, yRange);
}

QMap<QString, bool> SettingsManager::getGraphSettingsCheckbox(){
    QMap<QString, bool> map;
    map[G_S_AXIS_TITLE_KEY] = settings->value(G_S_AXIS_TITLE_KEY, -1).toBool();
    map[G_S_DISPLAY_TARGET_KEY] = settings->value(G_S_DISPLAY_TARGET_KEY, -1).toBool();
    map[G_S_CLEAR_LINE_KEY] = settings->value(G_S_CLEAR_LINE_KEY, -1).toBool();
    return map;
}

// Returns a vector with bottom and upper range, empty if unset setting
QVector<int> SettingsManager::getGraphSettingsYRange(){
    QStringList splits = settings->value(G_S_Y_RANGE_KEY, -1).toString().split(":");
    QVector<int> splitsVec(2);
    if (splits.size() == 2) {
        splitsVec[0] = splits[0].toInt();
        splitsVec[1] = splits[1].toInt();
    } else {
        return QVector<int>();
    }
    return splitsVec;
}


/*---------------- jDE Statistics setters ----------------*/

void SettingsManager::recordAlgorithmRun() {
    int count = settings->value(J_S_ALGORITHM_RUN_COUNT, 0).toInt();
    settings->setValue(J_S_ALGORITHM_RUN_COUNT, count + 1);
}
void SettingsManager::recordAlgorithmStop() {
    int count = settings->value(J_S_ALGORITHM_STOP_COUNT, 0).toInt();
    settings->setValue(J_S_ALGORITHM_STOP_COUNT, count + 1);
}
void SettingsManager::addToCumulativeRuntime(double runtime) {
    double cumulativeRuntime = settings->value(J_S_CUMULATIVE_RUNTIME, 0.0).toDouble();
    settings->setValue(J_S_CUMULATIVE_RUNTIME, cumulativeRuntime + runtime);
}
void SettingsManager::recordGraphImport() {
    int count = settings->value(J_S_GRAPH_IMPORT_COUNT, 0).toInt();
    settings->setValue(J_S_GRAPH_IMPORT_COUNT, count + 1);
}
void SettingsManager::recordGraphExport() {
    int count = settings->value(J_S_GRAPH_EXPORT_COUNT, 0).toInt();
    settings->setValue(J_S_GRAPH_EXPORT_COUNT, count + 1);
}
void SettingsManager::recordArgumentsImport() {
    int count = settings->value(J_S_ARGUMENTS_IMPORT_COUNT, 0).toInt();
    settings->setValue(J_S_ARGUMENTS_IMPORT_COUNT, count + 1);
}
void SettingsManager::recordArgumentsExport() {
    int count = settings->value(J_S_ARGUMENTS_EXPORT_COUNT, 0).toInt();
    settings->setValue(J_S_ARGUMENTS_EXPORT_COUNT, count + 1);
}
void SettingsManager::recordGraphSettingsSave() {
    int count = settings->value(J_S_GRAPH_SETTINGS_SAVE_COUNT, 0).toInt();
    settings->setValue(J_S_GRAPH_SETTINGS_SAVE_COUNT, count + 1);
}
void SettingsManager::resetAlgorithmSettings() {
    settings->setValue(J_S_ALGORITHM_RUN_COUNT, 0);
    settings->setValue(J_S_ALGORITHM_STOP_COUNT, 0);
    settings->setValue(J_S_CUMULATIVE_RUNTIME, 0);
    settings->setValue(J_S_GRAPH_IMPORT_COUNT, 0);
    settings->setValue(J_S_GRAPH_EXPORT_COUNT, 0);
    settings->setValue(J_S_ARGUMENTS_IMPORT_COUNT, 0);
    settings->setValue(J_S_ARGUMENTS_EXPORT_COUNT, 0);
    settings->setValue(J_S_GRAPH_SETTINGS_SAVE_COUNT, 0);
}



/*---------------- App Statistics setters ----------------*/

void SettingsManager::recordAppOpened() {
    int count = settings->value(A_S_APP_OPENED_COUNT, 0).toInt();
    settings->setValue(A_S_APP_OPENED_COUNT, count + 1);
}
void SettingsManager::recordPage1Opened(){
    int count = settings->value(A_S_PAGE1_OPENED_COUNT, 0).toInt();
    settings->setValue(A_S_PAGE1_OPENED_COUNT, count + 1);
}
void SettingsManager::recordPage2Opened(){
    int count = settings->value(A_S_PAGE2_OPENED_COUNT, 0).toInt();
    settings->setValue(A_S_PAGE2_OPENED_COUNT, count + 1);
}
void SettingsManager::recordPage3Opened(){
    int count = settings->value(A_S_PAGE3_OPENED_COUNT, 0).toInt();
    settings->setValue(A_S_PAGE3_OPENED_COUNT, count + 1);
}
void SettingsManager::recordLastUsage(QDateTime time) {
    settings->setValue(A_S_LAST_USAGE, time);
}
void SettingsManager::recordActionFailed() {
    int count = settings->value(A_S_ACTION_FAILED_COUNT, 0).toInt();
    settings->setValue(A_S_ACTION_FAILED_COUNT, count + 1);
}


/*---------------- jDE statistics getters ----------------*/

int SettingsManager::getAlgorithmRunCount() const {
    return settings->value(J_S_ALGORITHM_RUN_COUNT, 0).toInt();
}

int SettingsManager::getAlgorithmStopCount() const {
    return settings->value(J_S_ALGORITHM_STOP_COUNT, 0).toInt();
}

double SettingsManager::getAverageRuntime() const {
    int runCount = settings->value(J_S_ALGORITHM_RUN_COUNT, 0).toInt();
    if (runCount == 0) {
        return 0.0; // Avoid division by zero
    }
    double cumulativeRuntime = settings->value(J_S_CUMULATIVE_RUNTIME, 0.0).toDouble();
    return cumulativeRuntime / runCount;
}

int SettingsManager::getGraphImportCount() const {
    return settings->value(J_S_GRAPH_IMPORT_COUNT, 0).toInt();
}

int SettingsManager::getGraphExportCount() const {
    return settings->value(J_S_GRAPH_EXPORT_COUNT, 0).toInt();
}

int SettingsManager::getArgumentsImportCount() const {
    return settings->value(J_S_ARGUMENTS_IMPORT_COUNT, 0).toInt();
}

int SettingsManager::getArgumentsExportCount() const {
    return settings->value(J_S_ARGUMENTS_EXPORT_COUNT, 0).toInt();
}

int SettingsManager::getGraphSettingsSaveCount() const {
    return settings->value(J_S_GRAPH_SETTINGS_SAVE_COUNT, 0).toInt();
}


/*---------------- App statistics getters ----------------*/

int SettingsManager::getAppOpenCount() {
    return settings->value(A_S_APP_OPENED_COUNT, 0).toInt();
}
// Returns empty if doesn't exist
QDateTime SettingsManager::getLastUsage() {
    return settings->value(A_S_LAST_USAGE, QDateTime()).toDateTime();
}
int SettingsManager::getActionFailedCount() {
    return settings->value(A_S_ACTION_FAILED_COUNT, 0).toInt();
}

int SettingsManager::getMostFrequentlyUsedPage() {
    int page1Count = settings->value(A_S_PAGE1_OPENED_COUNT, 0).toInt();
    int page2Count = settings->value(A_S_PAGE2_OPENED_COUNT, 0).toInt();
    int page3Count = settings->value(A_S_PAGE3_OPENED_COUNT, 0).toInt();

    if (page1Count >= page2Count && page1Count >= page3Count) {
        return 1;
    } else if (page2Count >= page1Count && page2Count >= page3Count) {
        return 2;
    }
    return 3;
}






