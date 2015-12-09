#include <stdio.h>
#include <stdlib.h>

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
    return 0;
}
