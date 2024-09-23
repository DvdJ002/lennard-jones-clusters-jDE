#include "settingsmanager.h"

SettingsManager::SettingsManager() : settings(new QSettings("DvdJ002", "LennardJonesVisualization")) {
    initializeSettings();
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





