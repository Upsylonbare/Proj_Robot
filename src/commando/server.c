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
#include "util.h"
#include "pilot.h"
#include "server.h"
#include "config.h"

static void Server_sendMsg(int socket_data);
static void Server_readMsg(int socket_data);
static void Server_askMvt(Comm_order_e direction);

static int socket_listen;
static int not_received_quit_char;

char* orderAsText[O_ORDER_NB] = {
    "Quit",
    "Left",
    "Right",
    "Forward",
    "Backward",
    "No",
    "Logs"
};

void Server_start()
{
    LOG_SERVER("\r\n");
    int socket_data;
    int opt = 1;
    struct sockaddr_in server_address;
    not_received_quit_char = 1;
    Pilot_start();
    /* Creation du socket : AF_INET = IP, SOCK_STREAM = TCP */
    socket_listen = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_listen == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("failed setsockopt");
        exit(EXIT_FAILURE);
    }
    /* Type d'adresse = IP */
    server_address.sin_family = AF_INET;
    /* Port TCP ou le service est accessible */
    server_address.sin_port = htons(SERVER_PORT);
    /* On s'attache a toutes les interfaces */
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
     /* On attache le socket a l'adresse indiquee */
    LOG_SERVER("SERVER is on port %d\r\n", SERVER_PORT);
    if(bind(socket_listen, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    /* Mise en ecoute du socket */
    if(listen(socket_listen, MAX_PENDING_CONNECTIONS) == -1)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    };
    socket_data = accept(socket_listen, NULL, 0);
    if(socket_data == -1)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    while(not_received_quit_char)
    {
        Server_readMsg(socket_data);
    }
}

void Server_stop()
{
    LOG_SERVER("\r\n");
    close(socket_listen);
    Pilot_stop();
}

static void Server_sendMsg(int socket_data)
{
    Comm_logs_s send_logs;
    Pilot_check();
    PilotState local_pilot = Pilot_getState();
    LOG_SERVER("Pilot luminosity: %f speed: %d collision: %d\r\n", local_pilot.luminosity, local_pilot.speed, local_pilot.collision);
    send_logs.collision = htonl(local_pilot.collision);
    send_logs.luminosity = htonl(local_pilot.luminosity);
    send_logs.speed = htonl(local_pilot.speed);
    write(socket_data, &send_logs, sizeof(send_logs));
    LOG_SERVER("Just send Pilot luminosity: %f speed: %d collision: %d\r\n",send_logs.luminosity, send_logs.speed, send_logs.collision);
}

static void Server_readMsg(int socket_data)
{
    Comm_order_e received_order;
    read(socket_data, &received_order, sizeof(received_order));
    received_order = ntohl(received_order);
    LOG_SERVER("Juste received %s order\r\n", orderAsText[received_order]);
    if(received_order == O_LOG)
    {
        Server_sendMsg(socket_data);
    }
    else if(received_order == O_QUIT)
    {
        not_received_quit_char = 0;
    }
    else
    {
        Server_askMvt(received_order);
    }
}

static void Server_askMvt(Comm_order_e direction)
{
    VelocityVector local_vector;
    local_vector.dir = direction;
    LOG_SERVER("Client ask %s direction\r\n", orderAsText[direction]);
    local_vector.power = NOMINAL_POWER;
    Pilot_setVelocity(local_vector);
}