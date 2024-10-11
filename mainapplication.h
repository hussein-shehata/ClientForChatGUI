#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QWidget>
#include <QStringListModel>
#include "winsock2.h"
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainApplication;
}
QT_END_NAMESPACE

class MainApplication : public QWidget
{
    Q_OBJECT

public:
    MainApplication(QWidget *parent = nullptr);
    ~MainApplication();

    bool OpenCredentialsDialog();

    SOCKET ClientSocket;
    QString ClientName;
    QString MessageToBeSent;
    QString MessageToBeDisplayed;
    void UpdateViewChat();

private slots:
    void on_ConnectButton_clicked();

    void on_SendButton_clicked();

    void on_SendPrivateMessageButton_clicked();

    void on_pushButton_clicked();

private:
    int StartConnection(SOCKET& ClientSocket);
    int EndConnection();
    void AddStringToViewChat(const QString& Message);
    void UpdateMemberList();
    bool ClientIsConnected = false;
    std::thread* Worker1;

    Ui::MainApplication *ui;
    QStringListModel *model;

};
#endif // MAINAPPLICATION_H
