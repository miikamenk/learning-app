#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QDialog {
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = nullptr);
    ~SettingsForm();

private:
    Ui::SettingsForm *ui;
};

#endif // SETTINGSFORM_H
