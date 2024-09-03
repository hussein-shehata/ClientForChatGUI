#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InputDialog)
{
    ui->setupUi(this);
}

InputDialog::~InputDialog()
{
    delete ui;
}

QString InputDialog::GetClientName()
{
    return ClientName;
}

void InputDialog::on_OKButton_clicked()
{
    QString InputName = ui->NameTextEdit->text();

    if(InputName != "")
    {
        ClientName = InputName;
        qDebug() <<ClientName;
        accept();
    }
    else
    {
        reject();
    }
}


void InputDialog::on_CancelButton_clicked()
{
    reject();
}

