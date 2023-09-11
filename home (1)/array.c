/* Project: dict2
   array.c :
			  = the implementation of module sorted array of the project
*
* Created by Zhonghao Sheng
04/09/2023
* 
* Acknowledgement: 
* In this file, the function skipHeaderLine was adapted 
* from W3.8 of Workshop Week 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "array.h"

// Data definitions, internal within array
// A sorted array is defined with its size, used size and the stored data
struct sortedArray {
	int size;
	int used;
	void **data;
	int numBits;
	int numChars;
	int numStrings;
};


// Creates & returns an empty sorted array
struct sortedArray *arrayCreate(int tableSize) {
	struct sortedArray *newArray  = (struct sortedArray *) malloc(sizeof(struct sortedArray));
	assert(newArray);
	newArray->size = tableSize;
	newArray->used = newArray->numBits = newArray->numChars = newArray->numStrings = 0;
	newArray->data = malloc(sizeof(*(newArray->data)) * tableSize);
	assert(newArray->data);
	return newArray;
}

// Free the array, with the help of function "freeData" for freeing the data component
void arrayFree(struct sortedArray *arr, void (*dataFree)(void *)){
	assert(arr != NULL);
	for (int i = 0; i < arr->used; i++) {
		dataFree(arr->data[i]);
	}
	free(arr->data);
	free(arr);
} 

// Shrinks the array, to reduce array size to the same 
// as the number of elements used
// The following struct was adapted from arrayShrink of W2.6 in Workshop Week 2
void arrayShrink(struct sortedArray *arr) {
	if (arr->size != arr->used) {
		// The new size should be at least INIT_SIZE
		arr->size = (arr->used > INIT_SIZE) ? arr->used : INIT_SIZE;
		arr->data = realloc(arr->data, arr->size * sizeof(*(arr->data)));
		assert(arr->data);
	}
}

// Insert the item in the sorted array
void arrayInsert(struct sortedArray *arr, void *data, int (*compare)(void *, void *), void *(*dataGetKey)(void *)){
	if (arr->used == 0) {
		arr->data[0] = data;
		(arr->used)++;
		return;
	}
	
	// if the input array is full, resize the array
    if (arr->size == arr->used) {
		arr->size <<= 1;
		void *newData = realloc(arr->data, arr->size * sizeof(*(arr->data)));
		assert(newData);
		arr->data = newData;
	}

	// compare the input data with the data inside of the array
	// insert the data while maintaining the array sorted
	int i = arr->used - 1;
	while (i >= 0 && compare(dataGetKey(data), dataGetKey(arr->data[i])) < 0) {
		arr->data[i + 1] = arr->data[i];
		i--;
	}
	arr->data[i + 1] = data;
	(arr->used)++;
}

// Binary search to find the first string that matches the given prefix
// returns -1 if not found, otherwise returns the index found
int arrayBinarySearch(struct sortedArray *arr, void *(*dataGetKey)(void *), int (*keyCompare)(char *, char *, int *, int *, int *), void *key) {
	int mid, lo = 0, hi = arr->used - 1;
	while (lo <= hi) {
		mid = (lo + hi) / 2;
		int cmpResult = keyCompare(dataGetKey(arr->data[mid]), key, &arr->numBits, &arr->numChars, &arr->numStrings);
		
		if (cmpResult == 0) {
			return mid;
		} else if (cmpResult < 0) {
			lo = mid + 1;
		} else {
			hi = mid - 1;
		}
	}
	return -1;
}

// print the the number of comparisons for bit, char and string
void printComparison(FILE *f, sortedArray_t *arr, void *key) {
	fprintf(f, "%s --> b%d c%d s%d\n", (char *)key, arr->numBits, arr->numChars, arr->numStrings);
	arr->numBits = arr->numChars = arr->numStrings = 0;
}

// the function search and then print the query key to the required file
void arraySearchAndPrint(struct sortedArray *arr, void *key, void *(*dataGetKey)(void *), 
					   int (*keyCompare)(char *, char *, int *, int *, int *), 
					   void (*printData)(FILE *, void *, int), 
					   FILE *outFile, FILE *stdOutFile) {

	int flag = 0;
	int midIndex = arrayBinarySearch(arr, dataGetKey, keyCompare, key);

	if (midIndex < 0) {
		// if unmatched, print the title only
		fprintf(outFile, "%s\n", (char *)key);
	} else {
		int leftIndex = midIndex - 1;
		while (leftIndex >= 0 && keyCompare(dataGetKey(arr->data[leftIndex]), key, &arr->numBits, &arr->numChars, &arr->numStrings) == 0) {
			leftIndex--;
		}
		for (int i = leftIndex + 1; i < midIndex; i++) {
			printData(outFile, arr->data[i], flag);
			flag += 1;
		}

		// print the data with midIndex
		printData(outFile, arr->data[midIndex], flag);
		flag += 1;

		int rightIndex = midIndex + 1;
		while (rightIndex < arr->used && keyCompare(dataGetKey(arr->data[rightIndex]), key, &arr->numBits, &arr->numChars, &arr->numStrings) == 0) {
			rightIndex++;
		}
		for (int i = midIndex + 1; i < rightIndex; i++) {
			printData(outFile, arr->data[i], flag);
		}
		printComparison(stdOutFile, arr, key);
	}
}

