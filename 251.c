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
 * Sender 251 sends messages to receiver1.
 * When sender 251 receives a kill signal, it notifies
 * receiver1 that it is about to terminate before doing so.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "get_info.h"
#include "event.h"

#define MARKER 251

int
main (int   argc,
      char *argv[])
{
    int msgflag = IPC_CREAT | 0666;
    key_t key = ftok ("/home", 1);
    int id = msgget (key, msgflag);
    if (id == -1) return 1;

    event msg;
    msg.type = 1;
    msg.sender = -MARKER;
    get_info (id, (msgbuf*) &msg, EVENT_SIZE, 1);

    srand (time (0));

    while (1) {
        msg.type = 1;
        msg.data = rand () * MARKER;
        msg.sender = MARKER;
        msgsnd (id, &msg, EVENT_SIZE, 0);

        // This delay is not for synchronization.
        // It is here to prevent the message queue from
        // being flooded with messages faster than they can be processed,
        // which causes the queue to reach the max amount of messages.
        usleep (5000);
    }

    return 0;
}
