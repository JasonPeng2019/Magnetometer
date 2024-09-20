/*
 * queue.c
 *
 *  Created on: Sep 6, 2024
 *      Author: jason.peng
 */

#ifndef QUEUE_QUEUE_C_
#define QUEUE_QUEUE_C_

#include <stdlib.h>
#include <stdio.h>
#include "inc/queue.h"

Node * create_Node(void * data);
void free_Node(Node * node);

/* @brief: creates a dynamically allocated Node;
 * 
 * @params: data to be in node
 * 
 * @return: dynamically allocated Node
 */
Node * create_Node(void * data){
	Node * ret = malloc(sizeof(Node));
	ret->Data = data;
	ret->Next = NULL;
	return ret;
}

/*
 * @brief: free node
 * 
 * @params: node to be freed
 * 
 * @return: None
 */

void free_Node(Node * node){
	void * temp = (void*)node->Data;
	node->Data = NULL;
	node->Next = NULL;
	free(temp);
	free(node);
}


/* @brief: preps a queue and initializes values
 * 
 * @params: queue object to be prepped (must be pre-initialized)
 * 
 * @return: nothing - preps object in place
 */
void Prep_Queue(Queue * que){
	que->Head = NULL;
	que->Tail = NULL;
	que->Size = 0;
}

/*
 * @brief: load up a node with data (data) into the queue (que)
 * 
 * @params: queue object to load new node
 * @params: data to be loaded into new node
 * 
 * @return: 1 (success) or 0 (failure)
 */

uint8_t Enqueue(Queue * que, void * data){
	Node * new_Node = create_Node(data);
	if (new_Node != NULL){
		if (que->Size == 0){
			que->Head = new_Node;
			que->Tail = new_Node;
		} 
		else {
			que->Tail->Next = new_Node;
			que->Tail = new_Node;
		}
		que->Size++;
		return 1;
	} 
	else
	{
		printf("Enqueue malloc error\r\n"); // how does the printf work here?
		return 0;
	}
}


/*
 * @brief: dequeue the first item (FIFO) in the queue 
 * 
 * @params: queue to dequeue
 * 
 * @return: data of node being freed
 */
void * Dequeue(Queue * que){
	if (que->Size > 0){
		void * return_value = (void*)que->Head->Data;
		
		if (que->Size == 1){
			free(que->Head);
			que->Head = NULL;
			que->Tail = NULL;
		}
		else {
			Node * traq = que->Head;
			que->Head = traq->Next;
			free(traq);
		}
		
		que->Size--;
		return return_value;
	} else {
		return NULL;
	}
}


void Dequeue_Free(Queue * que){
	if (que->Size > 0){
		Node * traq = que->Head;
		que->Head = que->Head->Next;
		free_Node(traq);
		que->Size--;
	}
}

void * Queue_Peek(Queue * que, uint32_t index){
	if (index < que->Size){
		Node * travler = que->Head;
		uint32_t counter = 0;
		for(; counter < index; counter++)
			travler = (Node *)travler->Next;

		return travler->Data;
	}
	else {
		printf("Error in queue peek: index out of range\r\n");
		return NULL;
	}
}

#endif