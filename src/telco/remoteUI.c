#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "commun.h"
#include "util.h"
#include "client.h"

typedef struct
{
    char character;
}RemoteUI_s;

RemoteUI_s* remote;

static void RemoteUI_setIP(/*ip*/);
static void RemoteUI_captureChoice();
static void RemoteUI_askMvt(Comm_order_e direction);
static void RemoteUI_display();
static void RemoteUI_run();
static void RemoteUI_quit();
static void RemoteUI_eraseLog();
static void RemoteUI_askClearLog();
static void RemoteUI_ask4Log();

void RemoteUI_start()
{
    remote = (RemoteUI_s*)malloc(sizeof(RemoteUI_s));
    RemoteUI_setIP();
    LOG_REMOTEUI("GOING IDLE\r\n");
    printf("Press :\r\n");
    printf("z for forward\r\n");
    printf("s for backward\r\n");
    printf("q for left\r\n");
    printf("d for right\r\n");
    printf("e for stop\r\n");
    printf("a for quit\r\n");
    printf("l for log\r\n");
    printf("c for clear log\r\n");
    system("stty -echo");
    system("stty cbreak"); //for enable getchar without press enter
    RemoteUI_run();
    //tc sett att
    // exec ve/ fork

}

void RemoteUI_stop()
{
    printf("\n"); //for keeping last pilot state
    system("stty -cbreak"); //for disable getchar without press enter
    system("stty echo"); //for echo chqracters
    Client_stop();
}

static void RemoteUI_setIP(/*ip*/)
{
    Client_start();
}

static void RemoteUI_captureChoice()
{
    ASSERT_PRINTERROR(remote);
    remote->character = getchar();
    LOG_REMOTEUI("Char captured is %c\r\n",remote->character);
}

static void RemoteUI_askMvt(Comm_order_e order)
{
    Comm_order_e dir_order;
    dir_order = order;
    LOG_REMOTEUI("I ask %s\r\n",orderAsText[order]);
    Client_sendMsg(dir_order);
}

static void RemoteUI_ask4Log()
{
    Comm_order_e log_order;
    log_order = O_LOG;
    LOG_REMOTEUI("I ask %s\r\n",orderAsText[log_order]);
    Client_sendMsg(log_order);
    Comm_logs_s logs = Client_readMsg();
    if(logs.collision == 0)
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
    printf("Luminosite : %f ", logs.luminosity);
    printf("Vitesse : %d ",logs.speed);
    LOG_REMOTEUI("\r\n");
}

static void RemoteUI_askClearLog()
{
    LOG_REMOTEUI("\r\n");
    RemoteUI_eraseLog();
}

static void RemoteUI_eraseLog()
{
    LOG_REMOTEUI("\r\n");
    printf("\033[2K\r");
}

static void RemoteUI_quit()
{
    LOG_REMOTEUI("\r\n");
    Client_sendMsg(O_QUIT);
}

static void RemoteUI_run()
{
    LOG_REMOTEUI("\r\n");
    while(remote->character != QUIT_CHAR)
    {
        RemoteUI_display();
        RemoteUI_captureChoice();
        switch (remote->character)
        {
        case QUIT_CHAR:
            RemoteUI_quit();
            break;
        case CLEAR_CHAR:
            RemoteUI_askClearLog();
            break;
        case LOG_CHAR:
            RemoteUI_ask4Log();
            break;
        case LEFT_CHAR:
            RemoteUI_askMvt(O_LEFT);
            break;
        case RIGHT_CHAR:
            RemoteUI_askMvt(O_RIGHT);
            break;
        case FORWARD_CHAR:
            RemoteUI_askMvt(O_FORWARD);
            break;
        case BACKWARD_CHAR:
            RemoteUI_askMvt(O_BACKWARD);
            break;
        case STOP_CHAR:
            RemoteUI_askMvt(O_NO);
            break;
        default:
            break;
        }
    }
}

static void RemoteUI_display()
{
    LOG_REMOTEUI("\r\n");
}