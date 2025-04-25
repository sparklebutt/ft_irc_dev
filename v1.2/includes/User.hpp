#pragma once
#include <string>

// https://modern.ircdocs.horse/#client-to-server-protocol-structure
// Names of IRC entities (clients, servers, channels) are casemapped

class Server;

class User {
	private:
		int _fd;
		//char buffer[1024];// to recv() too
		//std::string _ip;
		std::string _Nickname;
		std::string _Fullname;
		int ping_sent; // std::chrono::steady_clock
		// pointer to current channel object ?
		// list of channels user is in 

		bool _acknowledged = false;
	public:
		User();
		User(int fd);
		~User();
		std::string receive_message(int fd);
		int getFd();

		bool get_acknowledged();
		void set_acknowledged();
};
