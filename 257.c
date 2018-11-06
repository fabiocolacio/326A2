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
 * Sender 257 sends messages to receiver 2.
 * Sender 257 terminates if receiver 2 terminates.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "event.h"

#define MARKER 257

int
main (int   argc,
      char *argv[])
{
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    if (id == -1) return 1;

    event msg;

    srand (time (0));

    while (1) {
        ssize_t err = 0;

        // Send to receiver 2
        msg.type = 2;
        msg.data = rand () * MARKER;
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        // Send messages in place of receiver 2.
        // This allows us to constantly listen for
        // receiver2's termination message without
        // blocking execution.
        msg.type = MARKER;
        msg.sender = 2;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        // Listen for a termination message from receiver 2
        msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);
        if (msg.sender == -2) break;

        // This delay is not for synchronization.
        // It is here to prevent the message queue from
        // being flooded with messages faster than they can be processed,
        // which causes the queue to reach the max amount of messages.
        usleep (5000);
    }

    return 0;
}

