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
    int   msgflag = IPC_CREAT | 0666;

    key_t key = ftok ("/home", 1);
    int   id = msgget (key, msgflag);

    int   total_messages = 0;

    int   dead_programs = 0;

    if (id == -1) {
        return -1;
    }

    srand (time (0));

    while (dead_programs < 4 && total_messages < 5000) {
        event msg;

        msgrcv (id, &msg, EVENT_SIZE, 2, 0);

        if (msg.sender == 997) {
            printf ("Sender 997: %d\n", msg.data);
            total_messages += 1;
            msg.type = 997;
            msg.data = 2 * rand ();
            msgsnd (id, &msg, EVENT_SIZE, 0);
        }
        else if (msg.sender == 257) {
            printf ("Sender 257: %d\n", msg.data);
            total_messages += 1;
        }
        else if (msg.sender == -997 ||
                 msg.sender == -225 ||
                 msg.sender == -251 ||
                 msg.sender == -1) {
            dead_programs += 1;
        }
    }

    if (dead_programs >= 4) {
        msgctl (id, IPC_RMID, NULL);
    }

    return 0;
}

