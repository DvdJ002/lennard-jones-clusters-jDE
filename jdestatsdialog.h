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

        QFont demiboldFont = generalFont;
        demiboldFont.setWeight(QFont::DemiBold);

        // Create horizontal layouts for each statistic
        QVBoxLayout *runLayout = new QVBoxLayout();
        QHBoxLayout *runInnerLayout = new QHBoxLayout();  // Inner layout to hold label and number

        QLabel *runLabel = new QLabel("Times ran", this);
        runLabel->setFont(generalFont);
        algorithmRunLabel = new QLabel(QString::number(settingsManager->getAlgorithmRunCount()), this);
        algorithmRunLabel->setFont(demiboldFont);
        algorithmRunLabel->setAlignment(Qt::AlignCenter);

        // Add the label and number into the inner layout
        runInnerLayout->addWidget(runLabel, 3);
        runInnerLayout->addWidget(algorithmRunLabel, 1);

        // Create horizontal line
        QFrame *horizontalLine1 = new QFrame(this);
        horizontalLine1->setFrameShape(QFrame::HLine);
        horizontalLine1->setFrameShadow(QFrame::Sunken);

        // Add the inner layout and horizontal line to the vertical layout
        runLayout->addLayout(runInnerLayout);
        runLayout->addWidget(horizontalLine1);  // Add the horizontal line after the inner layout
        mainLayout->addLayout(runLayout);


        // Create layout for "Times stopped"
        QVBoxLayout *stopLayout = new QVBoxLayout();
        QHBoxLayout *stopInnerLayout = new QHBoxLayout();

        QLabel *stopLabel = new QLabel("Times stopped", this);
        stopLabel->setFont(generalFont);
        algorithmStopLabel = new QLabel(QString::number(settingsManager->getAlgorithmStopCount()), this);
        algorithmStopLabel->setFont(demiboldFont);
        algorithmStopLabel->setAlignment(Qt::AlignCenter);

        stopInnerLayout->addWidget(stopLabel, 3);
        stopInnerLayout->addWidget(algorithmStopLabel, 1);

        // Create horizontal line for "Times stopped"
        QFrame *horizontalLine2 = new QFrame(this);
        horizontalLine2->setFrameShape(QFrame::HLine);
        horizontalLine2->setFrameShadow(QFrame::Sunken);

        stopLayout->addLayout(stopInnerLayout);
        stopLayout->addWidget(horizontalLine2);
        mainLayout->addLayout(stopLayout);

        // Avg runtime
        QVBoxLayout *runtimeLayout = new QVBoxLayout();
        QHBoxLayout *runtimeInnerLayout = new QHBoxLayout();

        QLabel *runtimeLabel = new QLabel("Average runtime (s)", this);
        runtimeLabel->setFont(generalFont);
        averageRuntimeLabel = new QLabel(QString::number(settingsManager->getAverageRuntime()), this);
        averageRuntimeLabel->setFont(demiboldFont);
        averageRuntimeLabel->setAlignment(Qt::AlignCenter);

        runtimeInnerLayout->addWidget(runtimeLabel, 3);
        runtimeInnerLayout->addWidget(averageRuntimeLabel, 1);

        QFrame *horizontalLine3 = new QFrame(this);
        horizontalLine3->setFrameShape(QFrame::HLine);
        horizontalLine3->setFrameShadow(QFrame::Sunken);

        runtimeLayout->addLayout(runtimeInnerLayout);
        runtimeLayout->addWidget(horizontalLine3);
        mainLayout->addLayout(runtimeLayout);

        // Graph imported/exported
        QVBoxLayout *graphLayout = new QVBoxLayout();   // Outer vertical layout
        QHBoxLayout *graphInnerLayout = new QHBoxLayout();  // Inner horizontal layout for label and number

        QLabel *graphLabel = new QLabel("Graph imported/exported", this);
        graphLabel->setFont(generalFont);  // Apply the general font
        QString graphInfo = QString::number(settingsManager->getGraphImportCount()) +
                            "/" + QString::number(settingsManager->getGraphExportCount());
        graphImportExportLabel = new QLabel(graphInfo, this);
        graphImportExportLabel->setFont(demiboldFont);  // Apply the general font
        graphImportExportLabel->setAlignment(Qt::AlignCenter);

        // Add label and number into the inner layout with stretch factors
        graphInnerLayout->addWidget(graphLabel, 3);  // Stretch factor 3 for the label
        graphInnerLayout->addWidget(graphImportExportLabel, 1);  // Stretch factor 1 for the info

        // Create horizontal line
        QFrame *horizontalLineGraph = new QFrame(this);
        horizontalLineGraph->setFrameShape(QFrame::HLine);
        horizontalLineGraph->setFrameShadow(QFrame::Sunken);

        // Add inner layout and horizontal line into outer layout
        graphLayout->addLayout(graphInnerLayout);
        graphLayout->addWidget(horizontalLineGraph);
        mainLayout->addLayout(graphLayout);


        // Arguments imported/exported
        QVBoxLayout *argsLayout = new QVBoxLayout();  // Outer vertical layout
        QHBoxLayout *argsInnerLayout = new QHBoxLayout();  // Inner horizontal layout for label and number

        QLabel *argsLabel = new QLabel("Arguments imported/exported", this);
        argsLabel->setFont(generalFont);  // Apply the general font
        QString argsInfo = QString::number(settingsManager->getArgumentsImportCount()) +
                           "/" + QString::number(settingsManager->getArgumentsExportCount());
        argumentsImportExportLabel = new QLabel(argsInfo, this);
        argumentsImportExportLabel->setFont(demiboldFont);  // Apply the general font
        argumentsImportExportLabel->setAlignment(Qt::AlignCenter);

        // Add label and number into the inner layout with stretch factors
        argsInnerLayout->addWidget(argsLabel, 3);  // Stretch factor 3 for the label
        argsInnerLayout->addWidget(argumentsImportExportLabel, 1);  // Stretch factor 1 for the info

        // Create horizontal line
        QFrame *horizontalLineArgs = new QFrame(this);
        horizontalLineArgs->setFrameShape(QFrame::HLine);
        horizontalLineArgs->setFrameShadow(QFrame::Sunken);

        // Add inner layout and horizontal line into outer layout
        argsLayout->addLayout(argsInnerLayout);
        argsLayout->addWidget(horizontalLineArgs);
        mainLayout->addLayout(argsLayout);


        // Updated graph preferences layout
        QVBoxLayout *settingsLayout = new QVBoxLayout();  // Outer vertical layout
        QHBoxLayout *settingsInnerLayout = new QHBoxLayout();  // Inner horizontal layout for label and number

        QLabel *settingsLabel = new QLabel("Updated graph preferences", this);
        settingsLabel->setFont(generalFont);  // Apply the general font
        graphSettingsSaveLabel = new QLabel(QString::number(settingsManager->getGraphSettingsSaveCount()), this);
        graphSettingsSaveLabel->setFont(demiboldFont);  // Apply the general font
        graphSettingsSaveLabel->setAlignment(Qt::AlignCenter);

        // Add label and number into the inner layout with stretch factors
        settingsInnerLayout->addWidget(settingsLabel, 3);  // Stretch factor 3 for the label
        settingsInnerLayout->addWidget(graphSettingsSaveLabel, 1);  // Stretch factor 1 for the info

        // Create horizontal line
        QFrame *horizontalLineSettings = new QFrame(this);
        horizontalLineSettings->setFrameShape(QFrame::HLine);
        horizontalLineSettings->setFrameShadow(QFrame::Sunken);

        // Add inner layout and horizontal line into outer layout
        settingsLayout->addLayout(settingsInnerLayout);
        settingsLayout->addWidget(horizontalLineSettings);
        mainLayout->addLayout(settingsLayout);

        // Button for resetting the settings
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        resetButton = new QPushButton(tr("Reset settings"), this);
        resetButton->setMinimumWidth(120);
        resetButton->setFocusPolicy(Qt::NoFocus);
        buttonLayout->addSpacerItem(
            new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum)
        );

        connect(resetButton, &QPushButton::clicked, this, &JDEStatsDialog::resetSettings);

        buttonLayout->addWidget(resetButton, 0, Qt::AlignRight);
        mainLayout->addLayout(buttonLayout);

        setMinimumWidth(375);

        resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        setLayout(mainLayout);
    }

    void resetAllLabels(){
        algorithmRunLabel->setText(QString::number(0));
        algorithmStopLabel->setText(QString::number(0));
        averageRuntimeLabel->setText(QString::number(0.0));
        graphImportExportLabel->setText(QString::number(0) + "/" + QString::number(0));
        argumentsImportExportLabel->setText(QString::number(0) + "/" + QString::number(0));
        algorithmRunLabel->setText(QString::number(0));
        graphSettingsSaveLabel->setText(QString::number(0));
    }

private slots:
    void resetSettings(){
        QMessageBox::StandardButton resetConfirmation;
        resetConfirmation = QMessageBox::question(
            nullptr, "Confirm Action", "Are you sure you want to reset jDE settings?",
            QMessageBox::Yes | QMessageBox::Cancel
        );

        // Check the user's response
        if (resetConfirmation == QMessageBox::Yes) {
            settingsManager->resetAlgorithmSettings();
            resetAllLabels();
            QMessageBox::information(nullptr, "Confirmed", "jDE settings were reset");
        }
    }

private:
    SettingsManager *settingsManager;

    QLabel *algorithmRunLabel;
    QLabel *algorithmStopLabel;
    QLabel *averageRuntimeLabel;
    QLabel *graphImportExportLabel;
    QLabel *argumentsImportExportLabel;
    QLabel *graphSettingsSaveLabel;
    QPushButton *resetButton;

    // Size constants
    const int WINDOW_WIDTH = 410;
    const int WINDOW_HEIGHT = 160;
    const int FONT_SIZE_POINTS = 9;
};

#endif // JDESTATSDIALOG_H
