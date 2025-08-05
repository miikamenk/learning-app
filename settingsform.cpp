#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    setWindowTitle("Settings");
}

SettingsForm::~SettingsForm()
{
    delete ui;
}
