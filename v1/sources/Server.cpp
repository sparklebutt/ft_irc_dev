
#include "Server.hpp"

Server::Server() {/*default contructor*/ }

Server::Server(int port , std::string password)
{
	// parameterized constructor 
	_port = port;
	_password = password; 
	//_fd = server_fd;
}
//SETTERS

void Server::setFd(int fd){
	_fd = fd;
}
/*void set_port(int const port);
void set_password(std::string const password);*/

// getters
int Server::getPort() const{
	return _port;
}

std::string Server::get_password() const{
	return _password;
}
Server::~Server() { /*deconstructor*/ }