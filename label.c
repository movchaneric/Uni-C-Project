#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fileCheck.h"
#include "label.h"



int check_label_exist(label_list_node head, char *label_name)
{
    while(head)
    {
        if(strcmp(head->label_name, label_name) == 0)
            return 1;
        head = head->next_label;
    }
    return 0;
}
label_list_node get_label(label_list_node head, char *label_name)
{
    while(head)
    {
        if(strcmp(head->label_name, label_name) == 0)
            return head;
        head = head->next_label;
    }
    return NULL;
}
int label_list_empty(label_list_node head)
{
    return (head)? 0 : 1;
}
label_list_node next_label(label_list_node* head)
{
    label_list_node p;
    if(!label_list_empty(*head))
    {
        p = *head;
        *head = (*head)->next_label;
        return p;
    }
    return NULL;
}
void free_label_list(label_list_node* head)
{
    label_list_node pt,next;
    if(!label_list_empty(*head))
    {
        pt = *head;
        while(pt)
        {
            next = pt->next_label;
            free(pt->label_name);
            free(pt);
            pt = next;
        }
        *head = NULL;
    }
}
int add_label_extern(label_list_node* head, char *file_line, int start_index)
{
    int start,end,count=0;
    while(check_txt_sides(file_line, start_index) && start_index >= 0)
    {
        start = check_for_space(file_line, start_index);
        end = check_txt_end(file_line, start);
        if(end>-1)
        {
            if(SetNext(head, file_line, start, end - start , 0, 2) < 0 )
                return -2;
            count++;
            start_index = check_for_comma(file_line, end);
            if((start_index == -1 && check_txt_sides(file_line, end)) ||
                    (start_index != -1 && !check_txt_sides(file_line, start_index)))
                return -3;
        }
        else
        {
            return -1;
        }
    }
    return count;
}
int SetNext(label_list_node* head, char *command, int from, int length, int adress, short type)
{
    label_list_node pt;
    if(*head)
    {
        pt = *head;
        while(pt->next_label)
        {
            pt = pt->next_label;
        }
        pt->next_label = (label_list_node)malloc(sizeof(label_node));
        pt = pt->next_label;
    }
    else
    {
        *head = (label_list_node)malloc(sizeof(label_node));
        pt = *head;
    }
    if(pt && (pt->label_name = (char *)malloc((length+1)*sizeof(char))))
    {
        pt->next_label = NULL;
        pt->label_name = strncpy(pt->label_name, command+from,length);
        pt->label_name[length] = '\0';
        pt->decimal_address = adress;
        pt->type = type;
    }
    else
    {
        return -2;
    }
    return 1;
}
void calculate_label(label_list_node head, int ic)
{
    while(head)
    {
        if(head->type <= 1)
            head->decimal_address += ic;
        head = head->next_label;
    }
}
