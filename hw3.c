#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

void parseString(char* string, int* nbHost);
int readline(int fd,char *ptr,int maxlen);
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

    int n;
    int host_fd[5] = {0};
    struct sockaddr_in host_sin[5];
    for (int i = 0; i < nbHost; i++) {
        host_fd[i] = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&host_sin[i], sizeof(host_sin[i]));
        host_sin[i].sin_family = AF_INET;
        host_sin[i].sin_addr.s_addr = inet_addr(hosts[i]);
        host_sin[i].sin_port = htons(atoi(ports[i]));
       
        /* set nonblock */
        int flags = fcntl(host_fd[i], F_GETFL, 0);
        fcntl(host_fd[i], F_SETFL, flags | O_NONBLOCK);
        if ((n = connect(host_fd[i], (struct sockaddr*)&host_sin[i], sizeof(host_sin))) < 0) {
            if (errno != EINPROGRESS) ;
        }
    }
    
    printf("debug\n");
    printf("<br>\n");
    printf("fd = %d\n", host_fd[0]);

    char buff[2048];
    while (1) {
        n = readline(host_fd[0], buff, strlen(buff) + 1);
        printf("n = %d\n", n);
        n = write(host_fd[0], "ls", 3);
        printf(buff);
    }
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

int readline(int fd,char *ptr,int maxlen) {
	int n,rc;
	char c;
	*ptr = 0;
	for(n=1;n<maxlen;n++)
	{
		if((rc=read(fd,&c,1)) == 1)
		{
			*ptr++ = c;	
			if(c=='\n')  break;
		}
		else if(rc==0)
		{
			if(n==1)     return(0);
			else         break;
		}
		else
			return(-1);
	}
	return(n);
}

