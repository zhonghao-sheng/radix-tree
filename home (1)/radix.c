/* Project: dict3
	radix.c :
			  = the implementation of module radix tree of the project
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
#include "radix.h"

// Structure for a radix tree node
struct radixNode {
	int prefixBits;
	char *prefix;
	struct radixNode *branchA;
	struct radixNode *branchB;
	void **data;
	int dataCount;
};

// Structure for the radix tree
struct radixTree {
	struct radixNode *root;
	int numBits;
	int numChars;
	int numStrings;
};

// Function to create a new radix tree and initialize it
struct radixTree *createTree() {
	struct radixTree *tree = (struct radixTree *)malloc(sizeof(struct radixTree));
	assert(tree);
	tree->root = NULL;
	tree->numBits = tree->numChars = tree->numStrings = 0;
	return tree;
}

// Function to create a new radix tree node
struct radixNode *createNode(int prefixBits, char *prefix, void *data) {
	struct radixNode *node = (struct radixNode *)malloc(sizeof(struct radixNode));
	assert(node);

	// setup for prefix
	node->prefixBits = prefixBits;
	node->prefix = strdup(prefix);

	node->branchA = NULL;
	node->branchB = NULL;

	// setup for the data array
	node->data = (void **)malloc(sizeof(void *));
	assert(node->data);
	node->data[0] = data;
	(node->dataCount)++;

	return node;
}

// // Function to free memory allocated for the radix tree
// void freeRadixTree(RadixNode *node) {
//     freeRadixTree(node->branchA);
//     freeRadixTree(node->branchB);
//     free(node->prefix);
//     free(node);
// }

// The function retrieves the nth bit from a character in the key.
int getBit(char key, int n) {
	 return (key >> (BIT_LEN - 1 - n)) & 1;
}

// Function to extract a specified number of bits from a given key starting from a particular position
// The incomplete part of the string will be stored as 0
// char *splitStem(char *key, int start, int length) {
//     assert(key);

//     int charIndex = start / 8;
//     int bitOffset = start % 8;
//     int remainingBits = length;
//     int resultIndex = 0;
//     // char *result = (char *)malloc((length / 8 + 1) * sizeof(char));
// 	// assert(result);
// 	// Calculate the size of the result buffer based on the number of bits to extract
//     int resultSize = (length / 8) + ((length % 8 != 0) ? 1 : 0);
//     char *result = (char *)malloc(resultSize + 1);
// 	assert(result);

// 	// Initialize the result buffer with zeros
// 	memset(result, 0, resultSize + 1);

//     // Extract the bits from the incomplete character
//     if (bitOffset > 0) {
//         char currentChar = key[charIndex];
//         int bitsToCopy = 8 - bitOffset;

//         char bits = (currentChar >> (8 - bitsToCopy - bitOffset)) & ((1 << bitsToCopy) - 1);
//         result[resultIndex] |= (bits << (8 - bitOffset - bitsToCopy));

//         // Update offsets and counters
//         remainingBits -= bitsToCopy;
//         bitOffset = 0;
//         resultIndex++;
//         charIndex++;
//     }

//     while (remainingBits > 0) {
//         char currentChar = key[charIndex];

//         // Calculate the number of bits to copy from the current character
//         int bitsToCopy = (remainingBits < (8 - bitOffset)) ? remainingBits : (8 - bitOffset);

//         // Extract the bits from the current character and store them in the result
//         char bits = (currentChar >> (8 - bitsToCopy - bitOffset)) & ((1 << bitsToCopy) - 1);

//         // If we are starting with a new character in key, simply store the bits
//         result[resultIndex] |= (bits << (8 - bitOffset - bitsToCopy));

//         // Update offsets and counters
//         remainingBits -= bitsToCopy;
//         bitOffset = 0;
//         resultIndex++;

//         if (remainingBits <= 0 || (resultIndex >= resultSize)) {
//             break;
//         }

//         if (bitOffset == 0) {
//             charIndex++;
//         }
//     }

//     result[resultIndex] = '\0';
//     return result;
// }

char *splitStem(char *input, int start, int length) {
    // Calculate the number of bytes needed to store the extracted bits
    int bytesNeeded = (length + 7) / 8; // Round up to the nearest byte
    char *result = (char *)malloc(bytesNeeded + 1); // +1 for the null-terminator
	assert(result);

    // Initialize the result string with all zeros
    for (int i = 0; i < bytesNeeded; i++) {
        result[i] = 0;
    }

    // Copy bits from 'input' to 'result'
    int resultBitIndex = 0;
    for (int i = start; i < start + length; i++) {
        int inputByteIndex = i / 8;
        int inputBitIndex = 7 - (i % 8); // Reverse bit order within a byte

        char inputByte = input[inputByteIndex];
        char inputBit = (inputByte >> inputBitIndex) & 0x01;

        // Set the corresponding bit in result
        int resultByteIndex = resultBitIndex / 8;
        int resultLocalBitIndex = 7 - (resultBitIndex % 8);
        result[resultByteIndex] |= (inputBit << resultLocalBitIndex);

        resultBitIndex++;
    }

    // Null-terminate the result string
    result[bytesNeeded] = '\0';

    return result;
}


// Function to insert a key and associated data into the radix tree
struct radixTree *insertKey(struct radixTree *tree, void *data, void *(*dataGetKey)(void *)) {
	assert(tree && data && dataGetKey);

	// the key here is assumed to be a string
	char *key = (char *)dataGetKey(data);
	int keyBits = (strlen(key) + 1) * BIT_LEN;
	int keyBitIndex = 0;

	// if the tree is empty, create the first node for it
	if (tree->root == NULL) {
		tree->root = createNode(keyBits, key, data);
		return tree;
	}
	struct radixNode *current = tree->root;
	struct radixNode *parent = NULL;

	// Track the current bit and character position in the key
	int keyIndex = 0;
	int bitOffset = 0;
	int flag = 0;
	while (current != NULL) {

		// checking what's the index of the char and the bit
		int bitIndex = 0;
		while (key[keyIndex] != '\0' && current->prefix[keyIndex] != '\0' && getBit(key[keyIndex], bitOffset) == getBit(current->prefix[keyIndex], bitOffset)) {
			bitIndex++;
			bitOffset++;
			if (bitOffset == BIT_LEN) {
				bitOffset = 0;
				keyIndex++;
			}
		}

		// If the input shares a common prefix with the current node
		if (bitIndex == current->prefixBits) {
			if (key[keyIndex] == '\0') {
				(current->dataCount)++;
				current->data = (void **)realloc(current->data, sizeof(void *) * current->dataCount);
				assert(current->data);
				current->data[current->dataCount - 1] = data;
				return tree;

			// record the parent node here
			parent = current;
				
			// The input goes to branch A
			} else if (getBit(key[keyIndex], bitOffset) == 0) {
				if (current->branchA == NULL) {
					int remainingBits = current->prefixBits - bitOffset;
					printf("remainingBits: %d\n", remainingBits);
					char *remainingKey = splitStem(current->prefix, bitIndex, remainingBits);

					// if (remainingBits < BIT_LEN) {
					// 	remainingKey = (char *)realloc(remainingKey, BIT_LEN);
					// 	assert(remainingKey);
					// 	remainingKey[remainingBits] = '\0';
					// }

					current->branchA = createNode(remainingBits, remainingKey, data);
					free(remainingKey);
					parent->branchA = current->branchA;
					return tree;
				} else {
					current = current->branchA;
				}

			// The input goes to branch B
			} else if (getBit(key[keyIndex], bitOffset) == 1) {
				if (current->branchB == NULL) {
					int remainingBits = current->prefixBits - bitOffset;
					printf("remainingBits: %d\n", remainingBits);
					char *remainingKey = splitStem(current->prefix, bitIndex, remainingBits);

					// if (remainingBits < BIT_LEN) {
					// 	remainingKey = (char *)realloc(remainingKey, BIT_LEN);
					// 	assert(remainingKey);
					// 	remainingKey[remainingBits] = '\0';
					// }

					current->branchB = createNode(remainingBits, remainingKey, data);
					free(remainingKey);
					parent->branchB = current->branchB;
					return tree;
				} else {
					current = current->branchB;
				}
			}

		} else {
			// Split the node because key differs from the current node's prefix
			char *commonPrefix = splitStem(current->prefix, 0, bitIndex);
			struct radixNode *newNode = createNode(bitIndex, commonPrefix, current->data);
			newNode->branchA = current->branchA;
			newNode->branchB = current->branchB;
			// printf("%d\n", current->prefixBits);
			int remainingBits = current->prefixBits - bitIndex;
			keyBits -= bitIndex;
			keyBitIndex += bitIndex;
			
			if (flag == 0) {
				tree->root = newNode;
				parent = newNode;
			}
			flag++;

			newNode->data = NULL;
			newNode->dataCount = 0;

			// handle the remaining str originally in the tree and the remaining key
			char *remainingStr = splitStem(current->prefix, bitIndex, remainingBits);
			char *remainingKey = splitStem(key, keyBitIndex, keyBits);
			// printf("Str: %s | Key: %s\n", remainingStr, remainingKey);
			// printf("Str: %d  Key: %d\n", remainingBits, keyBits);
			// printf("bit: %d\n", getBit(remainingStr[0], 0));
			
			if (getBit(current->prefix[keyIndex], bitOffset) == 0) {
				// The original str goes to branch A so that key goes to branch B
				newNode->branchA = createNode(remainingBits, remainingStr, current->data);
				newNode->branchB = createNode(keyBits, remainingKey, data);

			} else if (getBit(current->prefix[keyIndex], bitOffset) == 1) {
				// The original str goes to branch B so that key goes to branch A
				newNode->branchB = createNode(remainingBits, remainingStr, current->data);
				parent->branchA = createNode(keyBits, remainingKey, data);
			}

			free(remainingKey);
			free(remainingStr);
			return tree;
		}
	 }
	return tree;
}

// the function search and then print the query key to the required file
void radixSearchAndPrint(struct radixTree *tree, char *key, 
					   int (*keyCompare)(char *, char *, int, int, int *, int *, int *), 
					   void (*printData)(FILE *, void *, int), 
					   FILE *outFile, FILE *stdOutFile) {
	assert(tree);
    struct radixNode *current = tree->root;
    int keyIndex;
    int bitIndex = 0;
	int flag = 0;

    while (current != NULL) {
		// printf("str: %s, key: %s\n", current->prefix, key);
        if (keyCompare(current->prefix, key, current->prefixBits, bitIndex, &tree->numBits, &tree->numChars, &tree->numStrings) == 0) {
			bitIndex += current->prefixBits;
			keyIndex = bitIndex / BIT_LEN;

            if (key[keyIndex] == '\0') {
                // Key found in the tree, print associated data
				for (int i = 0; i < current->dataCount; i++) {
					printData(outFile, current->data[i], flag);
					flag += 1;
				}
				printComparisonRadix(stdOutFile, tree, key);
                return;
            } else {
				// Continue traversal based on the next bit of the query
            	int nextBit = getBit(key[keyIndex], bitIndex % BIT_LEN);
				// printf("next bit: %d bitIndex: %d\n", nextBit, bitIndex);

                // Key shares a common prefix with the current node
				// goes to branchA or B depends on its next bit
                if (nextBit == 0 && current->branchA != NULL) {
                    current = current->branchA;

                } else if (nextBit == 1 && current->branchB != NULL) {
                    current = current->branchB;

                } else {
					// No matching branch
					fprintf(outFile, "%s\n",key);
					return;
				}
            }
        } else {
            // if unmatched, print the title only
			fprintf(outFile, "%s\n", key);
			return;
        }
    }
    return;
}

// print the the number of comparisons for bit, char and string
void printComparisonRadix(FILE *f, struct radixTree *tree, char *key) {
	fprintf(f, "%s --> b%d c%d s%d\n", (char *)key, tree->numBits, tree->numChars, tree->numStrings);
	tree->numBits = tree->numChars = tree->numStrings = 0;
}
