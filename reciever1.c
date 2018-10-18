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

    key_t key_997 = ftok ("/home", 997);
    int   id_997 = msgget (key_997, msgflag);

    if (id_997 == -1) {
        return -1;
    }

    srand (time (0));

    while (1) {
        event msg;

        msgrcv (id_997, &msg, EVENT_SIZE, 997, 0);
        printf ("Sender 997: %d\n", msg.data);

        msg.type = 1;
        msg.data = rand ();
        msgsnd (id_997, &msg, EVENT_SIZE, 0);
    }

    return 0;
}

