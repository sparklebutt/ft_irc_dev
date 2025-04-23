#include "User.hpp"
#include "Server.hpp"
#include <unistd.h>
#include <string.h>
#include<iostream>
#include <sys/socket.h>

User::User() {/*default constructor*/}

User::User(int fd) : _fd(fd) { }

User::~User() {/*default deconstructor*/}

int User::getFd() { return _fd; }

std::string User::receive_message(int fd) {
	char buffer[1024]; //buffer to read incoming message
	ssize_t bytes_read = 0; // bytes to read from socket  
	memset(buffer, 0, sizeof(buffer));	// set all values to 0

	bytes_read = recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT); // last flag makes recv non blocking 
	if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
	}
	else if (bytes_read < 0)
	{
		perror("recv gailed");
		return ""; // this should mayb change to a throw.
	}
	else if (bytes_read == 0)
	{
		std::cout << "client disconnected closing socket" <<std::endl;
		close(fd);
		return "";
	}
	return static_cast<std::string>(buffer);
}