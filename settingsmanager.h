#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QString>
#include <QVariant>

class SettingsManager
{
public:
    SettingsManager();
    void initializeSettings();

    void setValue(const QString &key, const QVariant &value);
    QVariant getValue(const QString &key) const;

    // Automatic export specific functions
    void setAutomaticExportPreferences(const bool saveArguments, const bool saveResults, const bool saveTimestamp);
    QMap<QString, bool> getAutomaticExportPreferences();
    void setAutomaticExportFile(const QString outputFile);
    QString getAutomaticExportFile();
    bool automaticExportExist();

    // Graph settings specific functions
    void setGraphSettings(const bool darkMode, const bool displayTarget, const QString yRange, const bool clearLine);
    QMap<QString, bool> getGraphSettingsCheckbox();
    QVector<int> getGraphSettingsYRange();

    // Key constants
    const QString A_X_FILE_KEY = "a_x_output_file";
    const QString A_X_ARGUMENTS_KEY = "a_x_arguments_bool";
    const QString A_X_RESULTS_KEY = "a_x_results_bool";
    const QString A_X_TIMESTAMP_KEY = "a_x_timestamp_bool";

    const QString G_S_AXIS_TITLE_KEY = "a_x_axis_title";
    const QString G_S_DISPLAY_TARGET_KEY = "a_x_display_target";
    const QString G_S_Y_RANGE_KEY = "a_x_y_range";
    const QString G_S_CLEAR_LINE_KEY = "a_x_clear_line";

private:
    QSettings *settings;
};

#endif // SETTINGSMANAGER_H
