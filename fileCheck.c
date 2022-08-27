#include "fileCheck.h"
#include "converter.h"

int file_index = -1;
char temp_str[ONEHUNDRED];

char * opcode_sign[SIXTEEN] = {"mov\0","cmp\0","add\0","sub\0",
                               "not\0","clr\0","lea\0","inc\0",
                               "dec\0","jmp\0","bne\0","red\0",
                               "prn\0","jsr\0","rts\0","hlt\0"};

char * directives[FIVE] = {"data\0","string\0",
                           "struct\0","entry\0",
                           "extern\0"};

int new_num(char * command, int iterator, int endNum, int *DC, dirc_list_node * head){
    int newNumber = text_to_number(command,iterator,endNum);
    int added;
    if(newNumber > MAXINT || newNumber < -MAXINT)
        return -1;
    if ((added = new_node(head, newNumber, DC) < 0))
        return added;
    return 1;
}
int new_node(dirc_list_node *head , int number, int *dc_address){

    dirc_list_node temp;
    dirc_list_node new_node = (dirc_list_node)malloc(sizeof(dirc_list));

    if(new_node == NULL){
        return -2;
    }
    new_node->decimal_data=number;
    new_node->dc= *dc_address;
    new_node->next_node=NULL;
    *dc_address += 1;

    if(*head==NULL){
        *head = new_node;
        return 1;
    }

    temp=*head;
    while(temp->next_node!=NULL){
        temp=temp->next_node;
    }
    temp->next_node = new_node;
    return 1;

}
/*directive funt*/
int directive_Nstr(char * file_line, int start_index, dirc_list_node * head, int * dc_address){

    int added;
    int iterator = start_index;

    if(file_line[iterator]!='\"'){
        return -1;
    }
    iterator++;

    while(file_line[iterator]!='\"' && check_txt_sides(file_line,iterator)) {
        if ((added = new_node(head, file_line[iterator], dc_address)) < 0)
            return added;
        iterator++;
    }

    if(file_line[iterator] != '\"')
        return -1;

    if((added = new_node(head,'\0',dc_address))<0){
        return added;
    }

    return iterator+1;
}
int dirc_string(char *file_line, int start_index, int *dc_address, dirc_list_node *head){

    int iterator = start_index + 7;
    iterator = check_for_space(file_line,iterator);

    return directive_Nstr(file_line,iterator,head,dc_address);
}
int dirc_struct(char *file_line, int start_index, int *dc_address, dirc_list_node *head) {
    int iterator = start_index + 7;
    int end_of_num;
    int num;
    int endStruct;

    iterator = check_for_space(file_line, iterator);
    if ((end_of_num = check_if_number(file_line, iterator)) < 0) {
        return -1;
    }

    num = new_num(file_line,iterator,end_of_num-1,dc_address,head);

    if(num<0)
        return num;

    iterator = end_of_num;
    iterator=check_for_space(file_line,iterator);

    if(file_line[iterator]!=',')
        return -1;

    iterator=check_for_space(file_line,iterator+1);

    if(file_line[iterator]!='\"')
        return -1;

    if((endStruct = directive_Nstr(file_line,iterator,head,dc_address))<0)
        return endStruct;

    return endStruct;
}
int dirc_data(char *file_line, int start_index, int *dc_address, dirc_list_node *head){

    int number;
    int end_of_num;
    int iterator = start_index + 5;

    iterator=check_for_space(file_line,iterator);

    if(!check_txt_sides(file_line,iterator))
        return -1;

    if((end_of_num = check_if_number(file_line,iterator))<0)
        return end_of_num;

    number=new_num(file_line,iterator,end_of_num - 1,dc_address,head);

    if(number < 0){
        return number;
    }

    iterator = end_of_num;

    while(check_txt_sides(file_line,iterator)) {

        iterator=check_for_space(file_line,iterator);

        if(file_line[iterator]!=',')
            return iterator;

        iterator=check_for_space(file_line,iterator + 1);

        if((end_of_num = check_if_number(file_line,iterator))<0)
            return end_of_num;

        number = new_num(file_line,iterator,end_of_num - 1,dc_address,head);

        if(number<0)
            return number;

        iterator = end_of_num;
    }
    return iterator;
}

void free_dirc_list(dirc_list_node * head_node){
    dirc_list_node pt,temp;
    temp = *head_node;
    while(temp)
    {
        pt = temp->next_node;
        free(temp);
        temp = pt;
    }
    *head_node = NULL;
}
void print_dirc_list(dirc_list_node head){
    dirc_list_node temp = head;
    while(temp){
        printf("(%d,%d)\n",temp->decimal_data,temp->dc);
        temp=temp->next_node;
    }
}

/*external function related functions*/
int add_extern_list(externInternal * external_head , char *label_name , int address, short is_external)
{

    externInternal externalPointer, temp = (externInternal)malloc(sizeof(externInternalList));

    if(temp && (temp->decimal_address = decimal_to_base32(address)))
    {
        temp->is_external = is_external;
        temp->label_name = label_name;
        temp->next_externinternal_node = NULL;
        if(*external_head){
            externalPointer = *external_head;
            while(externalPointer->next_externinternal_node)
                externalPointer = externalPointer->next_externinternal_node;
            externalPointer->next_externinternal_node = temp;
        }
        else{*external_head = temp;}
    }
    else{
        return -2;
    }
    return 1;
}
int is_externLst_empty(externInternal head){
    if(!head)
        return 1;
    return 0;
}
externInternal next_extern(externInternal after){
    return after->next_externinternal_node;
}
char * extern_list (externInternal *head){
    int length;
    externInternal external_ptr;
    char *base32;
    char *newString;

    if(*head)
    {
        external_ptr = *head;
        length = strlen(external_ptr->label_name) + 4;

        newString = (char *)malloc(length*sizeof(char));

        if(newString)
        {
            strcpy(newString,external_ptr->label_name);
            strcat(newString, " ");
            base32 = binary_to_base32(external_ptr->decimal_address);
            if(!base32){
                return "couldnt allocate memory properly\0";
            }

            strcat(newString, base32);
            newString[length-1]= '\0';

            return newString;
        }
        else{return "couldnt allocate memory properly\0";}
        *head = external_ptr->next_externinternal_node;
    }else{
        return NULL;
    }
}
void free_extern_list (externInternal *head)
{
    externInternal extern_ptr;
    externInternal temp;
    temp = *head;

    while(temp)
    {
        extern_ptr = temp->next_externinternal_node;
        free(temp->decimal_address);
        free(temp);
        temp = extern_ptr;
    }
    *head = NULL;
}
void print_extern_list(externInternal head){
    externInternal temp = head;
    while(temp){
        printf("(%hi,%s,%s)\n", temp->is_external, temp->label_name, temp->decimal_address);
        temp=temp->next_externinternal_node;
    }
}

/*File check functions related*/
int check_if_directive (char *file_line, int start_index){
    int iterator = 1;
    char directive[SEVEN];
    int i;

    if(file_line[start_index] == '.'){

        /*Check for spaces tabs or EOL*/
        while((file_line[start_index + iterator] != ' ') &&
              (file_line[start_index + iterator] !='\t') &&
              (file_line[start_index+iterator] !='\0')){

            if(iterator >= SEVEN)
                return -1;
            directive[iterator - 1] = file_line[start_index + iterator];
            iterator++;
        }
        directive[iterator-1]='\0';
        for (i = 0; i < 5; i++){
            if(strcmp(directive,directives[i]) == ZERO){
                return i+1;
            }
        }
        return -1;
    }
    return -1;
}
int check_if_opcode (char *file_line, int start_index){
    char order[FIVE];
    int i;
    int iterator = 0;

    while (iterator<4 && file_line[start_index + iterator] != '\0' && !isspace(file_line[start_index + iterator])){
        order[iterator] = file_line[start_index + iterator];
        iterator++;
    }

    order[iterator]='\0';

    for (i = 0; i < 16; i++){
        if(strcmp(order,opcode_sign[i]) == ZERO){
            return i;
        }
    }
    return -1;
}
int check_if_label(char *file_line, int start_index){
    int line_iterator = 1;

    if (isalpha(file_line[start_index])){

        while(isalpha(file_line[start_index + line_iterator]) ||
              isdigit(file_line[start_index + line_iterator])){
            if(line_iterator > MAX_LABEL_LEN){
                return -1;
            }
            line_iterator++;
        }
        if (file_line[start_index+line_iterator] == ':'){
            return start_index + line_iterator;
        }
    }
    return -1;
}
int check_if_number(char *file_line, int start_index)
{
    int start = start_index ;
    int end = 0;

    if(start+end < MAX_LABEL_LEN && (file_line[start+end] == '-' || file_line[start+end] == '+'))
        start++;
    while (file_line[start+end] != '\0' && file_line[start+end] >= '0' && file_line[start+end]<='9'){
        end++;
    }
    return (end == 0)? -1: start+end;
}
int check_for_space(char *file_line, int start_index)
{
    char c;
    while((c = file_line[(start_index)]) != '\0' && isspace(c))
        start_index++;
    return start_index;
}
int check_for_comma(char *file_line, int start_index){
    start_index = check_for_space(file_line,start_index);
    if(file_line[start_index] == ',')
    {
        start_index++;
        return start_index;
    }
    else
    {
        return -1;
    }
}
int check_txt_sides(char *file_line, int start_index){
    start_index = check_for_space(file_line,start_index);
    return (file_line[start_index] != '\0')? 1 : 0;
}
int check_txt_end(char *file_line, int start_index){
    char chr;
    while((chr = file_line[(start_index)]) != '\0' && !isspace(chr) && (chr != ','))
        start_index++;
    return start_index;
}

char *get_file_line(FILE *fp1)
{
    char c;
    char *arr;
    int i;

    if((c=getNext(fp1))!= EOF)
        set_next_char(c);
    else
        return NULL;

    arr = (char *)malloc(81 *sizeof(char));
    if(arr)
    {
        i = 0;
        while((c = getNext(fp1)) != EOF && c != '\n' && i<(81 -1))
        {
            arr[i] = c;
            i++;
        }
        arr[i] = '\0';
        if(i <= (81-1) && c!= EOF && c != '\n')
        {
            set_next_char(c);
            while((c = getNext(fp1)) != EOF && c != '\n')
                ;
            if(c == EOF)
                set_next_char(c);
            return "ERROR!: line is too long please check\0";
        }
        else
            return arr;

    }
    else
    {
        return "ERROR: Couldnt allocate memory!\0";
    }
}
char getNext(FILE *fp1){
    if(file_index>-1)
    {
        return (temp_str[file_index--]);
    }
    return getc(fp1);
}
void set_next_char(char chr){
    (temp_str[++file_index]) = chr;
}
