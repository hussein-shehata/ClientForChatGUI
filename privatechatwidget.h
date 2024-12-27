#ifndef PRIVATECHATWIDGET_H
#define PRIVATECHATWIDGET_H

#include "BackEnd/ClientCode.hpp"
#include "winsock2.h"
#include <psdk_inc/_socket_types.h>
#include <QWidget>

namespace Ui {
class PrivateChatWidget;
}

class PrivateChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChatWidget(QWidget *parent = nullptr, SOCKET ClientSocket = -1, QString ReceivingName = "");
    ~PrivateChatWidget();

private slots:
    void on_SendPrivateMessageBtn_clicked();

private:
    Ui::PrivateChatWidget *ui;
    SOCKET ClientSocket;
    QString ReceivingName;
};

#endif // PRIVATECHATWIDGET_H
