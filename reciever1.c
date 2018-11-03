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
    event msg;

    if (id == -1) {
        return -1;
    }

    srand (time (0));

    while (dead_programs < 4 && dead_senders < 2) {
        msgrcv (id, &msg, EVENT_SIZE, 1, 0);

        if (msg.sender == 997) {
            printf ("Sender 997: %d\n", msg.data);
            msg.type = 997;
            msg.data = (2 * rand ()) + 1;
            msg.sender = 1;
            msgsnd (id, &msg, EVENT_SIZE, 0);
        }
        else if (msg.sender == 251) {
            printf ("Sender 251: %d\n", msg.data);
        }
        else if (msg.sender == -997 || msg.sender == -251) {
            dead_senders += 1;
            dead_programs += 1;
        }
        else if (msg.sender == -2 || msg.sender == -257) {
            dead_programs += 1; 
        }
    }

    if (dead_programs >= 4) {
        // We are the last program alive. Deallocate the queue here.
        msgctl (id, IPC_RMID, NULL);
    }
    else {
        msg.sender = -1;

        msg.type = 257;
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

