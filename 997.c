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
    key_t  key = ftok ("/home", MARKER);
    int    msgflag = 0666;
    int    id = msgget (key, msgflag);

    if (id == -1) {
        return 1;
    }

    event  msg;

    srand (time (0));

    while (1) {
        ssize_t err = 0;

        // Send a random message
        msg.type = MARKER;
        msg.data = rand () * MARKER;

        msgsnd (id, &msg, EVENT_SIZE, 0);
        msgrcv (id, &msg, EVENT_SIZE, 1, 0);
        printf ("Sender 1 ACK: %d\n", msg.data);
        if (msg.data < 100) {
            break;
        }

        /*

        msgsnd (id, &msg, EVENT_SIZE, 0);
        msgrcv (id, &msg, EVENT_SIZE, 2, 0);
        printf ("Sender 2 ACK: %d\n", msg.data);
        if (msg.data < 100) {
            break;
        }

        */
    }

    return 0;
}

