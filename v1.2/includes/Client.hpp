#pragma once
#include <string>
// https://modern.ircdocs.horse/#client-to-server-protocol-structure
// Names of IRC entities (clients, servers, channels) are casemapped
//#include "IrcMessage.hpp"
class Server;
class IrcMessage;

class Client {
	private:
		int _fd;
		int _timer_fd;
		int _failed_response_counter = 0;
		std::string _read_buff;
		std::string _send_buff;
 		//std::string _ip;
		std::string _nickName;
		std::string _ClientName;
		std::string _fullName;
		//IrcMessage _msg;
		//std::string _prefixes; // Client permissions 
		//int ping_sent; // std::chrono::steady_clock
		// pointer to current channel object ?
		// list of channels Client is in 

		bool _acknowledged = false;
	public:
		Client();
		Client(int fd, int timerfd);
		~Client();
		void receive_message(int fd, Server& server);
		int getFd();
		int get_failed_response_counter();
		void set_failed_response_counter(int count);
		bool get_acknowledged();
		void set_acknowledged();
		int get_timer_fd();
		std::string getNickname();
		std::string getClientName();
		std::string getfullName();
		void setDefaults(int num);

		void sendPing();
		void sendPong();

		bool change_nickname(std::string nickname, int fd);

		std::string getReadBuff();
		void setReadBuff(const std::string& buffer);
};
