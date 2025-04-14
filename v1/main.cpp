#include <string>
#include <socket>
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
		if (fd EPOLLLIN)
		// do acton 
	}
}

int main(int argc, char** argv)
{
	int port_number = 4242;
	std::string password = "password";
	//if (argc != 2)
		exit(1);
	if (argc == 2)
	{
		if (!validate_port(argv[1]))
			exit(1);
		if (!validate_password(argv[2]))
			exit(1);
		port_number = argv[1];
		password = argv[2];	
//		server_class(password, port_number);
	}

	
	// Communication between client and server has to be done via TCP/IP (v4 or v6)
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// 1. ipv4  2.what type of socket (we want stream), 3. for auto type (this case its tcp )
	if (server_fd == -1)
		std:::cout<<"something went wrong"<<std::endl;
		server(port, password, server_fd);
	// then set up non blocking 
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
	std::cout << "Server is listening on port 8080 (non-blocking mode)..." << std::endl;
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





int main(la la)
{
	char *port = 4242;
	char *password = "passwrd"


//	if (argv == 2)
//	{
//		int check = 0
//		if (validate_arv[1] && validate_arv[2])
//			check  +1;
//		port = argv[1]
//		passwoord = argv[2]
//	else
//		using default port 4242 and password passwrod				
//	}

	server(port, password)

}