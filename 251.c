#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "get_info.h"
#include "event.h"

#define MARKER 251

int
main (int   argc,
      char *argv[])
{
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    if (id == -1) return 1;

    event msg;
    msg.type = 1;
    msg.sender = -MARKER;
    get_info (id, (msgbuf*) &msg, EVENT_SIZE, 1);

    srand (time (0));

    while (1) {
        msg.type = 1;
        msg.data = rand () * MARKER;
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        usleep (5000);
    }

    return 0;
}
