#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "event.h"

#define MARKER 2

int
main (int   argc,
      char *argv)
{
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    if (id == -1) return -1;

    int total_messages = 0;
    event msg;

    srand (time (0));

    while (total_messages < 5000) {
        msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);
        total_messages += 1;

        printf ("Sender %d: %d\n", msg.sender, msg.data);

        if (msg.sender == 997) {
            msg.type = 997;
            msg.data = rand ();
            msg.sender = MARKER;
            msgsnd (id, &msg, EVENT_SIZE, 0);
        }
    }

    msg.sender = -2;
    msg.type = 257;
    msgsnd (id, &msg, EVENT_SIZE, 0);

    return 0;
}

