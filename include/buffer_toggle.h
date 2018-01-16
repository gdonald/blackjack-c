#ifndef BUFFER_TOGGLE_H
#define BUFFER_TOGGLE_H

#include <termios.h>
#include <unistd.h>

struct termios t;

void bt_on();
void bt_off();

#endif
