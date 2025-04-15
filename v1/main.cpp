#include <string>
#include <iostream>

#include "Server.hpp"
#include "serverUtilities.hpp"


int validate_port(int& port)
{
	// if (! valid range of ports)
	// return 0;
	// if (!port in use)
	//	return 0;
	// else return port;

}
std::string validate_password(std::string& password)
{
	// if (password empty)
	//	return nullptr/empty string;
//	if (password too big)
//		return nullptr/empty string
//	else password;
}INCLUDES = ./includes

int loop()
{
	// set up test user
	while (true)
	{
		// read to buffer from test user
		int fd = epoll_wait;
		if (fd && EPOLLLIN)
		// do acton 
	}
}
/**
 * @brief We first set up default values that will benefit us during testing.
 * we then see if we are recieving arguments from command line and validate them.
 * We set up server scoket and store required data in server class
 * @attention We set up a test client to test our servers ability to connect to irssi
 * 
 * We create a loop where we read from user socket into a buffer , print that into a log file and /or terminal 
 * so we can see the irc protocol. 
 * set up epoll
 * 
 * @param argc argument count
 * @param argv 1 = port 2 = password
 * @return int 
 */
int main(int argc, char** argv)
{
	int port_number = 4242;
	std::string password = "password";
	//if (argc != 2)
	//	exit(1);

	// if we give arguments we validate the arguments else we 
	// use defaults , this must be refined later . 
	if (argc == 2)
	{
		if (!validate_port(argv[1]))
			exit(1);
		if (!validate_password(argv[2]))
			exit(1);
		port_number = argv[1];
		password = argv[2];	
	}
	else
	{
		std::cout<<"using default port and password"<<std::endl;
	}
	// instantiate server object with assumed port and password
	Server server(port_number, password);
	// set up server socket through utility function
	if (setupServerSocket(&server) == 1)
		std::cout<<"server socket setup failure"<<std::endl;
	
	epoll(); // add server_fd
	loop(); //begin server loop


    // 5. Close socket (example only)
    close(server_fd);

	return 0;
}

