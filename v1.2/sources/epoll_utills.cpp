
#include "epoll_utils.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h> //perror

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
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
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
    if (flags == -1) {
        perror("fcntl failed to get flags");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl failed to set O_NONBLOCK");
        return -1;
    }
    return 0;
}
