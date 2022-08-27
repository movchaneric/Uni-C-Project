#define LABELMAXLEN 30

char *entry_convert(char *file_line, int starting_index, int ending_index);
int file_span(const char *fptr);
int covert_data_decode(int ic_address);
char *opcode_to_decode(int opcode_num, char *file_line, int starting_index, int *ic_address);
void hltSys();