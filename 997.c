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
 * Sender 997 sends messages to receiver1 and receiver2.
 * Sender 997 requires ACK messages from both receivers.
 * If sender 997 receives an ACK message with a value smaller than 100,
 * it terminates. Before termination, sender 997 notifies receiver1 that it will
 * do so.
 */

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
        // Send to receiver 1
        msg.type = 1;
        msg.data = rand ();
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        // Send to receiver 2
        msg.type = 2;
        msg.data = rand () * MARKER;
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        // Listen for ACK
        msgrcv (id, &msg, EVENT_SIZE, 997, 0);
        printf ("Receiver %d: %d\n", msg.sender, msg.data);

        if (msg.data < 100) break;
    }

    // Tell receiver1 that we are terminating
    msg.sender = -MARKER;
    msg.type = 1;
    msgsnd (id, &msg, EVENT_SIZE, 0);

    return 0;
}
