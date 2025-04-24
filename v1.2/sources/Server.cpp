
#include "Server.hpp"
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
// SETTERS
void Server::setFd(int fd) { _fd = fd; }
void Server::set_signal_fd(int fd) { _signal_fd = fd; }
// note we may want to check here for values below 0
void Server::set_event_pollfd(int epollfd)  { _epoll_fd = epollfd; }
void Server::set_client_count(int val) { 
	_client_count += val;
}

// GETTERS
int Server::getFd() const { return _fd; }
int Server::get_signal_fd() const { return _signal_fd; }
int Server::get_client_count() const { return _client_count; }
int Server::getPort() const{ return _port; }
int Server::get_event_pollfd() const { return _epoll_fd; }

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

void Server::remove_user(int epollfd, int client_fd) {
	close(client_fd);
	epoll_ctl(epollfd, EPOLL_CTL_DEL, client_fd, 0);
	_users.erase(client_fd);
	_client_count--;
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

void Server::shutdown()
{
	// close all sockets 
	for (auto it = _users.begin(); it != _users.end(); ++it)
	{
		close(it->first);
	}
	// close server socket
	close(_fd);
	// close signal fd
	close(_signal_fd);
	// close epoll fd
	// close(_epoll_fd);
	// delete users
	for (auto it = _users.begin(); it != _users.end(); ++it)
	{
		it->second.reset();
	}
	_users.clear();
	// delete channels




	std::cout<<"server shutdown completed"<<std::endl;
	exit(0);
}
Server::~Server() {
	shutdown();
	// delete array/map of users
	// delete array/map of channels	
	/*deconstructor*/
}