#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "commun.h"
#include "util.h"
#include "client.h"

#ifdef LOG_ENABLE
#define LOG_REMOTEUI(...) do{printf(YELLOW);printf(__VA_ARGS__);printf(DEFAUT);}while(0)
#else
#define LOG_REMOTEUI(...)
#endif

typedef struct
{
    char character;
}RemoteUI_s;

RemoteUI_s* remote;

static void RemoteUI_setIP(/*ip*/);
static void RemoteUI_captureChoice();
static void RemoteUI_askMvt(Comm_dir_e direction);
static void RemoteUI_display();
static void RemoteUI_run();
static void RemoteUI_quit();
static void RemoteUI_eraseLog();
static void RemoteUI_askClearLog();
static void RemoteUI_ask4Log();

void RemoteUI_start()
{
    remote = (RemoteUI_s*)malloc(sizeof(RemoteUI_s));
    LOG_REMOTEUI("REMOTEUI START\r\n");
    RemoteUI_setIP();
    LOG_REMOTEUI("REMOTEUI GOING IDLE\r\n");
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
    LOG_REMOTEUI("REMOTEUI CAPTURE CHOICE\r\n");
    if(remote)
    {
        remote->character = getchar();
    }
    else
    {
        LOG_REMOTEUI("ERROR REMOTEUI CAPTURE CHOICE\r\n");
    }
}

static void RemoteUI_askMvt(Comm_dir_e direction)
{
    LOG_REMOTEUI("REMOTEUI ASK MVMT\r\n");
    Comm_datas_s data_to_be_send;
    data_to_be_send.comm_dir = direction;
    Client_sendMsg(data_to_be_send);
}

static void RemoteUI_ask4Log()
{
    LOG_REMOTEUI("REMOTEUI ASK FOR LOG\r\n");
    //deal with socket (Client Read)
}

static void RemoteUI_askClearLog()
{
    LOG_REMOTEUI("REMOTEUI ASK CLEAR LOG\r\n");
    RemoteUI_eraseLog();
}

static void RemoteUI_eraseLog()
{
    LOG_REMOTEUI("REMOTEUI ASK ERASE LOG\r\n");
    printf("\033[2K\r");
}

static void RemoteUI_quit()
{
    LOG_REMOTEUI("REMOTEUI QUIT\r\n");
    RemoteUI_stop();
}

static void RemoteUI_run()
{
    LOG_REMOTEUI("REMOTEUI RUN\r\n");
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
            RemoteUI_askMvt(C_LEFT);
            break;
        case RIGHT_CHAR:
            RemoteUI_askMvt(C_RIGHT);
            break;
        case FORWARD_CHAR:
            RemoteUI_askMvt(C_FORWARD);
            break;
        case BACKWARD_CHAR:
            RemoteUI_askMvt(C_BACKWARD);
            break;
        case STOP_CHAR:
            RemoteUI_askMvt(C_NO);
            break;
        default:
            break;
        }
    }
}

static void RemoteUI_display()
{
    LOG_REMOTEUI("REMOTEUI DISPLAY\r\n");
}