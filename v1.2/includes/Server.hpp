#pragma once
#include <string>
#include <map>
#include <memory>

//#include "user.hpp" // can this be handled withoout including the whole hpp

// connection registration
// https://modern.ircdocs.horse/#connection-registration

/**
 * @brief The server class manages server related requests and 
 * redirects to client/user, message handling or channel handling when
 * required.
 * 
 * @note 1. utalizing a map of shared pointers in map of clients/users
 * allows for quick look up of clent and gives potenatial access to other objects
 * such as class for information lookup, such as permissions.
 * shared_pointers also are more memory safe than manual memory management 
 * and can simply be removed once not needed. 
 * 
 * @note 2. unordered map is a little faster if we choose to use that  
 *  
 */
class User;
class Server {
	private:
		int _port;
		// int _server_socket;
		int _fd;
		std::string _password;
		// num of clients 
		// int = fd of client client = client object
		std::map<int, std::shared_ptr<User>> _users; //unordered map?
		// num of channels 
		// string = name of channel channle = channel object
		// std::map<std::string, std::shared_ptr<Channel>> chanels
	public:
		Server();
		Server(int port, std::string password);
		~Server();

		// setters
		//void set_port(int const port);
		//void set_password(std::string const password);
		void create_user(int epollfd);
		void add_user(int client_fd, User user);
		// add channel
		// remove client
		// remove channel
		void setFd(int fd);
		//int getFd() const;
		// getters
		// find matching client to fd 
		// get client returns client
		// get channel
		// 
		int getPort() const;
		int getFd() const;
		std::shared_ptr<User> get_user(int fd);
		// read to private buffer returns string to parse. 
		// message handling 
		std::string get_password() const;
		//std::string recieve_message(); // send to message handling

};

