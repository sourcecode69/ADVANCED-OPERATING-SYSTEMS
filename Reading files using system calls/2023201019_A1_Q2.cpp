#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
using namespace std;

void reverse(char buf1[], long long a){
    int i =0; 
    int j=a-1;
    while(i<j){
        swap(buf1[i],buf1[j]);
        i++;
        j--;
    }
}

void fun(long long fd, long long fd1,char buf1[], long long start, long long end, long long filesize, long long part)
{

    long long last = filesize%part;
    char bufmod[last]; 
    //cout<<last<<endl;
    float percent = 0;
    long long written=0;

    if(last!=0){
         lseek(fd,-last,SEEK_CUR);
         read(fd,bufmod,last);
         reverse(bufmod,last);
         written = last;
         percent = ((float)written/filesize)*100;
         cout<<"\033[2J\033[1;1H";
         cout<< percent <<"%"<<endl;
         write(fd1,bufmod,last);
          lseek(fd,-last,SEEK_CUR);
          }

          long long loop = filesize/part;


         while(loop >0){
         lseek(fd,-part,SEEK_CUR);

         read(fd,buf1,part);
         reverse(buf1,part);
         write(fd1,buf1,part);
         lseek(fd,-part,SEEK_CUR);
         loop--;
         written =written+part;
         percent = ((float)written/filesize)*100;
         cout<<"\033[2J\033[1;1H";
         cout<<percent <<"%"<<endl;
          }
}


int main(int argc, char *argv[])
{
    long long fd=0;
    long long start =0;
    long long end =0; 
     float percent = 0;
    int written=0;
    cin>> start;
    cin>>end;
    long long part = 5000;
    char buf1[part];
    if(argc != 2){
        cout<<"error !"<<endl;
        return 1;
    }
    string in= argv[1];
    fd = open(argv[1] , O_RDWR);
     string st = "2_";
     long long local =string(argv[1]).find_last_of('/');

    if(local == string::npos){
         st = st+argv[1];
    }
    else{
        st = st+string(argv[1]).substr(local+1);
    }

    if(fd==-1){
        perror("cant open");
        return 1;
    }

    char curr[1000];
    const char *op = "Assignment1_2";
    string p;
    if(getcwd(curr,sizeof(curr)) == NULL){
        cout<<"error"<<endl;
        return 1;
    }

    p=string(curr) + '/' + op;//in this path we make directory.
    const char *exist = "Assignment1_2";
    if(access(exist,F_OK)!=0 && mkdir(p.c_str(),0755) !=0){
        perror("");
        return 1;
    }
   
    p=p+'/'+ st;

    fd = open(argv[1] , O_RDWR);
    off_t filesize1 = lseek(fd, start,SEEK_SET);



    cout<<filesize1<<endl;
    long long fd1= open(p.c_str() , O_CREAT |O_TRUNC| O_RDWR | S_IRWXU , 00700);
    //correct
     fun(fd,fd1,buf1,0, start,filesize1,part);

     //copy the part 

     long long copysize = end-start+1;
     long long ls = copysize%part;
     lseek(fd,start,SEEK_CUR);

     if(ls!=0){
     //lseek(fd,start,SEEK_CUR);
        written = ls;
         percent = ((float)written/copysize)*100;
         cout<<"\033[2J\033[1;1H";
         cout<< percent <<"%"<<endl;

     char bufc[ls];
     //lseek(fd,start,SEEK_CUR);
     read(fd,bufc,ls);
     write(fd1,bufc,ls);


     }
     
        long long copyloop = copysize/part;
        while(copyloop>0){
            read(fd,buf1,part);
            write(fd1,buf1,part);
             written =written+part;
         percent = ((float)written/copysize)*100;
         cout<<"\033[2J\033[1;1H";
         cout<<percent <<"%"<<endl;
            copyloop--;
        }


    //last reverse
     off_t fileend= lseek(fd,0,SEEK_END);
     long long filesize2= fileend-end-1;

     fun(fd,fd1,buf1,end, fileend,filesize2,part);

     close(fd);
         close(fd1);

    }