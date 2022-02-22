/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
  
#include "config.h"
#include "util.h"
#include "robot.h"
#include "../../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"
#include <stdlib.h>
#include <stdio.h>


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
    LOG_ROBOT("\r\n");
    Robot_new();   //Malloc
    Robot_init();  //affectation des champs de structure + ouverture du port avec simu
}

void Robot_stop(void)
{
    LOG_ROBOT("\r\n");
    Robot_deinit(); //deinit
    Robot_free(); //vide memoire
}

void Robot_new(void)
{
    LOG_ROBOT("\r\n");
    robot = (Robot*) malloc(sizeof(Robot));
    ASSERT_PRINTERROR(robot);
}

void Robot_free(void)
{
    LOG_ROBOT("\r\n");
    ASSERT_PRINTERROR(robot);
    free(robot);
}

void Robot_setWheelsVelocity(int mr, int ml)
{
    ASSERT_PRINTERROR(robot);
    ASSERT_PRINTERROR(Motor_setCmd(robot->motor_right, mr) != -1);
    ASSERT_PRINTERROR(Motor_setCmd(robot->motor_left, ml) != -1);
    LOG_ROBOT("Just set motorR: %d and motorL: %d\r\n", mr, ml);
}

int Robot_getRobotSpeed(void)
{
    ASSERT_PRINTERROR(robot)
    int ml = 0;
    int mr = 0;
    ASSERT_PRINTERROR((mr = Motor_getCmd(robot->motor_right)) != E_CMD);
    ASSERT_PRINTERROR((ml = Motor_getCmd(robot->motor_left)) != E_CMD);
    int speed = abs(mr + ml)/2;
    LOG_ROBOT("Speed is %d",speed);
    return speed;


}

SensorState Robot_getSensorState()
{
    SensorState state = {0};
    ContactStatus contact_status = ERROR;
    ASSERT_PRINTERROR(robot);
    contact_status = ContactSensor_getStatus(robot->contact_bumper);
    switch (contact_status)
    {
    case ERROR:
            LOG_ROBOT("ERROR\r\n");
        break;
    case RELEASED:
            state.collision = NO_BUMP;
            LOG_ROBOT("RELEASED = NO BUMP\r\n");
        break;
    case PRESSED:
            state.collision = BUMPED;
            LOG_ROBOT("PRESSED = BUMPED\r\n");
        break;
    default:
            LOG_ROBOT("Uninteresting value\r\n");
        break;
    }        
    ASSERT_PRINTERROR((state.luminosity = LightSensor_getStatus(robot->light_sensor)) != -1);
    return state;
}

static void Robot_init()
{
    LOG_ROBOT("\r\n");
    ASSERT_PRINTERROR(ProSE_Intox_init(INTOX_IP, INTOX_PORT) == 0);
    ASSERT_PRINTERROR(robot);
    ASSERT_PRINTERROR(((robot->motor_right = Motor_open(RIGHT_MOTOR)) != NULL));
    ASSERT_PRINTERROR(((robot->motor_left = Motor_open(LEFT_MOTOR)) != NULL));
    ASSERT_PRINTERROR(((robot->contact_bumper = ContactSensor_open(FRONT_BUMPER)) != NULL));
    ASSERT_PRINTERROR(((robot->contact_under = ContactSensor_open(FLOOR_SENSOR)) != NULL));
    ASSERT_PRINTERROR(((robot->light_sensor = LightSensor_open(LIGHT_SENSOR)) != NULL));
}

static void Robot_deinit()
{
    LOG_ROBOT("\r\n");
    ASSERT_PRINTERROR(robot);
    ASSERT_PRINTERROR(Motor_close(robot->motor_right) == 0);
    ASSERT_PRINTERROR(Motor_close(robot->motor_left) == 0);
    ASSERT_PRINTERROR(ContactSensor_close(robot->contact_bumper) == 0);
    ASSERT_PRINTERROR(ContactSensor_close(robot->contact_under) == 0);
    ASSERT_PRINTERROR(LightSensor_close(robot->light_sensor) == 0);
}