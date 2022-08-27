#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*#include "checkFileLine.h"*/
#include "fileCheck.h"
#include "label.h"
#include "converter.h"
#include "lineCalculator.h"
#define LABELMAXLEN 30
#define FOURTEEN 14
#define FIFTHTEEN 15
#define TWELVE 12
#define NINE 9
#define EIGHT 8
#define SEVEN 7
#define SIX 6
#define THREE 3
#define TWO 2
#define ONE 1
#define ZERO 0

char * count_num_of_lines (int num, int *l, char *file_line, int start_index)
{
    int first_word;
    int second_word;
    int line_number = 1;
    start_index += THREE;
    if(num >= FOURTEEN)
    {
        if(num == FIFTHTEEN)
            start_index += TWO;
        if(check_txt_sides(file_line,start_index))
            return "ERROR:Unexpected label after order\0";
    }
    else
    {
        if(!check_txt_sides(file_line,start_index))
            return "ERROR:Unexpected operator after order\0";
        start_index = check_for_space(file_line, start_index);
        first_word = get_typeof_symbol(file_line, start_index);
        switch (first_word)
        {
            case 0:
                if(num <= THREE || num == TWELVE)
                    line_number += ONE;
                else
                    return "ERROR: number 0,first word can't be in first\0";
                break;
            case 1:
                line_number += ONE;
                break;
            case 2:
                line_number += TWO;
                break;
            case 3:
                if(num != SIX)
                    line_number += ONE;
                else
                    return "ERROR: number 3,first word can't be in first\0";
                break;
        }
        start_index = check_txt_end(file_line,start_index);
        if(num <= THREE || num == SIX)
        {
            start_index = check_for_comma(file_line,start_index);
            if(start_index == -1)
                return "ERROR: Unexpected comma\0";
            start_index = check_for_space(file_line,start_index);
            second_word = get_typeof_symbol(file_line,start_index);
            switch (second_word)
            {
                case ZERO:
                    if(num == ONE)
                        line_number += ONE;
                    else
                        return "ERROR: number 0,second word can't be in first\0";
                    break;
                case 1:
                    line_number += ONE;
                    break;
                case 2:
                    line_number += TWO;
                    break;
                case 3:
                    if(first_word == THREE){
                        line_number += ZERO;
                    }else{
                        line_number += ONE;
                    }
                    break;
            }
        }
        else{
            if(check_txt_sides(file_line,start_index))
                return "ERROR:Unexpected text\0";
        }
    }
    *l = line_number;
    return NULL;
}

int get_typeof_symbol(char *file_line, int start_index)
{
    int i=0;
    if(file_line[start_index] == '#')
        return 0;
    if(file_line[start_index] == 'r' && file_line[start_index + 1] >= '0' && file_line[start_index + 1] <= '7')
        return 3;
    while(file_line[start_index + i] != '\0' && !isspace(file_line[start_index + i]) && file_line[start_index + i] != '.' && file_line[start_index + i] != ',')
        i++;
    return (file_line[start_index + i] == '.') ? 2 : 1;
}

char *calc_label_space(char *file_line, int start_index, int end_index, label_list_node head)
{
    int i;
    int label_len = end_index - start_index +1;
    char *binary_str=NULL;
    static char label_name[LABELMAXLEN];
    label_list_node temp = head;

    for(i = 0; i < label_len; i++)
    {
        label_name[i] = file_line[start_index + i];
    }
    label_name[i] = '\0';
    while(temp)
    {
        if(strcmp(temp->label_name, label_name) == 0)
        {
            binary_str = decimal_to_binary(temp->decimal_address);

            if(binary_str[0] == '%'){
                return "Couldn't allocate memory properly\0";
            }

            for(i = 0; i < 8; i++)
                binary_str[i] = binary_str[i + TWO];
            if(temp->type == 2)
            {
                binary_str[NINE] = '1';
                binary_str[EIGHT] = '0';
            }else{
                binary_str[NINE] = '0';
                binary_str[EIGHT] = '1';
            }
            binary_str[SEVEN] = '|';
            return binary_str;
        }temp = temp->next_label;
    }
    return "Label doesnt exists!\0";
}