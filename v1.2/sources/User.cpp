#include "User.hpp"
//#include "Server.hpp"

User::User() {/*default constructor*/}

User::User(int fd) : _fd(fd) { }

User::~User() {/*default deconstructor*/}

int User::getFd() { return _fd; }
//std::string User::recieve_message(int fd) {}