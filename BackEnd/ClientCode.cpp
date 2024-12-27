//============================================================================
// Name        : Chat-Room.cpp
// Author      : Hussein
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include "stdafx.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <cstring>

#include <thread>

#include "ClientCode.hpp"
#include "ClientMessageClass.hpp"
using namespace std;

// static char ClientName [100]="";
ClientMessage ClientMessageToBeSent;
ClientMessage ClientMessageReceived;
int MaxLength = 52000;
bool FinishedReceivingNamesFromServer = true;

bool DisconnectedClient = false;

// #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void ClearAboveLine(void)
{
  cout << "\033[A";  // Move cursor up one line
  cout << "\033[2K"; // Clear the entire line
}


void SendNameToServer(SOCKET& ServerSocket, string ClientName)
{
	ClientMessageToBeSent.SetName(ClientName);
	ClientMessageToBeSent.SetNewNameFlag(true);
	char Buffer[260];
	int Length = ClientMessageToBeSent.Serialize(Buffer);

    int ByteCount = ::send(ServerSocket, Buffer, Length, 0);

	if(ByteCount > 0)
	{
//		cout<<"Sending Name Succeded"<<endl;
	}
	else
	{
		cout<<"Sending Name Failed"<<endl;
		WSACleanup();
	}
	ClientMessageToBeSent.SetNewNameFlag(false);
}
//TODO Change This function to receive the message to be sent and make that we sent to the server the name and the message as a class and make the message a string not a char array

void SendToServer(SOCKET ServerSocket, int MaxLength)
{
	while(1)
	{
		string Message ="";
		getline(cin, Message);
		SendToServerMessage(ServerSocket, MaxLength, Message);
		ClearAboveLine();
	}
}



void SendToServerMessage(SOCKET ServerSocket, int MaxLength, string Message)
{
    if(Message == "#Exit")
    {
        ClientMessageToBeSent.SetExitFlag(true);
    }
	ClientMessageToBeSent.SetMessage(Message);
    cout<<"Message to Be Sent"<<Message<<endl;
	char Buffer[52000];
	int Length = ClientMessageToBeSent.Serialize(Buffer);
	int ByteCount = send(ServerSocket, Buffer, Length, 0);
    if(Message == "#Exit")
    {
        ClientMessageToBeSent.SetExitFlag(false);
        // Do handshake to confirm that the server removed this client.
        // we will break from the below infinite loop when we receive "#Deleted" message from the server, we will parse it in "UpdateViewChat" method.
        while(true)
        {
            if(DisconnectedClient == true)
            {
                break;
            }
        }
    }

	if(ByteCount > 0)
	{
        // cout<<"Sent Successfully"<<endl;
	}
	else
	{
        // cout<<"Send Failed"<<endl;
        //NOTE will comment this for testing behavior
		// WSACleanup();
	}
}


void SentPrivateMessage(SOCKET ServerSocket, string Message, string ReceivingClientName)
{
  ClientMessageToBeSent.SetMessage(Message);
  ClientMessageToBeSent.SetPrivateMessageFlag(true);
  ClientMessageToBeSent.SetRecevingEndName(ReceivingClientName);
  char Buffer[52000];
  int Length = ClientMessageToBeSent.Serialize(Buffer);
  int ByteCount = send(ServerSocket, Buffer, Length, 0);

  if(ByteCount > 0)
    {
      //			  cout<<"Sent Successfully"<<endl;
    }
  else
    {
      cout<<"Send Private Message Failed"<<endl;
      WSACleanup();
    }
  ClientMessageToBeSent.SetPrivateMessageFlag(false);
}



ClientMessage ReceiveFromServer(SOCKET ServerSocket, int MaxLength, Flags& ReceivedFlags) //TODO we can make a parameter to receive the incoming string if we want to
{
  while(1)
    {
        char Buffer[MaxLength + 100];
        int ByteCount = recv(ServerSocket, Buffer, (MaxLength + 100), 0);
        // N.B : to handle that we are blocked here till we receive message from server, so we will send dummy message from server to unblock the above statement and receive all the users in the
        // function ReceiveMembersNamesFromServer

        if(ByteCount > 0)
        {
            ClientMessageReceived.Deserialize(Buffer);
            ReceivedFlags = ClientMessageReceived.GetFlags();
            return (ClientMessageReceived );
        }
        else
        {
            //	  cout<<"Receiving Failed"<<endl;
            WSACleanup();
        }

    }
}

vector<string> ReceiveMembersNamesFromServer(SOCKET ServerSocket)
{
    FinishedReceivingNamesFromServer = false;
    char Buffer[MaxLength];
    vector<string> Result;
    ClientMessageToBeSent.SetMessage("");
    ClientMessageToBeSent.SetRequestingMembersUpdate(true);
    int Length = ClientMessageToBeSent.Serialize(Buffer);
    int ByteCount = send(ServerSocket, Buffer, Length, 0);
    while(1)
    {
        int ByteCount = recv(ServerSocket, Buffer, MaxLength, 0);

        if(ByteCount > 0)
        {
            ClientMessageReceived.Deserialize(Buffer);
            if(ClientMessageReceived.GetClientMessage() == "EndOfClients")
            {
                ClientMessageToBeSent.SetRequestingMembersUpdate(false);
                FinishedReceivingNamesFromServer = true;
                return Result;
            }
            else if (ClientMessageReceived.GetRequestingMembersUpdate() == 1)
            {
                Result.push_back(ClientMessageReceived.GetName());
            }
            else
            {
              
            }
        }

    }
}



int inet_ptonnn(int af, const char *src, void *dst)
{
  // struct sockaddr_storage ss;
  // int size = sizeof(ss);
  // char src_copy[INET6_ADDRSTRLEN+1];

  // ZeroMemory(&ss, sizeof(ss));
  // /* stupid non-const API */
  // strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  // src_copy[INET6_ADDRSTRLEN] = 0;

  // if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
  //   switch(af) {
  //     case AF_INET:
  //   *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
  //   return 1;
  //     case AF_INET6:
  //   *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
  //   return 1;
  //   }
  // }
  // return 0;
}


void SetColor(int textColor)
{
    cout << "\033[" << textColor << "m";
}


void TestSerialization()
{
	string SentName = "Hussein";
	string SentMessage = "Hello my message is hello bs fe haga 8areba msh 3arf eh hia lsa bs hnshof";

	string ReceivedMessage = "";
	string ReceivedName = "";
	ClientMessageToBeSent.SetName(SentName);
	ClientMessageToBeSent.SetMessage(SentMessage);
	char Buffer[52000];
	int Length = ClientMessageToBeSent.Serialize(Buffer);

	cout<<"First Buffer is ::: ";
		for (int idx = 0; idx < Length;idx++)
		{
			cout<<Buffer[idx];
		}
		cout<<endl;

	const char* ReceivedBuffer = &Buffer[0];

	ClientMessageReceived.Deserialize(ReceivedBuffer);
	ReceivedMessage = ClientMessageReceived.GetClientMessage();
	ReceivedName = ClientMessageReceived.GetName();

	cout<<"Name is "<< ReceivedName <<endl<<"Message is "<<ReceivedMessage<<endl;
}

// int main()
// {
//   enableANSI(); // Enable ANSI escape codes on Windows
//   SOCKET 	clientSocket;
//   int port = 55555;
//   WSADATA wsaData;
//   int wsaerr;
//   WORD wVersionRequested = MAKEWORD(2,2);
//   wsaerr = WSAStartup(wVersionRequested, &wsaData);
//   if(wsaerr != 0)
//     {
//       cout<<"The Winsock dll not found"<<endl;
//       return 0;
//     }
//   else
//     {
//       cout<<"The Winsock dll found"<<endl;
//       cout<<"The status " << wsaData.szSystemStatus<<endl;
//     }



//   clientSocket = INVALID_SOCKET;
//   clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//   if(clientSocket == INVALID_SOCKET)
//     {
//       cout<<"Error at socket() : "<<WSAGetLastError()<<endl;
//       WSACleanup();
//       return 0;
//     }
//   else
//     {
//       cout<< "Socket is OK !"<<endl;
//     }


//   sockaddr_in clientservice;
//   clientservice.sin_family = AF_INET;
// //  inet_pton()
//   clientservice.sin_port = htons(port);
//   inet_ptonnn(AF_INET, ("127.0.0.1"), &clientservice.sin_addr); //TODO most likely this will cause issue

//   if( connect(clientSocket, (SOCKADDR*)&clientservice, sizeof(clientservice)) == SOCKET_ERROR)
//     {
//       cout<<"Client Connect () failed : " <<endl;
//       WSACleanup();
//       return 0;
//     }
//   else
//     {
//       cout<<"Client Connect () is OK!"<<endl;
//       cout<<"Client can send and receive now..."<<endl;
//     }

// //
//   SendNameToServer(clientSocket);

//   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//   SetConsoleTextAttribute(hConsole, 9);
//   thread Worker1(SendToServer, clientSocket, 300);
//   thread Worker2(ReceiveFromServer,clientSocket, 300);


//   Worker1.join();
//   Worker2.join();


//   system("pause");
//   WSACleanup();

//   return 0;
// }
