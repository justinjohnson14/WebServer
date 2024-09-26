#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int main()
{
    char buffer[1024];
    FILE* fp = fopen("test.txt", "r");

    fread(buffer, sizeof(char), 1024, fp);

    char* line;
    char* end_ln;
    int lineCount = 0;

    line = strtok_r(buffer, "\r", &end_ln);
    while(line != NULL){
        //char* end_wd;
        //char* word = strtok_r(line, " ", &end_wd);

        if(line == "\n")
        {
            printf("%s", line);
        }
        else {
            printf("%s\n", line);
        }
        
        line = strtok_r(NULL, "\n", &end_ln);
        lineCount++;
    }
}

/*
while(word != NULL)
        {
            int wordCount = 0;

            printf("%s\n", word);

            word = strtok_r(NULL, " ", &end_wd);
            wordCount++;
        }
*/