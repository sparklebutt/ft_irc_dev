
#include "Server.hpp"
//#incude ""
#include <sys/types.h>
#include <unistd.h>

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
	//_users
	//empty array/map of user objects
	//empty array/map of channel objects
}
//SETTERS

int Server::getFd() const {
	return _fd;
}

void Server::create_user(int epollfd) {
 	// Handle new incoming connection
	int client_fd = accept(getFd(), nullptr, nullptr);
 	if (client_fd < 0) {
		perror("accept failed");
		// return ?
 	} else {
		// Add the client socket to epoll for monitoring
	 	// how to test the non blocking is working?
 		make_socket_unblocking(client_fd);
		setup_epoll(epollfd, client_fd, EPOLLIN);
		// create an instance of new user and add to server map
		_users[client_fd] = std::make_shared<User>(client_fd);
		std::cout<<"New user created , fd value is  == "<<_users[client_fd]->getFd()<<std::endl;

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


std::string Server::get_password() const{
	return _password;
}
Server::~Server() { 
	// delete array/map of users
	// delete array/map of channels	
	/*deconstructor*/
}