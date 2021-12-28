/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
  
#include "config.h"
#include "robot.h"
#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef LOG_ENABLE
#define LOG_ROBOT(...) do{printf(BLUE);printf(__VA_ARGS__);printf(DEFAUT);}while(0)
#else
#define LOG_ROBOT(...)
#endif

typedef struct Robot_s
{
    Motor * motor_right;
    Motor * motor_left;
    ContactSensor * contact_bumper;
    ContactSensor * contact_under;
    LightSensor * light_sensor;
} Robot;

Robot* robot;

//Static prototypes
static void Robot_init();
static void Robot_deinit();

//Public functions
void Robot_start(void)
{
    LOG_ROBOT("ROBOT START\r\n");
    Robot_new();   //Malloc
    Robot_init();  //affectation des champs de structure + ouverture du port avec simu
}

void Robot_stop(void)
{
    LOG_ROBOT("ROBOT STOP\r\n");
    Robot_deinit(); //deinit
    Robot_free(); //vide memoire
}

void Robot_new(void)
{
    LOG_ROBOT("ROBOT NEW\r\n");
    robot = (Robot*) malloc(sizeof(Robot));
}

void Robot_free(void)
{
    LOG_ROBOT("ROBOT FREE\r\n");
    if(robot)
    {
        LOG_ROBOT("ROBOT FREE\r\n");
        free(robot);
    }
    else
    {
        LOG_ROBOT("ERROR ROBOT FREE : robot is null\r\n");
    }
}

void Robot_setWheelsVelocity(int mr, int ml)
{
    LOG_ROBOT("ROBOT SET WHEELS VELOCITY\r\n");
    if(robot)
    {
        if
        (
            (Motor_setCmd(robot->motor_right, mr) == -1) ||
            (Motor_setCmd(robot->motor_left, ml) == -1)
        )
        {
            LOG_ROBOT("ERROR ROBOT SET WHEELS VELOCITY\r\n");
        }
    }
    else
    {
        LOG_ROBOT("ERROR ROBOT SET WHEELS VELOCITY : robot is null\r\n");
    }
}

int Robot_getRobotSpeed(void)
{
    LOG_ROBOT("ROBOT GET SPEED\r\n");
    if(robot)
    {
        int ml = 0;
        int mr = 0;
        if
        (
            ((mr = Motor_getCmd(robot->motor_right)) == E_CMD) ||
            ((mr = Motor_getCmd(robot->motor_right)) == E_CMD)
        )
        {
            LOG_ROBOT("ERROR ROBOT GET SPEED : error motor get command\r\n"); 
            return 0;
        }
        else
        {
            return abs(mr + ml)/2;
        }
    }
    else
    {
        LOG_ROBOT("ERROR ROBOT GET SPEED : robot is null\r\n");
        return 0;
    }

}

SensorState Robot_getSensorState()
{
    LOG_ROBOT("ROBOT GET SENSOR STATE\r\n");
    SensorState state = {0};
    ContactStatus contact_status = ERROR;
    if(robot)
    {
        contact_status = ContactSensor_getStatus(robot->contact_bumper);
        switch (contact_status)
        {
        case ERROR:
                LOG_ROBOT("ERROR ROBOT GET SENSOR STATE : error with contact bumper\r\n");
            break;
        case RELEASED:
                state.collision = NO_BUMP;
            break;
        case PRESSED:
                state.collision = BUMPED;
            break;
        default:
                LOG_ROBOT("ERROR ROBOT GET SENSOR STATE : error with switch contact status value\r\n");
            break;
        }        
        if((state.luminosity = LightSensor_getStatus(robot->light_sensor)) == -1)
        {
            LOG_ROBOT("ERROR ROBOT GET SENSOR STATE : error with light sensor value\r\n");
        }
    }
    else
    {
        LOG_ROBOT("ERROR ROBOT GET SENSOR STATE : robot is null\r\n");
        state.collision = 0;
        state.luminosity = 0;
    }
    return state;
}

static void Robot_init()
{
    LOG_ROBOT("ROBOT INIT\r\n");
    if(ProSE_Intox_init(INTOX_IP, INTOX_PORT) == 0)
    {
        if(robot)
        {
            if
            (
                ((robot->motor_right = Motor_open(RIGHT_MOTOR)) != NULL) &&
                ((robot->motor_left = Motor_open(LEFT_MOTOR)) != NULL) &&
                ((robot->contact_bumper = ContactSensor_open(FRONT_BUMPER)) != NULL) &&
                ((robot->contact_under = ContactSensor_open(FLOOR_SENSOR)) != NULL) &&
                ((robot->light_sensor = LightSensor_open(LIGHT_SENSOR)) != NULL)
            )
            {
                LOG_ROBOT("ERROR ROBOT INIT : open motors and sensors\r\n");
            }
        }
        else
        {
            LOG_ROBOT("ERROR ROBOT INIT : robot is NULL\r\n");
        }
    }
    else
    {
        LOG_ROBOT("ERROR ROBOT INIT : intox connection can't be initialized\r\n");
    }
}

static void Robot_deinit()
{
    LOG_ROBOT("ROBOT DEINIT\r\n");
    if(robot)
    {
        if
        ( 
            (Motor_close(robot->motor_right) != 0) && 
            (Motor_close(robot->motor_left) != 0) &&
            (ContactSensor_close(robot->contact_bumper) != 0) &&
            (ContactSensor_close(robot->contact_under) != 0) &&
            (LightSensor_close(robot->light_sensor) != 0)
        )
        {
            LOG_ROBOT("ERROR ROBOT DEINIT : close motors and sensors\r\n");
        }
    }
    else
    {
        LOG_ROBOT("ERROR ROBOT DEINIT : robot is NULL\r\n");
    }
}