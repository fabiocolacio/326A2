#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "event.h"

int
main (int   argc,
      char *argv)
{
    int   msgflag = IPC_CREAT | IPC_EXCL | 0666;

    key_t key = ftok ("/home", 1);
    int   id = msgget (key, msgflag);

    int   dead_programs = 0;
    int   dead_senders = 0;

    if (id == -1) {
        return -1;
    }

    srand (time (0));

    while (dead_senders < 2) {
        event msg;

        msgrcv (id, &msg, EVENT_SIZE, 1, 0);

        if (msg.data % 997 == 0) {
            printf ("Sender 997: %d\n", msg.data);
            msg.type = 997;
            msg.data = rand ();
            msgsnd (id, &msg, EVENT_SIZE, 0);
        }
        else if (msg.data % 251 == 0) {
            printf ("Sender 251: %d\n", msg.data);
        }
        else if (msg.data == -997 || msg.data == -251) {
            dead_senders += 1;
            dead_programs += 1;
        }
        else if (msg.data == -2 || msg.data == -257) {
            dead_programs += 1; 
        }
    }

    if (dead_programs >= 4) {
        // We are the last program alive. Deallocate the queue here.
        msgctl (id, IPC_RMID, NULL);
    }

    return 0;
}

