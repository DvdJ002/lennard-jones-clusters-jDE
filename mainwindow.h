#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "jdeworker.h"
#include "exportmanager.h"
#include "settingsmanager.h"
#include "fitnessgraph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Button slots
    void on_button_mainscreen_clicked();
    void on_button_screen2_clicked();
    void on_button_info_clicked();
    void on_button_start_clicked();
    void on_button_resetgraph_clicked();
    void on_button_savegraphsettings_clicked();
    void on_button_resetyrange_clicked();

    // Menu action slots
    void quit_app();
    void action_import_jde_arguments();
    void action_export_jde_arguments();
    void action_jde_automatic_export();
    void action_export_jde_graph();
    void action_import_jde_graph();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    ExportManager *exportManager;
    JDEWorker *jdeWorker = NULL; QThread* jdeWorkerThread = NULL;
    SettingsManager *settingsManager;
    FitnessGraph *mainFitnessGraph;


    void setup_ui_elements();
    void initialize_jde_worker();
    void start_worker();
    void update_best_fitness(double fitness, double elapsed);
    void display_algo_results(std::string results, double runtime);
    bool is_jde_running();
    bool is_input_valid();
    void prompt_warning_message(std::string infoText, std::string warningText);
    void prompt_info_message(std::string infoText);
    void applyGraphSettings();
};
#endif // MAINWINDOW_H
