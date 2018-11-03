#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "event.h"

#define MARKER 257

int
main (int   argc,
      char *argv[])
{
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    int dead_programs = 0;
    event msg;

    if (id == -1) {
        return 1;
    }

    srand (time (0));

    while (1) {
        ssize_t err = 0;

        msg.type = 2;
        msg.data = rand () * MARKER;
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msg.type = MARKER;
        msg.sender = 2;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);
        if (msg.sender < 0)
            dead_programs += 1;
        if (msg.sender == -2)
            break;
    }

    if (dead_programs >= 4) {
        msgctl (id, IPC_RMID, NULL);
    }
    else {
        msg.sender = -MARKER;

        msg.type = 1;
        msgsnd (id, &msg, EVENT_SIZE, 0); 

        msg.type = 2;
        msgsnd (id, &msg, EVENT_SIZE, 0); 

        msg.type = 251;
        msgsnd (id, &msg, EVENT_SIZE, 0); 

        msg.type = 997;
        msgsnd (id, &msg, EVENT_SIZE, 0); 
    }

    return 0;
}

