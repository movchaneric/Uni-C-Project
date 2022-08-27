#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fileCheck.h"
#include "label.h"
#include "fileDecoder.h"
#include "converter.h"
#include "lineCalculator.h"
#include "secondPassFunc.h"


extern dirc_list_node dirc_list_head;
extern label_list_node label_list_head;
extern decoder decode_list_head;
extern externInternal extInt_list_head;

char *decode_mover(unsigned int arr[], int n, int *ic_address)
{
    int i;
    char *binary;
    for(i = 0; i < n; i++){

        binary = decimal_to_binary(arr[i]);

        if(binary[0] == '%' || add_new_decode(&decode_list_head,*ic_address,binary) == -2)
            return "Couldnt allocate memeory!\0";

        (*ic_address) += 1;
    }
    return NULL;
}
char *entry_convert(char *file_line, int start_index, int end)
{
    /*name of the label*/
    static char name[LABELMAXLEN];

    /*head to directive_line*/
    label_list_node temp = label_list_head;

    int Nlength = end - start_index +1, i, result;

    /*enter label name into ""*/
    for(i = 0; i < Nlength; i++)
    {
        name[i] = file_line[start_index+i];
    }
    name[i] = '\0';


    while(temp)
    {
        if(strcmp(temp->label_name,name) == 0)
        {
            if(temp->type == 2)
                return "ERROR entry Symbol can't be external one.\0";
            result = add_extern_list(&extInt_list_head, temp->label_name, temp->decimal_address, 0);
            if(result == -2)
                return "ERROR: couldnt allocate memory.\0";
            else
                return NULL;
        }
        temp = temp->next_label;
    }
    return "ERROR: Label doesn't exist in file.\0";
}

int covert_data_decode(int ic_address)
{
    decoder last = decode_list_head,temp;
    dirc_list_node head = dirc_list_head;
    char *binary;
    if(last)
        while(last->next_decode_item)
            last = last->next_decode_item;
    else
    {
        if(head)
        {
            if((temp = (decoder)malloc(sizeof(decode_item))) && (binary = decimal_to_binary(head->decimal_data)))
            {
                temp->decimal_address = ic_address + head-> dc;
                temp->binary_str = binary;
                decode_list_head = temp;
                last = temp;
            }
            else{
                return -2;
            }
        }
    }

    while(head)
    {
        if((temp = (decoder)malloc(sizeof(decode_item))) && (binary = decimal_to_binary(head->decimal_data)))
        {
            temp->decimal_address = ic_address + head->dc;
            temp->binary_str = binary;
            last->next_decode_item = temp;
            last = last->next_decode_item;
        }
        else{
            return -2;
        }
        head = head->next_node;
    }
    return last->decimal_address;
}
char *opcode_to_decode(int num, char *file_line, int start_index, int *ic_address)
{
    int result;
    int numberOfLine = 0;
    label_list_node temp;
    unsigned int nextLine[5] = {0,0,0,0,0};
    int first_word,second_word,end,i;
    static char label_name[LABELMAXLEN];

    numberOfLine++;
    nextLine[0]|= (num<<6);
    start_index+=3;
    if(num>=14)
    {
        if(num == 15)
            start_index+=2;
        if(check_txt_sides(file_line,start_index))
            return "ERROR: unexpected text\0";
        else
            return decode_mover(nextLine,numberOfLine,ic_address);
    }

    if(!check_txt_sides(file_line,start_index))
        return "`ERROR: expected operate after action name.\0";
    start_index = check_for_space(file_line,start_index);
    first_word = get_typeof_symbol(file_line,start_index);
    switch (first_word)
    {
        case 0:
            if(num<=3 || num == 12){
                start_index++;
                end = 0;
                end = check_if_number(file_line,start_index);
                if(end>0)
                {
                    result = text_to_number(file_line,start_index,end-1);
                    if(result>128 || result<-128)
                        return "ERROR: MAX RANGE IN |128| \0";
                    nextLine[numberOfLine]|= ((result<<2)&(-4));

                    numberOfLine++;
                    start_index = end;
                }
                else
                    return "Number is expected after #\0";
            }
            else
                return "First word cant appear here\0";
            break;
        case 1:
            end = 0;
            end = check_txt_end(file_line, start_index);
            if(end - start_index >= LABELMAXLEN)
                return "Label is too long!\0";
            for(i=0;i<end - start_index;i++)
                label_name[i] = file_line[start_index+i];
            label_name[i] = '\0';
            if((temp = get_label(label_list_head,label_name)))
            {
                nextLine[numberOfLine]|= (temp->decimal_address)<<2;
                if(temp->type != 2)
                    nextLine[numberOfLine]|= 2;
                else
                {
                    nextLine[numberOfLine]|= 1;
                    if( add_extern_list(&extInt_list_head, temp->label_name , (*ic_address) + numberOfLine, 1) == -2)
                        return "Couldnt allocate memeory\0";
                }
                if(num<=3 || num == 6)
                    nextLine[0]|= 1 << 4;
                else
                    nextLine[0]|= 1 << 2;

                numberOfLine++;
                start_index = end;
            }
            else
                return "Label couldnt appear here\0";
            break;
        case 2:
            end = 0;
            while(file_line[start_index + end] != '.')
                end++;
            if(end >= LABELMAXLEN)
                return "Label is too long\0";
            for(i=0;i<end;i++)
                label_name[i] = file_line[start_index + i];
            label_name[i] = '\0';
            if((temp = get_label(label_list_head,label_name)))
            {
                if(temp->type == 1 || temp->type == 2)
                {
                    start_index += end+1;
                    nextLine[numberOfLine]|= (temp->decimal_address)<<2;
                    numberOfLine++;
                    if(file_line[start_index] == '1' || file_line[start_index] == '2')
                        nextLine[numberOfLine]|= (file_line[start_index] - '0')<<2;
                    else
                        return "Comma expected after label\0";
                    if(temp->type != 2){
                        nextLine[numberOfLine-1]|= 2;
                        nextLine[numberOfLine]|= 0;
                    }
                    else{
                        nextLine[numberOfLine-1]|= 1;
                        nextLine[numberOfLine]|= 0;
                        if( add_extern_list(&extInt_list_head, temp->label_name , (*ic_address) + numberOfLine, 1) == -2)
                            return "Couldnt allocate memeory\0";
                    }

                    if(num<=3 || num == 6)
                        nextLine[0]|= 2<<4;
                    else
                        nextLine[0]|= 2<<2;
                    numberOfLine++;
                    start_index++;
                }
                else
                    return "No struct label!\0";
            }
            else
                return "First element in struct doesnt exist\0";
            break;
        case 3:
            if(num != 6)
            {
                start_index++;
                if(num<=3)
                {
                    nextLine[0]|= 3<<4;
                    nextLine[numberOfLine]|= (file_line[start_index] - '0')<<6;
                }
                else
                {
                    nextLine[0]|= 3<<2;
                    nextLine[numberOfLine]|= (file_line[start_index] - '0')<<2;
                }
                numberOfLine++;
                start_index++;
            }
            else
                return "First word cant appear here\0";
            break;
    }

    if(num <= 3 || num == 6)
    {
        start_index = check_for_comma(file_line,start_index);
        if(start_index<0)
            return "Comma is expected after\0";
        start_index = check_for_space(file_line,start_index);
        second_word = get_typeof_symbol(file_line,start_index);
        switch(second_word)
        {
            case 0:
                if(num == 1)
                {
                    start_index++;
                    end = 0;
                    end = check_if_number(file_line,start_index);
                    if(end>0)
                    {
                        result = text_to_number(file_line,start_index,end-1);
                        if(result>128 || result<-128)
                            return "ERROR: Range must be |128| \0";
                        nextLine[numberOfLine]|= ((result<<2)&(-4));

                        numberOfLine++;
                        start_index = end;
                    }
                    else
                        return "NUMBER IS EXPECTED AFTER #\0";
                }
                else
                    return "ERROR in second word\0";
                break;
            case 1:
                end = 0;
                end = check_txt_end(file_line, start_index);
                if(end - start_index >= LABELMAXLEN)
                    return "LABEL IS TOO LONG! \0";
                for(i=0;i<end - start_index;i++)
                    label_name[i] = file_line[start_index+i];
                label_name[i] = '\0';
                if((temp = get_label(label_list_head,label_name)))
                {
                    nextLine[numberOfLine]|= (temp->decimal_address)<<2;
                    if(temp->type != 2)
                        nextLine[numberOfLine]|= 2;
                    else
                    {/*EXTERNAL label*/
                        nextLine[numberOfLine]|= 1;
                        if( add_extern_list(&extInt_list_head, temp->label_name , (*ic_address) + numberOfLine, 1) == -2)
                            return "Couldn't allocate memory.\0";
                    }
                    nextLine[0]|= 1<<2;
                    numberOfLine++;
                    start_index = end;
                }
                else
                    return "ERROR: label in second operate doesn't exist!!\0";
                break;
                /*Check for .*/
            case 2:
                end = 0;
                while(file_line[start_index + end]!= '.')
                    end++;
                if(end >= LABELMAXLEN)
                    return "Label is too lone!\0";
                for(i=0;i<end;i++)
                    label_name[i] = file_line[start_index + i];
                label_name[i] = '\0';
                if((temp = get_label(label_list_head,label_name)))
                {
                    if(temp->type == 1 || temp->type == 2)
                    {
                        start_index += end+1;
                        nextLine[numberOfLine]|= (temp->decimal_address)<<2;
                        numberOfLine++;
                        if(file_line[start_index] == '1' || file_line[start_index] == '2')
                            nextLine[numberOfLine]|= (file_line[start_index] - '0')<<2;
                        else
                            return "Expected 1 or 2 after the dot\0";
                        if(temp->type != 2){
                            nextLine[numberOfLine-1]|= 2;
                            nextLine[numberOfLine]|= 0;
                        }
                        else{
                            nextLine[numberOfLine-1]|= 1;
                            nextLine[numberOfLine]|= 0;
                            if( add_extern_list(&extInt_list_head, temp->label_name , (*ic_address) + numberOfLine, 1) == -2)
                                return "Couldnt allocate memory properly\0";
                        }
                        nextLine[0]|= 2<<2;
                        numberOfLine++;
                        start_index++;
                    }
                    else
                        return "Unexpected label of struct\0";
                }
                else
                    return "NO action after label\0";
                break;
            case 3:
                start_index++;
                nextLine[0]|= 3<<2;
                if(first_word == 3)
                {
                    nextLine[numberOfLine-1]|= (file_line[start_index] - '0')<<2;
                }
                else
                {
                    nextLine[numberOfLine]|= (file_line[start_index] - '0')<<2;
                    numberOfLine++;
                }
                start_index++;
                break;
        }
    }
    else
    {
        if(check_txt_sides(file_line,start_index))
            return "ERROR:  unexpected text after operate.\0";
    }
    if(check_txt_sides(file_line,start_index))
        return "ERROR: unexpected text after end of file_line.\0";
    return decode_mover(nextLine,numberOfLine,ic_address);
}

int file_span(const char *fileName)
{
    int entry_flag=0;
    int external_flag=0;
    char *ob_file = NULL;
    char *ent_file = NULL;
    char *external_file = NULL;
    char *textToPrint = NULL;
    FILE *fp1,*fp2;
    decoder ptCode;
    externInternal ptEAE;
    if(is_empty_decoder(decode_list_head))
    {
        ob_file = concatinate_str(fileName,".ob\0");
        if(!ob_file || !(fp1 = fopen(ob_file,"w")))
            return -1;
        fprintf(fp1,"m\tf\n");
        ptCode = decode_list_head;
        while(ptCode)
        {
            textToPrint = current_decoder(&ptCode);
            if(!textToPrint)
            {
                fclose(fp1);
                unlink(ob_file);
                return -1;
            }
            fprintf(fp1,"%s\n",textToPrint);
        }
        fclose(fp1);
    }
    if(!is_externLst_empty(extInt_list_head))
    {
        ptEAE = extInt_list_head;
        while(ptEAE)
        {
            entry_flag = (entry_flag || !(ptEAE->is_external));
            external_flag = (external_flag || (ptEAE->is_external));
            ptEAE = ptEAE->next_externinternal_node;
        }
        if (external_flag)
        {
            external_file = concatinate_str(fileName,".ext\0");
            if(!external_file || !(fp1 = fopen(external_file,"w")))
            {
                unlink(ob_file);
                return -1;
            }
        }
        if(entry_flag)
        {
            ent_file = concatinate_str(fileName,".ent\0");
            if(!ent_file || !(fp2 = fopen(ent_file,"w")))
            {
                unlink(ob_file);
                if(external_file) {unlink(external_file);}
                return -1;
            }
        }

        ptEAE = extInt_list_head;
        while(ptEAE)
        {
            if(ptEAE->is_external)
            {
                fprintf(fp1,"%s\t%s\n",ptEAE->label_name,ptEAE->decimal_address);
            }else
            {
                fprintf(fp2,"%s\t%s\n",ptEAE->label_name,ptEAE->decimal_address);
            }
            ptEAE = ptEAE->next_externinternal_node;
        }
        if (external_flag)
            fclose(fp1);
        if(entry_flag)
            fclose(fp2);
    }

    return 1;
}
void hltSys(){
    free_dirc_list(&dirc_list_head);
    free_label_list(&label_list_head);
    free_extern_list(&extInt_list_head);
    free_decode_list(&decode_list_head);
}