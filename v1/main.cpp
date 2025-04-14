#include <string>
#include <iostream>
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
}

int loop()
{
	while (true)
	{
		int fd = epoll_wait;
		if (fd && EPOLLLIN)
		// do acton 
	}
}

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
	int test = setupServerSocket(&server);
	
	epoll(); // add server_fd
	loop(); //begin server loop


    // 5. Close socket (example only)
    close(server_fd);
	// add port and password to server info

	// set up addressing
	// open socket for listening

	// store socket indo in server


	return 0;
}

