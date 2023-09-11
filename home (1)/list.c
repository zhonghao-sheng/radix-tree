/* Project: dict1
   list.c :
			  = the implementation of module linkedList of the project
*
* Created by Rong Zhao and Zhonghao Sheng
18/08/2023
* 
* Acknowledgement: 
* In this file, the functions listCreate, freeList, listAppend were adapted 
* from W3.8 of Workshop Week 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

// Data definitions, internal within list
// A list node
// The following struct was copied from struct node of W3.8 in Workshop Week 3
typedef struct node node_t;
struct node {
	void *data;    // Points to the data element of the node
	node_t *next;  // Points to the next node in the list
};

// A linked list is defined as a couple of pointers
struct list {
	node_t *head;  // Points to the first node of the list
	node_t *tail;  // Points to the last node of the list 
	size_t n;      // Number of elements in the list
};


// Creates & returns an empty linked list
// The following function was copied from function listCreate of W3.8 in Workshop Week 3
list_t *listCreate() {
	list_t *list = malloc(sizeof(*list));
	assert(list);
	list->head = list->tail = NULL;
	list->n = 0;
	return list;
}

// Free the list, with the help of function "freeData" for freeing the data component
// The following function was copied from function listFree of W3.8 in Workshop Week 3
/* Free all memory used by a list, including used by data component
    Requires argument dataFree which is a function.
    The parameter can be declared as function pointer, which can be read as broken down here:
           (*dataFree)           : dataSize is a pointer to
           (*dataFree)(void *)   :  a function that has "void *" as argument
      void (*dataFree)(void *)   :  and that returns void
    (the above break-down is application of the right-left rule, 
      see https://cseweb.ucsd.edu/~gbournou/CSE131/rt_lt.rule.html)
*/
void listFree(list_t *list, void (*dataFree)(void *)){
	assert(list != NULL);
	node_t *curr = list->head; // curr points to the first node
	while (curr) {             // While curr not reaching the end of list
		node_t *tmp = curr;
		curr = curr->next;     // Advance curr to the next next node 
		dataFree(tmp->data);       // Frees the previous node including
		free(tmp);             //    Freeing the space used by data
	}
	free(list);                // Free the list itself
} 

// Inserts a new node with value "data" to the end of "list"
// The following function was copied from function listAppend of W3.8 in Workshop Week 3
void listAppend(list_t *list, void *data) {
	assert(list);

	// Creates a new node and set data to provided value
	node_t *new = malloc(sizeof(*new));
	assert(new);
	new->data = data;
	new->next = NULL;                // "new" is the terminating node!
	if (list->head == NULL) {          // If the original list is empty
		// "new" becomes the only node of the list
		list->head = list->tail = new;
	} else {
		list->tail->next = new;  // Connects the tail node to "new"
		list->tail = new;        // Repairs the pointer "tail"
	}

	// Updates the number of elements in the list
	(list->n)++;
}

// Returns the head of a linked list
node_t *listGetHeadNode(struct list *dataList) {
	return dataList->head;
}

// the function search and then print the query key to the required file
// returns the number of matching component
int listSearchAndPrint(list_t* list, void* key, void *(*dataGetKey)(void*), 
					   int (*keyCompare)(void*, void*), 
					   void (*printData)(FILE *f, void *data, int count), 
					   FILE *outFile) {
    node_t *curr = list->head;
	int count = 0;
    while (curr != NULL) {
        if (keyCompare(dataGetKey(curr->data), key) == 0) {
            // print the data to the output file
			printData(outFile, curr->data, count);
			count++;
        }
        curr = curr->next;
    }
	// if nothing matched, print the query only
	if (count == 0) {
		fprintf(outFile, "%s\n", (char *)key);
	}
	return count;
}

