#include "mainapplication.h"
#include "ui_mainapplication.h"
#include "inputdialog.h"
#include "BackEnd/ClientCode.hpp"
#include "winsock2.h"
#include <psdk_inc/_socket_types.h>
#include "privatechatwidget.h"
#include <thread>

MainApplication::MainApplication(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainApplication)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    ui->ServerChatView->setModel(model);

    Worker1 = new std::thread(this->UpdateViewChat,this);
}

MainApplication::~MainApplication()
{
    EndConnection();
    delete Worker1;
    delete ui;
}

void MainApplication::EndConnection()
{
    if(DisconnectedClient == true)
    {
        // Arleady Disconnected
        cout<<"Arleady Disconnected"<<endl;
        return;
    }
    QString ClientMessage = "#Exit";
    SendToServerMessage(ClientSocket, 52000, ClientMessage.toStdString());
    closesocket(ClientSocket);
    // Cleanup Winsock
    WSACleanup();
    cout<<"We Ended Connection"<<endl;
}

int MainApplication::StartConnection(SOCKET& clientSocket)
{
    int port = 55555;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if(wsaerr != 0)
    {
        cout<<"The Winsock dll not found"<<endl;
        return -1;
    }
    else
    {

    }



    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket == INVALID_SOCKET)
    {
        cout<<"Error at socket() : "<<WSAGetLastError()<<endl;
        WSACleanup();
        return -1;
    }
    else
    {

    }


    sockaddr_in clientservice;
    clientservice.sin_family = AF_INET;
    //  inet_pton()
    clientservice.sin_port = htons(port);
    inet_pton(AF_INET, ("127.0.0.1"), &clientservice.sin_addr); //TODO most likely this will cause issue

    if( ::connect(clientSocket, (SOCKADDR*)&clientservice, sizeof(clientservice)) == SOCKET_ERROR)
    {
        cout<<"Client Connect () failed : " <<endl;
        WSACleanup();
        return -1;
    }
    else
    {
        DisconnectedClient = false;
        return 1;
    }
}

bool MainApplication::OpenCredentialsDialog()
{
    InputDialog InputCredentialsDialog;
    auto ret = InputCredentialsDialog.exec();

    if(ret == QDialog::Accepted)
    {
        ClientName = InputCredentialsDialog.GetClientName();
        return true;
    }
    else
    {
        return false;
    }

}

void MainApplication:: on_ConnectButton_clicked()
{
    int ret = StartConnection(ClientSocket);
    if(ret == -1)
    {
        //Failed connection
        //TODO handle it later
    }
    else
    {
        bool Result = OpenCredentialsDialog();
        if(Result == true)
        {
            SendNameToServer(ClientSocket, ClientName.toStdString());
            // Make the client able to send messages
            ui->SendButton->setEnabled(true);
            ui->StatusLabel->setText("Connected.");
            ClientIsConnected = true;
        }
        else
        {
            // User did not enter his Credentials so wont open send to him till he enters his name
            ui->LabelToUser->setText("You have to enter your credentials to be able to send and receive.");
        }
    }
}


void MainApplication::on_SendButton_clicked()
{
    QString ClientMessage = ui->ClientMessageEditBox->text();
    SendToServerMessage(ClientSocket, 52000, ClientMessage.toStdString());
    ui->ClientMessageEditBox->clear();
}


void MainApplication::UpdateViewChat()
{
    Flags ReceivedFlags ={.NewNameFlag = 0,
                            .NewAvatarFlag = 0,
                            .PrivateMessageFlag = 0,
                            .RequestingMembersUpdate = 0,
                            .NotifyingNewMemberFlag = 0,
                            .ExitFlag = 0};
    while(1)
    {
        if(ClientIsConnected == false)
        {
            continue;
        }
        ClientMessage MessageToBeDisplayed = (ReceiveFromServer(ClientSocket,52000, ReceivedFlags)) ;
        if(MessageToBeDisplayed.GetClientMessage() == "#Deleted")
        {
            DisconnectedClient = true;
            // Cleanup Winsock
            break;
        }
        if(ReceivedFlags.NotifyingNewMemberFlag == 1)
        {
            UpdateMemberList();
        }
        if(ReceivedFlags.PrivateMessageFlag == 1)
        {
            // Display the private message in the tab for the sender
            UpdatePrivateChats(MessageToBeDisplayed);
        }
        string ParsedMessage = MessageToBeDisplayed.GetName() + " : " + MessageToBeDisplayed.GetClientMessage();
        AddStringToViewChat(QString::fromStdString(ParsedMessage));
    }
}

void MainApplication::UpdatePrivateChats(const ClientMessage& MessageToBeDisplayed)
{
    // TODO Make a new tab if the sender name is not in the arleady opened tabs
}

void MainApplication::UpdateMemberList()
{
    vector<string> Result;
    Result = ReceiveMembersNamesFromServer(ClientSocket);
    ui->ServerMembersList->clear();
    for (auto ClientName: Result)
    {
        ui->ServerMembersList->addItem(QString::fromStdString(ClientName));
    }
}

void MainApplication::AddStringToViewChat(const QString& Message)
{
    QStringList currentList = model->stringList();
    currentList.append(Message);
    model->setStringList(currentList);
}



void MainApplication::on_SendPrivateMessageButton_clicked()
{

    ui->ServerMembersList->currentItem();
    QString ReceivingEndName = ui->ServerMembersList->currentItem()->text();
    // SentPrivateMessage(ClientSocket,"Hello this is private",ReceivingEndName);
    ui->tabWidget->insertTab(ui->tabWidget->count(), new PrivateChatWidget(ui->tabWidget, ClientSocket, ReceivingEndName), QIcon(QString("")) ,ReceivingEndName);

}


void MainApplication::on_pushButton_clicked()
{
    EndConnection();
}

