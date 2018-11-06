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
 * receiver2 listens to messages from senders 997 and 257.
 * receiver2 terminates when it has received a total of 5000 messages.
 * When receiver2 receives a message from 997, it sends a randomized message back to 997.
 * Before terminating, receiver2 sends a notification to 257 that it will terminate, by
 * sending a message with a sender value of -2.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "event.h"

#define MARKER 2

int
main (int   argc,
      char *argv)
{
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    if (id == -1) return -1;

    int total_messages = 0;
    event msg;

    srand (time (0));

    while (total_messages < 5000) {
        msgrcv (id, &msg, EVENT_SIZE, MARKER, 0);
        total_messages += 1;

        printf ("Sender %d: %d\n", msg.sender, msg.data);

        if (msg.sender == 997) {
            msg.type = 997;
            msg.data = rand () * MARKER;
            msg.sender = MARKER;
            msgsnd (id, &msg, EVENT_SIZE, 0);
        }
    }

    msg.sender = -2;
    msg.type = 257;
    msgsnd (id, &msg, EVENT_SIZE, 0);

    return 0;
}

