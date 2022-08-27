#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef mask
#define mask 1
#endif

int binary_to_decimal(char *binary_str);                        /* Receives an array of chars holding a number in its binary form, and returns the number in decimal*/
char * decimal_to_binary(int num);                       /* Receives a number and returns an array holding the number in binary form */
char *binary_to_base32(char *binary_str);                        /* Receives a char array holding a number in binary, and returns an array holding the number in the special 32 base */
char * decimal_to_base32(int num);                     /* Receives a number, and returns an array holding the number in the special 32 base */
char* concatinate_str(const char *s1, const char *s2);       /* input: two strings.  Output: one string that is the combination of both strings*/
int text_to_number(char *text, int from, int end);       /* Input: Receives a string and integer 'from', and integer 'end'.  Output: A number that is a substring of this string from 'from' to 'end'*/


