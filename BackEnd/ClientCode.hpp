/*
 * ClientCode.hpp
 *
 *  Created on: Sep 1, 2024
 *      Author: Hussein Shehata
 */

#ifndef CLIENTCODE_HPP_
#define CLIENTCODE_HPP_

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <cstring>
#include "string"
#include <vector>
#include <thread>

#include "ClientMessageClass.hpp"
using namespace std;
extern bool DisconnectedClient;

void enableANSI();
void ClearAboveLine(void);
void SendNameToServer(SOCKET& ServerSocket, string ClientName);
void SendToServer(SOCKET ServerSocket, int MaxLength);
void SentPrivateMessage(SOCKET ServerSocket, string Message, string ReceivingClientName);
ClientMessage ReceiveFromServer(SOCKET ServerSocket, int MaxLength, Flags& ReceivedFlags);
int inet_ptonnn(int af, const char *src, void *dst);
void SetColor(int textColor);
int Run();

void SendToServerMessage(SOCKET ServerSocket, int MaxLength, string Message);

vector<string> ReceiveMembersNamesFromServer(SOCKET ServerSocket);


#endif /* CLIENTCODE_HPP_ */
