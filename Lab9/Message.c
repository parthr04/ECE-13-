#include <stdint.h>
#include "BattleBoats.h"
#include <string.h>
#include "Message.h"
#include <stdio.h>
#include "BOARD.h"


typedef enum {
    STATE_IDLE,
    RECORDING_DATA,
    RECORDING_CHECKSUM,
} DataDecodeState;

#define dataLength 2
#define BD4 70
#define BD3 48
#define BD2 57
#define BD1 65
#define LINE_END_CHAR '\n'
#define DATA_END_CHAR '*'
#define INIT_CHAR '$'
#define BinToHex "0123456789ABCDEF"

static int dataCounter = 0;

static char dataBuffer[MESSAGE_MAX_PAYLOAD_LEN];

static char validationString[3];

static DataDecodeState currentState = STATE_IDLE;


//*****************************************************************************



/**
 * Given a payload string, calculate its checksum
 * 
 * @param payload       //the string whose checksum we wish to calculate
 * @return   //The resulting 8-bit checksum 
 */

uint8_t Message_CalculateChecksum(const char* payload) {
    uint8_t result = 0;
    for (int x = 0; x < strlen(payload); x++) {
        result ^= payload[x];
    }
    return result;
}



//******************************************************************************

/**
 * ParseMessage() converts a message string into a BB_Event.  The payload and
 * checksum of a message are passed into ParseMessage(), and it modifies a
 * BB_Event struct in place to reflect the contents of the message.
 * 
 * @param payload       //the payload of a message
 * @param checksum      //the checksum (in string form) of  a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //A BB_Event which will be modified by this function.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checksum
 *              the checksum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */

int Message_ParseMessage(const char* payload, const char* checksum_str, BB_Event * msg_event) {
    
    // Reset parameters
    msg_event->param0 = 0;
    msg_event->param1 = 0;
    msg_event->param2 = 0;

    char payloadCopy[MESSAGE_MAX_PAYLOAD_LEN];
    strncpy(payloadCopy, payload, MESSAGE_MAX_PAYLOAD_LEN - 1);
    payloadCopy[MESSAGE_MAX_PAYLOAD_LEN - 1] = '\0'; // Ensures null-termination

    // Checksum string must be length 2
    if (strlen(checksum_str) != 2) {
        msg_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    // Convert the checksum string
    uint8_t calculatedChecksum = strtoul(checksum_str, NULL, 16);
    
    // Compare calculated and provided checksums
    if (Message_CalculateChecksum(payload) != calculatedChecksum) {
        msg_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    // Tokenizing the string// Change a bit//
    char *token = strtok(payloadCopy, ",");
    int tokenCount;
    if (strcmp(token, "CHA") == 0) {
        tokenCount = 1;
        msg_event->type = BB_EVENT_CHA_RECEIVED;
    } else if (strcmp(token, "ACC") == 0) {
        tokenCount = 1;
        msg_event->type = BB_EVENT_ACC_RECEIVED;
    } else if (strcmp(token, "SHO") == 0) {
        tokenCount = 2;
        msg_event->type = BB_EVENT_SHO_RECEIVED;
    } else if (strcmp(token, "REV") == 0) {
        tokenCount = 1;
        msg_event->type = BB_EVENT_REV_RECEIVED;
    } else if (strcmp(token, "RES") == 0) {
        tokenCount = 3;
        msg_event->type = BB_EVENT_RES_RECEIVED;
    } else {
        msg_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    // Parsing tokens
    for (int i = 0; i < tokenCount; i++) {
        token = strtok(NULL, ",");
        if (!token) {
            msg_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }
        
        // Convert token to integer
        uint16_t paramValue = atoi(token);
        switch (i) {
            case 0: msg_event->param0 = paramValue; break;
            case 1: msg_event->param1 = paramValue; break;
            case 2: msg_event->param2 = paramValue; break;
        }
    }
    
    // Check for extra tokens//change a bit 
    if (strtok(NULL, ",")) {
        msg_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    // Success
    return SUCCESS;
}

//******************************************************************************

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 */
int Message_Encode(char *message_string, Message message_to_encode) {
    char payload[MESSAGE_MAX_PAYLOAD_LEN];
    char finalMessage[MESSAGE_MAX_LEN];
    uint8_t checksum;

    // Handling different message types
    switch (message_to_encode.type) {
        case MESSAGE_NONE:
            return 0;
        case MESSAGE_ACC:
            sprintf(payload, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            break;
        case MESSAGE_CHA:
            sprintf(payload, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            break;
        case MESSAGE_SHO:
            sprintf(payload, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            break;
        case MESSAGE_REV:
            sprintf(payload, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            break;
        case MESSAGE_RES:
            sprintf(payload, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
            break;
        case MESSAGE_ERROR:
            // For MESSAGE_ERROR, no specific action is defined
            return -1;
    }

    checksum = Message_CalculateChecksum(payload);

    // Construct the complete message
    sprintf(finalMessage, MESSAGE_TEMPLATE, payload, checksum);
    strcpy(message_string, finalMessage);

    // Return the length of the encoded message
    return strlen(message_string);
}

//******************************************************************************

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                          if char_in is the last character of an invalid message,
 *                              then decoded_message should have an ERROR type.
 *                          otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event *decoded_message_event) {

    // Check the current state of decoding
    switch (currentState) {
        case STATE_IDLE:
            // Wait for the initial character ('$'). If not received, return error.
            if (char_in == INIT_CHAR) {
                currentState = RECORDING_DATA;
                dataCounter = 0; // Reset the counter for new message
            } else {
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                return STANDARD_ERROR;
            }
            break;

        case RECORDING_DATA:
            // Record the data until '*' is received.
            if (char_in == DATA_END_CHAR) {
                dataBuffer[dataCounter] = '\0'; // Add null terminator
                currentState = RECORDING_CHECKSUM;
                dataCounter = 0; // Reset counter for checksum
            } else if (dataCounter < MESSAGE_MAX_PAYLOAD_LEN) {
                dataBuffer[dataCounter++] = char_in;
            } else {
                // Payload length exceeded
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_PAYLOAD_LEN_EXCEEDED;
                currentState = STATE_IDLE;
                return STANDARD_ERROR;
            }
            break;

        case RECORDING_CHECKSUM:
            // Record checksum until '\n' is received.
            if (char_in == LINE_END_CHAR) {
                validationString[dataCounter] = '\0'; // Null terminate checksum string
                currentState = STATE_IDLE; // Return to idle state for next message
                dataCounter = 0;
                // Parse message and checksum
                int result = Message_ParseMessage(dataBuffer, validationString, decoded_message_event);
                return result ? SUCCESS : STANDARD_ERROR;
            } else if (dataCounter < dataLength) {
                validationString[dataCounter++] = char_in;
            } else {
                // Checksum length exceeded
                currentState = STATE_IDLE;
                return STANDARD_ERROR;
            }
            break;
    }

    return SUCCESS;
}