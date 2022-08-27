/*
 * With the function in this file well check macros and behave accordingly
 * "macroLL==macroLinkedList
 */

#ifndef PREPROCESS
#define PREPROCESS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "converter.h"

#define BASE 32
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define TEN 10
#define NINE 9
#define LEN 100

/*linked list struct to hold all macros one after the other*/
struct macro {
    char macro_name[LEN];
    char macro_content[LEN];
    struct macro* next_macro;
};

void add_macro_to_linked_list(struct macro *, char [], char []);

void print_macros(struct macro*);

void insert_macro_name(char [],struct macro*);

void insert_macro_content(struct macro*,FILE *);

int is_macro_or_endmacro(char []);

int pre_proccess_on_macros(char **,struct macro*,int );

int pre_proccess_before_span(char *argv[] , struct macro* head, int i );

void file_macro_span(int i, char *argv[], struct macro *head);

/*void macro_spaning(int ,struct macro* , char *[]);*/

#endif //PREPROCESS