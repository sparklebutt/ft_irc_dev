#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include "Server.hpp"
#include "serverUtilities.hpp"

int setupServerSocket(Server &server)
{
	// Communication between client and server has to be done via TCP/IP (v4 or v6)
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// 1. ipv4  2.what type of socket (we want stream), 3. for auto type (this case its tcp )
	if (server_fd == -1)
		std::cout<<"something went wrong"<<std::endl;
	// then set up non blocking 
	server.setFd(server_fd);
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "Failed to set socket to non-blocking mode" << std::endl;
		close(server_fd);
		return 1;
	}

	sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);
    // 4. Bind and listen
    if (bind(server_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return 1;
    }
	std::cout<<"server is listening on port"<<server.getPort()<<std::endl;
	return 0;
}