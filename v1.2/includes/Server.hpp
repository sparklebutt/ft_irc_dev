#pragma once
#include <string>
#include <map>
#include <memory>
#include "ServerError.hpp"
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
		int _client_count = 0;
		int _fd;
		int _current_client_in_progress;
		int _signal_fd;
		int _epoll_fd;
		std::string _password;
		std::map<int, std::shared_ptr<User>> _users; //unordered map?
		// loop through both to find when ping pong 
		// map client fd to sent ping time
		// map client fd to last sent message 

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
		void remove_user(int epollfd, int client_fd);
		// remove channel

		// SETTERS
		void setFd(int fd);
		void set_signal_fd(int fd);
		void set_client_count(int val);
		void set_event_pollfd(int epollfd);
		void set_current_client_in_progress(int fd);
		// get channel

		// GETTERS
		int getPort() const;
		int getFd() const;
		int get_signal_fd() const;
		int get_client_count() const;
		int get_event_pollfd() const;
		int get_current_client_in_progress() const;
		std::string get_password() const;

		// returns a user shared_pointer from the map
		std::shared_ptr<User> get_user(int fd);
		// returns the whole map 
		std::map<int, std::shared_ptr<User>>& get_map();
		// message handling
		void handle_client_connection_error(ErrorType err_type);
		void acknowladgeUser();
		void shutdown();

};

/**
 * @example template <bool ReadOnly>
typename std::conditional<ReadOnly, const std::map<int, std::shared_ptr<User>>&, std::map<int, std::shared_ptr<User>>&>::type 
Server::get_map() {
    return _users;
}
const std::map<int, std::shared_ptr<User>>& readonly_users = server.get_map<true>(); // Read only access
std::map<int, std::shared_ptr<User>>& modifiable_users = server.get_map<false>(); 

this is if we want to create a const return type so that accidental changes can not be made, it would be good practice to 
learn to do so 
 */