#include "converter.h"
#define ELEVEN 11
#define TEN 10
#define NINE 9
#define THREE 3
#define FIVE 5
#define FOUR 4
#define TWO 2

char base32[32]={'!','@','#','$','%',
                  '^','&','*','<','>',
                  'a','b','c','d','e',
                  'f','g','h','i','j',
                  'k','l','m','n','o',
                  'p','q','r','s','t',
                  'u','v'};

char * decimal_to_base32(int decimal_num){

    int i = 0,j,first_part = 0 , second_part = 0,p  = 0,half_p = 4,second_half_p = 9;
    char* result = (char*) malloc(FOUR * sizeof(char *));
    unsigned int temp_num = decimal_num;
    char placeholder_char;
    char remainder_arr[TEN];

    /*Check memory allocation*/
    if(result == NULL){
        printf("Error - Couldn't allocate memory! \n");
        exit(1);
    }

/*fill remainder_arr 1 if there is a remainder and 0 if there is not */
    while ( i < TEN){
        remainder_arr[i] = temp_num % 2 + '0';
        temp_num /= 2;
        i++;
    }
/*swap elements*/
    for (j = 0; j < FIVE; j++) {
        placeholder_char = remainder_arr[NINE - j];
        remainder_arr[NINE - j] = remainder_arr[j];
        remainder_arr[j] = placeholder_char;
    }

    if(!result){
        return "Coultnot allocate memeroy";
    }

    while (p <= 4) {
        first_part += pow(2, p++) * (remainder_arr[half_p--] - '0');
    }
    p = 0;
    while (p <= 4) {
        second_part += pow(2, p++) * (remainder_arr[second_half_p--] - '0');
    }
    result[0] = base32[first_part];
    result[1] = base32[second_part];
    return result;
}
char *binary_to_base32(char *binary_str){

    int count=0;
    int num=0;
    int i=0;
    int position=1;

    char * newBase32 = malloc(THREE * sizeof(char));

    if(!newBase32){
        return "Couldn't allocate memory properly\0";
    }
    for (i = 9; i > -1; i--){

        num += pow(2,count) * (binary_str[i]-'0');
        count++;

        if(count == FIVE){
            newBase32[position]=base32[num];
            position--;
            count=0;
            num=0;
        }
    }
    newBase32[TWO]='\0';
    return newBase32;
}


int binary_to_decimal(char *binary_str){
    int i;
    int num=0;

    for (i = 9; i > -1; i--){
        num += pow(TWO, NINE-i) * (binary_str[i]-'0');
    }

    if(binary_str[0] == '1'){
        return 0-(pow(2,10)-num);
    }

    return num;
}
char * decimal_to_binary(int num){
    int i = 0;
    int j;
    unsigned int temp_num = num;
    char placeholder_char;
    char* result = (char*) malloc(ELEVEN * sizeof(char *));

    /*Check memory allocation*/
    if(result == NULL){
        printf("Error - Couldn't allocate memory! \n");
    }

/*fill remainder_arr 1 if there is a remainder and 0 if there is not */
    while ( i < TEN){
        *(result +i) = temp_num % TWO + '0';
        temp_num /= TWO;
        i++;
    }

/*swap elements*/
    for (j = 0; j < FIVE; j++) {
        placeholder_char = *(result + NINE - j);
        *(result + NINE - j) = *(result + j);
        *(result + j) = placeholder_char;
    }
    return result;
}

char* concatinate_str(const char *destStr, const char *srcStr)
{
    char *result = malloc(strlen(destStr) + strlen(srcStr) + 1);

    if(result){
        strcpy(result, destStr);
        strcat(result, srcStr);
        return result;
    }
    /*Failed to allocate memory*/
    else{return NULL;}
}
int text_to_number(char *text, int from, int end){
    int i = 0;
    int result = 0,start = from;
    if(text[start] == '-' || text[start] == '+')
        start++;
    while(end-i >= start)
    {
        result += pow(TEN,i) * ((int)(text[end-i] - '0'));
        i++;
    }
    return (text[from] == '-')? -result: result;
}