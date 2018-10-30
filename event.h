#ifndef __EVENT_H__
#define __EVENT_H__

#define EVENT_SIZE sizeof (long) + sizeof (int)

typedef struct {
    long type;
    long data;
    int  sender;
} event;

#endif

