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

void wrlock(pthread_rwlock_t *lock){
    if (pthread_rwlock_wrlock(lock) != 0) {
        fprintf(stderr, "Some problems with wrlock node\n");
    }
}

void rdlock(pthread_rwlock_t *lock){
    if (pthread_rwlock_rdlock(lock) != 0) {
        fprintf(stderr, "Some problems with rdlock node\n");
    }
}


void unlock(pthread_rwlock_t *lock){
    if(pthread_rwlock_unlock(lock)) {
        fprintf(stderr, "Some problems with unlock node\n");
    }
}

void* swap(void* arg){
    Storage* storage = (Storage*)arg;
    Node* current;
    Node* previous;
    Node* previousPrevious;
   
    while (1) {
        previousPrevious = NULL;
	    wrlock(&(storage->first->rwlock));
        previousPrevious = storage->first;
	    wrlock(&(previousPrevious->next->rwlock));
        previous = previousPrevious->next;
	    while(1){
	        wrlock(&(previous->next->rwlock));
	        current = previous->next;
	        if (current->next == NULL){
		        unlock(&(current->rwlock));
		        unlock(&(previous->rwlock));
		        unlock(&(previousPrevious->rwlock));
		        break;
	        }
	        if (rand() % 2 == 0){
		        previousPrevious->next = current;
		        previous->next = current->next;
		        current->next = previous;
		        unlock(&(previousPrevious->rwlock));
		        previousPrevious = current;
	        }else{
	            unlock(&(previousPrevious->rwlock));
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
	    rdlock(&(storage->first->rwlock));
        current = storage->first;
	    unlock(&(storage->first->rwlock));
        previous_length = -1;

        while (current != NULL) {
	        rdlock(&(current->rwlock));
	        current_length = strlen(current->value);
            if (previous_length != -1 && previous_length < current_length) {
                ++ascending;
            }
	        previous_length = current_length;
	        unlock(&(current->rwlock));
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
	    rdlock(&(storage->first->rwlock));
        current = storage->first;
	    unlock(&(storage->first->rwlock));
        previous_length = -1;

        while (current != NULL) {
	        rdlock(&(current->rwlock));
	        current_length = strlen(current->value);
            if (previous_length != -1 && previous_length > current_length) {
                ++descending;
            }
	        previous_length = current_length;
	        unlock(&(current->rwlock));
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
	    rdlock(&(storage->first->rwlock));
        current = storage->first;
	    unlock(&(storage->first->rwlock));
        previous_length = -1;

        while (current != NULL) {
	        rdlock(&(current->rwlock));
	        current_length = strlen(current->value);
            if (previous_length != -1 && previous_length == current_length) {
                ++equal;
            }
	        previous_length = current_length;
	        unlock(&(current->rwlock));
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