#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Client.hpp"
#define  RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")
// Forward declaration (optional but can sometimes help compile times)
// class std::string; // No, string is included above.

// RFC 2812

class IrcMessage
{
	private:
		// int current_fd;
    	std::string _prefix;
    	std::string _command;
    	std::vector<std::string> _paramsList;

	public:
    	IrcMessage();
    	~IrcMessage();
    	// parse incoming
    	bool parse(const std::string& rawMessage);
    	// parse outgoing
    	std::string toRawString() const;
    	void setPrefix(const std::string& prefix);
    	void setCommand(const std::string& command); 
    	const std::string& getPrefix() const;
    	const std::string& getCommand() const;
    	const std::vector<std::string>& getParams() const;
		const std::string getParam(unsigned long index) const ;
		void printMessage(const IrcMessage& msg);


	// araveala has added this to help give you full control
	// naming is changable

	void handle_message(Client& Client, const std::string message, Server& server);//std::shared_ptr<Client> Client
};