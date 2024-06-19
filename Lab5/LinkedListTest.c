#include "LinkedList.h"
#include <stdio.h>
#include <string.h>

int main() {
    // Test LinkedListNew
    printf("Testing LinkedListNew...\n");
    char* testData = "Test Data";
    ListItem* newItem = LinkedListNew(testData);
    
    if (newItem != NULL && strcmp(newItem->data, testData) == 0) {
        printf("LinkedListNew Test Passed\n");
    } else {
        printf("LinkedListNew Test Failed\n");
    }

    // Test LinkedListCreateAfter
    printf("Testing LinkedListCreateAfter...\n");
    ListItem* list = LinkedListNew("First");
    ListItem* newItem2 = LinkedListCreateAfter(list, "Second");

    if (newItem2 != NULL && strcmp(newItem2->data, "Second") == 0 && list->nextItem == newItem2) {
        printf("LinkedListCreateAfter Test Passed\n");
    } else {
        printf("LinkedListCreateAfter Test Failed\n");
    }

    // Test LinkedListRemove
    printf("Testing LinkedListRemove...\n");
    char* removedData = LinkedListRemove(newItem);
    
    if (removedData != NULL && strcmp(removedData, testData) == 0) {
        printf("LinkedListRemove Test Passed\n");
    } else {
        printf("LinkedListRemove Test Failed\n");
    }

    // Test LinkedListSize
    printf("Testing LinkedListSize...\n");
    ListItem* listSizeTest = LinkedListNew("One");
    int size = LinkedListSize(listSizeTest);
    if (size == 1) {
        printf("LinkedListSize Test Passed\n");
    } else {
        printf("LinkedListSize Test Failed\n");
    }

    // Test LinkedListGetFirst
    printf("Testing LinkedListGetFirst...\n");
    ListItem* firstItem = LinkedListGetFirst(listSizeTest);
    if (firstItem != NULL && strcmp(firstItem->data, "One") == 0) {
        printf("LinkedListGetFirst Test Passed\n");
    } else {
        printf("LinkedListGetFirst Test Failed\n");
    }

    // Test LinkedListGetLast
    printf("Testing LinkedListGetLast...\n");
    ListItem* lastItem = LinkedListGetLast(listSizeTest);
    if (lastItem != NULL && strcmp(lastItem->data, "One") == 0) {
        printf("LinkedListGetLast Test Passed\n");
    } else {
        printf("LinkedListGetLast Test Failed\n");
    }

    // Test LinkedListSwapData
    printf("Testing LinkedListSwapData...\n");
    ListItem* swapItem1 = LinkedListNew("A");
    ListItem* swapItem2 = LinkedListNew("B");
    LinkedListSwapData(swapItem1, swapItem2);
    if (strcmp(swapItem1->data, "B") == 0 && strcmp(swapItem2->data, "A") == 0) {
        printf("LinkedListSwapData Test Passed\n");
    } else {
        printf("LinkedListSwapData Test Failed\n");
    }

    // Test LinkedListPrint
    printf("Testing LinkedListPrint...\n");
    ListItem* printList = LinkedListNew("One");
    LinkedListCreateAfter(printList, "Two");
    LinkedListCreateAfter(printList, "Three");
    printf("Expected Output: [One, Two, Three]\n");
    printf("Actual Output: ");
    LinkedListPrint(printList);
    
    
}
