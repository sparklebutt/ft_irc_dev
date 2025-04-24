#pragma once

class Server;
namespace Global {
	inline Server* server = nullptr;
}

namespace config {
	constexpr int MAX_CLIENTS = 10;
	constexpr int TIMEOUT_CLIENT = 100;
	constexpr int BUFFER_SIZE = 1024;
}

namespace errVal {
	constexpr int FAILURE = 1;
	constexpr int SUCCESS = 0;
}
/**
 * @brief i do not know if all of these are needed but here 
 * is a list of some irc error codes  
 * 
 */
namespace IRCerr {
	constexpr int ERR_NOSUCHNICK = 401;
	constexpr int ERR_NOSUCHSERVER = 402;
	constexpr int ERR_NOSUCHCHANNEL = 403;
	constexpr int ERR_CANNOTSENDTOCHAN = 404;
	constexpr int ERR_TOOMANYCHANNELS = 405;
	constexpr int ERR_WASNOSUCHNICK = 406;
	constexpr int ERR_NOTREGISTERED = 451;
	constexpr int ERR_NEEDMOREPARAMS = 461;
	constexpr int ERR_ALREADYREGISTRED = 462;
	constexpr int ERR_PASSWDMISMATCH = 464;
	constexpr int ERR_UNKNOWNCOMMAND = 421;
	constexpr int ERR_UNKNOWNMODE = 472;
	constexpr int ERR_NOSUCHMODE = 472;
}

namespace IRCMessage {
	std::string error_msg = "ERROR :Server cannot accept connection, closing.\r\n";
	std::string welcome_msg = ":server 001 OK\r\n";
	std::string ping_msg = "PING :server\r\n";
	std::string pong_msg = "PONG :server\r\n";
	std::string pass_msg = "PASS :password\r\n";
	std::string nick_msg = "NICK :nickname\r\n";
	std::string user_msg = "USER :username 0 * :realname\r\n";
}
/*
This is like global variables but its encapsulated in the Config, so its much harder to 
mix variables of the same name. This is however using namespace , 2 questions: 
1. do you want to learn them now when they are forbidden in the modules
2. are they forbidden in this project .
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
    constexpr int MAX_NUM = 0;
    constexpr const char* PRIV = "!???";

    // Example of a more complex configuration (bitmask permissions)
    enum Permissions {
        READ    = 1 << 0,
        WRITE   = 1 << 1,
        EXECUTE = 1 << 2
    };
}

#endif // CONFIG_H
this is used in code like this:
Config::MAX_NUM
*/
