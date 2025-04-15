
#include "Server.hpp"

Server::Server() {/*default contructor*/ }

Server::Server(int port , std::string password) {
	_port = port;
	_password = password; 
}
//SETTERS

void Server::setFd(int fd){
	_fd = fd;
}

// getters
int Server::getPort() const{
	return _port;
}

int Server::getFd() const {
	return _fd;
}
std::string Server::get_password() const{
	return _password;
}
Server::~Server() { /*deconstructor*/ }