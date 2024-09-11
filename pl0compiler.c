//This code is authored by: Richard Zhang and Ethan Jewell

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Define all of the functions we will use to check syntax
#define MAX_ARRAY 500

typedef enum { oddsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
            multsym = 6, slashsym = 7, fisym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym =
            12, gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17,
            semicolonsym = 18, periodsym = 19, becomessym = 20,
            beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, dosym = 26,
            callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
            readsym = 32, elsesym = 33} token_type;

typedef struct {
        int kind; // const = 1, var = 2, proc = 3
        char name[10]; // name up to 11 chars
        int val; // number (ASCII value)
        int level; // L level
        int addr; // M address
        int mark; // to indicate unavailable or deleted
} symbol;

int SymbolTableCheck(char * name);
void Program();
void Block();
void ConstDeclaration();
int VarDeclaration();
void Statement();
void Condition();
void Expression();
void Term();
void Factor();
void getNextToken();

char ** tokens;
int token;
int tokensIndex = 0;

int codeIndex = 0;

int varAddress = 3;

int tokenCount = 0;

int curLevel = 0;

int jmpCount = 0;
int jmpAddr[15];

int proCount = 1;

// Symbol Table
symbol symbol_table[50];
int symbol_table_size = 0;

// Stores output lines
char ** output;
int outputIndex = 0;

int main(int argc, char **argv) {

    tokens = (char **) malloc(MAX_ARRAY * sizeof(char*));
    output = (char**) malloc(sizeof(char*) * 250);

    char reserved[15][10] = {"const", "var", "procedure", "call", "begin", "end", "if", "fi", "then", "%else", "while", "do", "read", "write", "odd"};

    // File input pointer
	FILE * fp = NULL;
	fp = fopen(argv[1], "r");
    char buffer[50] = "";
    int numberBuffer;
    char curRead;
    int bufferIndex = 0;

    if (fp == NULL) {
        printf("File not found...\n");
    }

    fseek(fp, 0, SEEK_SET); // Resets file position to beginning

    // PHASE 1 - Check for input
    while(fscanf(fp, "%c", &curRead) != EOF) {

        // Checking for extraneous spaces
        if(isspace(curRead) || (int)curRead == 10) continue;
        
        // Reset our buffer to empty for new lexeme
        if (bufferIndex == 0) {
            strcpy(buffer, "");
        }

        // Check if current char read is a letter or digit
		if (isalpha(curRead)) {
            
            // Concatenate to end of buffer
            buffer[bufferIndex] = curRead;
            bufferIndex++;

            // PHASE 2 - Check if letter or digit until end of 
            while(fscanf(fp, "%c", &curRead) != EOF) {

                // PHASE 3 - Check if lexeme matches a Token ID
                if (isspace(curRead) || (!isalpha(curRead) && !isdigit(curRead))) {
                    
                    // Token type will default to identifier
                    int value = identsym;
                    buffer[bufferIndex] = '\0';

                    // Check if lexeme is contained within reserved words token list
                    for (int i = 0; i < 15; i++) {
                        if (strcmp(buffer, reserved[i]) == 0) {
                            switch(i) {
                                // numbersym
                                case 0:
                                    value = constsym;
                                    break;
                                // varsym
                                case 1:
                                    value = varsym;
                                    break;
                                // procsym
                                case 2:
                                    value = procsym;
                                    break;
                                // callsym
                                case 3:
                                    value = callsym;
                                    break;
                                // beginsym
                                case 4:
                                    value = beginsym;
                                    break;
                                // endsym
                                case 5:
                                    value = endsym;
                                    break;
                                // ifsym
                                case 6:
                                    value = ifsym;
                                    break;
                                // ifsym
                                case 7:
                                    value = fisym;
                                    break;
                                // thensym
                                case 8:
                                    value = thensym;
                                    break;
                                // thensym
                                case 9:
                                    value = elsesym;
                                    break;
                                // whilesym
                                case 10:
                                    value = whilesym;
                                    break;
                                // dosym
                                case 11:
                                    value = dosym;
                                    break;
                                // readsym
                                case 12:
                                    value = readsym;
                                    break;
                                // writesym
                                case 13:
                                    value = writesym;
                                    break;
                                case 14:
                                    value = oddsym;
                                    break;
                            }
                        }
                    }       
                    
                    // Add our token to our token list if short enough
                    if (bufferIndex < 11) {

                        // Print our token
                        buffer[bufferIndex] = '\0';

                        // Add to token list
                        char * temp = (char *) malloc(sizeof(char) * 12);
                        sprintf(temp, "%d", value);

                        tokens[tokenCount] = temp;
                        tokenCount++;

                        // If an identifier, add the name as well
                        if (value == identsym) {
                            // Add to token list
                            temp = (char *) malloc(sizeof(char) * 12);
                            strcpy(temp, buffer);
                            tokens[tokenCount] = temp;
                            tokenCount++;
                        }

                    } else {
                        // Print our token but with type invalid
                        buffer[bufferIndex] = '\0';
                        printf("%s\t\tError: <name is too long>\n", buffer);
                        exit(0);
                    }

                    // Reset our buffer index
                    bufferIndex = 0;
                    fseek(fp, -1, SEEK_CUR); // Resets file position to read next char on loop
                    break;


                }

                // Add current read character to our lexeme
                buffer[bufferIndex] = curRead;
                bufferIndex++;

            }
        } 
        
        // PHASE 4 - Digit is first token
        else if (isdigit(curRead)) {

            // Concatenate to end of buffer
            buffer[bufferIndex] = curRead;
            bufferIndex++;

            // PHASE 5 - Repeat check for digits
            while(fscanf(fp, "%c", &curRead) != EOF && isdigit(curRead)) {
                buffer[bufferIndex] = curRead;
                bufferIndex++;
            }

            buffer[bufferIndex] = '\0'; // End String

            // Error for if number is too long
            if (bufferIndex >= 6) {
                printf("%s\t\tError: <number too long>\n", buffer);
                exit(0);
            }
            // PHASE 6 - Non-digit value is found and does output
            else {
                // Add to token list
                char * temp = (char *) malloc(sizeof(char) * 12);
                sprintf(temp, "%d", numbersym);
                tokens[tokenCount] = temp;
                tokenCount++;

                // Add literal to token list
                temp = (char *) malloc(sizeof(char) * 12);
                sprintf(temp, "%d", atoi(buffer));
                tokens[tokenCount] = temp;
                tokenCount++;
            }
            bufferIndex = 0;
            fseek(fp, -1, SEEK_CUR); // Resets file position to read next char on loop

        }
        // If the current char read is a special character
        else {

            int value = -1; // stores value for token list

            switch(curRead) {

                // plussym
                case '+':
                    value = plussym;
                    break;

                // minussyyn
                case '-':
                    value = minussym;
                    break;

                // multsym
                case '*':
                    value = multsym;
                    break;

                // slashsym (for comments)
                case '/':
                    fscanf(fp, "%c", &curRead); // lookahead
                    if(curRead == '*') {
                        while(fscanf(fp, "%c", &curRead) != EOF)
                            if(curRead == '/') break;
                    }
                    // For division symbol
                    else {
                        value = slashsym;
                        fseek(fp, -1, SEEK_CUR); // Resets file position
                    }
                    break;

                // lparentsym
                case '(':
                    value = lparentsym;
                    break;

                // rparentsym
                case ')':
                    value = rparentsym;
                    break;

                // eqlsym 
                case '=':
                    value = eqlsym;
                    break;

                // commasym
                case ',':
                    value = commasym;
                    break;
                
                // periodsym
                case '.':
                    value = periodsym;
                    break;

                // lessym & leqsym & neqsym
                case '<':
                    fscanf(fp, "%c", &curRead); // lookahead
                    if(curRead == '=') { // leqsym
                        value = leqsym;
                    }
                    else if(curRead == '>') { // neqsym
                        value = neqsym;
                    }
                    else { // lessym
                        fseek(fp, -1, SEEK_CUR); // Resets file position
                        value = lessym;
                    }
                    break;

                // gtrsym & geqsym
                case '>':
                    fscanf(fp, "%c", &curRead); // lookahead
                    if(curRead == '=') {
                        value = geqsym;
                    }
                    else {
                        fseek(fp, -1, SEEK_CUR); // Resets file position
                        value = gtrsym;
                    }
                    break;

                // semicolonsym
                case ';':
                    value = semicolonsym;
                    break;
                
                // becomessym (assumes equal sign will appear after colon)
                case ':':
                    fscanf(fp, "%c", &curRead); // skips current read
                    value = becomessym;
                    break;

                default:
                    printf("%c\t\tError: <invalid symbol>\n", curRead);
                    exit(0);

            }

            // Add to token list if a proper sym
            if (value != -1) {
                char * temp = (char *) malloc(sizeof(char) * 12);
                sprintf(temp, "%d", value);
                tokens[tokenCount] = temp;
                tokenCount++;
            }

        }
	}

    // Output header
    output[outputIndex] = (char *) malloc(sizeof(char) * 250);
    strcpy(output[outputIndex], "Line\tOP\tL\tM\n");
    outputIndex++;

    Program();
    
    // Close our file pointer
    fclose(fp);

    // Loop through the token list and free them
    for (int i = 0; i < tokenCount; i++) {
        if (tokens[i] != NULL) {
            free(tokens[i]);
        }
    }

    // Adding JMP to beginning of Assembly Code
    for(int i = 0; i<jmpCount; i++) {
        sprintf(output[i+1], "%d\tJMP\t0\t%d\n", i, jmpAddr[jmpCount-i-1]*3);
    }

    // Print out the code to the screen
    fp = fopen(argv[1], "r");

    while (fscanf(fp, "%c", &curRead) != EOF) {
        printf("%c", curRead);
    }

    fclose(fp);

    printf("\n\nNo errors, program is syntatically correct\n");

    printf("\nAssembly Code:\n\n");
    // Loop through the output list and both free and print them to the screen
    for (int z = 0; z < outputIndex; z++) {
        printf("%s", output[z]);
    }

    fp = fopen("elf.txt", "w");
    
    char opcode[12][4] = {"LIT","OPR","LOD","STO","CAL","INC","JMP","JPC","SYS","RTN", "ADD", "SUB"};

    char * currentPrintLine;
    for (int z = 1; z < outputIndex; z++) {
        currentPrintLine = strtok(output[z], "\t");
        for (int x = 0; x < 4; x++) {
            if (x == 1) {
                for (int q = 0; q < 12; q++) {
                    if (strcmp(currentPrintLine, opcode[q]) == 0) {
                        switch(q) {
                            // LIT
                            case 0:
                                fprintf(fp, "1");
                                break;
                            // OPR
                            case 1:
                                fprintf(fp, "2");
                                break;
                            // LOD
                            case 2:
                                fprintf(fp, "3");
                                break;
                            // STO
                            case 3:
                                fprintf(fp, "4");
                                break;
                            // CAL
                            case 4:
                                fprintf(fp, "5");
                                break;
                            // INC
                            case 5:
                                fprintf(fp, "6");
                                break;
                            // JMP
                            case 6:
                                fprintf(fp, "7");
                                break;
                            // JPC
                            case 7:
                                fprintf(fp, "8");
                                break;
                            // SYS
                            case 8:
                                fprintf(fp, "9");
                                break;
                            // RTN
                            case 9:
                                fprintf(fp, "9");
                                break;
                            // ADD
                            case 10:
                                fprintf(fp, "2");
                                break;
                            // SUB
                            case 11:
                                fprintf(fp, "2");
                                break;
                        }
                    }  
                }
            } else if(x != 0) {
                fprintf(fp, "%s", currentPrintLine);
            }
            if (x != 3 && x != 0) {
                fprintf(fp, "\t");
            } else if (x == 3) {
                //fprintf(fp, "\n");
            }
            currentPrintLine = strtok(NULL, "\t");
        }
    }

    fclose(fp);

    // Free our token list
    free(tokens);

    return 0;
}

// Gets next token in token list
void getNextToken() {
    if(tokensIndex >= tokenCount) {
        //printf("Error: <out of bounds error in Token List>\n");
        //exit(0);
        token = -1;
        return;
    } else {
        char * temp = tokens[tokensIndex];
        token = atoi(temp);
        tokensIndex++;
    }
}

// Return index of the token with the specified name, or -1 otherwise
int SymbolTableCheck(char * name) {
    for (int i = symbol_table_size; i >= 0; i--) {
        if (strcmp(symbol_table[i].name, name) == 0 && symbol_table[i].level <= curLevel) {
            return i;
        }
    }
    return -1;
}

void Program() {
    getNextToken();
    Block();
    if (token != periodsym) {
        printf("Error: <program must end with a period>\n");
        exit(0);
    }
    // emit HALT
    output[outputIndex] = (char *) malloc(sizeof(char) * 250);
    sprintf(output[outputIndex], "%d\tSYS\t0\t3\n", codeIndex);
    outputIndex++;
    codeIndex++;
}

void Block() {
    ConstDeclaration();
    int numVars = VarDeclaration();
    varAddress = 3;
    output[outputIndex] = (char *) malloc(sizeof(char) * 250);
    sprintf(output[outputIndex], "%d\tJMP\t0\t3\n", codeIndex);
    outputIndex++;
    codeIndex++;
    while (token == procsym) {
        getNextToken();
        if (token != identsym) {
            printf("Error: <incorrect symbol after procedure declaration>\n");
            exit(0);
        }
        char * temp = tokens[tokensIndex];
        tokensIndex++;
        if (SymbolTableCheck(temp) != -1) {
            printf("Error: <symbol name has already been declared>\n");
            exit(0);
        }
        symbol newSymbol;
        strcpy(newSymbol.name, temp);
        newSymbol.val = 0;
        newSymbol.kind = 3;
        newSymbol.level = curLevel;
        newSymbol.addr = proCount;
        newSymbol.mark = 0;
        symbol_table[symbol_table_size] = newSymbol;
        symbol_table_size++;
        curLevel++; // increment level
        proCount++;
        getNextToken();
        if (token != semicolonsym) {
            printf("Error: <semicolon or comma missing>\n");
            exit(0);
        }
        getNextToken();
        Block();
        if (token != semicolonsym) {
            printf("Error: <semicolon or comma missing>\n");
            exit(0);
        }
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tRTN\t0\t0\n", codeIndex);
        outputIndex++;
        codeIndex++;
        getNextToken();
        curLevel--; // decrement level
        while(symbol_table[symbol_table_size-1].level > curLevel) symbol_table_size--; // removing unusable identifiers from symbol table
    }
    // emit INC (M = 3 + numVars)
    output[outputIndex] = (char *) malloc(sizeof(char) * 250);
    sprintf(output[outputIndex], "%d\tINC\t0\t%d\n", codeIndex, numVars + 3);
    jmpAddr[jmpCount++] = codeIndex; // keeps track of where to JMP
    outputIndex++;
    codeIndex++;
    Statement();
}

void ConstDeclaration() {
    if (token == constsym) {
        do {
            getNextToken();
            if (token != identsym) {
                printf("Error: <const, var, procedure must be followed by identifier>\n");
                exit(0);
            }
            char * temp = tokens[tokensIndex];
            tokensIndex++;
            if (SymbolTableCheck(temp) != -1) {
                printf("Error: <symbol name has already been declared>\n");
                exit(0);
            }
            symbol newSymbol;
            strcpy(newSymbol.name, temp);
            getNextToken();
            if (token != eqlsym) {
                printf("Error: <constants must be assigned with =>\n");
                exit(0);
            }
            getNextToken();
            if (token != numbersym) {
                printf("Error: <constants must be assigned an integer value>\n");
                exit(0);
            }
            getNextToken();
            newSymbol.val = token;
            newSymbol.kind = 1;
            newSymbol.level = curLevel;
            newSymbol.addr = 0;
            newSymbol.mark = 0;
            symbol_table[symbol_table_size] = newSymbol;
            symbol_table_size++;
            getNextToken();
        } while (token == commasym);

            if (token != semicolonsym) {
                printf("Error: <constant and variable declarations must be followed by a semicolon>\n");
                exit(0);
            }
            getNextToken();
    }
}

int VarDeclaration() {
    int numVars = 0;

    if (token == varsym) {
        do {
        numVars++;
        getNextToken();
        if (token != identsym) {
            printf("Error: <const, var, procedure must be followed by identifier>\n");
            exit(0);
        }
        char * temp = tokens[tokensIndex];
        tokensIndex++;
        if (SymbolTableCheck(temp) != -1 && symbol_table[SymbolTableCheck(temp)].level == curLevel) {
            printf("Error: <symbol name has already been declared>\n");
            exit(0);
        }
        // Add to symbol table
        symbol newSymbol;
        strcpy(newSymbol.name, temp);
        newSymbol.val = 0;
        newSymbol.kind = 2;
        newSymbol.level = curLevel;
        newSymbol.addr = varAddress++;
        newSymbol.mark = 0;
        symbol_table[symbol_table_size] = newSymbol;
        symbol_table_size++;
        getNextToken();
        } while (token == commasym);
            
        if (token != semicolonsym) {
            printf("Error: <constant and variable declarations must be followed by a semicolon>\n");
            exit(0);
        }
        getNextToken();
    }

    return numVars;
}

void Statement() {
    if (token == identsym) {
        char * temp = tokens[tokensIndex];
        tokensIndex++;
        int symIndex = SymbolTableCheck(temp);
        if (symIndex == -1) {
            printf("Error: <undeclared identifier>\n");
            exit(0);
        }
        if (symbol_table[symIndex].kind != 2) {
            printf("Error: <assignment to constant or procedure is not allowed>\n");
            exit(0);
        }
        getNextToken();
        if (token != becomessym) {
            printf("Error: <assignment statements must use :=>\n");
            exit(0);
        }
        getNextToken();
        Expression();
        // Emit STO (M = symbol_table[symbolIndex].addr)
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tSTO\t%d\t%d\n", codeIndex, curLevel - symbol_table[symIndex].level, symbol_table[symIndex].addr);
        outputIndex++;
        codeIndex++;
        return;
    }
    if (token == callsym) {
        getNextToken();
        if (token != identsym) {
            printf("Error: <call must be followed by an identifier>\n");
            exit(0);
        }
        char * temp = tokens[tokensIndex];
        tokensIndex++;
        int symIndex = SymbolTableCheck(temp);
        if (symIndex == -1) {
            printf("Error: <undeclared identifier>\n");
            exit(0);
        }
        if (symbol_table[symIndex].kind != 3) {
            printf("Error: <call of a constant or variable is meaningless>\n");
            exit(0);
        }
        getNextToken();
        // Emit CAL (M = symbol_table[symbolIndex].addr)
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tCAL\t%d\t%d\n", codeIndex, curLevel - symbol_table[symIndex].level, symbol_table[symIndex].addr * 3);
        outputIndex++;
        codeIndex++;
        return;
    }
    if (token == beginsym) {
        do {
            getNextToken();
            Statement();
        } while (token == semicolonsym);
        if (token != endsym) {
            printf("Error: <begin must be followed by end>\n");
            exit(0);
        }
        getNextToken();
        return;
    }
    if (token == ifsym) {
        getNextToken();
        Condition();
        int jpcIndex = codeIndex * 3;
        // emit jpc
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tJPC\t0\t%d\n", codeIndex, jpcIndex);
        outputIndex++;
        codeIndex++;
        if (token != thensym) {
            printf("Error: <if must be followed by then>\n");
            exit(0);
        }
        getNextToken();
        Statement();
        if (token != fisym) {
            printf("Error: <then must be followed by fi>\n");
            exit(0);
        }
        getNextToken();
        // code[jpcIndex].M = current code index
        sprintf(output[jpcIndex/3+1], "%d\tJPC\t0\t%d\n", jpcIndex/3, codeIndex*3);
        return;
    }
    if (token == whilesym) {
        getNextToken();
        int loopIndex = codeIndex*3;
        Condition();
        if (token != dosym) {
            printf("Error: <while must be followed by do>\n");
            exit(0);
        }
        getNextToken();
        int jpcIndex = codeIndex*3;
        // emit JPC
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tJPC\t0\t%d\n", codeIndex, jpcIndex);
        outputIndex++;
        codeIndex++;
        Statement();
        // emit JMP (M = loopIndex)
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tJMP\t0\t%d\n", codeIndex, loopIndex);
        outputIndex++;
        codeIndex++;
        // code[jpcIndex].M = current code index
        sprintf(output[jpcIndex/3+1], "%d\tJPC\t0\t%d\n", jpcIndex/3, codeIndex*3);
        return;
    }
    if (token == readsym) {
        getNextToken();
        if (token != identsym) {
            printf("Error: <const, var, procedure must be followed by identifier>\n");
            exit(0);
        }
        char * temp = tokens[tokensIndex];
        tokensIndex++;
        int symIndex = SymbolTableCheck(temp);
        if (symIndex == -1) {
            printf("Error: <undeclared identifier>\n");
            exit(0);
        }
        getNextToken();
        // emit READ
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tSYS\t0\t2\n", codeIndex);
        outputIndex++;
        codeIndex++;
        // emit STO (M = symbol_table[symIndex].addr)
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tSTO\t%d\t%d\n", codeIndex, curLevel - symbol_table[symIndex].level, symbol_table[symIndex].addr);
        outputIndex++;
        codeIndex++;
        return;
    }
    if (token == writesym) {
        getNextToken();
        Expression();
        // emit WRITE
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tSYS\t0\t1\n", codeIndex);
        outputIndex++;
        codeIndex++;
        return;
    }
    
}

void Condition() {
    if (token == oddsym) {
        getNextToken();
        Expression();
        // emit ODD
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tOPR\t0\t11\n", codeIndex);
        outputIndex++;
        codeIndex++;
    } else {
        Expression();
        if (token == eqlsym) {
            getNextToken();
            Expression();
            // emit EQL
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tOPR\t0\t5\n", codeIndex);
            outputIndex++;
            codeIndex++;
        } else if (token == neqsym) {
            getNextToken();
            Expression();
            // emit NEQ
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tOPR\t0\t6\n", codeIndex);
            outputIndex++;
            codeIndex++;
        } else if (token == lessym) {
            getNextToken();
            Expression();
            // emit LSS
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tOPR\t0\t7\n", codeIndex);
            outputIndex++;
            codeIndex++;
        } else if (token == leqsym) {
            getNextToken();
            Expression();
            // emit LEQ
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tOPR\t0\t8\n", codeIndex);
            outputIndex++;
            codeIndex++;
        } else if (token == gtrsym) {
            getNextToken();
            Expression();
            // emit GTR
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tOPR\t0\t9\n", codeIndex);
            outputIndex++;
            codeIndex++;
        } else if (token == geqsym) {
            getNextToken();
            Expression();
            // emit GEQ
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tOPR\t0\t10\n", codeIndex);
            outputIndex++;
            codeIndex++;
        } else {
            printf("Error: <condition must contain comparison operator>\n");
            exit(0);
        }
    }
}

void Expression() {

    Term();
    while (token == plussym || token == minussym) {
        if (token == plussym) {
            getNextToken();
            Term();
            // emit ADD
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tADD\t0\t1\n", codeIndex);
            outputIndex++;
            codeIndex++;
        } else {
            getNextToken();
            Term();
            // emit SUB
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tSUB\t0\t2\n", codeIndex);
            outputIndex++;
            codeIndex++;
        }
    }

}

void Term() {
    Factor();
    while (token == multsym || token == slashsym) {
        if (token == multsym) {
            getNextToken();
            Factor();
            // emit MUL
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tOPR\t0\t3\n", codeIndex);
            outputIndex++;
            codeIndex++;
        } else if (token == slashsym) {
            getNextToken();
            Factor();
            // emit DIV
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tOPR\t0\t4\n", codeIndex);
            outputIndex++;
            codeIndex++;
        }
    }
}

void Factor() {
    if (token == identsym) {
        char * temp = tokens[tokensIndex];
        tokensIndex++;
        int symIndex = SymbolTableCheck(temp);
        if (symIndex == -1) {
            printf("Error: <undeclared identifier>\n");
            exit(0);
        }
        if (symbol_table[symIndex].kind == 1) {
            // emit LIT (M = table[symIndex].val)
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tLIT\t0\t%d\n", codeIndex, symbol_table[symIndex].val);
            outputIndex++;
            codeIndex++;
        } else if (symbol_table[symIndex].kind == 3) {
            printf("Error: <expression must not contain a procedure identifier>\n");
            exit(0);
        } else {
            // emit LOD (M = table[symIndex].addr)
            output[outputIndex] = (char *) malloc(sizeof(char) * 250);
            sprintf(output[outputIndex], "%d\tLOD\t%d\t%d\n", codeIndex, curLevel - symbol_table[symIndex].level, symbol_table[symIndex].addr);
            outputIndex++;
            codeIndex++;
        }
        getNextToken();
    } else if (token == numbersym) {
        // emit LIT
        getNextToken();
        output[outputIndex] = (char *) malloc(sizeof(char) * 250);
        sprintf(output[outputIndex], "%d\tLIT\t0\t%d\n", codeIndex, token);
        outputIndex++;
        codeIndex++;
        getNextToken();
    } else if (token == lparentsym) {
        getNextToken();
        Expression();
        if (token != rparentsym) {
            printf("Error: <right parenthesis must follow left parenthesis>\n");
            exit(0);
        }
        getNextToken();
    } else {
        printf("Error: <arithmetic equations must contain operands, parentheses, numbers, or symbols>\n");
        exit(0);
    }
}

