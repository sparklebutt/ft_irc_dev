//#include <map>
#include "Client.hpp"
#include "IrcResources.hpp"
#include <memory>
#include "IrcMessage.hpp"

#include <sys/socket.h>
#include <string>
#include <iostream>



void IrcMessage::dispatch_nickname(int client_fd, const std::string& oldnick, std::string newnick, std::map<int, std::shared_ptr <Client>>& clientsMap) {

	std::string cli = "client"; // username?
	std::string user_message = RPL_NICK_CHANGE(oldnick, cli, newnick);
	std::string broadcast_message= SERVER_MSG_NICK_CHANGE(oldnick, newnick);
	//int flag = 1; //does this need to change for any message??
	
	
	for (auto& pair : clientsMap) {
		std::cout<<"checking fd value = "<<pair.first<<"\n";
		if (pair.first == client_fd)
		{
			_messageQue.push_back(user_message);
			std::cout<<"user message going in = "<<getQueueMessage()<<"\n";
			//send(pair.first, user_message.c_str(), user_message.length(), 0);

		}
		else
		{
			pair.second->getMsg().queueMessage(broadcast_message); //_messageQue.push_back(broadcast_message);
			std::cout<<"broadcast message going in = "<<pair.second->getMsg().getQueueMessage()<<"\n";
			//send(pair.first, broadcast_message.c_str(), broadcast_message.length(), 0);

		}
		//setsockopt(pair.first, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag));
		        // Enable EPOLLOUT for clients that need to send data
	}
//	broadcast(client_fd, user_message, broadcast_message);
}

/*void broadcast(int client_fd, std::map<int, std::shared_ptr <Client>>& clientsMap) {
	int flag = 1; //does this need to change for any message??
	for (auto& pair : clientsMap) {
        // Use pair.first (client_fd) and pair.second (Client object) here
		std::cout<<"checking fd value = "<<pair.first<<"\n";

		if (pair.first == client_fd)
			send(pair.first, user_message.c_str(), user_message.length(), 0);
		else
			send(pair.first, broadcast_message.c_str(), broadcast_message.length(), 0);

		setsockopt(pair.first, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag));
	}
}*/