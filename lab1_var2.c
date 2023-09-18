#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 30
#define MAX_LINE_LENGTH 1024

typedef struct WordCount {
    char word[MAX_WORD_LENGTH + 1]; // +1 for terminating \0
    int count;
} WordCount;

int isWordValid(const char *word) {
    if (strlen(word) <= MAX_WORD_LENGTH) {
        return 1;
    } else {
        return 0;
    }
}

//function for sorting
int cmpfunc(const void *a, const void *b) {
    return ((WordCount *)b)->count - ((WordCount *)a)->count;
}

int main() {
    char filename[] = "lab2text.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open the file");
        return 1;
    }

    WordCount *wordCounts = NULL;
    int wordCountsCapacity = 0;
    int wordCountsNumber = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) { //reads a line from the `file` and stores it into the `line` array
        char delimiters[] = " \t\n\r\f\v.,;:!?'\"()[]{}<>|/\\+-=*&^%$#@`~";
        char *token = strtok(line, delimiters); //gets the first token
        while (token != NULL) {

            if (isWordValid(token)) {
                int thisWordExists = 0;
                for (int i = 0; i < wordCountsNumber; i++) {
                    if (strcmp(wordCounts[i].word, token) == 0) { //compares 2 strings, 0 if equal
                        wordCounts[i].count++;
                        thisWordExists = 1;
                        break;
                    }
                }

                if (!thisWordExists) {
                    if (wordCountsNumber >= wordCountsCapacity) { //if array is full
                        wordCountsCapacity += 5; //we add 5 more slots
                        wordCounts = realloc(wordCounts, sizeof(WordCount) * wordCountsCapacity); //and resize the array
                    }
                    strcpy(wordCounts[wordCountsNumber].word, token); //add the word to the array
                    wordCounts[wordCountsNumber].count = 1; //with the count of 1
                    wordCountsNumber++;
                }
            }
            token = strtok(NULL, delimiters);
        }
    }

    fclose(file);

    qsort(wordCounts, wordCountsNumber, sizeof(WordCount), cmpfunc); //sorts the array in descending order

    for (int i = 0; i < wordCountsNumber; i++) {
        printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
    }

    free(wordCounts);

    return 0;
}
