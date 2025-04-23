#include "User.hpp"
#include "Server.hpp"
#include <unistd.h>
#include <string.h>
#include<iostream>
#include <sys/socket.h>
#include "config.h"

User::User() {/*default constructor*/}

User::User(int fd) : _fd(fd) { }

User::~User() {/*default deconstructor*/}

int User::getFd() { return _fd; }

bool User::get_acknowledged() {
	return _acknowledged;
}

void User::set_acknowledged() {
	_acknowledged = true;
}

/**
 * @brief Reads using recv() to a char buffer as data recieved from the socket
 * comes in as raw bytes, std::string does not handle this kind of raw data very well,
 * string can also cause un predictable behaviour due to null terminator.
 * 
 * if bytes read is 0 client is is suspected to be disconnected and related cleanup
 * should follow
 * 
 * @return FAIL an empty string or throw 
 * SUCCESS the char buffer converted to std::string
 */
std::string User::receive_message(int fd) {
	char buffer[config::BUFFER_SIZE];
	ssize_t bytes_read = 0;
	memset(buffer, 0, sizeof(buffer));

	bytes_read = recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT); // last flag makes recv non blocking 
	if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
	}
	else if (bytes_read < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
//			std::cout<<"no data to  handle in message receive, skipping"<<std::endl;
			return "";
		}
		perror("recv gailed");
		// throw error
		return ""; // this should mayb change to a throw.
	}
	else if (bytes_read == 0) {
		std::cout << "client disconnected closing socket" <<std::endl;
		close(fd);
		return ""; // this should be handled with a disconnected function + cleanup.
	}
	return std::string(buffer);
}