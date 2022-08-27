#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preProccess.h"
#include "fileCheck.h"
#include "label.h"
#include "fileDecoder.h"
#include "converter.h"
#include "lineCalculator.h"
#include "secondPassFunc.h"
#define SymbolMax 30
#define ERROR_INIT \
error_flag = ON;\
free(command);\
continue;

enum Status {OFF,ON};

dirc_list_node dirc_list_head = NULL;
label_list_node label_list_head = NULL;
decoder decode_list_head = NULL;
externInternal extInt_list_head = NULL;

short error_flag;
char label_name[LINEMAXSIZE];

void first_file_pass(FILE *file_spanned);
void second_file_pass(FILE *file_spanned);

short error_flag;
char label_name[LINEMAXSIZE];

int main(int argc, char const *argv[])
{
    int i,flag;
    char *file_name = NULL;
    FILE *file;

    file_name = (char*) malloc(sizeof(char) * 100);
    if(argc <= 1)
    {
        printf("No File where typed.\n");
        exit(0);
    }
    for	(i=1; i < argc; i++)
    {
        /*Preprocess and macro Spanning*/
        struct macro* macro_tail = NULL;
        struct macro* macro_head = NULL;

        macro_tail = (struct macro*) malloc(sizeof(struct macro));
        macro_head = (struct macro*) malloc(sizeof(struct macro));

        macro_tail = macro_head;
        flag = pre_proccess_before_span(argv,macro_head,i);

        /*span macros*/
        if(!flag){
            file_macro_span(i,argv,macro_tail);
        }

        file_name = (char*)malloc(sizeof(char) * 100);
        file_name = concatinate_str(argv[i], ".am\0");

        if(file_name){
            file = fopen(file_name, "r");
            if (file)
            {
                first_file_pass(file);
                if(error_flag == ON)
                {
                    printf(" Error inside the file\n", file_name);
                    fclose(file);
                    hltSys();
                    continue;
                }
                rewind(file);
                second_file_pass(file);
                if(error_flag == ON)
                {
                    printf("Error inside the file\n", file_name);
                    fclose(file);
                    hltSys();
                    continue;
                }
                if(file_span(argv[i]) != 1)
                {
                    printf("Error: could not creat output files to: %s\n", file_name );
                }
                else
                {
                    printf(" file.am , file.ent, file.ext, file.ob been created!\n", file_name);
                }
                fclose(file);
            }else{
                printf("No input file %s\n", argv[i] );
            }
            hltSys();
        }
        else
        {
            printf("Couldnt create file %s\n", argv[i]);
        }
    }
    return 1;
}

void first_file_pass(FILE *file_spanned)
{
    error_flag = OFF;
    short label_flag = OFF;
    int index, end, first_result, second_result;
    int label_start_index, label_length;
    int ic_address = 100, dc_address = 0, L = 0;
    int line_number = 0;
    char *final_text = NULL, *command = NULL;


    while((command = get_file_line(file_spanned))){

        index = 0;
        L = 0;
        label_flag = OFF;
        line_number++;

        /*comment check and pass*/
        if(!check_txt_sides(command, index) ||
                            command[check_for_space(command, 0)] == ';')
            continue;

        if(command[index] == '`')
        {
            printf("Fail in line: %d %s\n", line_number, (command + index + 1));
            error_flag = ON;
            continue;
        }

        index = check_for_space(command, index);
        end = check_if_label(command, index);

        if(end>0){

            label_flag = ON;
            label_start_index = index;
            label_length = end - index;
            if(label_length >= MAX_LABEL_LEN)
            {
                printf("line: %d label_name can't be longer than 30\n", line_number);
                ERROR_INIT
            }
            index = check_for_space(command, end + 1);
            if(!check_txt_sides(command, index))
            {
                printf("line: %d expected a method label_name or directive after label_name\n", line_number);
                ERROR_INIT
            }
        }
        if(command[index] == '.')
        {
            first_result = check_if_directive(command, index);
            if(first_result <= 0)
            {
                printf("line: %d unknown directive label_name\n", line_number);
                ERROR_INIT
            }
            if(first_result <= 3)
            {
                if(label_flag)
                {
                    label_flag = OFF;
                    strncpy(label_name, (command + label_start_index), label_length);
                    label_name[label_length] = '\0';
                    if(check_label_exist(label_list_head, label_name))
                    {
                        printf("line: %d label already exist\n", line_number);
                        ERROR_INIT
                    }
                    if(first_result == 3)
                        second_result = SetNext(&label_list_head, command, label_start_index, label_length, dc_address, 1);/*1- for sruct.*/
                    else
                        second_result = SetNext(&label_list_head, command, label_start_index, label_length, dc_address, 0);
                    if(second_result == -2)
                    {
                        printf("line: %d could not add Symbol.\n", line_number);
                        ERROR_INIT
                    }
                }
                switch(first_result)
                {
                    case 1:
                        second_result = dirc_data(command, index, &dc_address, &dirc_list_head);
                        if(second_result < 0 && second_result != -2)
                        {
                            printf("line: %d  after directive label_name must contain 2 numbers \n", line_number);
                            ERROR_INIT
                        }
                        break;
                    case 2:
                        second_result = dirc_string(command, index, &dc_address, &dirc_list_head);
                        if(second_result < 0 && second_result != -2)
                        {
                            printf("line: %d expected text after directive label_name\n", line_number);
                            ERROR_INIT
                        }
                        break;
                    case 3:
                        second_result = dirc_struct(command, index, &dc_address, &dirc_list_head);
                        if(second_result < 0 && second_result != -2)
                        {
                            printf("line: %d expected number and text after directive label_name\n", line_number);
                            ERROR_INIT
                        }
                        break;
                }
                if(second_result == -2)
                {
                    printf("line: %d couldn't add decimal_data.\n", line_number);
                    ERROR_INIT
                }
                if(check_txt_sides(command, second_result))
                {
                    printf("line: %d unexpected text after directive order.\n", line_number);
                    ERROR_INIT
                }
            }
            else
            {
                if(label_flag)
                {
                    label_flag = OFF;
                    printf("line: %d unexpected label in this type of directive.\n", line_number);
                }
                if(first_result == 5)
                {
                    index += 7;
                    if(check_txt_sides(command, index))
                    {
                        index = check_for_space(command, index);
                        second_result = add_label_extern(&label_list_head, command, index);
                        if(second_result < 0)
                        {
                            switch(second_result)
                            {
                                case -2:
                                    printf("line: %d couldn't allocate memory for label\n", line_number);
                                    ERROR_INIT
                                    break;
                                case -3:
                                    printf("line: %d unexpected text after adding label.\n", line_number);
                                    ERROR_INIT
                                    break;
                                default :
                                    printf("line: %d unexpected text after adding label.\n", line_number);
                                    ERROR_INIT
                            }
                        }
                    }
                    else{
                        printf("line: %d expected label_name after directive extern\n", line_number);
                        ERROR_INIT
                    }
                }
            }
            free(command);
            continue;
        }
        if(label_flag)
        {
            label_flag = OFF;
            strncpy(label_name, (command + label_start_index), label_length);
            label_name[label_length] = '\0';
            if(check_label_exist(label_list_head, label_name))
            {
                printf("line: %d label already exist\n", line_number);
                ERROR_INIT
            }
            second_result = SetNext(&label_list_head, command, label_start_index, label_length, ic_address, 3);/*3- for code Symbol.*/
            if(second_result == -2)
            {
                printf("line: %d couldnt allocate memeory for label\n", line_number);
                ERROR_INIT
            }
        }
        first_result = check_if_opcode(command, index);
        if(first_result < 0)
        {
            printf("line: %d) unknown directive label_name.\n", line_number);
            ERROR_INIT
        }
        final_text = count_num_of_lines(first_result, &L, command, index);
        if(final_text != NULL && final_text[0] == '`')
        {
            printf("Fail in line: %d, index: %d %s.\n", line_number, index, (final_text + 1));
            ERROR_INIT
        }
        ic_address+=L;
        free(command);
    }
    calculate_label(label_list_head, ic_address);
}
void second_file_pass(FILE *file_spanned)
{
    error_flag = OFF;
    int index, end;
    int first_result, second_result;
    int ic_address = 100, line_num = 0;
    char *final_text = NULL, *command = NULL;
    /*start traverse on the file for the second time*/
    while((command = get_file_line(file_spanned)))
    {
        index = 0;
        line_num++;

        /*check for comment*/
        if(!check_txt_sides(command, index) ||
           command[check_for_space(command, 0)] == ';')
            continue;

        if(command[index] == '`'){
            printf("Error: in (line: %d) %s\n", line_num, (command + index + 1) );
            error_flag = ON;
            continue;
        }

        index = check_for_space(command, index);
        end = check_if_label(command, index);

        if(end>0)
            index = check_for_space(command, end + 1);
        if(command[index] == '.'){
            first_result = check_if_directive(command, index);
            if(first_result == 4){
                index += 7;
                if(check_txt_sides(command, index)){
                    index = check_for_space(command, index);
                    end = check_txt_end(command, index);
                    if((end-index) > MAX_LABEL_LEN){
                        printf("line: %d label is too long \n", line_num);
                        ERROR_INIT
                    }
                    final_text = entry_convert(command, index, end - 1);
                    if(final_text){
                        printf("Fail in line: %d index: %d %s.\n", line_num, index, final_text);
                        ERROR_INIT
                    }
                    if(check_txt_sides(command, end)){
                        printf("Fail in line: %d, index: %d unexpected text after label_name.\n", line_num, check_for_space(command, end));
                        ERROR_INIT
                    }
                }else{
                    printf("line: %d expected label_name after entry. \n", line_num);
                    ERROR_INIT
                }
            }
            free(command);
            continue;
        }

        first_result = check_if_opcode(command, index);

        if(first_result < 0)
        {
            ERROR_INIT
        }
        final_text = opcode_to_decode(first_result, command, index, &ic_address);
        if(final_text != NULL && final_text[0] == '`')
        {
            printf("Fail in line: %d, index: %d, %s\n", line_num, index, (final_text + 1));
            ERROR_INIT
        }
        free(command);
    }
    second_result = covert_data_decode(ic_address);
    if(second_result == -2)
    {
        printf("Coultn't allocate memeory properly!\n");
        error_flag = ON;
    }
}