/*
 * ClientMessageClass.hpp
 *
 *  Created on: Sep 1, 2024
 *      Author: Hussein Shehata
 */

#ifndef CLIENTMESSAGECLASS_HPP_
#define CLIENTMESSAGECLASS_HPP_
#include <string>


using namespace std;

// TODO do other flags if needed
typedef struct
{
		unsigned char NewNameFlag : 1;
		unsigned char NewAvatarFlag : 1;
		unsigned char PrivateMessageFlag :1;
		unsigned char RequestingMembersUpdate :1;

}Flags;

class ClientMessage{
private:
	string Name;
	string Message;
	string RecevingEndName;
	// Flags to notify server which command is this message
	Flags MessageFlags;

public:
	ClientMessage();
	string GetName(void) const;
	string GetClientMessage(void) const;
	string GetRecevingEndName(void) const;

	void SetName(string NewName);
	void SetMessage(string NewMessage);
	void SetRecevingEndName(string Name);

	unsigned char GetNewNameFlag(void);
	void SetNewNameFlag(bool NewFlag);

	unsigned char GetPrivateMessageFlag(void);
	void SetPrivateMessageFlag(bool);

	unsigned char GetRequestingMembersUpdate(void);
	void SetRequestingMembersUpdate(bool);

	int Serialize(char* Buffer);
	void Deserialize(const char* Buffer);
	void SetFlags(Flags ReveivedMessageFlags);

};



#endif /* CLIENTMESSAGECLASS_HPP_ */
