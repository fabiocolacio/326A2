/*
 * Fabio Colacio and Mariel Trajano
 *
 * Associated Files
 * ----------------
 * event.h
 * receiver1.c
 * receiver2.c
 * 251.c
 * 257.c
 * 997.c
 *
 * Description
 * -----------
 * receiver1 listens to messages from senders 997 and 251.
 * receiver1 terminates when both 997 and 251 have terminated.
 * It knows that 997 has terminated if the message sender is -997.
 * It knows that 251 has terminated if the message sender is -251.
 * When receiver1 receives a message from 997, it sends a randomized message back to 997.
 * Before terminating receiver1 deletes the message queue.
 */

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
    // Open the message queue
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    if (id == -1) return -1;

    int dead_senders = 0;
    event msg;

    srand (time (0));

    while (dead_senders < 2) {
        // Listen for messages
        msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);
        printf ("Sender %d: %d\n", msg.sender, msg.data);

        if (msg.sender == 997) {
            // Send ACK
            msg.type = 997;
            msg.data = rand ();
            msg.sender = 1;
            msgsnd (id, &msg, EVENT_SIZE, 0);
        }
        else if (msg.sender <= 0) {
            // A sender has terminated
            dead_senders += 1;
        }
    }

    // Free the message queue
    msgctl (id, IPC_RMID, NULL);

    return 0;
}

