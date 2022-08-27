/*
 * preproccess.h function implementation
*/
#ifndef PREPROCCESS
#define PREPROCCESS
#include "preProccess.h"

/*Print all macros */
void print_macros(struct macro* head){
    struct macro* temp_macro = NULL;

    temp_macro = (struct macro*) malloc(sizeof(struct macro));

    temp_macro = head->next_macro;
    while(temp_macro != NULL){
        printf("Name of macro: %s\n",(temp_macro->macro_name));
        printf("Macro %s content: %s",temp_macro->macro_name,temp_macro->macro_content);
        printf("\n");
        temp_macro = temp_macro->next_macro;
    }
}

/*Add new macro to linked list to keep track of all macros
*/
void add_macro_to_linked_list(struct macro *head,char macro_name[],char macro_contect[]){
    struct macro* new_macro = NULL;
    new_macro = (struct macro*) malloc(sizeof(struct macro));
    if(new_macro == NULL){
        printf("Error - Couldnt allocate memory for new macro! \n");
        exit(1);
    }
    /*initialize values of macro_name and macro_content to '\0' at the beginning
     * using memset to save lines and avoid garbage values*/
    memset(new_macro->macro_name, '\0', LEN);
    memset(new_macro->macro_content, '\0', LEN);
    strcpy(new_macro->macro_name,macro_name);
    strcpy(new_macro->macro_content,macro_contect);
    head->next_macro = new_macro;
}
/*
 *"is_macro_or_endmacro" Check if start of line in macro or endmacro
 * index to traverse the line and check for spaces and \n
 * macro_index to copy the relevent char if there is no space or \n
*/
int is_macro_or_endmacro(char line[]){
    int index = 0, macro_index = 0;
    char temp[LEN];

    /*init values of temp with '\0'*/
    memset(temp, '\0', LEN);

    /*Check for spaces with isspace()*/
    while(isspace(line[index])){
        index++;
    }
    /*NO space and NO new line*/
    while(!isspace(line[index]) && line[index] !='\n'){
        temp[macro_index] = line[index];
        index++;
        macro_index++;
    }

    if(!strcmp(temp,"macro")){
        return 1;
    }
    if(!strcmp(temp,"endmacro")){
        return 2;
    }
    return 0;

}

/*
 *"insert_macro_name" enter macro name from file to macro_temp
 * index to traverse the line and check for spaces and \n
 * name_index to copy the relevent char if there is no space or \n
*/
void insert_macro_name(char line[], struct macro *macro_temp){
    char name[LEN];
    int index = 0, name_index = 0;
    memset(name, '\0', LEN);
    /*Check for spaces and continue*/
    while(isspace(line[index])){
        index++;
    }

    while (!isspace(line[index]) && line[index] != '\n')
        index++;
    while(isspace(line[index]))
        index ++;

    /*Check if no spaces and no new line*/
    while(!isspace(line[index]) && line[index] != '\n'){
        name[name_index] = line[index];
        name_index++;
        index++;
    }
    strcpy(macro_temp->macro_name,name);
}

/*
 *"insert_macro_content" enter traverse from file and move to macro table
 * index to traverse the line and check for spaces and \n
 * name_index to copy the relevent char if there is no space or \n
*/
void insert_macro_content(struct macro *macro_temp,FILE *file){
    char content[LEN];
    char line[LEN];
    memset(content, '\0', LEN);
    memset(content, '\0', LEN);
    /*read line from file*/
    fgets(line, LEN, file);
    /*Check if not endmacro */
    while(is_macro_or_endmacro(line) != 2){
        strlcat(content, line, LEN);
        fgets(line, LEN, file);
    }
    /*move all contect to macro_temp contect*/
    strcpy(macro_temp->macro_content,content);
}

/*traverse first on file to look for macros and insert them into the linked-list*/
int pre_proccess_before_span(char *argv[] , struct macro* head, int i ){
    FILE *fptr;
    char read_line[500];
    memset(read_line, '\0', LEN);

    /*open file from argv list*/
    fptr = fopen(argv[i],"r");
    printf("file name is %s",argv[1]);

    /*Check if file is openable*/
    if(fptr == NULL){
        printf("Couldnt open file %s ! \n",argv[i]);
        return 1;
    }
    while(fgets(read_line, 500, fptr)){
        struct macro* new_macro = NULL;
        new_macro = (struct macro*) malloc(sizeof(struct macro));

        /*if there is a "macro"  add the new macro to the list*/
        if(is_macro_or_endmacro(read_line) == 1){
            insert_macro_name(read_line,new_macro);
            insert_macro_content(new_macro,fptr);
            head->next_macro = new_macro;
            head= new_macro;
        }
    }
    return 0;
}

/*while traversing file when name of macro apears will insert the content of the macro in the right place in the file*/
int file_macro_call(struct macro* head, FILE* file, char line[]){
    int index = 0;
    int macro_index = 0;
    struct macro* macro_temp = NULL;
    char macro_name[LEN];

    /*init values of macro_temp with '\0'*/
    memset(macro_name, '\0', LEN);

    /*allocate memory*/
    macro_temp = (struct macro*)malloc(sizeof(struct macro));

    if(macro_temp == NULL){
        printf("Couldnt allocate memory\n\n");
        return 0;
    }

    macro_temp = head;

    /*Check for spaces with isspace()*/
    while(isspace(line[index])){
        index++;
    }
    /* if NO space and NO new line*/
    while( (!isspace(line[index])) && line[index] != '\n'){
        macro_name[macro_index] = line[index];
        macro_index++;
        index++;
    }
    while(macro_temp != NULL){
        /*if !strcmp == 1 -> true => replce content of current macro with the macro call*/
        if(!strcmp(macro_temp->macro_name,macro_name)){
            fprintf(file, "%s", macro_temp->macro_content);
            return 1;
        }
        /*Move on to the next macro in the linkedlist*/
        macro_temp = macro_temp->next_macro;
    }
    return 0;
}

/*after traversing the file file_macro_span will insert all macros in the right place*/
void file_macro_span(int i, char *argv[], struct macro *head) {
    /*macro flag*/
    int is_macro = 0;

    /*init files*/
    FILE *file_write;
    FILE *file_read;

    char line[LEN];
    char file_name[LEN];
    char file_name2[LEN];

    memset(line, '\0', LEN);
    strcpy(file_name, argv[i]);
    strcpy(file_name2, argv[i]);

    /*concatenate .as suffix to file*/
    strncat(file_name2, ".am", 3);

    file_read = fopen(file_name, "r");
    file_write = fopen(file_name2, "w");

    if (file_read == NULL)
        printf("Cannot open file: %s\n", argv[i]);

    while (fgets(line, LEN, file_read)){
        if (!is_macro)
        {
            /*Check if start of macro*/
            if (!file_macro_call(head, file_write, line))
            {
                if (is_macro_or_endmacro(line) == 0){
                    /*print to file corresponding line from macro linked list*/
                    fprintf(file_write, "%s", line);
                }else{
                    is_macro = 1;
                }
            }
        }
        else
        {
            if (is_macro_or_endmacro(line) == 2)
                is_macro = 0;
        }
    }
    fclose(file_write);
    fclose(file_read);
}


#endif /* PREPROCCESS */

