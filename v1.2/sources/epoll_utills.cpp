
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

int setup_epoll_timer(int epoll_fd, int timeout_seconds) {
	int timer_fd = createTimerFD(timeout_seconds);  //Creates a timer specific to this client

	struct epoll_event timer_event;
	timer_event.events = EPOLLIN;  //Triggers when timer expires
	timer_event.data.fd = timer_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timer_fd, &timer_event);
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

int create_epollfd(Server &server)
{
	int epollfd = epoll_create1(0);
	
	if (epollfd == errVal::FAILURE)
		throw ServerException(ErrorType::EPOLL_FAILURE_0, "could not create epollfd");	
	setup_epoll(epollfd, server.getFd(), EPOLLIN | EPOLLHUP);
	std::cout<<"seting up epoll signal fd------"<<std::endl;
	make_socket_unblocking(server.getFd());
	setup_signal_handler();
	server.set_event_pollfd(epollfd);
	return epollfd;
}
int createTimerFD(int timeout_seconds) {
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);  //creates a timer FD
    if (timer_fd == -1)
        throw std::runtime_error("Failed to create timer FD");

    struct itimerspec timer_value;
    timer_value.it_value.tv_sec = timeout_seconds;  //First timeout duration
    timer_value.it_value.tv_nsec = 0;
    timer_value.it_interval.tv_sec = 0;  //One-shot timeout (resets manually)
    timer_value.it_interval.tv_nsec = 0;

    timerfd_settime(timer_fd, 0, &timer_value, NULL);  //Set timer expiration

    return timer_fd;
}

void resetClientTimer(int timer_fd, int timeout_seconds) {
    std::cout<<"timer should be reseting checking seconds to set"<<config::TIMEOUT_CLIENT<<std::endl;

	struct itimerspec timer_value;
	/*if (timerfd_gettime(timer_fd, &timer_value) == 0) {
		std::cout << "Timer reset! Remaining seconds: " << timer_value.it_value.tv_sec << std::endl;
	} else {
		std::cerr << "Failed to get timer status!" << std::endl;
	}*/
	timer_value.it_value.tv_sec = timeout_seconds;
    timer_value.it_value.tv_nsec = 0;
    timerfd_settime(timer_fd, 0, &timer_value, NULL);  //resets timeout
	/*if (timerfd_gettime(timer_fd, &timer_value) == 0) {
		std::cout << "after setting timer Timer reset! Remaining seconds: " << timer_value.it_value.tv_sec << std::endl;
	} else {
		std::cerr << "Failed to get timer status!" << std::endl;
	}*/
	
}
