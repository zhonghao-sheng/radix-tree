/* Project: dict1, 2 & 3
   array.h :
			  = the interface of module sorted array of the project
*
* Created by Zhonghao Sheng
04/09/2023
* 
* Acknowledgement: 
* In this file, the function skipHeaderLine was adapted 
* from W3.8 of Workshop Week 3
*/

#ifndef _ARRAY_H_
#define _ARRAY_H_
#include <stdio.h>

// Constant definitions
#define INIT_SIZE 2

// Data definitions
typedef struct sortedArray sortedArray_t;

// Creates & returns an empty sorted array
sortedArray_t *arrayCreate(int tableSize);

// Free the array, with the help of function "freeData" for freeing the data component
void arrayFree(struct sortedArray *arr, void (*dataFree)(void *));

// Shrinks the array, to reduce array size to the same 
// as the number of elements used
// The following struct was adapted from arrayShrink of W2.6 in Workshop Week 2
void arrayShrink(struct sortedArray *arr);

// Insert the item in the sorted array
void arrayInsert(struct sortedArray *arr, void *data, int (*compare)(void *, void *), void *(*dataGetKey)(void *));

// the function search and then print the query key to the required file
void arraySearchAndPrint(struct sortedArray *arr, void *key, void *(*dataGetKey)(void *), 
					   int (*keyCompare)(char *, char *, int *, int *, int *), 
					   void (*printData)(FILE *, void *, int), 
					   FILE *outFile, FILE *stdOutFile);

#endif
