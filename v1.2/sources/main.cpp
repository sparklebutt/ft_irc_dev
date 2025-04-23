#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Server.hpp"
#include "serverUtilities.hpp"
#include "User.hpp"
#include "general_utilities.hpp"
#include "config.h"
#include <unistd.h> // close()
#include <string.h>

#include "epoll_utils.hpp"
#include <sys/epoll.h>
// irssi commands 
// / WINDOW LOG ON 
// this can be opend in new terminal tail -f ~/ircTAB
// /help 
// /raw open `/file of choice
// open fileofchoice

/**
 * @brief This loop functions job is to keep the server running and accepting connections.
 * It will also help manage incoming messages from clients that will be redirected to user class methods 
 * and from there too message handling class/function. 
 * 
 * @param server the instantiated server object that will be used to manage the server and its data
 * @return int 
 * 
 * @note epoll_pwait() gives us signal handling , potentially we can add signal fds to this epoll() events,
 * we might be able to just catch them here, and avoid possible interuption issues.
 * 
 * other things to consider setsocket options? would these be helpfull ?
 * how to test if everything is non blocking
 */
int loop(Server &server)
{
	// applying a ping pong test 
	int server_ping_count = 0;
	int server_max_loop = 60;
	int is_client = 0;

	int epollfd = epoll_create1(0);
	setup_epoll(epollfd, server.getFd(), EPOLLIN);
	make_socket_unblocking(server.getFd());
	struct epoll_event events[config::MAX_CLIENTS];
	while (true)
	{
		server_ping_count++;
		// from epoll fd, in events struct 
		int nfds = epoll_wait(epollfd, events, config::MAX_CLIENTS, 50);
		// if nfds == -1 we have perro we should be able to print with perror.
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].events & EPOLLIN) {
                int fd = events[i].data.fd; // Get the associated file descriptor
				if (fd == server.getFd()) {
					server.create_user(epollfd);
					is_client++; // should be in server.hpp
				}
				else {
					std::string buffer;
					buffer = server.get_user(fd)->receive_message(fd);
					std::cout << "Received: " << buffer << std::endl;
					if (buffer.find("PONG")) {
						std::cout<<" PONG recived server_ping_count = "<<server_ping_count<<std::endl;;
					}
				}
            }
		}
		// this will be its own function !!!
		//std::cout<<"-----server_ping_count ----"<< server_ping_count<<std::endl;
		if (server_ping_count >= server_max_loop && is_client > 0) { 
			std::map<int, std::shared_ptr<User>> users = server.get_map();
			for (std::map<int, std::shared_ptr<User>>::iterator it = users.begin(); it != users.end(); it++)
			{
				std::cout<<"-----sending ping ----"<<std::endl;
				send(it->first, "PING :server/r/n", 14, 0);
				server_ping_count = 0;	
			}

		}


	}
	return 0;
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
	int port_number = 6666;
	std::string password = "password";
	//if (argc != 3)
	//	exit(1);

	// if we give arguments we validate the arguments else we 
	// use defaults , this must be refined later . 
	if (argc == 2)
	{
		std::cout<<"need 2 arguments"<<std::endl;
		exit(1);
	}
	if (argc == 3)
	{
		port_number = validate_port(argv[1]);
		if (validate_password(argv[2]).empty())
		{
			std::cout<<"empty password"<<std::endl;
			exit(1);
		}
		password = argv[2];	
	}
	else {
		std::cout<<"Attempting to use default port and password"<<std::endl;
	}
	// signals

	// instantiate server object with assumed port and password
	Server server(port_number, password);
	// set up server socket through utility function
	if (setupServerSocket(server) == errVal::FAILURE)
		std::cout<<"server socket setup failure"<<std::endl;
	loop(server); //begin server loop
	// clean up
    close(server.getFd());
	return 0;
}