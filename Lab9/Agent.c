#include "Agent.h"
#include "Ascii.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Message.h"
#include "BattleBoats.h"
#include "BOARD.h"
#include "Buttons.h"
#include "CircularBuffer.h"
#include "FieldOled.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Uart1.h"
#include "Negotiation.h"
#include "Field.h"

struct Agent {
    AgentState state;
    NegotiationData secret;
    NegotiationData hash;
    Field own;
    Field other;
    Message message;
};

static struct Agent agent;
static int turnCount = 0;;
static char *errorMSG;
static FieldOledTurn turn;

#define RAND_SIZE 0xFFFF
#define ALL_SUNK 0b00000000


void AgentInit(void) {
    agent.state = AGENT_STATE_START;
    turnCount = 0;
    turn = FIELD_OLED_TURN_NONE;
}


Message AgentRun(BB_Event event) {
    
    switch (event.type) {
        case BB_EVENT_START_BUTTON:
            if (agent.state == AGENT_STATE_START) {
                agent.secret = rand() & RAND_SIZE;
                agent.message.param0 = NegotiationHash(agent.secret);
                agent.message.type = MESSAGE_CHA;
                FieldInit(&agent.own, &agent.other);               
                FieldAIPlaceAllBoats(&agent.own);
                agent.state = AGENT_STATE_CHALLENGING;
            } else {
                agent.message.type = MESSAGE_NONE;
            }
            break;
        case BB_EVENT_RESET_BUTTON:
            agent.message.type = MESSAGE_NONE;
            char *tempWelcome = "Press BTN4 to start \nor wait for challenge\n";
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(tempWelcome);
            OledUpdate();
            AgentInit();
            return agent.message;
            break;
        case BB_EVENT_CHA_RECEIVED:            
            if (agent.state == AGENT_STATE_START) {
                agent.secret = rand() & RAND_SIZE; 
                agent.hash = event.param0;
                agent.message.type = MESSAGE_ACC;
                agent.message.param0 = agent.secret;
                FieldInit(&agent.own, &agent.other);
                FieldAIPlaceAllBoats(&agent.own);
                agent.state = AGENT_STATE_ACCEPTING;
            } else {
                agent.message.type = MESSAGE_NONE;
            }
            break;
        case BB_EVENT_ACC_RECEIVED:
            if (agent.state == AGENT_STATE_CHALLENGING) {
                agent.message.type = MESSAGE_REV;
                agent.message.param0 = agent.secret;
                NegotiationOutcome outcome = NegotiateCoinFlip(agent.secret, event.param0);
                if (outcome == HEADS) {
                    turn = FIELD_OLED_TURN_MINE;
                    agent.state = AGENT_STATE_WAITING_TO_SEND;
                } else {
                    turn = FIELD_OLED_TURN_THEIRS;
                    agent.state = AGENT_STATE_DEFENDING;
                }
            } else {
                agent.message.type = MESSAGE_NONE;
            }
            break;
        case BB_EVENT_REV_RECEIVED:
            if (agent.state == AGENT_STATE_ACCEPTING) {
                NegotiationOutcome outcome = NegotiateCoinFlip(agent.secret, event.param0);
                if (NegotiationVerify(event.param0, agent.hash) == FALSE) {
                    char *cheat = "cheating message here, press reset button to start again\n";
                    OledDrawString(cheat);
                    OledUpdate();
                    agent.state = AGENT_STATE_END_SCREEN;
                    agent.message.type = MESSAGE_NONE;
                    return agent.message;
                }
                if (outcome == TAILS) {
                    turn = FIELD_OLED_TURN_MINE;
                    GuessData guess = FieldAIDecideGuess(&agent.other);
                    agent.message.type = MESSAGE_SHO;
                    agent.message.param0 = guess.row;
                    agent.message.param1 = guess.col;
                    agent.state = AGENT_STATE_ATTACKING;
                } else {
                    agent.message.type = MESSAGE_NONE;
                    turn = FIELD_OLED_TURN_THEIRS;
                    agent.state = AGENT_STATE_DEFENDING;
                }
            } else {
                agent.message.type = MESSAGE_NONE;
            }
            break;
        case BB_EVENT_SHO_RECEIVED:
            if (agent.state == AGENT_STATE_DEFENDING) {
                GuessData opGuess;
                opGuess.row = event.param0;
                opGuess.col = event.param1;
                FieldRegisterEnemyAttack(&agent.own, &opGuess);
                agent.message.type = MESSAGE_RES;
                agent.message.param0 = event.param0;
                agent.message.param1 = event.param1;
                agent.message.param2 = opGuess.result; 
                if (FieldGetBoatStates(&agent.own) == ALL_SUNK) {
                    agent.message.type = MESSAGE_NONE;
                    char *defeat = "defeat :(\n";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(defeat);
                    OledUpdate();
                    agent.state = AGENT_STATE_END_SCREEN;
                    return agent.message;
                } else {
                    turn = FIELD_OLED_TURN_MINE;
                    agent.state = AGENT_STATE_WAITING_TO_SEND;
                }
                
            } else {
                agent.message.type = MESSAGE_NONE;
            }
            break;
        case BB_EVENT_RES_RECEIVED:
            if (agent.state == AGENT_STATE_ATTACKING) {
                GuessData ownGuess;
                ownGuess.row = event.param0;
                ownGuess.col = event.param1;
                ownGuess.result = event.param2;
                FieldUpdateKnowledge(&agent.other, &ownGuess);
                if (FieldGetBoatStates(&agent.other) == ALL_SUNK) {
                    agent.message.type = MESSAGE_NONE;
                    char *victory = "victory :)\n";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(victory);
                    OledUpdate();
                    agent.state = AGENT_STATE_END_SCREEN;
                    return agent.message;
                } else {
                    agent.message.type = MESSAGE_NONE;
                    turn = FIELD_OLED_TURN_THEIRS;
                    agent.state = AGENT_STATE_DEFENDING;
                }
            } else {
                agent.message.type = MESSAGE_NONE;
            }
            break;
        case BB_EVENT_MESSAGE_SENT:
            if (agent.state == AGENT_STATE_WAITING_TO_SEND) {
                turnCount++;
                GuessData guess = FieldAIDecideGuess(&agent.other); 
                agent.message.type = MESSAGE_SHO;
                agent.message.param0 = guess.row;
                agent.message.param1 = guess.col;               
                agent.state = AGENT_STATE_ATTACKING;                
            } else {
                agent.message.type = MESSAGE_NONE;
            }
            break;
        case BB_EVENT_ERROR:
            switch (event.param0) {
                case BB_ERROR_BAD_CHECKSUM:
                    errorMSG = "Bad checksum";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(errorMSG);
                    OledUpdate();
                    break;
                case BB_ERROR_PAYLOAD_LEN_EXCEEDED:
                    errorMSG = "Payload len exceeded";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(errorMSG);
                    OledUpdate();
                    break;
                case BB_ERROR_CHECKSUM_LEN_EXCEEDED:
                    errorMSG = "checksum len exceeded";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(errorMSG);
                    OledUpdate();
                    break;
                case BB_ERROR_CHECKSUM_LEN_INSUFFICIENT:
                    errorMSG = "checksum len insufficient";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(errorMSG);
                    OledUpdate();
                    break;
                case BB_ERROR_INVALID_MESSAGE_TYPE:
                    errorMSG = "invalid msg type";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(errorMSG);
                    OledUpdate();
                    break;
                case BB_ERROR_MESSAGE_PARSE_FAILURE:
                    errorMSG = "message parse failure";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(errorMSG);
                    OledUpdate();
                    break;
                default:
                    errorMSG = "message parse failure";
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(errorMSG);
                    OledUpdate();
                    break;
            }
            agent.state = AGENT_STATE_END_SCREEN;
            agent.message.type = MESSAGE_ERROR;
            return agent.message;
            break;
        case BB_EVENT_NO_EVENT: case BB_EVENT_SOUTH_BUTTON: case BB_EVENT_EAST_BUTTON:
            agent.message.type = MESSAGE_NONE;
            break;
    }
    OledClear(OLED_COLOR_BLACK);
    FieldOledDrawScreen(&agent.own, &agent.other, turn, turnCount);
    OledUpdate();
    return agent.message;
}

AgentState AgentGetState(void) {
    return agent.state;
}

void AgentSetState(AgentState newState) {
    agent.state = newState;
}

