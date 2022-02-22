#ifndef COMMUN_H
#define COMMUN_H

typedef struct 
{
    int speed;
    int collision;
    float luminosity;
} Comm_logs_s;

typedef enum
{
    O_QUIT=0,
    O_LEFT, 
    O_RIGHT, 
    O_FORWARD, 
    O_BACKWARD, 
    O_NO,
    O_LOG,
    O_ORDER_NB
}Comm_order_e;

extern char* orderAsText[O_ORDER_NB];
#endif /*COMMUN_H*/