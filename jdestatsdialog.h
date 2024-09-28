#ifndef JDESTATSDIALOG_H
#define JDESTATSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include "qlabel.h"
#include "settingsmanager.h"

class JDEStatsDialog : public QDialog {
    Q_OBJECT

public:
    JDEStatsDialog(QWidget *parent, SettingsManager *settManager)
        : QDialog(parent), settingsManager(settManager)
    {
        setWindowTitle("Algorithm Statistics");
        setUpLayout();
    }

    void setUpLayout(){
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        QFont generalFont;
        generalFont.setPointSize(FONT_SIZE_POINTS);  // Set your desired font size

        // Create horizontal layouts for each statistic
        QHBoxLayout *runLayout = new QHBoxLayout();
        QLabel *runLabel = new QLabel("Times ran", this);
        runLabel->setFont(generalFont);  // Apply the general font
        algorithmRunLabel = new QLabel(QString::number(settingsManager->getAlgorithmRunCount()), this);
        algorithmRunLabel->setFont(generalFont);  // Apply the general font
        algorithmRunLabel->setAlignment(Qt::AlignCenter);
        runLayout->addWidget(runLabel);
        runLayout->addWidget(algorithmRunLabel);
        mainLayout->addLayout(runLayout);

        QHBoxLayout *stopLayout = new QHBoxLayout();
        QLabel *stopLabel = new QLabel("Times stopped", this);
        stopLabel->setFont(generalFont);  // Apply the general font
        algorithmStopLabel = new QLabel(QString::number(settingsManager->getAlgorithmStopCount()), this);
        algorithmStopLabel->setFont(generalFont);  // Apply the general font
        algorithmStopLabel->setAlignment(Qt::AlignCenter);
        stopLayout->addWidget(stopLabel);
        stopLayout->addWidget(algorithmStopLabel);
        mainLayout->addLayout(stopLayout);

        QHBoxLayout *runtimeLayout = new QHBoxLayout();
        QLabel *runtimeLabel = new QLabel("Average runtime (s)", this);
        runtimeLabel->setFont(generalFont);  // Apply the general font
        averageRuntimeLabel = new QLabel(QString::number(settingsManager->getAverageRuntime()), this);
        averageRuntimeLabel->setFont(generalFont);  // Apply the general font
        averageRuntimeLabel->setAlignment(Qt::AlignCenter);
        runtimeLayout->addWidget(runtimeLabel);
        runtimeLayout->addWidget(averageRuntimeLabel);
        mainLayout->addLayout(runtimeLayout);

        QHBoxLayout *graphLayout = new QHBoxLayout();
        QLabel *graphLabel = new QLabel("Graph imported/exported", this);
        graphLabel->setFont(generalFont);  // Apply the general font
        QString graphInfo = QString::number(settingsManager->getGraphImportCount()) +
                            "/" + QString::number(settingsManager->getGraphExportCount());
        graphImportExportLabel = new QLabel(graphInfo, this);
        graphImportExportLabel->setFont(generalFont);  // Apply the general font
        graphImportExportLabel->setAlignment(Qt::AlignCenter);
        graphLayout->addWidget(graphLabel);
        graphLayout->addWidget(graphImportExportLabel);
        mainLayout->addLayout(graphLayout);

        QHBoxLayout *argsLayout = new QHBoxLayout();
        QLabel *argsLabel = new QLabel("Arguments imported/exported", this);
        argsLabel->setFont(generalFont);  // Apply the general font
        QString argsInfo = QString::number(settingsManager->getArgumentsImportCount())
                           + "/" + QString::number(settingsManager->getArgumentsExportCount());
        argumentsImportExportLabel = new QLabel(argsInfo, this);
        argumentsImportExportLabel->setFont(generalFont);  // Apply the general font
        argumentsImportExportLabel->setAlignment(Qt::AlignCenter);
        argsLayout->addWidget(argsLabel);
        argsLayout->addWidget(argumentsImportExportLabel);
        mainLayout->addLayout(argsLayout);

        QHBoxLayout *settingsLayout = new QHBoxLayout();
        QLabel *settingsLabel = new QLabel("Updated graph preferences", this);
        settingsLabel->setFont(generalFont);  // Apply the general font
        graphSettingsSaveLabel = new QLabel(QString::number(settingsManager->getGraphSettingsSaveCount()), this);
        graphSettingsSaveLabel->setFont(generalFont);  // Apply the general font
        graphSettingsSaveLabel->setAlignment(Qt::AlignCenter);
        settingsLayout->addWidget(settingsLabel);
        settingsLayout->addWidget(graphSettingsSaveLabel);
        mainLayout->addLayout(settingsLayout);

        setMinimumWidth(350);
        resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        setLayout(mainLayout);
    }


private:
    SettingsManager *settingsManager;

    QLabel *algorithmRunLabel;
    QLabel *algorithmStopLabel;
    QLabel *averageRuntimeLabel;
    QLabel *graphImportExportLabel;
    QLabel *argumentsImportExportLabel;
    QLabel *graphSettingsSaveLabel;

    // Size constants
    const int WINDOW_WIDTH = 375;
    const int WINDOW_HEIGHT = 160;
    const int FONT_SIZE_POINTS = 9;
};

#endif // JDESTATSDIALOG_H
