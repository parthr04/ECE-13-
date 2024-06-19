#include <stdio.h>
#include "Negotiation.h"
#include "Uart1.h"
#include "BOARD.h"

int main() {
    int iter = 0;

    printf("\nTesting Negotiation:\n");

    // Testing NegotiationHash()
    printf("\nTesting NegotiationHash()\n");

    NegotiationData secret = 11;
    NegotiationData correctHash = (secret * secret) % 0xBEEF;
    
    if (NegotiationHash(secret) == correctHash) {
        printf("\tBasic NegotiationHash() test passed\n");
        iter++;
    } else {
        printf("\tFailed basic NegotiationHash() test\n");
    }

    if (NegotiationHash(65535) == 34011 && NegotiationHash(0) == 0) {
        printf("\tOverflow and zero value tests passed for NegotiationHash()\n");
        iter += 2;
    } else {
        printf("\tFailed overflow and zero value tests for NegotiationHash()\n");
    }

    // Testing NegotiateCoinFlip()
    printf("\nTesting NegotiateCoinFlip()\n");

    switch (NegotiateCoinFlip(secret, 20)) {
    case HEADS:
        iter++;
        // fall through
    case TAILS:
        iter++;
        printf("\tPassed NegotiateCoinFlip() test\n");
        break;
    default:
        printf("\tFailed NegotiateCoinFlip() test\n");
        break;
    }

    // Testing NegotiationVerify()
    printf("\nTesting NegotiationVerify()\n");

    if (!NegotiationVerify(20, correctHash) && NegotiationVerify(secret, correctHash)) {
        printf("\tPassed basic NegotiationVerify() test\n");
        iter++;
    } else {
        printf("\tFailed basic NegotiationVerify() test\n");
    }

    if (NegotiationVerify(65535, 34011)) {
        printf("\tOverflow test passed for NegotiationVerify()\n");
        iter++;
    } else {
        printf("\tFailed overflow test for NegotiationVerify()\n");
    }

    printf("\nDone testing!\n");
    printf("\nFinal result: %d/7 tests passed!\n", iter);

    while (1);
}