
#include "epoll_utils.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h> //perror
#include <iostream> // remove if nmo printing required
#include "Server.hpp"
#include "signal_handler.h"
#include "config.h"
#include "ServerError.hpp"
#include <sys/timerfd.h>

#include <string.h>
/**
 * @brief void Server::modifyEpollEvent(int client_fd, uint32_t newEvents) {
    if (epollEventMap.find(client_fd) != epollEventMap.end()) {
        epollEventMap[client_fd].events |= newEvents;  // ✅ Modify existing flags
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &epollEventMap[client_fd]);
    } else {
        std::cerr << "err FD " << client_fd << " not found in epoll map!" << std::endl;
    }
}
 * 
 */

/**
 * @brief Creates a struct for epoll events such as EPOLLIN
 * for monitoring and adds the created struct to a kernel-managed data structure,
 * this is then monitored by epoll_wait()
 * 
 * @note other events could potentially be added 
 */
int Server::setup_epoll(int epoll_fd, int fd, uint32_t events)
{
	struct epoll_event event;
	event.events = events; // Monitor for read events
	event.data.fd = fd; // File descriptor to monitor
	int add_result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
	if (add_result == -1) {
		std::cerr << "EPOLL ADD ERROR: FD " << fd << " failed! Errno: " << strerror(errno) << std::endl;
	} else {
		std::cout << "✅ FD " << fd << " successfully added to epoll!" << std::endl;
	}
	/*if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == errVal::FAILURE)
		throw ServerException(ErrorType::EPOLL_FAILURE_1, "could not add fd to epoll");*/
	_epollEventMap[fd] = event;
	/*std::cout << "Current epoll event map state:" << std::endl;
	for (const auto& [fd, ev] : _epollEventMap) {
		std::cout << "FD " << fd << " → Events: "
				<< ((ev.events & EPOLLIN) ? " READ " : "")
				<< ((ev.events & EPOLLOUT) ? " WRITE " : "")
				<< ((ev.events & EPOLLERR) ? " ERROR " : "")
				<< std::endl;
	}*/
	return 0;
}

int Server::setup_epoll_timer(int epoll_fd, int timeout_seconds) {
	int timer_fd = createTimerFD(timeout_seconds);  //Creates a timer specific to this client

	struct epoll_event timer_event;
	timer_event.events = EPOLLIN;  //Triggers when timer expires
	timer_event.data.fd = timer_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timer_fd, &timer_event);
	_epollEventMap[timer_fd] = timer_event;
	return timer_fd;
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

int Server::create_epollfd()
{
	int epollfd = epoll_create1(0);
	
	if (epollfd == errVal::FAILURE)
		throw ServerException(ErrorType::EPOLL_FAILURE_0, "could not create epollfd");	
	setup_epoll(epollfd, _fd, EPOLLIN | EPOLLHUP);
	std::cout<<"seting up epoll signal fd------"<<std::endl;
	make_socket_unblocking(_fd);
	setup_signal_handler();
	_epoll_fd = epollfd;
	//server.set_event_pollfd(epollfd);
	return epollfd;
}
int Server::createTimerFD(int timeout_seconds) {
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);  //creates a timer FD
    if (timer_fd == -1)
        throw std::runtime_error("Failed to create timer FD");

    struct itimerspec timer_value;
    timer_value.it_value.tv_sec = timeout_seconds;  //First timeout duration
    timer_value.it_value.tv_nsec = 0;
    timer_value.it_interval.tv_sec = 0;  //One-shot timeout (resets manually)
    timer_value.it_interval.tv_nsec = 0;

    timerfd_settime(timer_fd, 0, &timer_value, NULL);  //Set timer expiration
	//_epollEventMap[timer_fd] = timer_value;
    return timer_fd;
}

void Server::resetClientTimer(int timer_fd, int timeout_seconds) {
    //std::cout<<"timer should be reseting checking seconds to set"<<config::TIMEOUT_CLIENT<<std::endl;
	//grab the correct fd
	struct itimerspec timer_value;
	timer_value.it_value.tv_sec = timeout_seconds;
    timer_value.it_value.tv_nsec = 0;
    timerfd_settime(timer_fd, 0, &timer_value, NULL);  //resets timeout
}
#include <poll.h>
#include <sys/socket.h>
/*void Server::readyEpollout(int client_fd, int epoll_fd) {
	std::cout<<"readying epollout now----------------\n";
	//int optval;
	//socklen_t optlen = sizeof(optval);
	//getsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen);
	int optval = 0;
	setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	std::cout << "SO_REUSEADDR for FD " << client_fd << ": " << optval << std::endl;
	
	std::cout << "Current epoll event map state:" << std::endl;
	for (const auto& [fd, ev] : _epollEventMap) {
		std::cout << "FD " << fd << " → Events: "
				<< ((ev.events & EPOLLIN) ? " READ " : "")
				<< ((ev.events & EPOLLOUT) ? " WRITE " : "")
				<< ((ev.events & EPOLLERR) ? " ERROR " : "")
				<< std::endl;
	}
	int flags = fcntl(client_fd, F_GETFL, 0);
	if (!(flags & O_NONBLOCK)) {
		std::cerr << "WARNING: FD " << client_fd << " is in blocking mode!" << std::endl;
		fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);  // ✅ Make non-blocking
	}
	struct epoll_event& event = _epollEventMap[client_fd];//get_epoll_event_struct(client_fd);

	std::cout << "FD after copying " << client_fd << " → Events: "
				<< ((event.events & EPOLLIN) ? " READ " : "")
				<< ((event.events & EPOLLOUT) ? " WRITE " : "")
				<< ((event.events & EPOLLERR) ? " ERROR " : "")
				<< std::endl;


	//if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &event) == -1 && errno == EINVAL) {
	//	std::cerr << "EPOLL ERROR: FD " << client_fd << " is NOT in epoll set! Errno: " << strerror(errno) << std::endl;
	//}
	event.events  |= EPOLLOUT | EPOLLONESHOT;//|= EPOLLOUT;  //enable writing
	event.data.fd = client_fd;
	
	//epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);  // ✅ Remove FD
	//_epollEventMap[client_fd].events |= EPOLLOUT;  // ✅ Enable write tracking
	//epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &_epollEventMap[client_fd]);  
	
	std::cout << "Modifying FD " << client_fd << " with events: "
	<< ((event.events & EPOLLIN) ? " READ " : "")
	<< ((event.events & EPOLLOUT) ? " WRITE " : "") << std::endl;
	std::cout << "FD " << client_fd << " - Events Before MOD: " << _epollEventMap[client_fd].events << std::endl;
	int res = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &_epollEventMap[client_fd]);
	std::cout << "FD " << client_fd << " - Events After MOD: " << _epollEventMap[client_fd].events << std::endl;
	std::cout<<res<<"\n";
	//if (epoll_ctl(EPOLL_CTL_MOD, client_fd, &_epollEventMap[client_fd]))
	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &event) == errVal::FAILURE)
	//epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &ev);
		perror("EPOLLOUT ERROR:: ");
	if (fcntl(client_fd, F_GETFD) == -1) {
		perror("EPOLLOUT CHECK FD");
		//std::cerr << "ERROR: client_fd is invalid or closed!" << std::endl;
	}
	if (fcntl(client_fd, F_GETFD) == -1) {
		std::cerr << "⚠ FD " << client_fd << " was closed or removed unexpectedly!" << std::endl;
	}
	
	struct pollfd pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN | POLLOUT;
	int poll_res = poll(&pfd, 1, 1000);
	if (poll_res > 0) {
		std::cout << "✅ POLLOUT detected via poll()!" << std::endl;
	}
	int result = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &ev);
	if (result == -1 && errno == EINVAL) {
    	perror("EPOLLOUT IN SET YAY OR NAY:: ");
		//std::cerr << "ERROR: FD " << client_fd << " is not in epoll set!" << std::endl;
	}
	//std::cout<<"ERROR COCCURED TRYIN TO ADD EPOLLOUT";
}*/