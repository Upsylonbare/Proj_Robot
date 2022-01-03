/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer

#include "pilot.h"
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#ifdef LOG_ENABLE
#define LOG_ADMINUI(...) do{printf(YELLOW);printf(__VA_ARGS__);printf(DEFAUT);}while(0)
#else
#define LOG_ADMINUI(...)
#endif

typedef struct Admin_s
{
    char character;
    VelocityVector velocity_vector;

} Admin;

Admin* admin;

PilotState pilotstate;

static void Admin_init();
static void Admin_GetChar();
static void AdminUI_IDLE();

void AdminUI_start()
{
    LOG_ADMINUI("ADMINUI START\r\n");
    Pilot_start();
    Admin_init();
    AdminUI_IDLE();
}

void AdminUI_stop()
{
    LOG_ADMINUI("ADMINUI STOP\r\n");
    Pilot_stop();
}

void AdminUI_new()
{
    LOG_ADMINUI("ADMINUI NEW\r\n");
    admin = (Admin*) malloc(sizeof(Admin));
}

void AdminUI_free()
{
    LOG_ADMINUI("ADMINUI FREE\r\n");
    if(admin)
    {
        free(admin);
    }
    else
    {
        LOG_ADMINUI("ERROR ADMINUI FREE : error admin is null\r\n");
    }
}

static void AdminUI_IDLE()
{
    LOG_ADMINUI("ADMINUI GOING IDLE\r\n");
    printf("Press :\r\n");
    printf("z for forward\r\n");
    printf("s for backward\r\n");
    printf("q for left\r\n");
    printf("d for right\r\n");
    printf("e for stop\r\n");
    printf("a for quit\r\n");
    system("stty -echo");
    system("stty cbreak"); //for enable getchar without press enter
    while(admin->character != QUIT_CHAR)
    {
        Admin_GetChar();
        admin->velocity_vector.power = NOMINAL_POWER;
        Pilot_setVelocity(admin->velocity_vector);
        pilotstate = Pilot_getState();
        Log_Pilot(pilotstate);
    }
    printf("\n"); //for keeping last pilot state
    system("stty -cbreak"); //for disable getchar without press enter
    system("stty echo"); //for echo chqracters

}

static void Admin_GetChar()
{
    LOG_ADMINUI("ADMINUI GET CHAR\r\n");
    if(admin)
    {
        admin->character = getchar();
        if(admin->character == LEFT_CHAR) admin->velocity_vector.dir = LEFT;
        else if(admin->character == RIGHT_CHAR) admin->velocity_vector.dir = RIGHT;
        else if(admin->character == FORWARD_CHAR) admin->velocity_vector.dir = FORWARD;
        else if(admin->character == BACKWARD_CHAR) admin->velocity_vector.dir = BACKWARD;
        else if(admin->character == STOP_CHAR) admin->velocity_vector.dir = NO;
        else if(admin->character == QUIT_CHAR) admin->velocity_vector.dir = NO;
    }
    else
    {
        LOG_ADMINUI("ERROR ADMINUI GET CHAR\r\n");
    }

}

static void Admin_init()
{
    LOG_ADMINUI("ADMINUI INIT\r\n");
    if(admin)
    {
        admin->character = (char) STOP_CHAR;
        admin->velocity_vector.dir = NO;
        admin->velocity_vector.power = NO;
    }
    else
    {
        LOG_ADMINUI("ERROR ADMINUI INIT\r\n");
    }
}