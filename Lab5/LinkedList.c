#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "BOARD.h"

//**********************************************
ListItem* LinkedListNew(char* data) {
    ListItem* newItem = (ListItem*)malloc(sizeof(ListItem));
    
    if (newItem != NULL) {
        newItem->previousItem = NULL;
        newItem->nextItem = NULL;
        newItem->data = data;
    }
    
    return newItem;
}

//**********************************************
ListItem* LinkedListCreateAfter(ListItem* item, char* data) {
    ListItem* newItem = (ListItem*)malloc(sizeof(ListItem));
    
    if (newItem != NULL) {
        newItem->data = data;
        
        if (item != NULL) {
            newItem->nextItem = item->nextItem;
            newItem->previousItem = item;
            if (item->nextItem != NULL) {
                item->nextItem->previousItem = newItem;
            }
            item->nextItem = newItem;
        } else {
            newItem->previousItem = NULL;
            newItem->nextItem = NULL;
        }
    }
    
    return newItem;
}

//**********************************************
char* LinkedListRemove(ListItem* item) {
    if (item == NULL) {
        return NULL; // Return NULL to signal an error
    }

    char* data = item->data;

    if (item->previousItem != NULL) {
        item->previousItem->nextItem = item->nextItem;
    }
    if (item->nextItem != NULL) {
        item->nextItem->previousItem = item->previousItem;
    }

    free(item); // Free the memory used by the ListItem struct

    return data;
}

//**********************************************
int LinkedListSize(ListItem* list) {
    int size = 0;

    while (list != NULL) {
        size++;
        list = list->nextItem;
    }

    return size;
}

//**********************************************
ListItem* LinkedListGetFirst(ListItem* list) {
    if (list == NULL) {
        return NULL; // Return NULL for an invalid list
    }

    while (list->previousItem != NULL) {
        list = list->previousItem;
    }

    return list;
}

//**********************************************
ListItem* LinkedListGetLast(ListItem* list) {
    if (list == NULL) {
        return NULL; // Return NULL for an invalid list
    }

    while (list->nextItem != NULL) {
        list = list->nextItem;
    }

    return list;
}

//**********************************************
int LinkedListSwapData(ListItem* firstItem, ListItem* secondItem) {
    if (firstItem == NULL || secondItem == NULL) {
        return STANDARD_ERROR; // Return STANDARD_ERROR for invalid arguments
    }

    char* tempData = firstItem->data;
    firstItem->data = secondItem->data;
    secondItem->data = tempData;

    return SUCCESS;
}

//**********************************************
int LinkedListPrint(ListItem* list) {
    if (list == NULL) {
        return STANDARD_ERROR; // Return STANDARD_ERROR for a NULL list
    }

    printf("[");
    while (list != NULL) {
        printf("%s", list->data);
        list = list->nextItem;
        if (list != NULL) {
            printf(", ");
        }
    }
    printf("]\n");

    return SUCCESS;
}

