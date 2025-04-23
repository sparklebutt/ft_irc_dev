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
 * It will also help manage incoming messages from clients that will be redirected to a message
 * handling class/function. 
 * 
 * @param server the instantiated server object that will be used to manage the server and its data
 * @return int 
 */
int loop(Server &server)
{
	// set buffer function in client?
	//std::basic_string<char> buffer;	// should switch to cpp methods now this is showing something.
	// using char buffer[1024] is the best choice here as string can not handle input with nullterminators
	// trying to force the use of c++ methods here is far more prone to error , as far as i can tell
	// char buffer[1024];

	// ssize_t bytes_read = 0;

	// continuouse accepting will mess with the fd
	bool acknowledged = false; // just so we can get a stream of messages that come through

	// This creates an epoll instance and returns its file descriptor
	int epollfd = epoll_create1(0);
	setup_epoll(epollfd, server.getFd(), EPOLLIN);  // epollin is for incoming messages
	// how to test the non blocking is working?
	make_socket_unblocking(server.getFd());
	//setsockopt????
	struct epoll_event events[10]; // 10 is just for testing could be MAX_CLIENTS
	while (true)
	{
//---------- a fucntion body
		// from epoll fd, in events struct 
		int nfds = epoll_wait(epollfd, events, 10, -1); // - 1 is blocking and is not allowed?
		// epoll_pwait() gives us signal handling , potentially we can add signal fds to this epoll() events
		// we might be able to just catch them here, and avoid possible interuption issues.
		// if nfds == -1 we have aerro we should be ab;le to print with perror.
//-----------
		//std::cout << "debugg 1." << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].events & EPOLLIN) {
                int fd = events[i].data.fd; // Get the associated file descriptor
                if (fd == server.getFd()) {
					server.create_user(epollfd);
					/*if (!acknowledged) {
						// send message back soo server dosnt think we are dead
						const char* ok_msg = ":server 001 OK\r\n";
						send(fd, ok_msg, strlen(ok_msg), 0);
						acknowledged = true; // mark as acknowledged
					}*/

                } else {
					// send fd to server fucntion to find relevent client 
					
					// read to client buffer.
					// client handles messages
					///... the message is validated and formated perhaps with a data struct 
					// clearly defining what will happen next
                    // handle incoming data on a client socket 
					
					/*memset(buffer, 0, sizeof(buffer));	
					// read and reister bytes
					bytes_read = recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT); // last flag makes recv non blocking 
                    if (bytes_read > 0) {
                        buffer[bytes_read] = '\0';
						// show incoming message*/
						std::string buffer;
						
//						buffer = get_user(fd).recieve_message(fd);
						buffer = server.get_user(fd)->receive_message(fd);

						std::cout << "Received: " << buffer << std::endl;
						if (!acknowledged) {
							// send message back soo server dosnt think we are dead
							const char* ok_msg = ":server 001 OK\r\n";
							send(fd, ok_msg, strlen(ok_msg), 0);
							acknowledged = true; // mark as acknowledged
						}
					} /*else if (bytes_read == 0) {
                        std::cout << "Client disconnected. Closing socket." << std::endl;
                        close(fd); // remove the client socket
                    } else {
                        perror("read failed");
                    }*/
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
	if (setupServerSocket(server) == 1)
		std::cout<<"server socket setup failure"<<std::endl;
	loop(server); //begin server loop
    close(server.getFd());

	return 0;
}

