#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "event.h"

#define MARKER 1

int
main (int   argc,
      char *argv)
{
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    if (id == -1) return -1;

    int dead_senders = 0;
    event msg;

    srand (time (0));

    while (dead_senders < 2) {
        msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);

        printf ("Sender %d: %d\n", msg.sender, msg.data);

        if (msg.sender == 997) {
            msg.type = 997;
            msg.data = rand ();
            msg.sender = 1;
            msgsnd (id, &msg, EVENT_SIZE, 0);
        }
        else if (msg.sender <= 0) {
            dead_senders += 1;
        }
    }

    msgctl (id, IPC_RMID, NULL);

    return 0;
}

