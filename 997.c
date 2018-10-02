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
    int    msgflag = IPC_CREAT |  0666;
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

        // Wait for first response
        err = msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);
        if (err) printf ("%d\n", errno);
        if (msg.data < 100) break;

        // Wait for second response
        err = msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);
        if (err) printf ("%d\n", errno);
        if (msg.data < 100) break;
    }

    return 0;
}

