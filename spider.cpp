#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <queue>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define DEFAULT_PAGE_BUF_SIZE 1024 * 1024
#define MAX_PATH_LENGTH 1024

using namespace std;

char currentURL[MAX_PATH_LENGTH];
char request[MAX_PATH_LENGTH];
char host[MAX_PATH_LENGTH];
char result[MAX_PATH_LENGTH];
char path[MAX_PATH_LENGTH];
char filename[MAX_PATH_LENGTH];
queue<char*> q;

#include "itoa.h"
#include "bloom.h"
#include "url.h"

int main(int argc,char* argv[]){
    int socket_client;
    int num = 0;
    int isIndex;
    int ContentLength = DEFAULT_PAGE_BUF_SIZE;

    FILE *fp;
    struct sockaddr_in serveraddr;
    char ipaddress[]="127.0.0.1";

    strcpy(currentURL, argv[1]);

    while(!q.empty()){//保证开始时队列为空，可删去
        q.pop();
    }

    q.push(currentURL);

    chdir("pagecrawler");

    while(!q.empty()){
        strcpy(currentURL, q.front());
        q.pop();

        memset(path, 0, MAX_PATH_LENGTH);
        isIndex = getPath(currentURL);
        printf("Get path: %s\n",path);

        memset(host, 0, MAX_PATH_LENGTH);
        if(!url2host(currentURL, host)){//判断是否是http， 提取URL中的host
            num++;
            continue;
        }

        memset(request, 0, MAX_PATH_LENGTH);
        strcat(request,"GET ");
        if(isIndex == 0){
            strcat(request,"/ HTTP/1.1\r\nHost:news.sohu.com");
        } else {
            strcat(request,path);
            strcat(request," HTTP/1.1\r\nHost:news.sohu.com");
        }
        strcat(request,"\r\n");
        strcat(request,"Connection:Close\r\n\r\n");

        socket_client = socket(AF_INET,SOCK_STREAM,0);

        memset(&serveraddr,0,sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(atoi(argv[2]));
        serveraddr.sin_addr.s_addr = inet_addr(ipaddress);

        connect(socket_client,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

        send(socket_client,request,strlen(request),0);

        //Download Page
        char * PageBuf=(char *)malloc(ContentLength);
        memset(PageBuf, 0, ContentLength);

        int byteread = 0;
        int ret = 1;

        while(ret > 0) {
            ret = recv(socket_client, PageBuf + byteread, ContentLength - byteread, 0);
            if(ret > 0) {
                byteread = byteread + ret;
            }
            if(ContentLength - byteread < 100) {
                printf("\nRealloc memory...\n");
                ContentLength = ContentLength * 2;
                PageBuf = (char *)realloc(PageBuf, ContentLength);
            }
        }
        PageBuf[byteread] = '\0';
        close(socket_client);
        strcpy(result, host);
        strcat(result, ".txt");

        memset(filename, 0, MAX_PATH_LENGTH);
        itoa(num++,filename,10);
        strcat(filename,".txt");

        if((fp = fopen(filename, "w")) == NULL){
            return 15;
        }
        fputs(currentURL, fp);
        fputs("\n", fp);
        fputs(request, fp);
        fputs("\n", fp);
        fputs(PageBuf, fp);

        free(PageBuf);
        fclose(fp);

        printf("num = %d", num++);

        //analysis url
        searchURL(filename);

        memset(currentURL, 0, MAX_PATH_LENGTH);

        printf("Queue length: %d\n", int(q.size()));
    }
    return 0;
}
