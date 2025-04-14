#include <string>
class server{
	private:
		int _port;
		int _server_socket;
		int _fd;
		std::string _password;
		//map client_array();
	public:
		server();
		server(int port, std::string password);
		~server();

		// setters
		void set_port(int const port);
		void set_password(std::string const password);

		// getters
		int get_port() const;
		std::string get_password() const;

}

Server::server(int port , std::string password, int server_fd)
{
	_port = port;
	_password = password; 
	_fd = server_fd;
	return server;
}