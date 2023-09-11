/* Project: dict1, 2 & 3
   driver.c :
			  = the main program of the project 
*
* Created by Zhonghao Sheng
18/08/2023
* 
* Acknowledgement: 
* In this file, the functions getDictList and listQuerying was adapted 
* from W3.8 of Workshop Week 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "data.h"
#include "list.h"
#include "array.h"
#include "radix.h"

#define STAGE_ONE 1
#define STAGE_TWO 2
#define STAGE_THREE 3
#define LINKED_LIST STAGE_ONE
#define SORTED_LIST STAGE_TWO
#define RADIX_TREE STAGE_THREE
#define MIN_NUM_COMMAND 3

// Function to read data from "filename" and return a linked list of data
list_t *getDictList(char *filename);
// Function to read data from "filename" and return a sorted array of data
sortedArray_t *getDictSorted(char *filename);
void stageOneQuerying(char *dataFileName, FILE *inFile, FILE *stdOutFile, 
					  FILE *outFile, int stageNum);
void stageTwoQuerying(char *dataFileName, FILE *inFile, FILE *stdOutFile, 
					  FILE *outFile, int stageNum);
void stageThreeQuerying(char *dataFileName, FILE *inFile, FILE *stdOutFile, 
					  FILE *outFile, int stageNum);


int main(int argc, char *argv[]) {
	if (argc < MIN_NUM_COMMAND) {
		fprintf(stderr, "Incorrect number of input commands. ");
		exit(EXIT_FAILURE);
	}
	
	int stageNum = atoi(argv[1]);
	FILE *outFile = fopen(argv[3], "w");
	assert(outFile);

	// Runs respective query system
	switch (stageNum) {
		case STAGE_ONE:
			stageOneQuerying(argv[2], stdin, stdout, outFile, stageNum);
			break;
		case STAGE_TWO:
			stageTwoQuerying(argv[2], stdin, stdout, outFile, stageNum);
			break;
		case STAGE_THREE:
			stageThreeQuerying(argv[2], stdin, stdout, outFile, stageNum);
			break;
		
		default:
			fprintf(stderr, "Dictionary type %d not yet implemented\n", stageNum);
			exit(EXIT_FAILURE);
	}

	fclose(outFile);
	return 0;
}

// Reads data from "filename",
// Returns linked list of data
// The following function was taken from the function getDictList of Workshop3.8 Week 3
list_t *getDictList(char *filename) {
	FILE *f = fopen(filename, "r");
	assert(f);
	list_t *data = listCreate();
	skipHeaderLine(f);
	data_t *d;
	while ((d = dataRead(f)) != NULL) {
		listAppend(data, d);
	}
	fclose(f);
	return data;
}

// Reads data from "filename",
// Returns sorted array of data
sortedArray_t *getDictSorted(char *filename) {
	FILE *f = fopen(filename, "r");
	assert(f);
	sortedArray_t *data = arrayCreate(INIT_SIZE);
	skipHeaderLine(f);
	data_t *d;
	while ((d = dataRead(f)) != NULL) {
		arrayInsert(data, d, strCmp, (void *)getTradingName);
	}
	fclose(f);
	return data;
}

// Reads data from "filename",
// Returns radix tree of data
radixTree_t *getRadixTree(char *filename) {
	FILE *f = fopen(filename, "r");
	assert(f);
	radixTree_t *tree = createTree();
	skipHeaderLine(f);
	data_t *d;
	while ((d = dataRead(f)) != NULL) {
		insertKey(tree, d, (void *)getTradingName);
	}
	fclose(f);
	return tree;
}

// Querying with trading names on linked list_t
// Note that only sequential search is available for linked lists
// The following function was adapted from the function listQuerying of Workshop3.8 Week 3
void stageOneQuerying(char *dataFileName, FILE *inFile, FILE *stdOutFile, 
					  FILE *outFile, int stageNum) {
	assert(stageNum == LINKED_LIST);

	// Builds array of data
	list_t *dataList = getDictList(dataFileName);

	char *query = (char *)malloc(MAX_STRING_LEN * sizeof(char *));
	assert(query);
	
	// scanning the query from stdin
	while (fscanf(inFile, "%[^\n]\n", query) > 0) {
		int count = listSearchAndPrint(dataList, query, (void *)getTradingName, 
		strCmp, (void *)dataPrint, outFile);

		// print the corresponding result to stdout
		stageOnePrintToStdOut(stdOutFile, count, query);
	}
	free(query);
	listFree(dataList, (void *)dataFree);
}


// Querying with trading names on the sorted array
// Applying binary search on the sorted array
// The following function was adapted from the function listQuerying of Workshop3.8 Week 3
void stageTwoQuerying(char *dataFileName, FILE *inFile, FILE *stdOutFile, 
					  FILE *outFile, int stageNum) {
	assert(stageNum == SORTED_LIST);

	// Builds array of data
	sortedArray_t *arr = getDictSorted(dataFileName);

	char *query = (char *)malloc(MAX_STRING_LEN * sizeof(char *));
	assert(query);
	// scanning the query from stdin
	while (fscanf(inFile, "%[^\n]\n", query) > 0) {
		arraySearchAndPrint(arr, query, (void *)getTradingName, 
		strComForSorted, (void *)dataPrint, outFile, stdOutFile);

	}
	free(query);
	arrayShrink(arr);
	arrayFree(arr, (void *)dataFree);
}

void stageThreeQuerying(char *dataFileName, FILE *inFile, FILE *stdOutFile, 
					  FILE *outFile, int stageNum) {
	assert(stageNum == RADIX_TREE);

	// Builds radix tree of data
	radixTree_t *tree = getRadixTree(dataFileName);

	char *query = (char *)malloc(MAX_STRING_LEN * sizeof(char *));
	assert(query);
	// scanning the query from stdin
	while (fscanf(inFile, "%[^\n]\n", query) > 0) {
		radixSearchAndPrint(tree, query, 
		strCmpRadix, (void *)dataPrint, outFile, stdOutFile);

	}
	free(query);
}
