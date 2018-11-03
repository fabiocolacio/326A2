#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "event.h"

#define MARKER 997

int
main (int   argc,
      char *argv)
{
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    event msg;
    int dead_programs = 0;

    if (id == -1) {
        return 1;
    }

    srand (time (0));

    while (dead_programs < 4) {
        ssize_t err = 0;

        // Send a random message to two senders
        msg.type = 1;
        msg.data = rand () * MARKER;
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);
        msg.type = 2;
        msg.data = rand () * MARKER;
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        // receive acknowledgement message
        msgrcv (id, &msg, EVENT_SIZE, 997, 0);

        if (msg.sender == 1 || msg.sender == 2)
            printf ("Receiver %d ACK: %d\n", msg.sender, msg.data);
        else if (msg.sender < 1)
            dead_programs += 1;

        if (msg.data < 100)
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

        msg.type = 257;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msg.type = 251;
        msgsnd (id, &msg, EVENT_SIZE, 0);
    }

    return 0;
}
