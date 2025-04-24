#pragma once
#include <string>
#include <vector>

// Forward declaration (optional but can sometimes help compile times)
// class std::string; // No, string is included above.

// RFC 2812

class IrcMessage
{
private:
    std::string _prefix;
    std::string _command;
    std::vector<std::string> _paramsList;

public:
    IrcMessage();
    ~IrcMessage();
    bool parse(const std::string& rawMessage);
    void setPrefix(const std::string& prefix);
    void setCommand(const std::string& command); 
    const std::string& getPrefix() const;
    const std::string& getCommand() const;
    const std::vector<std::string>& getParams() const;
};