#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "event.h"

#define MARKER 251

int
main (int   argc,
      char *argv)
{
    int   msgflag = IPC_CREAT | 0666;

    key_t key = ftok ("/home", 1);
    int   id = msgget (key, msgflag);

    if (id == -1) {
        return 1;
    }

    event  msg;

    srand (time (0));

    while (1) {
        ssize_t err = 0;

        // Send a random message
        msg.type = 1;
        msg.data = rand () * MARKER;
	msg.sender = MARKER;

        msgsnd (id, &msg, EVENT_SIZE, 0);

    }

    event endmsg;

    endmsg.type = 1;
    endmsg.data = -251;

    msgsnd (id, &endmsg, EVENT_SIZE, 0); 

    return 0;
}
