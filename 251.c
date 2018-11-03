#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

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
    int dead_programs = 0;

    event msg;
    msg.type = 1;
    msg.sender = -MARKER;
    get_info (id, (msgbuf*) &msg, EVENT_SIZE, 1);

    if (id == -1) {
        return 1;
    }

    srand (time (0));

    while (dead_programs < 4) {
        ssize_t err = 0;

        // Send a random message
        msg.type = 1;
        msg.data = rand () * MARKER;
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msg.type = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);
        msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);
        if (msg.sender < 0)
            dead_programs += 1;
    }

    if (dead_programs >= 4) {
        msgctl (id, IPC_RMID, NULL);
    }
    else {
        msg.sender = -251;

        msg.type = 1;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msg.type = 2;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msg.type = 997;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msg.type = 257;
        msgsnd (id, &msg, EVENT_SIZE, 0);
    }

    return 0;
}
