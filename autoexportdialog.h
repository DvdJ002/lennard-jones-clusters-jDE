#ifndef AUTOEXPORTDIALOG_H
#define AUTOEXPORTDIALOG_H

#include "settingsmanager.h"

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

class AutoExportDialog : public QDialog {
    Q_OBJECT

public:
    AutoExportDialog(QWidget *parent, SettingsManager *settManager)
        : QDialog(parent), settingsManager(settManager)
    {
        // Create widgets
        fileLineEdit = new QLineEdit(this);
        QPushButton *fileButton = new QPushButton(tr("Select the output file"), this);

        checkBoxArgs = new QCheckBox(tr("Include arguments"), this);
        checkBoxResults = new QCheckBox(tr("Include results"), this);
        checkBoxTime = new QCheckBox(tr("Include timestamp"), this);

        QPushButton *okButton = new QPushButton(tr("OK"), this);
        QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);

        // Layout setup
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // Add file selection widgets
        QHBoxLayout *fileLayout = new QHBoxLayout;
        fileLayout->addWidget(fileLineEdit);
        fileLayout->addWidget(fileButton);
        mainLayout->addLayout(fileLayout);

        // Add checkboxes
        mainLayout->addWidget(checkBoxArgs);
        mainLayout->addWidget(checkBoxResults);
        mainLayout->addWidget(checkBoxTime);

        // Add buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        resize(WINDOW_WIDTH, WINDOW_HEIGHT);

        // Connect buttons to slots
        connect(fileButton, &QPushButton::clicked, this, &AutoExportDialog::chooseFile);
        connect(okButton, &QPushButton::clicked, this, &AutoExportDialog::saveSettings);
        connect(cancelButton, &QPushButton::clicked, this, &AutoExportDialog::reject);

        // Load the settings to pre-fill values
        fileLineEdit->setText(settingsManager->getAutomaticExportFile());
        QMap preferenceMap = settingsManager->getAutomaticExportPreferences();
        checkBoxArgs->setChecked(preferenceMap[settingsManager->A_X_ARGUMENTS_KEY]);
        checkBoxResults->setChecked(preferenceMap[settingsManager->A_X_RESULTS_KEY]);
        checkBoxTime->setChecked(preferenceMap[settingsManager->A_X_TIMESTAMP_KEY]);
    }

private slots:
    void chooseFile() {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Select file"));
        if (!filePath.isEmpty()) {
            fileLineEdit->setText(filePath);
        }
    }

    void saveSettings() {
        if (!fileLineEdit->text().isEmpty()) {
            QFileInfo fileInfo(fileLineEdit->text());
            if (!fileInfo.exists() || !fileInfo.isFile()) {
                QMessageBox::warning(this, tr("Invalid File"), tr("Invalid file path!"));
                return;
            }
        }

        settingsManager->setAutomaticExportFile(fileLineEdit->text());
        settingsManager->setAutomaticExportPreferences(
            checkBoxArgs->isChecked(), checkBoxResults->isChecked(), checkBoxTime->isChecked()
        );

        accept();  // Close the dialog after saving
    }

private:
    SettingsManager *settingsManager;
    QLineEdit *fileLineEdit;
    QCheckBox *checkBoxArgs;
    QCheckBox *checkBoxResults;
    QCheckBox *checkBoxTime;

    // Size constants
    const int WINDOW_WIDTH = 320;
    const int WINDOW_HEIGHT = 160;
};



#endif // AUTOEXPORTDIALOG_H
