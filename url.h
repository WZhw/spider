#ifndef URL_H_INCLUDED
#define URL_H_INCLUDED

int getPath(char currenturl[]){

    int i=20;
    int j=0;

    if (currenturl[20]=='/' && currenturl[21]!='\0'){
        while(currenturl[i]!='\0'){
            path[j]=currenturl[i];
            i++;
            j++;
        }
        path[j]='\0';
    } else {
        return 0;
    }
    return 1;
}



int searchURL(char currentpage[]){

    int state;
    int i,j=0,n;
    char currentchar;
    char urlbuf[MAX_PATH_LENGTH];
    char readbuf[MAX_PATH_LENGTH];
    char *searchedurl;
    char urlhttp[13]="http://news.";
    //int num = 0;

    state = 0;
    FILE *fp = fopen(currentpage,"r");
    printf("Open file:%s\n", currentpage);
    if(fp == NULL){
        printf("File open error!\n");
        return 0;
    }

    while(fgets(readbuf, MAX_PATH_LENGTH, fp)!= NULL){
        //printf("Read No.%d\n",num++);
        for(i=0; readbuf[i] != '\0'; i++){
            currentchar=readbuf[i];
            switch(state){
                case 0:if(currentchar == '<'){
                           state=1; break;
                       } else {
                           state=0; j=0; break;
                       }
                case 1:if(currentchar == 'a'){
                           state=2; break;
                       } else {
                           state=0; j=0; break;
                       }
                case 2:if(currentchar == 'h'){
                           state=3; break;
                       } else if(currentchar == '>'){
                           state=0; j=0; break;
                       } else {
                           state=2; break;
                       }
                case 3:if(currentchar == 'r'){
                           state=4; break;
                       } else if(currentchar == '>') {
                           state=0; j=0; break;
                       } else {
                           state=2; break;
                       }
                case 4:if(currentchar == 'e'){
                           state=5; break;
                       }
                       else if(currentchar == '>'){
                           state=0; j=0; break;
                       } else {
                           state=2; break;
                       }
                case 5:if(currentchar == 'f') {
                           state=6; break;
                       } else if(currentchar == '>'){
                           state=0; j=0; break;
                       } else {
                           state=2; break;
                       }
                case 6:if(currentchar == '='){
                           state=7; break;
                       }
                       else if(currentchar == '>'){
                           state=0; j=0; break;
                       } else {
                           state=2; break;
                       }
                case 7:if(currentchar == '"') {
                           state=10; break;
                       } else if(currentchar == ' ') {
                           state=7; break;
                       } else {
                           state=0; j=0; break;
                       }
                case 10:if((currentchar=='"')||(currentchar=='||')||(currentchar=='>')||(currentchar=='#')) {
                            state=0; j=0; break;
                        } else if(currentchar == '/') {
                            state=8;
                            urlbuf[j++]=currentchar;
                            break;
                        } else {
                            state=10;
                            urlbuf[j++]=currentchar;
                            break;
                        }
                case 8:if(currentchar == '"'){
                           state=9; break;
                       } else if(currentchar=='>') {
                           state=0; j=0; break;
                       } else {
                           state=8;
                           urlbuf[j++]=currentchar;
                           break;
                       }
                case 9:urlbuf[j] = '\0';      //char urlhttp[13]="http://news.";
                       state=0;
                       for(n = 0;n <= 11;n++){
                           if(urlbuf[n] != urlhttp[n]){
                               break;
                           }
                       }
                       if(n==12){
                           searchedurl=(char*)malloc(sizeof(urlbuf));
                           strcpy(searchedurl,urlbuf);
                           if(!bloomFilter(searchedurl) && (strlen(searchedurl) < MAX_PATH_LENGTH)) {
                                q.push(searchedurl);
                           }
                           state=0;
                           j=0;
                       }
                       break;
            }
        }
    }
    return 1;
}

int url2host(char url[],char host[]) {//判断是否是https, 将url中的hosts提取出来
    int i,j=0;
    int length;

    if(url[4]==':') { //http
        i=7;
    } else {  //https
         //i=8;
        return 0;
    }

    while(url[i]!='\0'){
        host[j++] = url[i++];
    }
    host[j]='\0';

    length=strlen(host);
    if(host[length-1]=='/'){
        host[length-1]='\0';
    }

    return 1;
}


#endif // URL_H_INCLUDED
