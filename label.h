typedef struct label_list * label_list_node;

typedef struct label_list{
    char *label_name;
    int decimal_address;
    short type;
    label_list_node next_label;
}label_node;


int SetNext(label_list_node* head, char *file_line, int start_index, int len, int address, short type);/*Input: all decimal_data needed for new symbol, and head of list. Action: add symbol to list. Discounts: all decimal_data is corect. Output: 1 if saccsess, or number<0 otherwise.*/
int check_label_exist(label_list_node head, char *label_name);/*Input: head of list and label_name to check existient. Output: 1 if exist 0 else.*/
label_list_node get_label(label_list_node head, char *label_name);/*Input: head of list and label_name to check existient. Output: pointer to node if exist, or NULL else.*/
int label_list_empty(label_list_node head);/*Input: head of list. Output: 1 if empty, 0 else.*/
label_list_node next_label(label_list_node* head);/*Input: head of List. Output: head of list add delete from list.*/
void free_label_list(label_list_node* head);/*Input: head of List. Output: delete all list*/
int add_label_extern(label_list_node* head, char *file_line, int start_index);/*Input: head of list, command and where to start from. Output: adding all Symbol to extern*/
void calculate_label(label_list_node head, int ic);/*Input: head of list and IC. Output: updating all symbols decimal_address.*/