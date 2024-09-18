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

