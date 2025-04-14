#include <string>
class Server{
	private:
		int _port;
		int _server_socket;
		int _fd;
		std::string _password;
		//map client_array();
	public:
		Server();
		Server(int port, std::string password);
		~Server();

		// setters
		//void set_port(int const port);
		//void set_password(std::string const password);
		void setFd(int fd);
		// getters
		int getPort() const;
		std::string get_password() const;

};

