
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
#include "config.h"
#include "ServerError.hpp"
//#include "SendException.hpp"
#include <algorithm> // find_if
#include <optional> // nullopt , signifies absence
class ServerException;

Server::Server() {/*default contructor*/ }

Server::Server(int port , std::string password) {
	_port = port;
	_password = password;
}
// ~~~SETTERS
void Server::setFd(int fd) { _fd = fd; }
void Server::set_signal_fd(int fd) { _signal_fd = fd; }
// note we may want to check here for values below 0
void Server::set_event_pollfd(int epollfd)  { _epoll_fd = epollfd; }
void Server::set_client_count(int val) {  _client_count += val; }
void Server::set_current_client_in_progress(int fd) { _current_client_in_progress = fd; }

// ~~~GETTERS
int Server::getFd() const { return _fd; }
int Server::get_signal_fd() const { return _signal_fd; }
int Server::get_client_count() const { return _client_count; }
int Server::getPort() const{ return _port; }
int Server::get_event_pollfd() const { return _epoll_fd; }
int Server::get_current_client_in_progress() const { return _current_client_in_progress; }

/**
 * @brief Here a client is accepted , error checked , socket is adusted for non-blocking
 * the client fd is added to the epoll and then added to the user map. a welcome message
 * is sent as an acknowlegement message back to irssi.
 */
void Server::create_user(int epollfd) {
 	// Handle new incoming connection
	int client_fd = accept(getFd(), nullptr, nullptr);
 	if (client_fd < 0) {
		throw ServerException(ErrorType::ACCEPT_FAILURE, "debuggin: create user");
	} /*if (errno == EINTR) {
		std::cerr << "accept() interrupted by signal, retrying..." << std::endl;
	} else if (errno == EMFILE || errno == ENFILE) {
		std::cerr << "Too many open files—server may need tuning!" << std::endl;
 	}*/ else {
 		make_socket_unblocking(client_fd);
		setup_epoll(epollfd, client_fd, EPOLLIN);
		int timer_fd = setup_epoll_timer(epollfd, config::TIMEOUT_CLIENT);
		// create an instance of new user and add to server map
		_users[client_fd] = {std::make_shared<User>(client_fd, timer_fd), timer_fd};
		std::cout<<"New user created , fd value is  == "<<_users[client_fd].first->getFd()<<std::endl;
// WELCOME MESSAGE
		set_current_client_in_progress(client_fd);
		if (!_users[client_fd].first->get_acknowledged()) {
			// send message back so server dosnt think we are dead
			// this might typically be a welcome message
			send(client_fd, IRCMessage::welcome_msg, strlen(IRCMessage::welcome_msg), 0);
			_users[client_fd].first->set_acknowledged();
		}
		set_client_count(1);
		_users[client_fd].first->setDefaults(get_client_count());
		//std::cout<<<<std::endl;
	}
}

void Server::remove_user(int epollfd, int client_fd) {
	close(client_fd);
	epoll_ctl(epollfd, EPOLL_CTL_DEL, client_fd, 0);
	close(get_user(client_fd)->get_timer_fd());
	epoll_ctl(epollfd, EPOLL_CTL_DEL, get_user(client_fd)->get_timer_fd(), 0);
	_users.erase(client_fd);
	_client_count--;
	std::cout<<"client has been removed"<<std::endl;
}


/**
 * @brief to find the user object in the users array
 *  and return a pointer to it
 *
 * @param fd the active fd
 * @return User* , shared pointers are a refrence themselves
 */
std::shared_ptr<User> Server::get_user(int fd) {
	for (std::map<int, std::pair<std::shared_ptr<User>, int>>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first == fd)
			return it->second.first;
	}
	throw ServerException(ErrorType::NO_USER_INMAP, "can not get_user()");
}

std::map<int, std::pair<std::shared_ptr<User>, int>>& Server::get_map() {
	return _users;
}

std::string Server::get_password() const{
	return _password;
}

// ERROR HANDLING INSIDE LOOP
void Server::handle_client_connection_error(ErrorType err_type)
{
	switch (err_type)
	{
		case ErrorType::ACCEPT_FAILURE:
			break;
		case ErrorType::EPOLL_FAILURE_1: {
			send(_current_client_in_progress, IRCMessage::error_msg, strlen(IRCMessage::error_msg), 0);
			close(_current_client_in_progress);
			_current_client_in_progress = 0;
			break;
		} case ErrorType::SOCKET_FAILURE: {
			send(_current_client_in_progress, IRCMessage::error_msg, strlen(IRCMessage::error_msg), 0);
			close(_current_client_in_progress);
			_current_client_in_progress = 0;
			break;
		} default:
			std::cerr << "Unknown error occurred" << std::endl;
			break;
	}
}
void Server::shutdown()
{
	// close all sockets
	for (std::map<int, std::pair<std::shared_ptr<User>, int>>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		close(it->first);
		close(it->second.second); // it->second refers to the pair , and second second refers to the second memebre of the pair
	}
	// close server socket
	close(_fd);
	// close signal fd
	//close(_signal_fd);
	// close epoll fd
	// close(_epoll_fd);
	// delete users
	for (std::map<int, std::pair<std::shared_ptr<User>, int>>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		it->second.first.reset();
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

void Server::checkTimers(int fd)
{
	// Using a lambda function to look for corresponding timer_fd
	auto it = std::find_if(_users.begin(), _users.end(), 
	[fd](const auto& pair) { return pair.second.second == fd; } );

	if (it == _users.end())
		return;
	if (it != _users.end()) {
		if (it->second.first->get_failed_response_counter() == 3)
		{
			remove_user(_epoll_fd, it->first);
			return ;
			//it->second.first
		}
		std::cout<< "should be sending  ping onwards "<<std::endl;
		it->second.first->sendPing();
		it->second.first->set_failed_response_counter(1);
		resetClientTimer(it->second.second, config::TIMEOUT_CLIENT);
	}
}
