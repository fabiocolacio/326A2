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
 * event.h defines the message structure that is
 * used by all senders and receiver programs in this
 * system.
 *
 * The type field specifies which program should receive the message being sent.
 * The data field specifies the contents of the message being sent.
 * The sender field specifies which program sent the message.
 */

#ifndef __EVENT_H__
#define __EVENT_H__

#define EVENT_SIZE sizeof (long) + sizeof (int)

typedef struct {
    long type;
    long data;
    int  sender;
} event;

#endif

