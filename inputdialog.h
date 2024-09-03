#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

     QString GetClientName(void);

private slots:
    void on_OKButton_clicked();

    void on_CancelButton_clicked();

private:
    QString ClientName;
    Ui::InputDialog *ui;
};

#endif // INPUTDIALOG_H
