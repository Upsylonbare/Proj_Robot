#ifndef CONFIG_H
#define CONFIG_H

#define LEFT_MOTOR MD
#define RIGHT_MOTOR MA
#define LIGHT_SENSOR S1
#define FRONT_BUMPER S3
#define FLOOR_SENSOR S2

#define LEFT_CHAR       'q'
#define RIGHT_CHAR      'd'
#define FORWARD_CHAR    'z'
#define BACKWARD_CHAR   's'
#define STOP_CHAR       'e'
#define QUIT_CHAR       'a'
#define CLEAR_CHAR      'c'
#define LOG_CHAR        'l'

#define DEFAULT_DIRECTION   NO
#define DEFAULT_POWER       0
#define DEFAULT_SPEED       0
#define DEFAULT_COLLISION   0
#define DEFAULT_LUMINOSITY  0

#define NOMINAL_POWER   50

#define INTOX_IP        "0.0.0.0"
#define INTOX_PORT      12345

#define SERVER_PORT     54321
#define MAX_PENDING_CONNECTIONS (5)

#endif /* CONFIG_H */





