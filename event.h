#ifndef __EVENT_H__
#define __EVENT_H__

#define EVENT_SIZE sizeof (long)

typedef struct {
    long type;
    long data;
} event;

#endif

