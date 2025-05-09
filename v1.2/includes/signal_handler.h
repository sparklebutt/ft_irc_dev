#pragma once
#include <signal.h>

extern sigset_t sigmask;

int signal_mask();
void handle_signal(int signum);
void setup_signal_handler();
int manage_signal_events(int signal_fd);