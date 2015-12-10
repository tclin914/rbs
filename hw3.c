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

#define F_CONNECTING 0
#define F_READING    1
#define F_WRITING    2
#define F_DONE       3

void parseString(char* string, int* nbHost);
int readline(int fd,char *ptr,int maxlen);
void printMsg(char* string);
char* hosts[5];
char* ports[5];
char* files[5];

const char* head = "<html>"
                   "<head>"
                   "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\" />"
                   "<title>Network Programming Homework 3</title>"
                   "</head>";
const char* body = "<body bgcolor=#336699>";
const char* font = "<font face=\"Courier New\" size=2 color=#FFFF99>";
const char* table1 = "<table width=\"800\" border=\"1\">"
                     "<tr>";
const char* table_element = "<td>%s</td>";
const char* table2 = "</tr>"
                     "<tr>"
                     "<td valign=\"top\" id=\"m0\"></td><td valign=\"top\" id=\"m1\"></td><td valign=\"top\" id=\"m2\"></td>"
                     "<td valign=\"top\" id=\"m3\"></td><td valign=\"top\" id=\"m4\"></td>"
                     "</tr>"
                     "</table>";
const char* script = "<script>document.all[\'m0\'].innerHTML += \"%s<br>\";</script>";
const char* tail = "</font>"
                   "</body>"
                   "</html>";
const char* br = "<br>";

int main(int argc,char *argv[])
{
    printf(head);
    printf(body);
    printf(font);
    printf(table1);

	char* queryString = getenv("QUERY_STRING");
    /* printf("queryString = %s<br>\n", queryString); */

    int nbHost;
    parseString(queryString, &nbHost);
    /* printf("nbHost = %d<br>\n", nbHost); */

    for (int i = 0; i < nbHost; i++) {
        printf(table_element, hosts[i]);
    }

    printf(table2);
    
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
        if ((n = connect(host_fd[i], (struct sockaddr*)&host_sin[i], sizeof(host_sin[i]))) < 0) {
            if (errno != EINPROGRESS) ;
        }
    }
    
    printf("fd = %d\n", host_fd[0]);

    fd_set rfds;
    fd_set wfds;
    fd_set rs;
    fd_set ws;

    int conn = 1;
    int nfds = FD_SETSIZE;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&rs);
    FD_ZERO(&ws);
    
    FD_SET(host_fd[0], &rs);
    FD_SET(host_fd[0], &ws);

    int status = F_CONNECTING;
    int error;
    char buf[15000];
    while (conn > 0) {
        memcpy(&rfds, &rs, sizeof(rfds));
        memcpy(&wfds, &ws, sizeof(wfds));

        if (select(nfds, &rfds, &wfds, (fd_set*)0, (struct timeval*)0) < 0) printf("select error\n");

        if (status == F_CONNECTING && (FD_ISSET(host_fd[0], &rfds) || FD_ISSET(host_fd[0], &wfds))) {
            if (getsockopt(host_fd[0], SOL_SOCKET, SO_ERROR, &error, &n) < 0 || error != 0) {
                printf("<br>non-blocking fail\n");
            }
            status = F_READING;
            printf("<br>F_CONNECTING\n");
        } else if (status == F_WRITING && FD_ISSET(host_fd[0], &wfds)) {
            n = write(host_fd[0], "ls", 3);
            status = F_READING;
        } else if (status == F_READING && FD_ISSET(host_fd[0], &rfds)) {
            printf("<br>");
            n = read(host_fd[0], buf, sizeof(buf) - 1);
            printMsg(buf);
            n = write(host_fd[0], "ls", 3);
            /* status = F_WRITING; */
            /* FD_CLR(host_fd[0], &rs); */
            /* conn--; */
        }
    }
    close(host_fd[0]);

    /* printf(tail); */
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
                if (strlen(pair + 3) > 0) (*nbHost)++;
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

void printMsg(char* string) {
    char* tmp = string;
    int c = 0;
    while (*tmp != '\0') {
        if (*tmp == '\n') {
            char* msg = strndup(string, c);
            printf(script, msg);
            string = ++tmp;
            c = 0;
            continue;
        }
        ++tmp;
        ++c;
    }
}
