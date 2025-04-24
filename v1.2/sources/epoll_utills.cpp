
#include "epoll_utils.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h> //perror
#include <iostream> // remove if nmo printing required
#include "Server.hpp"
#include "signal_handler.h"
#include "config.h"
#include "Server_error.hpp"
/**
 * @brief Creates a struct for epoll events such as EPOLLIN
 * for monitoring and adds the created struct to a kernel-managed data structure,
 * this is then monitored by epoll_wait()
 * 
 * @note other events could potentially be added 
 */
int setup_epoll(int epoll_fd, int fd, uint32_t events)
{
	struct epoll_event event;
	event.events = events; // Monitor for read events
	event.data.fd = fd; // File descriptor to monitor
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == errVal::FAILURE)
		throw ServerException(ErrorType::EPOLL_FAILURE_1, "could not add fd to epoll");
	return 0;
}

/**
 * @brief We set socket to nonblocking so that recv() dosnt wait for data,
 * if it where to wait for data it would freeze our programe and potentailly stop 
 * multiple client handling.
 * int flags captures the flags , fcntl sets the flag to non blocking. 
 * 
 * @note Now recv() will return a -1 but it will send a errno we
 * can catch to tell it to continue that is relevant to not reciveing data yet.
 * 
 */
int make_socket_unblocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
    if (flags == errVal::FAILURE) {
		throw ServerException(ErrorType::SOCKET_FAILURE, "fcntl failed to get flags");
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == errVal::FAILURE) {
		throw ServerException(ErrorType::SOCKET_FAILURE, "fcnntl failed to set O_NONBLOCK");
    }
    return 0;
}

int create_epollfd(Server &server)
{
	int epollfd = epoll_create1(0);
	
	if (epollfd == errVal::FAILURE)
		throw ServerException(ErrorType::EPOLL_FAILURE_0, "could not create epollfd");	
	setup_epoll(epollfd, server.getFd(), EPOLLIN);
	std::cout<<"seting up epoll signal fd------"<<std::endl;
	make_socket_unblocking(server.getFd());
	setup_signal_handler();
	server.set_event_pollfd(epollfd);
	return epollfd;
}
