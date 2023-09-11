/* Project: dict1, 2 & 3
   data.h :
			  = the interface of module data of the project
*
* Created by Zhonghao Sheng
05/09/2023
* 
* Acknowledgement: 
* In this file, the function skipHeaderLine was adapted 
* from W3.8 of Workshop Week 3
*/

#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>

// Constant definitions
#define MAX_STRING_LEN 128
#define MAX_LINE_LEN 512
#define CHAR_BIT 8

// Data definitions
typedef struct data data_t;

/*------------ Function definitions -----------------------------*/
// get the trading name from the data
char *getTradingName(struct data *inputData);

// returns 0 if two input strings are matched
int strCmp(void *vp1, void *vp2);

// returns 0 if the input query is a prefix of str
// returns 1 if the str is 'larger' than the query
// returns -1 if the str is 'smaller' than the query
int strComForSorted(char *str, char *query, int *numBits, int *numChars, int *numStrings);

// returns 0 if the input query is a prefix of str
// returns 1 if the str is 'larger' than the query
// returns -1 if the str is 'smaller' than the query
int bitwiseStrCom(char *str, char *query, int *numBits, int *numChars, int *numStrings);

// returns 0 if the input query is a prefix of str
// returns -1 if the prefix of str doesn't match the query
int strCmpRadix(char *str, char *query, int strBits, int queryBits, int *numBits, int *numChars, int *numStrings);

// Skip the header line of .csv file "f"
// The following function was copied from function skipHeaderLine 
// of W3.8 in Workshop Week 3
void skipHeaderLine(FILE *f);

// Scanning int data from the input file
void readIntField(FILE *f, int *value);

// Scanning double data from the input file
void readDoubleField(FILE *f, double *value);

// Scanning string data from the input file
// able to handle both strings with "" and without ""
void readStringField(FILE *f, char **value);

// Reads a data from file "f" to build a data_t data
// Returns the pointer, or NULL if reading is unsuccessful
data_t *dataRead(FILE *f);

// Prints a data record s to file "f"
void dataPrint(FILE *f, data_t *d, int count);

// Free the memory allocated at stage one
void dataFree(data_t *data);

// print the information corresponds to their keys to the input file
void stageOnePrintToStdOut(FILE *f, int count, char *key);

#endif
