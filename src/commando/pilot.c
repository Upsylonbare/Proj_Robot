/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer
#include "config.h"
#include "util.h"
#include "pilot.h"
#include "robot.h"
#include "../../../infox_prose-x86_64-v0.3/include/infox/prose/prose.h"
#include <stdlib.h>
#include <stdio.h>

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

char* eventAsText[EVENT_NB] = 
{
    "Init",
    "Set Velocity",
    "Set Velocity is stop",
    "Set Velocity is not stop",
    "Check",
    "Check is bump",
    "Check is not bump",
    "Stop"
};

typedef enum
{
    A_NOP = 0,
    A_GO_DEATH,
    A_SEND_MVT,
    A_CHECKVEL,
    A_CHECKBUMP,
    ACTION_NB
}action_e;

char* actionAsText[ACTION_NB] = 
{
    "No Action",
    "Go die",
    "Send Mvt",
    "Check Vel",
    "Check Bump"
};

char* stateAsText[STATE_NB] =
{
    "Forget",
    "Idle",
    "Running",
    "Set Vel",
    "Check",
    "Dead"
};

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
    LOG_PILOT("\r\n");
    Pilot_new();
    Robot_start();
}

void Pilot_stop()
{
    LOG_PILOT("\r\n");
    VelocityVector unused = {NO,0};
    Pilot_run(E_STOP, unused);
}

void Pilot_new()
{
    pilot = (Pilot *)malloc(sizeof(Pilot));
    ASSERT_PRINTERROR(pilot);
    pilot->state = S_IDLE;

}

void Pilot_free()
{
    LOG_PILOT("\r\n");
    ASSERT_PRINTERROR(pilot);
    free(pilot);
}

void Pilot_setVelocity(VelocityVector vel)
{
    LOG_PILOT("\r\n");
    Pilot_run(E_SETVEL, vel);
}

PilotState Pilot_getState()
{
    LOG_PILOT("\r\n");
    SensorState sensorstate = Robot_getSensorState();
    pilot->pilot_state.luminosity = sensorstate.luminosity;
    pilot->pilot_state.speed = Robot_getRobotSpeed();
    return pilot->pilot_state;
}

void Pilot_check()
{
    LOG_PILOT("\r\n");
    VelocityVector unused = {NO,0};
    Pilot_run(E_CHECK, unused);
}

//private functions

static void Pilot_run(event_e event, VelocityVector velocityVector)
{
    LOG_PILOT("PILOT RUN\r\n");

    transition_s aTransition = mySM[pilot->state][event];
    state_e aState = aTransition.destState;
    LOG_PILOT("I'm in state %s and I'm going in state %s\r\n", stateAsText[pilot->state], stateAsText[aState]);
    LOG_PILOT("because I just received the event %s so I do the action %s\r\n", eventAsText[event], actionAsText[aTransition.transAction]);
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
    ASSERT_PRINTERROR(vel.dir);
    switch (vel.dir)
    {
        case LEFT:
            Robot_setWheelsVelocity(-vel.power, vel.power);
            LOG_PILOT("New direction is Left\r\n");
            break;
        case RIGHT:
            Robot_setWheelsVelocity(vel.power, -vel.power);
            LOG_PILOT("New direction is Right\r\n");
            break;
        case FORWARD:
            Robot_setWheelsVelocity(vel.power, vel.power);
            LOG_PILOT("New direction is Forward\r\n");
            break;
        case BACKWARD:
            Robot_setWheelsVelocity(-vel.power, -vel.power);
            LOG_PILOT("New direction is Backward\r\n");
            break;
        case NO:
            Robot_setWheelsVelocity(DEFAULT_POWER, DEFAULT_POWER);
            LOG_PILOT("New direction is Stop\r\n");
            break;
        default:
            Robot_setWheelsVelocity(DEFAULT_POWER, DEFAULT_POWER);
            LOG_PILOT("Something werid with vel.dir value\r\n");        
            break;
    }
}

static bool_e Pilot_hasBumped()
{
    LOG_PILOT("\r\n");
    SensorState local = Robot_getSensorState();
    pilot->pilot_state.collision = local.collision == BUMPED ? BUMPED : NO_BUMP;
    return local.collision == BUMPED;
}

static void Pilot_performAction(action_e action, VelocityVector velocityVector)
{
    LOG_PILOT("\r\n");
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