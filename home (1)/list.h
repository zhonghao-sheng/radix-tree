/* Project: dict1
   list.h :
			  = the interface of module linkedList of the project
*
* Created by Rong Zhao and Zhonghao Sheng
18/08/2023
* 
* Acknowledgement: 
* In this file, the functions listCreate, freeList, listAppend were adapted 
* from W3.8 of Workshop Week 3
*/

#ifndef _LIST_H_
#define _LIST_H_
#include <stdio.h>

// Type definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct list list_t;
typedef struct node node_t;

// Creates & returns an empty linked list
// The following function was copied from function listCreate of W3.8 in Workshop Week 3
list_t *listCreate();

// Free the list, with the help of function "freeData" for freeing the data component
// The following function was copied from function listFree of W3.8 in Workshop Week 3
void listFree(list_t *list, void (*dataFree)(void *));

// Inserts a new node with value "data" to the end of "list" 
// The following function was copied from function listAppend of W3.8 in Workshop Week 3
void listAppend(list_t *list, void *data); 

// Returns the head of a linked list
node_t *listGetHeadNode(struct list *dataList);

// Returns the head of a linked list
void *nodeGetData(node_t *dataNode);

// the function search and then print the query key to the required file
// returns the number of matching component
int listSearchAndPrint(list_t* list, void* key, void *(*dataGetKey)(void*), 
                       int (*keyCompare)(void*, void*), 
                       void (*printData)(FILE *f, void *data, int count), 
                       FILE *outFile);

#endif
