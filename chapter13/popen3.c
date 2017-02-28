#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MYBUFSIZ BUFSIZ/10	//BUFSIZ-1

int main()
{
    FILE *read_fp;
    char buffer[MYBUFSIZ + 1];
    int chars_read;

    memset(buffer, '\0', sizeof(buffer));
    read_fp = popen("ps ax", "r");
    if (read_fp != NULL) {
        chars_read = fread(buffer, sizeof(char), MYBUFSIZ, read_fp);
        while (chars_read > 0) {
            buffer[chars_read - 1] = '\0';
            printf("Reading %d:-\n %s\n", MYBUFSIZ, buffer);
            chars_read = fread(buffer, sizeof(char), MYBUFSIZ, read_fp);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

