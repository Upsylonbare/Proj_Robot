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

} Admin;

Admin* admin;

static void AdminUI_captureChoice();
static void AdminUI_askMvt(Direction direction);
static VelocityVector AdminUI_translate(Direction direction);
static void AdminUI_ask4Log();
static void AdminUI_askClearLog();
static void AdminUI_eraseLog();
static void AdminUI_quit();
static void AdminUI_run();
static void AdminUI_display();



void AdminUI_start()
{
    LOG_ADMINUI("ADMINUI START\r\n");
    Pilot_start();
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
    AdminUI_run();
    //tc sett att
    // exec ve/ fork
}

void AdminUI_stop()
{
    printf("\n"); //for keeping last pilot state
    system("stty -cbreak"); //for disable getchar without press enter
    system("stty echo"); //for echo chqracters
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

static void AdminUI_captureChoice()
{
    LOG_ADMINUI("ADMINUI CAPTURE CHOICE\r\n");
    if(admin)
    {
        admin->character = getchar();
    }
    else
    {
        LOG_ADMINUI("ERROR ADMINUI CAPTURE CHOICE\r\n");
    }

}

static void AdminUI_askMvt(Direction direction)
{
    LOG_ADMINUI("ADMINUI ASK MVT\r\n");
    VelocityVector localvector = AdminUI_translate(direction);
    Pilot_setVelocity(localvector);
}

static VelocityVector AdminUI_translate(Direction direction)
{
    LOG_ADMINUI("ADMINUI TRANSLATE\r\n");
    VelocityVector localvector;
    localvector.dir = direction;
    localvector.power = NOMINAL_POWER;
    return localvector;
}

static void AdminUI_ask4Log()
{
    Pilot_check();
    PilotState localpilot = Pilot_getState();
    if(localpilot.collision == 0)
    {
        printf("\rCollision :");
        printf(GREEN);
        printf(" No ");
        printf(DEFAUT);
    }
    else
    {
        printf("\rCollision :");
        printf(RED);
        printf(" YES ");
        printf(DEFAUT);
    }
    printf("Luminosite : %f ", localpilot.luminosity);
    printf("Vitesse : %d ",localpilot.speed);
}

static void AdminUI_askClearLog()
{
    AdminUI_eraseLog();
}

static void AdminUI_eraseLog()
{
    printf("\033[2K\r");
}

static void AdminUI_quit()
{
    Pilot_stop();
}

static void AdminUI_run()
{
    while(admin->character != QUIT_CHAR)
    {
        AdminUI_display();
        AdminUI_captureChoice();
        switch (admin->character)
        {
        case QUIT_CHAR:
            AdminUI_quit();
            break;
        case CLEAR_CHAR:
            AdminUI_askClearLog();
            break;
        case LOG_CHAR:
            AdminUI_ask4Log();
            break;
        case LEFT_CHAR:
            AdminUI_askMvt(LEFT);
            break;
        case RIGHT_CHAR:
            AdminUI_askMvt(RIGHT);
            break;
        case FORWARD_CHAR:
            AdminUI_askMvt(FORWARD);
            break;
        case BACKWARD_CHAR:
            AdminUI_askMvt(BACKWARD);
            break;
        case STOP_CHAR:
            AdminUI_askMvt(NO);
            break;
        default:
            break;
        }
        
    }

}

static void AdminUI_display()
{
    LOG_ADMINUI("ADMINUI DISPLAY\r\n");
}