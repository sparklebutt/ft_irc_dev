#include "Client.hpp"
#include "Server.hpp"
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
//#include "config.h"
#include "ServerError.hpp"
#include "SendException.hpp"
#include "epoll_utils.hpp" // reset client timer
#include "IrcMessage.hpp"

Client::Client(){}

Client::Client(int fd, int timer_fd) : _fd(fd), _timer_fd(timer_fd){}

Client::~Client(){}

int Client::getFd(){
	return _fd;
}

bool Client::get_acknowledged(){
	return _acknowledged;
}

int Client::get_failed_response_counter(){
	return _failed_response_counter;
}

void Client::set_failed_response_counter(int count){
	std::cout<<"failed response counter is "
	<< _failed_response_counter << "new value to be added "
	<< count <<std::endl;
	if ( count < 0 && _failed_response_counter == 0)
		return ;	
	_failed_response_counter += count;
}

int Client::get_timer_fd(){
	return _timer_fd;
}

std::string Client::getNickname(){
	return _nickName;
}

std::string Client::getClientName(){
	return _ClientName;
}

std::string Client::getfullName(){
	return _fullName;
}

std::string Client::getReadBuff() {
	return _read_buff;

}

void Client::setReadBuff(const std::string& buffer) {
	_read_buff += buffer;
}

void Client::set_acknowledged(){
	_acknowledged = true;
}

/**
 * @brief Reads using recv() to a char buffer as data recieved from the socket
 * comes in as raw bytes, std		void sendPing();
 * @return FAIL an empty string or throw 
 * SUCCESS the char buffer converted to std::string
 */
void Client::receive_message(int fd, Server& server) {
	char buffer[config::BUFFER_SIZE];
	ssize_t bytes_read = 0;
	memset(buffer, 0, sizeof(buffer));
	IrcMessage msg;
	bytes_read = recv(fd, buffer, sizeof(buffer) , MSG_DONTWAIT); // last flag makes recv non blocking 
	if (bytes_read < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			std::cout<<"no data to  handle in message receive, skipping"<<std::endl;
			return ;
		}
		perror("recv gailed");
		// throw error
		return ;
	}
	if (bytes_read == 0) {
		std::cout<<"cntrl d maybe ???????????????"<<std::endl;
		//std::cout << "client disconnected closing socket" <<std::endl;
		throw ServerException(ErrorType::CLIENT_DISCONNECTED, "");
		//close(fd);
	}
	/*else if (test.find('\n') == std::string::npos)
	{
		// here we handle cntrl d buffering to string , using netcat
		// caution as some messages
		std::cout<<"NEW LINE ENDED THE STRING CNTLR D???"<<std::endl;
	}*/
	//if (buffer[bytes_read] == '\0')
//		std::cout<<"this was a nul terminated string "<<std::endl;

	// SUCCESS IN READ BUILD CLIENT BUFFER
	else if (bytes_read > 0) {
		//dont know yet how to deal with this part 
		/*if (test.find("PONG")) {
			std::cout<<" PONG recived server_ping_count = "<<get_failed_response_counter()<<std::endl;;
			resetClientTimer(_timer_fd, config::TIMEOUT_CLIENT);
			set_failed_response_counter(-1);
			return ;
		}*/
		std::cout << "Received raw buffer: [" << _read_buff << "]\n";

		setReadBuff(buffer);
		std::cout << "Received raw buffer: [" << _read_buff << "]\n";

		if (_read_buff.find("\r\n") != std::string::npos) // as in foundsize_t pos = 
		{
			//resetClientTimer(_timer_fd, config::TIMEOUT_CLIENT);
			set_failed_response_counter(-1);
			//std::string message = _read_buff.substr(0, pos); // because we have multple ŗņ
			msg.handle_message(*this, _read_buff, server);	
			// refactor buffer ?? 
			//_read_buff.erase(0, pos + 2);
			_read_buff.clear();
		}
		else
			std::cout<<"something has been read and a null added"<<std::endl;
	}
}

void Client::setDefaults(int num){
	_nickName = "anon"; //+ num;
	_ClientName = "Clientanon" + num;
	_fullName = "fullanon" + num;
}

void Client::sendPing() {
	safeSend(_fd, "PING :server/r/n");
}
void Client::sendPong() {
	safeSend(_fd, "PONG :server/r/n");
}


bool Client::change_nickname(std::string nickname, int fd){
	_nickName.clear();
	_nickName = nickname;
	std::cout<<"hey look its a fd = "<< fd << std::endl;
	//this->set_nickname(nickname);

//	else (0);
	return true;
}



