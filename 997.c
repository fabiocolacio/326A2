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
    int   msgflag = IPC_CREAT | 0666;

    key_t key = ftok ("/home", 1);
    int   id = msgget (key, msgflag);

    if (id == -1) {
        return 1;
    }

    event  msg;
    event msg2;

    srand (time (0));


    while (1) {
        ssize_t err = 0;

        // Send a random message to two senders
        msg.type = 1;
        msg.data = rand () * MARKER;
	msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

	msg2.type = 2;
	msg2.data = rand () * MARKER;
	msg2.sender = MARKER;
	msgsnd (id, &msg2, EVENT_SIZE, 0);

	// receive acknowledgement message
	msgrcv (id, &msg, EVENT_SIZE, 997, 0);

	if (msg.sender == 1)
	    printf ("Sender 1 ACK: %d\n", msg.data);
	else if (msg.sender == 2)
	    printf ("Sender 2 ACK: %d\n", msg.data);

	if (msg.data < 100)
	    break;

    }
 
    event endmsg;

    endmsg.type = 1;
    endmsg.data = -997;

    msgsnd (id, &endmsg, EVENT_SIZE, 0); 

    return 0;
}
