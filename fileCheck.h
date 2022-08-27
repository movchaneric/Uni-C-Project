#ifndef LINEMAXSIZE
#define LINEMAXSIZE 81
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "converter.h"

/*Const variables define*/
#define MAXINT 512
#define ONEHUNDRED 100
#define SIXTEEN 16
#define SEVEN 7
#define FIVE 5
#define ZERO 0
#define MAX_LABEL_LEN 30
#define LINE_LEN 81
#define MINUSONE -1

#ifndef directiveSize
#define directiveSize
#endif


typedef struct dirc_list * dirc_list_node;
typedef struct dirc_list{
    int decimal_data;
    int dc;
    dirc_list_node next_node;
} dirc_list;

typedef struct externInternalList * externInternal;
typedef struct externInternalList{
    short is_external;
    char *label_name;
    char *decimal_address;
    externInternal next_externinternal_node;
}externInternalList;

/*-----------------------------------------------------------------*/
/*directive function related functions*/
int directive_Nstr(char * file_line, int start_index, dirc_list_node * head, int * dc_address);

int new_num(char * file_line, int iterator, int endNum, int *DC, dirc_list_node * head);
int new_node(dirc_list_node *head , int number, int * dc_address);

int dirc_string(char *file_line, int start_index, int *dc, dirc_list_node *head_node);
int dirc_struct(char *file_line, int start_index, int *dc, dirc_list_node *head_node);
int dirc_data(char *file_line, int start_index, int *dc, dirc_list_node*head);

void free_dirc_list(dirc_list_node * head_node);
void print_dirc_list(dirc_list_node head_node);

/*external function related functions*/


int add_extern_list(externInternal *  , char * , int , short );
int is_externLst_empty(externInternal );
externInternal next_extern(externInternal );
char * extern_list (externInternal *);
void free_extern_list (externInternal *);
void print_extern_list(externInternal head);

int check_if_label(char *command, int from);
int check_if_directive (char *command, int from);
int check_if_opcode (char *command, int from);
int check_if_number(char *command, int from);

int check_for_space(char *file_line, int start_index);
int check_for_comma(char *file_line, int start_index);
int check_txt_sides(char *file_line, int start_index);
int check_txt_end(char *file_line, int start_index);
char *get_file_line(FILE *fptr);
void set_next_char(char c);
char getNext(FILE *fptr);

#endif // LINEMAXSIZE