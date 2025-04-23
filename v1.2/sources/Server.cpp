
#include "Server.hpp"
//#incude ""
#include <sys/types.h>
#include <unistd.h>
#include <string.h> //strlen
#include <iostream> // testing with cout
#include <sys/socket.h>
#include <sys/epoll.h>
#include "epoll_utils.hpp"
#include "User.hpp"
#include <map>
#include <memory> // shared pointers
Server::Server() {/*default contructor*/ }

Server::Server(int port , std::string password) {
	_port = port;
	_password = password;
}
//SETTERS

int Server::getFd() const {
	return _fd;
}

/**
 * @brief Here a client is accepted , error checked , socket is adusted for non-blocking
 * the client fd is added to the epoll and then added to the user map. a welcome message
 * is sent as an acknowlegement message back to irssi.
 */
void Server::create_user(int epollfd) {
 	// Handle new incoming connection
	int client_fd = accept(getFd(), nullptr, nullptr);
 	if (client_fd < 0) {
		perror("accept failed");
		// return ?
 	} else {
 		make_socket_unblocking(client_fd);
		setup_epoll(epollfd, client_fd, EPOLLIN);
		// create an instance of new user and add to server map
		_users[client_fd] = std::make_shared<User>(client_fd);
		std::cout<<"New user created , fd value is  == "<<_users[client_fd]->getFd()<<std::endl;
// WELCOME MESSAGE 
		if (!_users[client_fd]->get_acknowledged()) {
			// send message back so server dosnt think we are dead
			// this might typically be a welcome message 
			const char* ok_msg = ":server 001 OK\r\n";
			send(client_fd, ok_msg, strlen(ok_msg), 0);
			_users[client_fd]->set_acknowledged();
		}
	}
}

/*void Server::add_user(int client_fd, User user) {
	_users[client_fd] = std::make_shared<user>(client_fd);
}*/

void Server::setFd(int fd) {
	_fd = fd;
}

// getters
int Server::getPort() const{
	return _port;
}



/**
 * @brief to find the user object in the users array
 *  and return a pointer to it 
 * 
 * @param fd the active fd 
 * @return User* 
 */
std::shared_ptr<User> Server::get_user(int fd) {
	auto it = _users.find(fd);
	if (it != _users.end())
	{
		return it->second;
	}
	else
		return nullptr;
}

std::map<int, std::shared_ptr<User>> Server::get_map() {
	return _users;
}

std::string Server::get_password() const{
	return _password;
}
Server::~Server() { 
	// delete array/map of users
	// delete array/map of channels	
	/*deconstructor*/
}