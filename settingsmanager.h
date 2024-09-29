#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <algorithm>

class SettingsManager
{
public:
    SettingsManager();
    ~SettingsManager();
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
    void setGraphSettingsYRange(const QString yRange);
    QMap<QString, bool> getGraphSettingsCheckbox();
    QVector<int> getGraphSettingsYRange();

    // jDE statistics specific functions
    void recordAlgorithmRun();
    void recordAlgorithmStop();
    void addToCumulativeRuntime(double runtime);
    void recordGraphImport();
    void recordGraphExport();
    void recordArgumentsImport();
    void recordArgumentsExport();
    void recordGraphSettingsSave();

    int getAlgorithmRunCount () const;
    int getAlgorithmStopCount() const;
    double getAverageRuntime() const;
    int getGraphImportCount() const;
    int getGraphExportCount() const;
    int getArgumentsImportCount () const;
    int getArgumentsExportCount() const;
    int getGraphSettingsSaveCount() const;

    // Warning: will reset all algorithm settings
    void resetAlgorithmSettings();

    // App statistics specific functions
    void recordAppOpened();
    void recordPage1Opened();
    void recordPage2Opened();
    void recordPage3Opened();
    void recordLastUsage(QDateTime time);
    void recordActionFailed();

    int getAppOpenCount();
    int getMostFrequentlyUsedPage();
    QDateTime getLastUsage();
    int getActionFailedCount();



    // Key constants
    const QString A_X_FILE_KEY = "a_x_output_file";
    const QString A_X_ARGUMENTS_KEY = "a_x_arguments_bool";
    const QString A_X_RESULTS_KEY = "a_x_results_bool";
    const QString A_X_TIMESTAMP_KEY = "a_x_timestamp_bool";

    const QString G_S_AXIS_TITLE_KEY = "a_x_axis_title";
    const QString G_S_DISPLAY_TARGET_KEY = "a_x_display_target";
    const QString G_S_Y_RANGE_KEY = "a_x_y_range";
    const QString G_S_CLEAR_LINE_KEY = "a_x_clear_line";

    const QString J_S_ALGORITHM_RUN_COUNT = "j_s_algorithm_runs";
    const QString J_S_ALGORITHM_STOP_COUNT = "j_s_algorithm_stops";
    const QString J_S_CUMULATIVE_RUNTIME = "j_s_cumulative_runtime";
    const QString J_S_GRAPH_IMPORT_COUNT = "j_s_graph_import";
    const QString J_S_GRAPH_EXPORT_COUNT = "j_s_graph_export";
    const QString J_S_ARGUMENTS_IMPORT_COUNT = "j_s_arguments_import";
    const QString J_S_ARGUMENTS_EXPORT_COUNT = "j_s_arguments_export";
    const QString J_S_GRAPH_SETTINGS_SAVE_COUNT = "j_s_graph_settings_save";
    const QString A_S_APP_OPENED_COUNT = "a_s_app_opened";
    const QString A_S_PAGE1_OPENED_COUNT = "a_s_page1_opened";
    const QString A_S_PAGE2_OPENED_COUNT = "a_s_page2_opened";
    const QString A_S_PAGE3_OPENED_COUNT = "a_s_page3_opened";
    const QString A_S_LAST_USAGE = "a_s_last_usage";
    const QString A_S_ACTION_FAILED_COUNT = "a_s_action_failed";

private:
    QSettings *settings;
};

#endif // SETTINGSMANAGER_H
