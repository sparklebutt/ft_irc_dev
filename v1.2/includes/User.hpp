#pragma once
#include <string>

// https://modern.ircdocs.horse/#client-to-server-protocol-structure
// Names of IRC entities (clients, servers, channels) are casemapped

class Server;

class User {
	private:
		int _fd;
		int _timer_fd;
		int _failed_response_counter = 0;
		//char buffer[1024];// to recv() too
		//std::string _ip;
		std::string _nickName;
		std::string _userName;
		std::string _fullName;
		//std::string _prefixes; // user permissions 
		//int ping_sent; // std::chrono::steady_clock
		// pointer to current channel object ?
		// list of channels user is in 

		bool _acknowledged = false;
	public:
		User();
		User(int fd, int timer_fd);
		~User();
		std::string receive_message(int fd);
		int getFd();
		int get_failed_response_counter();
		void set_failed_response_counter(int count);
		bool get_acknowledged();
		void set_acknowledged();
		int get_timer_fd();
		std::string getNickname();
		std::string getuserName();
		std::string getfullName();
		void setDefaults(int num);

		void sendPing();
		void sendPong();

		bool change_nickname(std::sting nickname, _fd);
};
