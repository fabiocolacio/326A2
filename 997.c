#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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
    if (id == -1) return 1;

    event msg;

    srand (time (0));

    while (1) {
        msg.type = 1;
        msg.data = rand ();
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msg.type = 2;
        msg.data = rand ();
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        msgrcv (id, &msg, EVENT_SIZE, 997, 0);
        
        printf ("Receiver %d: %d\n", msg.sender, msg.data);

        if (msg.data < 100) break;
    }

    msg.sender = -MARKER;
    msg.type = 1;
    msgsnd (id, &msg, EVENT_SIZE, 0);

    return 0;
}
