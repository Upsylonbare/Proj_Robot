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

Pilot* pilot;

//static functions
static void Pilot_init();
static void Pilot_deinit();

//public functions

void Pilot_start()
{
    LOG_PILOT("PILOT START\r\n");
    Pilot_new();
    Pilot_init();
    Robot_start();
    Pilot_setVelocity(pilot->velocity_vector);
}

void Pilot_stop()
{
    LOG_PILOT("PILOT STOP\r\n");
    Robot_stop();
    Pilot_deinit();
    Pilot_free();
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
                LOG_PILOT("ERROR PILOT SET VELOCITY : error with vel.dir value\r\n");        
                break;
        }
    }
    else
    {
        LOG_PILOT("ERROR PILOT SET VELOCITY : vel.dir is null\r\n");
    }
}

PilotState Pilot_getState()
{
    LOG_PILOT("PILOT GET STATE\r\n");
    SensorState sensor_state = Robot_getSensorState();
    if(pilot)
    {
        pilot->pilot_state.collision = sensor_state.collision;
        pilot->pilot_state.luminosity = sensor_state.luminosity;
        pilot->pilot_state.speed = Robot_getRobotSpeed();    
    }
    else
    {
        LOG_PILOT("ERROR PILOT GET STATE\r\n");
    }
    return pilot->pilot_state;
}

static void Pilot_init()
{
    LOG_PILOT("PILOT INIT\r\n");
    if(pilot)
    {
    pilot->pilot_state.collision = DEFAULT_COLLISION;
    pilot->pilot_state.luminosity = DEFAULT_LUMINOSITY;
    pilot->pilot_state.speed = DEFAULT_SPEED;
    pilot->velocity_vector.dir = DEFAULT_DIRECTION;
    pilot->velocity_vector.power = DEFAULT_POWER;
    }
    else
    {
        LOG_PILOT("ERROR PILOT INIT\r\n");
    }
}

static void Pilot_deinit()
{
    LOG_PILOT("PILOT DEINIT\r\n");
    if(pilot)
    {
    pilot->pilot_state.collision = 0;
    pilot->pilot_state.luminosity = 0;
    pilot->pilot_state.speed = 0;
    pilot->velocity_vector.dir = NO;
    pilot->velocity_vector.power = 0;
    }
    else
    {
        LOG_PILOT("ERROR PILOT DEINIT\r\n");
    }
}