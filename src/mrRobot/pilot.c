/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer
#include "config.h"
#include "pilot.h"
#include "robot.h"
#include "../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"
#include <stdlib.h>
#include <stdio.h>


#ifdef LOG_ENABLE
#define LOG_PILOT(...) do{printf(GREEN);printf(__VA_ARGS__);printf(DEFAUT);}while(0)
#else
#define LOG_PILOT(...)
#endif

typedef enum
{
    SETVEL,
    CHECK,
    STOP,
}e_event;

Pilot* pilot;

//private protorypes
static void Pilot_run(e_event event, VelocityVector velocityVector);
static void Pilot_sendMvt(VelocityVector velocityVector);
static bool_e Pilot_hasBumped();

//public functions

void Pilot_start()
{
    LOG_PILOT("PILOT START\r\n");
    Pilot_new();
    Robot_start();
}

void Pilot_stop()
{
    VelocityVector unused = {NO,0};
    Pilot_run(STOP, unused);
}

void Pilot_new()
{
    LOG_PILOT("PILOT NEW\r\n");
    pilot = (Pilot *) malloc(sizeof(Pilot));
}

void Pilot_free()
{
    LOG_PILOT("PILOT FREE\r\n");
    if(pilot)
    {
        free(pilot);
    }
    else
    {
        LOG_PILOT("ERROR PILOT FREE : pilot is null\r\n");
    }
}

void Pilot_setVelocity(VelocityVector vel)
{
    LOG_PILOT("PILOT SET VELOCITY\r\n");
    Pilot_run(SETVEL, vel);
}

PilotState Pilot_getState()
{
    LOG_PILOT("PILOT GET STATE\r\n");
    SensorState sensorstate = Robot_getSensorState();
    pilot->pilot_state.luminosity = sensorstate.luminosity;
    pilot->pilot_state.speed = Robot_getRobotSpeed();
    return pilot->pilot_state;
}

void Pilot_check()
{
    LOG_PILOT("PILOT CHECK\r\n");
    VelocityVector unused = {NO,0};

    Pilot_run(CHECK, unused);
}

//private functions

static void Pilot_run(e_event event, VelocityVector velocityVector)
{
    LOG_PILOT("PILOT RUN ");
    switch(pilot->state)
    {
    case IDLE:
        LOG_PILOT("IDLE\r\n");

        switch (event)
        {
        case SETVEL:
            if(velocityVector.dir == NO)
            {
                pilot->state = IDLE;
                velocityVector.dir = NO;
                Pilot_sendMvt(velocityVector);
            }
            else
            {
                pilot->state = RUNNING;
                Pilot_sendMvt(velocityVector);
            }
            break;
        case STOP:
            velocityVector.dir = NO;
            Pilot_sendMvt(velocityVector);
            Robot_stop();
        default:
            break;
        }
        break;

    case RUNNING:
        LOG_PILOT("RUNNING\r\n");
        switch (event)
        {
        case SETVEL:
            if(velocityVector.dir == NO)
            {
                pilot->state = IDLE;
                velocityVector.dir = NO;
                Pilot_sendMvt(velocityVector);
            }
            else
            {
                pilot->state = RUNNING;
                Pilot_sendMvt(velocityVector);
            }
            break;
        case CHECK:
            if(Pilot_hasBumped())
            {
                pilot->state = IDLE;
                velocityVector.dir = NO;
                Pilot_sendMvt(velocityVector);
            }
            else
            {
                pilot->state = RUNNING;
            }
            break;
        case STOP:
            velocityVector.dir = NO;
            Pilot_sendMvt(velocityVector);
            Robot_stop();
        default:
            break;
        }
        break;
    default:
        break;
    }
}

static void Pilot_sendMvt(VelocityVector vel)
{
    LOG_PILOT("PILOT SEND MVT\r\n");
    if(vel.dir)
    {
        switch (vel.dir)
        {
            case LEFT:
                Robot_setWheelsVelocity(-vel.power, vel.power);
                break;
            case RIGHT:
                Robot_setWheelsVelocity(vel.power, -vel.power);
                break;
            case FORWARD:
                Robot_setWheelsVelocity(vel.power, vel.power);
                break;
            case BACKWARD:
                Robot_setWheelsVelocity(-vel.power, -vel.power);
                break;
            case NO:
                Robot_setWheelsVelocity(DEFAULT_POWER, DEFAULT_POWER);
                break;
            default:
                Robot_setWheelsVelocity(DEFAULT_POWER, DEFAULT_POWER);
                LOG_PILOT("ERROR PILOT SEND MVT : error with vel.dir value\r\n");        
                break;
        }
    }
    else
    {
        LOG_PILOT("ERROR PILOT SEND MVT : vel.dir is null\r\n");
    }
}

static bool_e Pilot_hasBumped()
{
    LOG_PILOT("PILOT HAS BUMPED ?\r\n");
    SensorState local = Robot_getSensorState();
    pilot->pilot_state.collision = local.collision == BUMPED ? BUMPED : NO_BUMP;
    return local.collision == BUMPED;
}
