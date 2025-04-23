#include "IrcMessage.hpp"
#include <iostream> // For potential debugging, can remove later
#include <sstream>  // For string stream operations
#include <cstddef>  // For size_t

// --- Constructor ---
IrcMessage::IrcMessage()
{
    // Default constructor, _prefix, _command, _paramsList are default constructed (empty)
}

// --- Destructor ---
IrcMessage::~IrcMessage()
{
    // Nothing dynamic allocated in this simple version, default destructor is fine
}

// --- Setters ---
void IrcMessage::setPrefix(const std::string& prefix)
{
    _prefix = prefix;
}

void IrcMessage::setCommand(const std::string& command)
{
    _command = command;
}

// Note: setParams is removed as params are handled by parse into the vector

// --- Getters ---
const std::string& IrcMessage::getPrefix() const
{
    return _prefix;
}

const std::string& IrcMessage::getCommand() const
{
    return _command;
}

const std::vector<std::string>& IrcMessage::getParams() const
{
    return _paramsList;
}

// Optional: Implement helper getters for parameters
/*
const std::string& IrcMessage::getParam(size_t index) const
{
    if (index < _paramsList.size()) {
        return _paramsList[index];
    }
    // Handle out of bounds access - throwing an exception is common
    throw std::out_of_range("Parameter index out of bounds");
}

size_t IrcMessage::getParamCount() const
{
    return _paramsList.size();
}
*/

// --- Parse Method (Core Logic) ---
bool IrcMessage::parse(const std::string& rawMessage)
{
    // 1. Clear previous state
    _prefix.clear();
    _command.clear();
    _paramsList.clear();

    // 2. Find the CRLF terminator (\r\n)
    size_t crlf_pos = rawMessage.find("\r\n");
    if (crlf_pos == std::string::npos) {
        // Message doesn't end with CRLF - invalid format
        std::cerr << "Error: Message missing CRLF terminator." << std::endl; // Basic error reporting
        return false;
    }

    // Work with the message content before CRLF
    std::string message_content = rawMessage.substr(0, crlf_pos);
    std::stringstream ss(message_content); // Use stringstream to easily extract parts

    std::string current_token;
    ss >> current_token; // Read the first token

    // 3. Check for prefix (starts with ':')
    if (!current_token.empty() && current_token[0] == ':') {
        _prefix = current_token.substr(1); // Store prefix (without the leading ':')
        ss >> _command; // Read the next token as the command
        if (_command.empty()) {
             // Prefix was present, but no command followed - invalid
             std::cerr << "Error: Prefix present but no command found." << std::endl;
             // Clear state to indicate failure
             _prefix.clear();
             return false;
        }
    } else {
        // No prefix, the first token is the command
        _command = current_token;
    }

    // Command is mandatory
    if (_command.empty()) {
        std::cerr << "Error: No command found." << std::endl;
        // State is already clear if prefix wasn't found first
        return false;
    }

    // 4. Process parameters
    // Read the rest of the stringstream line by line to handle trailing parameter
    std::string params_str;
    std::getline(ss, params_str); // Reads the rest of the line, including leading space

    // Trim leading whitespace from params_str (the space after command or prefix)
    size_t first_char = params_str.find_first_not_of(" ");
    if (first_char == std::string::npos) {
        // Only whitespace or empty string remaining - no parameters
        return true; // Parsing successful, message had prefix/command but no params
    }
    params_str = params_str.substr(first_char); // params_str now starts at the first non-space char

    // Check for trailing parameter (starts with ':')
    if (!params_str.empty() && params_str[0] == ':') {
        // The rest of the string (after the colon) is a single parameter
        _paramsList.push_back(params_str.substr(1)); // Store trailing param (without leading ':')
    } else {
        // Split remaining string by spaces into parameters
        std::stringstream params_ss(params_str);
        std::string param_token;
        while (params_ss >> param_token) {
            _paramsList.push_back(param_token);
        }
    }

    // If we reached here, parsing was successful
    return true;
}