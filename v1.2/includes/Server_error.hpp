#pragma once
#include <exception>

enum class ErrorType {
	CLIENT_DISCONNECTED,
	SERVER_SHUTDOWN
};

class ServerException : public std::exception {
	private:
		ErrorType type;
	public:
		ServerException(ErrorType t) : type(t) {}

    	ErrorType getType() const { return type; }

    	const char* what() const noexcept override {
        	switch (type) {
            	case ErrorType::CLIENT_DISCONNECTED: return "Client Disconnected";
            	case ErrorType::SERVER_SHUTDOWN: return "server shutdown destroying evrything hold .....";
				//case ErrorType::NETWORK_FAILURE: return "Network Failure";
            	//case ErrorType::INVALID_MESSAGE: return "Invalid Message";
            	default: return "Unknown Error";
			}
		}
};