#include <stdio.h>
#include <stdlib.h>
#include "converter.h"

typedef struct decode_list * decoder;
typedef struct decode_list{
    int decimal_address;
    char *binary_str;
    decoder next_decode_item;
} decode_item;

int is_empty_decoder(decoder head);
int add_new_decode(decoder * head , int address , char *binary_str);
decoder next_decoder(decoder current);
char * current_decoder(decoder * head);
void free_decode_list(decoder * head);