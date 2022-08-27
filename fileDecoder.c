#include "fileDecoder.h"
#define SIX 6
#define FIVE 5
#define THREE 3
#define TWO 2

char * current_decoder(decoder * head){
    char* charTemp;
    decoder temp= *head;
    char* converted = (char *)malloc(SIX *sizeof(char));

    if(!converted){
        return NULL;
    }

    (*head) = (*head) -> next_decode_item;
    memcpy(converted,decimal_to_base32((temp)->decimal_address),TWO);

    converted[TWO]='\t';

    charTemp = converted + THREE;

    memcpy(charTemp,binary_to_base32((temp)->binary_str),TWO);
    converted[FIVE]='\0';
    return converted;
}
int add_new_decode(decoder * head , int address , char *binary) {
    decoder new_decode = (decoder) malloc(sizeof(decode_item));
    decoder temp;

    if (new_decode == NULL) {
        return -2;
    }

    new_decode -> decimal_address = address;
    new_decode -> binary_str = binary;
    new_decode -> next_decode_item = NULL;

    if (is_empty_decoder(*head) > 0) {
        *head = new_decode;
        return 1;
    }

    temp = *head;
    while (temp->next_decode_item != NULL) {
        temp = temp->next_decode_item;
    }

    temp->next_decode_item = new_decode;
    return 1;
}
decoder next_decoder(decoder current){
    return current->next_decode_item;
}

int is_empty_decoder(decoder head){
    if(head==NULL)
        return 1;
    return -1;
}
void free_decode_list(decoder * head){
    decoder pt,temp;
    temp = *head;
    while(temp)
    {
        pt = temp->next_decode_item;
        free(temp->binary_str);
        free(temp);
        temp = pt;
    }
    *head = NULL;
}
