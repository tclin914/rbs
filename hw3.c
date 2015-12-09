#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseString(char* string, int* nbHost);
char* hosts[5];
char* ports[5];
char* files[5];

int main(int argc,char *argv[])
{
    printf("Content-type:text/html\n\n");
    printf("<head>");
    printf("<title>Hello world it is cgi test</title>\n");
    printf("</head>\n");
    printf("<body>\n");
    printf("<h1>Hello world </h1>\n");
    printf("</body>\n");

    char* queryString = getenv("QUERY_STRING");
    printf("queryString = %s\n", queryString);
    printf("<br>\n");

    int nbHost;
    parseString(queryString, &nbHost);
    printf("nbHost = %d\n", nbHost);
    return 0;
}

void parseString(char* string, int* nbHost) {
    char* pair = NULL;
    char* key;
    int count = 0;
    pair = strtok(string, "&");
    while (pair != NULL) {
        switch (count % 3) {
            case 0:
                hosts[count / 3] = (pair + 3);
                printf("%s\n", hosts[count / 3]);
                break;
            case 1:
                ports[count / 3] = (pair + 3);
                printf("%s\n", ports[count / 3]);
                break;
            case 2:
                files[count / 3] = (pair + 3);
                printf("%s\n", files[count / 3]);
                break;
        }
        ++count;
        pair = strtok(NULL, "&");
    }
    *nbHost = (count / 3);
}
