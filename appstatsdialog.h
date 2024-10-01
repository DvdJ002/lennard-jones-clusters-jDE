#ifndef APPSTATSDIALOG_H
#define APPSTATSDIALOG_H

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

class AppStatsDialog : public QDialog {
    Q_OBJECT

public:
    AppStatsDialog(QWidget *parent, SettingsManager *settManager)
        : QDialog(parent), settingsManager(settManager)
    {
        setWindowTitle("Application Statistics");
        setUpLayout();
    }

    void setUpLayout(){
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        QFont generalFont;
        generalFont.setPointSize(FONT_SIZE_POINTS);  // Set your desired font size
        QFont demiboldFont = generalFont;
        demiboldFont.setWeight(QFont::DemiBold);

        // Layout for times opened app
        QVBoxLayout *timesOpenedLayout = new QVBoxLayout();
        QHBoxLayout *timesOpenedInnerLayout = new QHBoxLayout();  // Inner layout to hold label and number

        QLabel *runLabel = new QLabel("Times opened", this);
        runLabel->setFont(generalFont);
        timesOpenedLabel = new QLabel(QString::number(settingsManager->getAppOpenCount()), this);
        timesOpenedLabel->setFont(demiboldFont);
        timesOpenedLabel->setAlignment(Qt::AlignCenter);

        timesOpenedInnerLayout->addWidget(runLabel, 3);
        timesOpenedInnerLayout->addWidget(timesOpenedLabel, 1);

        QFrame *horizontalLine1 = new QFrame(this);
        horizontalLine1->setFrameShape(QFrame::HLine);
        horizontalLine1->setFrameShadow(QFrame::Sunken);

        timesOpenedLayout->addLayout(timesOpenedInnerLayout);
        timesOpenedLayout->addWidget(horizontalLine1);
        mainLayout->addLayout(timesOpenedLayout);


        // Layout for most frequent page
        QVBoxLayout *mostFrequentLayout = new QVBoxLayout();
        QHBoxLayout *mostFrequentInnerLayout = new QHBoxLayout();

        QLabel *frequentLabel = new QLabel("Most frequent page", this);
        frequentLabel->setFont(generalFont);
        int mostFrequentIndex = settingsManager->getMostFrequentlyUsedPage();
        QString mostFrequentStr = pageNames[mostFrequentIndex - 1]
        + " (Page " + QString::number(mostFrequentIndex) + ")";
        mostFrequentPageLabel = new QLabel(mostFrequentStr, this);
        mostFrequentPageLabel->setFont(demiboldFont);
        mostFrequentPageLabel->setAlignment(Qt::AlignCenter);

        mostFrequentInnerLayout->addWidget(frequentLabel, 3);
        mostFrequentInnerLayout->addWidget(mostFrequentPageLabel, 1);

        QFrame *horizontalLine2 = new QFrame(this);
        horizontalLine2->setFrameShape(QFrame::HLine);
        horizontalLine2->setFrameShadow(QFrame::Sunken);

        mostFrequentLayout->addLayout(mostFrequentInnerLayout);
        mostFrequentLayout->addWidget(horizontalLine2);
        mainLayout->addLayout(mostFrequentLayout);


        // Layout for last usage
        QVBoxLayout *lastUsageLayout = new QVBoxLayout();
        QHBoxLayout *lastUsageInnerLayout = new QHBoxLayout();

        QLabel *lastLabel = new QLabel("Most recent usage", this);
        lastLabel->setFont(generalFont);
        lastUsageLabel = new QLabel(
            settingsManager->getLastUsage().toString("yyyy-MM-dd HH:mm").toUtf8(), this
        );
        lastUsageLabel->setFont(demiboldFont);
        lastUsageLabel->setAlignment(Qt::AlignCenter);

        lastUsageInnerLayout->addWidget(lastLabel, 3);
        lastUsageInnerLayout->addWidget(lastUsageLabel, 1);

        QFrame *horizontalLine3 = new QFrame(this);
        horizontalLine3->setFrameShape(QFrame::HLine);
        horizontalLine3->setFrameShadow(QFrame::Sunken);

        lastUsageLayout->addLayout(lastUsageInnerLayout);
        lastUsageLayout->addWidget(horizontalLine3);
        mainLayout->addLayout(lastUsageLayout);


        // Action failed count
        QVBoxLayout *actionedFailedLayout = new QVBoxLayout();   // Outer vertical layout
        QHBoxLayout *actionedFailedInnerLayout = new QHBoxLayout();  // Inner horizontal layout for label and number

        QLabel *failedLabel = new QLabel("Action errors", this);
        failedLabel->setFont(generalFont);  // Apply the general font

        actionFailedCountLabel = new QLabel(QString::number(settingsManager->getActionFailedCount()), this);
        actionFailedCountLabel->setFont(demiboldFont);  // Apply the general font
        actionFailedCountLabel->setAlignment(Qt::AlignCenter);

        actionedFailedInnerLayout->addWidget(failedLabel, 3);  // Stretch factor 3 for the label
        actionedFailedInnerLayout->addWidget(actionFailedCountLabel, 1);  // Stretch factor 1 for the info

        QFrame *horizontalLineGraph = new QFrame(this);
        horizontalLineGraph->setFrameShape(QFrame::HLine);
        horizontalLineGraph->setFrameShadow(QFrame::Sunken);

        actionedFailedLayout->addLayout(actionedFailedInnerLayout);
        actionedFailedLayout->addWidget(horizontalLineGraph);

        mainLayout->addLayout(actionedFailedLayout);


        setMinimumWidth(375);
        resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        setLayout(mainLayout);
    }

private:
    SettingsManager *settingsManager;

    QLabel *timesOpenedLabel;
    QLabel *mostFrequentPageLabel;
    QLabel *lastUsageLabel;
    QLabel *actionFailedCountLabel;

    // Size constants
    const int WINDOW_WIDTH = 410;
    const int WINDOW_HEIGHT = 130;
    const int FONT_SIZE_POINTS = 9;

    QList<QString> pageNames = {"jDE Algorithm", "Multi-threaded", "About"};
};

#endif // APPSTATSDIALOG_H
