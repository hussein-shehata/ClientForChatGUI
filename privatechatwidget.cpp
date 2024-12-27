#include "privatechatwidget.h"
#include "ui_privatechatwidget.h"


PrivateChatWidget::PrivateChatWidget(QWidget *parent, SOCKET ClientSocket, QString ReceivingName)
    : QWidget(parent)
    , ui(new Ui::PrivateChatWidget)
    , ClientSocket(ClientSocket)
    , ReceivingName(ReceivingName)
{
    ui->setupUi(this);
}

PrivateChatWidget::~PrivateChatWidget()
{
    delete ui;
}

void PrivateChatWidget::on_SendPrivateMessageBtn_clicked()
{
    QString ClientMessage = ui->PrivateMessageLineEdit->text();
    SentPrivateMessage(ClientSocket,ClientMessage.toStdString(), ReceivingName.toStdString());
    ui->PrivateChat->append(ClientMessage);
    ui->PrivateMessageLineEdit->clear();
}

