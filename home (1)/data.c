/* Project: dict1, 2 & 3
   data.c :
			  = the implementation of module data of the project
*
* Created by Zhonghao Sheng
05/09/2023
* 
* Acknowledgement: 
* In this file, the function skipHeaderLine was adapted 
* from W3.8 of Workshop Week 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"

// The details of data are visible only inside "data.c"
// and cannot be accessed in any other modules
struct data {
    // Define structure fields
    int census_year;
    int block_id;
    int property_id;
    int base_property_id;
    char *building_address;
    char *clue_small_area;
    char *business_address;
    char *trading_name;
    int industry_code;
    char *industry_description;
    char *seating_type;
    int number_of_seats;
    double longitude;
    double latitude;
};

// get the trading name from the data
char *getTradingName(struct data *inputData) {
    if (inputData == NULL) {
        return NULL;
    }
    return inputData->trading_name;
}

// returns 0 if two input strings are matched
int strCmp(void *vp1, void *vp2) {
    char *s1 = vp1;
    char *s2 = vp2;
    return strcmp(s1, s2);
}

// returns 0 if the input query is a prefix of str
// returns 1 if the str is 'larger' than the query
// returns -1 if the str is 'smaller' than the query
int strComForSorted(char *str, char *query, int *numBits, int *numChars, int *numStrings) {
    // Record the string comparison
    (*numStrings)++;

    // Use strncmp for string comparison
    int result = strncmp(str, query, strlen(query)); 
    int i;
    // Count the number of character comparisons
    for (i = 0; str[i] != '\0' && query[i] != '\0' && str[i] == query[i]; i++) {
        (*numChars)++;
    }
    // increase when there's a different char found
    (*numChars)++;
    // Estimate the number of bit comparisons based on character comparisons.
    (*numBits) = (*numChars) * CHAR_BIT;
    return result;
}


// // returns 0 if the input query is a prefix of str
// // returns 1 if the str is 'larger' than the query
// // returns -1 if the str is 'smaller' than the query
// // the function also records the number of comparison between bits, chars and strings
// int bitwiseStrCom(char *str, char *query, int *numBits, int *numChars, int *numStrings) {
//     // int lenStr = strlen(str);
//     int lenQuery = strlen(query);
//     // int minLen = (lenStr >= lenQuery) ? lenQuery : lenStr;

//     // Record the string comparison
//     (*numStrings)++;

//     for (int i = 0; i < lenQuery; i++) {
//         // Record the character comparison
//         (*numChars)++;

//         char tempStr = str[i];
//         char tempQuery = query[i];
        
//         // Compare the Most Significant Bits (MSB)
//         for (int i = CHAR_BIT - 1; i >= 0; i--) {
//             // Record the bit comparison count
//             (*numBits)++;

//             int bitStr = (tempStr >> i) & 1;
//             int bitQuery = (tempQuery >> i) & 1;

//             // Compare the MSB
//             if (bitStr != bitQuery) {
//                 return (bitStr > bitQuery) ? 1 : -1;
//             }
//         }
//     }
//     // after checking all the bits, we can confirm the query is a prefix of the str
//     return 0;
// }


// returns 0 if the input query is a prefix of str
// returns -1 if the prefix of str doesn't match the query
int strCmpRadix(char *str, char *query, int strBits, int queryBits, int *numBits, int *numChars, int *numStrings) {
    int queryBitLength = strlen(query) * CHAR_BIT - queryBits;

    // Record the string comparison
    (*numStrings)++;

    for (int i = 0; i < strBits && (i + queryBits) < queryBitLength; i++) {
        if (*numBits % CHAR_BIT == 0) {
            (*numChars)++;
        }

        // Record the bit comparison count
        (*numBits)++;

        // Extract the bits from str and query based on bit indices
        int bitStr = (str[i / CHAR_BIT] >> (CHAR_BIT - 1 - (i % CHAR_BIT))) & 1;
        int bitQuery = (query[(i + queryBits) / CHAR_BIT] >> (CHAR_BIT - 1 - ((i + queryBits) % CHAR_BIT))) & 1;

        // printf("compare: str: %d query: %d\n", bitStr, bitQuery);

        // Compare the bits
        if (bitStr != bitQuery) {
            return -1;
        }
    }

    // after checking all the bits, we can confirm the the prefix of str matches query
    return 0;
}


// Skip the header line of .csv file "f"
// The following function was copied from function skipHeaderLine 
// of W3.8 in Workshop Week 3
void skipHeaderLine(FILE *f) {
	while (fgetc(f) != '\n');
}

// Scanning int data from the input file
void readIntField(FILE *f, int *value) {
    fscanf(f, ",%d", value);
}

// Scanning double data from the input file
void readDoubleField(FILE *f, double *value) {
    fscanf(f, ",%lf", value);
}

// Scanning string data from the input file
// able to handle both strings with "" and without ""
void readStringField(FILE *f, char **value) {
    char line[MAX_STRING_LEN + 1];
    char delimiter;
    fscanf(f, ",%c", &delimiter);

    // if the scanned string contains symbol "
    if (delimiter == '\"') {
        fscanf(f, "%[^\"]\"", line);
    } else {
        fscanf(f, "%[^,\n]", line);
    }

    // Allocate space for string + '\0' + delimiter 
    // (so it's ((strlen + 1)[original] + 1[for the delimiter]) + 1[for '\0])
    *value = (char *)malloc((strlen(line) + 3) * sizeof(char));
    if (*value != NULL) {
        if (delimiter == '\"') {
            strcpy(*value, line);
            // removing the (") symbol and assigning '\0' at the end of the string
            (*value)[strlen(line)] = '\0';

        } else {
            // Store delimiter as the first character
            (*value)[0] = delimiter;
            strcpy(*value + 1, line);
            // assigning '\0' at the end of the string
            (*value)[strlen(line) + 1] = '\0';
        }
        
    } else {
        exit(EXIT_FAILURE);
    }
}

// Reads a field from file "f" to build a data_t data.
// Returns the pointer, or NULL if reading is unsuccessful.
data_t *dataRead(FILE *f) {
	data_t *data = (data_t *)malloc(sizeof(data_t));
    assert(data);

    if (fscanf(f, "%d", &data->census_year) != 1) {
        // Clean up memory in case of failure
        free(data);
        return NULL;
    }

    // scanning remaining data in order
    readIntField(f, &data->block_id);
    readIntField(f, &data->property_id);
    readIntField(f, &data->base_property_id);
    readStringField(f, &data->building_address);
    readStringField(f, &data->clue_small_area);
    readStringField(f, &data->business_address);
    readStringField(f, &data->trading_name);
    readIntField(f, &data->industry_code);
    readStringField(f, &data->industry_description);
    readStringField(f, &data->seating_type);
    readIntField(f, &data->number_of_seats);
    readDoubleField(f, &data->longitude);
    readDoubleField(f, &data->latitude);
    
    // Read and discard the newline character
    char newline;
    fscanf(f, "%c", &newline);

    return data;
}

// Prints data *d to file "f"
void dataPrint(FILE *f, data_t *d, int count) {
    if (count == 0) {
        fprintf(f, "%s\n", d->trading_name);
    }
    fprintf(f, "--> ");
    fprintf(f, "census_year: %d || ", d->census_year);
    fprintf(f, "block_id: %d || ", d->block_id);
    fprintf(f, "property_id: %d || ", d->property_id);
    fprintf(f, "base_property_id: %d || ", d->base_property_id);
    fprintf(f, "building_address: %s || ", d->building_address);
    fprintf(f, "clue_small_area: %s || ", d->clue_small_area);
    fprintf(f, "business_address: %s || ", d->business_address);
    fprintf(f, "trading_name: %s || ", d->trading_name);
    fprintf(f, "industry_code: %d || ", d->industry_code);
    fprintf(f, "industry_description: %s || ", d->industry_description);
    fprintf(f, "seating_type: %s || ", d->seating_type);
    fprintf(f, "number_of_seats: %d || ", d->number_of_seats);
    fprintf(f, "longitude: %.5lf || ", d->longitude);
    fprintf(f, "latitude: %.5lf || \n", d->latitude);
} 

// Free the memory allocated at stage one
void dataFree(struct data *data) {
    free(data->building_address);
    free(data->clue_small_area);
    free(data->business_address);
    free(data->trading_name);
    free(data->industry_description);
    free(data->seating_type);
    free(data);
}

// print the information corresponds to their keys to the input file
void stageOnePrintToStdOut(FILE *f, int count, char *key) {
	if(count == 0) {
		fprintf(f, "%s --> NOTFOUND\n", key);
	} else {
		fprintf(f, "%s --> %d\n", key, count);
	}
}

