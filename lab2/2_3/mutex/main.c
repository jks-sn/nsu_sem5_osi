#include "linkedlist.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int ascending = 0;
int descending = 0;
int equal = 0;

#define LENLIST 1000

void lock(pthread_mutex_t *lock){
    if (pthread_mutex_lock(lock)) {
        fprintf(stderr, "Failed to lock mutex\n");;
    }
}

void unlock(pthread_mutex_t *lock){
    if(pthread_mutex_unlock(lock)) {
        fprintf(stderr, "Failed to unlock mutex\n");
    }
}

void* swap(void* arg){
    Storage* storage = (Storage*)arg;
    Node* current;
    Node* previous;
    Node* previousPrevious;
   
    while (1) {
        previousPrevious = NULL;
        if(storage->first == NULL) {
            fprintf(stdout, "Linked list is empty");
            return NULL;
        }
        lock(&(storage->first->mutex));
        previousPrevious = storage->first;
        if(previousPrevious->next == NULL) {
            fprintf(stdout, "Linked list is too small(size = 1) for swap");
            return NULL;
        }
        lock(&(previousPrevious->next->mutex));
        previous = previousPrevious->next;
        if(previous->next == NULL) {
                fprintf(stdout, "Linked list is too small(size = 2) for swap");  //ну в теории конечно можно, но зачем?
                return NULL;
        }
        while(1) {
            lock(&(previous->next->mutex));
            current = previous->next;
            if (current->next == NULL) {
                unlock(&(current->mutex));
                unlock(&(previous->mutex));
                unlock(&(previousPrevious->mutex));
                break;
            }
            if (rand() % 2 == 0) {
                previousPrevious->next = current;
                previous->next = current->next;
                current->next = previous;
                unlock(&(previousPrevious->mutex));
                previousPrevious = current;
            }else {
                unlock(&(previousPrevious->mutex));
                previousPrevious = previous;
                previous = current;
            }
        }	
    }
}

void* searchAscending(void* arg) {
    Storage* storage = (Storage*)arg;
    Node* current;
    int previous_length;
    int current_length;

    while (1) {
	    lock(&(storage->first->mutex));
        current = storage->first;
	    unlock(&(storage->first->mutex));
        previous_length = -1;

            while (current != NULL) {
	        lock(&(current->mutex));
	            current_length = strlen(current->value);
            if (previous_length != -1 && previous_length < current_length) {
                ++ascending;
            }
	    previous_length = current_length;
	    unlock(&(current->mutex));
            current = current->next;
        }
    }

    return NULL;
}

void* searchDescending(void* arg) {
    Storage* storage = (Storage*)arg;
    Node* current;
    int previous_length;
    int current_length;

    while (1) {
	    lock(&(storage->first->mutex));
        current = storage->first;
	    unlock(&(storage->first->mutex));
        previous_length = -1;

        while (current != NULL) {
	    lock(&(current->mutex));
	    current_length = strlen(current->value);
        if (previous_length != -1 && previous_length > current_length) {
            ++descending;
        }
	    previous_length = current_length;
	    unlock(&(current->mutex));
            current = current->next;
        }
    }

    return NULL;
}

void* searchEqualLength(void* arg) {
    Storage* storage = (Storage*)arg;
    Node* current;
    int previous_length;
    int current_length;

    while (1) {
	lock(&(storage->first->mutex));
        current = storage->first;
	unlock(&(storage->first->mutex));
        previous_length = -1;

        while (current != NULL) {
	        lock(&(current->mutex));
	        current_length = strlen(current->value);
            if (previous_length != -1 && previous_length == current_length) {
                ++equal;
            }
	        previous_length = current_length;
	        unlock(&(current->mutex));
            current = current->next;
        }
    }
    return NULL;
}

void* listMonitor(){
    int i = 1;
    while(1) {
	    printf("%d) list stats: ascending length %d, descending length %d, equal length %d\n",i++, ascending, descending, equal);
	    sleep(1);
    }
}

int main() {
    pthread_t tid_mon, tid_asc, tid_des, tid_eq;
    pthread_t tid_swap_1, tid_swap_2, tid_swap_3;
    int err;
    Storage storage;
    initStorage(&storage);
    fillStorageRandomly(&storage, LENLIST);    
    err = pthread_create(&tid_mon, NULL, listMonitor, &storage);
    if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
	    return -1;
    }
    err = pthread_create(&tid_asc, NULL, searchAscending, &storage);
    if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
	    return -1;
    }
    err = pthread_create(&tid_des, NULL, searchDescending, &storage);
    if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
	    return -1;
    }
    err = pthread_create(&tid_eq, NULL, searchEqualLength, &storage);
    if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
	    return -1;
    }
    err = pthread_create(&tid_swap_1, NULL, swap, &storage);
    if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
	    return -1;
    }
    err = pthread_create(&tid_swap_2, NULL, swap, &storage);
    if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
	    return -1;
    }
    err = pthread_create(&tid_swap_3, NULL, swap, &storage);
    if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
	    return -1;
    }

    err = pthread_join(tid_swap_1, NULL);
    if(err) {
	    printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_join(tid_swap_2, NULL);
    if(err) {
	    printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_join(tid_swap_3, NULL);
    if(err) {
	    printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_join(tid_mon, NULL);
    if(err) {
	    printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_join(tid_asc, NULL);
    if(err) {
	    printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_join(tid_des, NULL);
    if(err) {
	    printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_join(tid_eq, NULL);
    if(err) {
	    printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }

    freeStorage(&storage);

    return 0;
}