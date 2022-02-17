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
    C_LEFT=1, 
    C_RIGHT, 
    C_FORWARD, 
    C_BACKWARD, 
    C_NO
}Comm_dir_e;


typedef struct
{
    Comm_logs_s comm_logs;
    Comm_dir_e  comm_dir;
} Comm_datas_s;

#endif /*COMMUN_H*/