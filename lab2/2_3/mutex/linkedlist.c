#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initStorage(Storage* storage) {
    storage->first = NULL;
}

Node* createNode(const char* value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        strncpy(newNode->value, value, MAX_STRING_LENGTH);
        //newNode->value[MAX_STRING_LENGTH - 1] = '\0';
        newNode->next = NULL;
        if(pthread_mutex_init(&newNode->mutex, NULL)) {
            fprintf(stderr, "Failed to init mutex for a new node\n");
            return NULL;
        }
    }
    return newNode;
}

void addToStorage(Storage* storage, const char* value) {
    Node* newNode = createNode(value);
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory for a new node\n");
        return;
    }

    if (storage->first == NULL) {
        storage->first = newNode;
    } else {
        Node* current = storage->first;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printStorage(Storage* storage) {
    Node* current = storage->first;
    printf("List Contents:\n");
    while (current != NULL) {
        printf("%s\n", current->value);
        current = current->next;
    }
}

void fillStorageRandomly(Storage* storage, int numElements) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charsetLength = sizeof(charset) - 1;

    for (int i = 0; i < numElements; ++i) {
        char randomString[MAX_STRING_LENGTH];
        int stringLength = rand() % (MAX_STRING_LENGTH - 1) + 1;

        for (int j = 0; j < stringLength; ++j) {
            randomString[j] = charset[rand() % charsetLength];
        }

        randomString[stringLength] = '\0';

        addToStorage(storage, randomString);
    }
}

void freeStorage(Storage* storage) {
    Node* current = storage->first;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        if(pthread_mutex_destroy(&temp->mutex)) {
            fprintf(stderr, "Failed to destroy mutex for a new node\n");
            return;
        }
        free(temp);
    }
}