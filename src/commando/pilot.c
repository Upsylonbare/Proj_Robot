/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer
#include "config.h"
#include "pilot.h"
#include "robot.h"
#include "../../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"
#include <stdlib.h>
#include <stdio.h>


#ifdef LOG_ENABLE
#define LOG_PILOT(...) do{printf(GREEN);printf(__VA_ARGS__);printf(DEFAUT);}while(0)
#else
#define LOG_PILOT(...)
#endif

typedef enum
{
    E_INITPILOT = 0,
    E_SETVEL,
    E_SETVEL_IS_STOP,
    E_SETVEL_NOT_STOP,
    E_CHECK,
    E_CHECK_IS_BUMP,
    E_CHECK_NOT_BUMP,
    E_STOP,
    EVENT_NB
}event_e;

typedef enum
{
    A_NOP = 0,
    A_GO_DEATH,
    A_SEND_MVT,
    A_CHECKVEL,
    A_CHECKBUMP
}action_e;

typedef struct
{
    state_e destState;
    action_e transAction;
}transition_s;

Pilot* pilot;

static transition_s mySM[STATE_NB][EVENT_NB] = 
{
    [S_IDLE][E_STOP] = {S_DEATH, A_GO_DEATH},
    [S_IDLE][E_SETVEL] = {S_SETVEL, A_CHECKVEL},
    [S_RUNNING][E_STOP] = {S_DEATH, A_GO_DEATH},
    [S_RUNNING][E_SETVEL] = {S_SETVEL, A_CHECKVEL},
    [S_RUNNING][E_CHECK] = {S_CHECK, A_CHECKBUMP},
    [S_SETVEL][E_SETVEL_IS_STOP] = {S_IDLE, A_NOP},
    [S_SETVEL][E_SETVEL_NOT_STOP] = {S_RUNNING, A_SEND_MVT},
    [S_CHECK][E_CHECK_NOT_BUMP] =  {S_RUNNING, A_NOP},
    [S_CHECK][E_CHECK_IS_BUMP] = {S_IDLE, A_NOP},
};

//private protorypes
static void Pilot_run(event_e event, VelocityVector velocityVector);
static void Pilot_sendMvt(VelocityVector velocityVector);
static bool_e Pilot_hasBumped();
static void Pilot_performAction(action_e action, VelocityVector velocityVector);

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
    Pilot_run(E_STOP, unused);
}

void Pilot_new()
{
    LOG_PILOT("PILOT NEW\r\n");
    pilot = (Pilot *)malloc(sizeof(Pilot));
    pilot->state = S_IDLE;
    printf("Pilot State : %d\r\n", pilot->state);
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
    Pilot_run(E_SETVEL, vel);
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

    Pilot_run(E_CHECK, unused);
}

//private functions

static void Pilot_run(event_e event, VelocityVector velocityVector)
{
    LOG_PILOT("PILOT RUN\r\n");

    transition_s aTransition = mySM[pilot->state][event];
    state_e aState = aTransition.destState;
    // printf("Pilot State : %d going in %d\r\n", pilot->state, aState);
    // printf("Pilot Event : %d so I do %d\r\n", event, aTransition.transAction);
    //printf("New state is %d\r\n", aState);
    if(aState != S_FORGET)
    {
        pilot->state = aState;
        Pilot_performAction(aTransition.transAction, velocityVector);
    }
    if(aState == S_IDLE)
    {
        VelocityVector unused = {NO,0};
        Pilot_sendMvt(unused);
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

static void Pilot_performAction(action_e action, VelocityVector velocityVector)
{
    LOG_PILOT("PILOT PERFORM ACTION\r\n");
    switch (action)
    {
    case A_NOP:
        //nothing
        break;
    case A_GO_DEATH:
    {   
        VelocityVector unused = {NO,0};
        Pilot_sendMvt(unused);
        Robot_stop();
        break;
    }
    case A_SEND_MVT:
        Pilot_sendMvt(velocityVector);
        break;  
    case A_CHECKVEL:
        if(velocityVector.dir == NO)
        {
            Pilot_run(E_SETVEL_IS_STOP, velocityVector);
        }
        else
        {
            Pilot_run(E_SETVEL_NOT_STOP, velocityVector);
        }
        break;
    case A_CHECKBUMP:
        if(Pilot_hasBumped())
        {
            Pilot_run(E_CHECK_IS_BUMP, velocityVector);
        }
        else
        {
            Pilot_run(E_CHECK_NOT_BUMP, velocityVector);
        }
        break;
    default:
        break;
    }
}