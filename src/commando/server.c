#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "commun.h"
#include "pilot.h"
#include "server.h"
#include "config.h"

#ifdef LOG_ENABLE
#define LOG_SERVER(...) do{printf(MAGENTA);printf(__VA_ARGS__);printf(DEFAUT);}while(0)
#else
#define LOG_SERVER(...)
#endif

static void Server_sendMsg(int socket_data);
static void Server_readMsg(int socket_data);
static void Server_askMvt(Comm_dir_e direction);

void Server_start()
{
    LOG_SERVER("SERVER START\r\n");
    int socket_listen;
    int socket_data;
    struct sockaddr_in server_address;
    Pilot_start();
    /* Creation du socket : AF_INET = IP, SOCK_STREAM = TCP */
    socket_listen = socket(AF_INET, SOCK_STREAM, 0);
    /* Type d'adresse = IP */
    server_address.sin_family = AF_INET;
    /* Port TCP ou le service est accessible */
    server_address.sin_port = htons(SERVER_PORT);
    /* On s'attache a toutes les interfaces */
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
     /* On attache le socket a l'adresse indiquee */
    bind(socket_listen, (struct sockaddr *)&server_address, sizeof(server_address));
    /* Mise en ecoute du socket */
    listen(socket_listen, MAX_PENDING_CONNECTIONS);
    socket_data = accept(socket_listen, NULL, 0);
    while(1)
    {
        Server_readMsg(socket_data);
        Server_sendMsg(socket_data);
    }
}

void Server_stop()
{
    LOG_SERVER("SERVER STOP\r\n");
    Pilot_stop();
}

static void Server_sendMsg(int socket_data)
{
    LOG_SERVER("SERVER SEND MESSAGE\r\n");
    Comm_datas_s send_datas;
    PilotState local_pilot = Pilot_getState();
    send_datas.comm_logs.collision = local_pilot.collision;
    send_datas.comm_logs.luminosity = local_pilot.luminosity;
    send_datas.comm_logs.speed = local_pilot.speed;
    send_datas.comm_logs = htonl(send_datas.comm_logs);
    write(socket_data, &send_datas, sizeof(send_datas));
}

static void Server_readMsg(int socket_data)
{
    LOG_SERVER("SERVER READ MESSAGE\r\n");
    Comm_datas_s received_datas;
    read(socket_data, &received_datas, sizeof(received_datas));
    received_datas.comm_dir = ntohl(received_datas.comm_dir);
    Server_askMvt(received_datas.comm_dir);
}

static void Server_askMvt(Comm_dir_e direction)
{
    LOG_SERVER("SERVER ASK MVT\r\n");
    VelocityVector local_vector;
    local_vector.dir = direction;
    local_vector.power = NOMINAL_POWER;
    Pilot_setVelocity(local_vector);
}