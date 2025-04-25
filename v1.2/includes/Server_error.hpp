#pragma once
#include <exception>

enum class ErrorType {
	CLIENT_DISCONNECTED,
	SERVER_SHUTDOWN,
	EPOLL_FAILURE_0,
	EPOLL_FAILURE_1,
	SOCKET_FAILURE,
	ACCEPT_FAILURE,
	NO_USER_INMAP
};

class ServerException : public std::exception {
	private:
		ErrorType _type;
		std::string _message; 
	public:
		ServerException(ErrorType t, const std::string& msg) : _type(t), _message(msg) {}

    	ErrorType getType() const { return _type; }

    	const char* what() const noexcept override {
			static std::string error_msg; // statics are more persistent
			error_msg = get_error_msg() +(_message.empty() ? "" : ": " + _message);
			return error_msg.c_str();
		}
	private:
		std::string get_error_msg() const {
		switch (_type) {
           	case ErrorType::CLIENT_DISCONNECTED: return "Client Disconnected";
        	case ErrorType::SERVER_SHUTDOWN: return "server shutdown destroying evrything hold .....";
			case ErrorType::EPOLL_FAILURE_0: return "Epoll failed to complete"; //clean
			case ErrorType::EPOLL_FAILURE_1: return "Epoll failed to complete"; // dirty 
			case ErrorType::SOCKET_FAILURE: return "socket failed to complete";
			case ErrorType::ACCEPT_FAILURE: return "could not accept client connection";
			case ErrorType::NO_USER_INMAP: return "no user matching fd found in map, does user exist?"; // refine
			//case ErrorType::NETWORK_FAILURE: return "Network Failure";
           	//case ErrorType::INVALID_MESSAGE: return "Invalid Message";
           	default: return "Unknown Error";
		}
	}
};