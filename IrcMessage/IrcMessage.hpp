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
    std::vector<std::string> _paramsList; // Renamed for clarity

public:
    // Constructor
    IrcMessage();
    ~IrcMessage();

    // Method to parse a raw IRC message string
    // Returns true if parsing was successful, false otherwise.
    bool parse(const std::string& rawMessage);

    // Setters
    // Consider passing by const reference (const std::string&) for efficiency
    // Setters - you'll likely remove setParams(const std::string&)
    // The parsing logic will populate the vector directly.
    void setPrefix(const std::string& prefix);
    void setCommand(const std::string& command); 

    // Getters (essential to retrieve the data)
    const std::string& getPrefix() const;
    const std::string& getCommand() const;
    const std::vector<std::string>& getParams() const;


     // Optional: Add helper getters for parameters if needed
    // For example, getting a specific parameter by index:
    // const std::string& getParam(size_t index) const;
    // size_t getParamCount() const; // To know how many parameters there are
};