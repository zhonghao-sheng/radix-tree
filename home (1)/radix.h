/* Project: dict3
   radix.h :
			  = the interface of module radix tree of the project
*
* Created by Zhonghao Sheng
04/09/2023
* 
* Acknowledgement: 
* In this file, the function skipHeaderLine was adapted 
* from W3.8 of Workshop Week 3
*/

#ifndef _RADIX_H_
#define _RADIX_H_
#include <stdio.h>

// Constant definitions
#define BIT_LEN 8

// Data definitions
typedef struct radixNode radixNode_t;
typedef struct radixTree radixTree_t;

// Function to create a new radix tree and initialize it
struct radixTree *createTree();

// Function to create a new radix tree node
struct radixNode *createNode(int prefixBits, char *prefix, void *data);

// // Function to free memory allocated for the radix tree
// void freeRadixTree(RadixNode *node);

// The function retrieves the nth bit from a character in the key.
int getBit(char key, int n);

// The function extracts a specified number of bits from a given key, starting from a particular position
char *splitStem(char *key, int start, int length);

// Function to insert a key and associated data into the radix tree
struct radixTree *insertKey(struct radixTree *tree, void *data, void *(*dataGetKey)(void *));

// the function search and then print the query key to the required file
void radixSearchAndPrint(struct radixTree *tree, char *key, 
					   int (*keyCompare)(char *, char *, int, int, int *, int *, int *), 
					   void (*printData)(FILE *, void *, int), 
					   FILE *outFile, FILE *stdOutFile);

// print the the number of comparisons for bit, char and string
void printComparisonRadix(FILE *f, struct radixTree *tree, char *key);


#endif
