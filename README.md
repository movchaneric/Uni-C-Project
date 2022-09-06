# C Project
# Imaginary Assembler for 32 base 
This project is an assignment in a C programming course.
The purpose of this project is to build an assembler for 32-instruction asssembly language, for an imaginary 12-bit CPU and 4096 bits of memory size.

**Task Summery: Create an assembler for fictional assembly language defined by openu course.**

# Stages:
**1.Macros stage** Parsing the original .as file and replacing macros.

**2.First pass** on .as file after macro stage in which we will only count the size of the data image and the instruction image and also we will add all the symbols to the symbol table and will verify that there are no errors in code.

**3.Second pass** on .as file assembler writes the words of each line of code in binary format, we inserting the words to the memory image in the correct position depending on the type of word .data or .instruction and we add each external operand appearence to the linked list of all the positions that external operands appeared.

**4.After Second pass will generate files:**

    1 .am => .as file after macro stage.
    2. .ob => 32 base coverted .as file.
    3. .ex => external labels and there location.
    4. .en => entry labels and there location.
