#pragma once
#include <string>
#include <sstream>
#include <iostream>
/**
 * @brief 
 * this is a header file that contains global variables and constants
 *
 * @notes: inline tells the compiler that multiple definitions of this variable
 * are allowed across different translation units, otherwise it would cause linker errors.
 * 
 * constexpr tells the compiler that the value is a constant expression, which will be evaluated 
 * at compile time. 
 * 
 * combining inline with constexpr allows us to define a variable in a header file
 * as 1 global instance of that variable. 
 * 
 * choosing not to use #define is safer as #define is not type safe and can cause
 * unexpected behavior if not used carefully.
 * 
 * alternatily we could define extern const char * instead of inline constexpr
 * but this would require us to define the variable in a .cpp file , since our values 
 * want to be imutable , this option is well suited for us.
 * 
 * this file could be seperated into config and error config, if we want to lower
 * inclusion ammounts in files, lets see
 */

class Server;
namespace Global {
	inline Server* server = nullptr;
}

enum class ErrorType {
	CLIENT_DISCONNECTED,
	SERVER_SHUTDOWN,
	EPOLL_FAILURE_0,
	EPOLL_FAILURE_1,
	SOCKET_FAILURE,
	ACCEPT_FAILURE,
	NO_USER_INMAP, // next up senderror
	BUFFER_FULL,
	BAD_FD,
	BROKEN_PIPE,
	UNKNOWN

};

/**
 * @brief Timeout for client shouyld be 3000 as irssi sends pings every 5 minutes 
 * we can set it low to showcase how we error handle in the case of a client disconnect
 * 
 */
namespace config {
	constexpr int MAX_CLIENTS = 10;
	constexpr int TIMEOUT_CLIENT = 3000; // this should be larger than epoll timeout
	constexpr int TIMEOUT_EPOLL = 0;
	
	constexpr int BUFFER_SIZE = 1024;
}

namespace errVal {
	constexpr int FAILURE = -1;
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
	constexpr int ERR_NICKNAMEINUSE = 433;
	constexpr int ERR_NOTREGISTERED = 451;
	constexpr int ERR_NEEDMOREPARAMS = 461;
	constexpr int ERR_ALREADYREGISTRED = 462;
	constexpr int ERR_PASSWDMISMATCH = 464;
	constexpr int ERR_UNKNOWNCOMMAND = 421;
	constexpr int ERR_UNKNOWNMODE = 472;
	constexpr int ERR_NOSUCHMODE = 472;
}

namespace IRCMessage {
	inline constexpr const char* error_msg = "ERROR :Server cannot accept connection, closing.\r\n";
	//inline constexpr const char* welcome_msg = ":server 001 testuser :OK\r\n";
	inline constexpr const char* welcome_msg = ":server 001 anon :Welcome to the IRC server\r\n";
	//":server 005 anon PREFIX=(o)@\r\n"
	//":server 005 anon NETWORK=myirc\r\n";
	inline constexpr const char* ping_msg = "PING :server\r\n";
	inline constexpr const char* pong_msg = "PONG :server\r\n";
	inline constexpr const char* pass_msg = "PASS :password\r\n";
	inline std::string get_nick_msg(const std::string& nickname) {
		std::stringstream ss;
        ss << "@ft_irc new nickname is now :" << nickname << "\r\n";
        return ss.str(); }
	inline constexpr const char* nick_msg = "*:*!user@localhost NICK :helooo\r\n";	
	//inline constexpr const char* nick_msg = ":NICK :helooo\r\n";
	//inline constexpr const char* nick_msg = ":*!user@localhost NICK :helooo\r\n";	
	//inline constexpr const char* nick_msg = "@ft_irc new nickname name is now :nickname\r\n";
	inline constexpr const char* user_msg = "USER :username 0 * :realname\r\n";
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
Config::MAX_NUM */
