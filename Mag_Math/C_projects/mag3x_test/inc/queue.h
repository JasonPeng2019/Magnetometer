/*
 *
 * queue.h
 *
 *  Created on: Sep 5, 2024
 *      Author: jason.peng
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef QUEUE_QUEUE_H_
#define QUEUE_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void * Data;
	void * Next;
} Node;

typedef struct {
	Node * Head;
	Node * Tail;
	uint32_t Size;
} Queue;

Node * create_Node(void * data);
void Prep_Queue(Queue * que);
uint8_t Enqueue(Queue * que, void * data);
void * Dequeue(Queue * que);
void Dequeue_Free(Queue * que);
void * Queue_Peek(Queue * que, uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_QUEUE_H_ */
