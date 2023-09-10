#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

using namespace std;

void reverse(char *buf1, int a){
    long long i =0; 
    long long j=a-1;
    while(i<j){
        swap(buf1[i],buf1[j]);
        i++;
        j--;
    }
}


int main(int argc, char *argv[])
{
    long long fd =0;
    long long part = 50000;
    char buf1[part];//80 bytes
    float percent = 0;
    if(argc != 2){
        cout<<"error !"<<endl;
        return 1;
    }
    string in= argv[1];
    fd = open(argv[1] , O_RDWR);
    if(fd==-1){
        perror("cant open");
        return 1;
    }

    char curr[1000];
    const char *op = "Assignment1_1";
    string p;
    if(getcwd(curr,sizeof(curr)) == NULL){
        cout<<"error"<<endl;
        return 1;
    }

    p=string(curr) + '/' + op;//in this path we make directory.
    const char *exist = "Assignment1_1";
    if( access(exist,F_OK)!=0 && mkdir(p.c_str(),0755) !=0){
        perror("");
        return 1;
    }
    string st= "1_";

    long long local =string(argv[1]).find_last_of('/');

    if(local == string::npos){
         st = st+argv[1];
    }
    else{
        st = st+string(argv[1]).substr(local+1);
    }
   


    p=p+'/'+ st;
    off_t filesize= lseek(fd,0,SEEK_END);// give data read between 0 and seekend. set it to seekend here also as we initialised.
    //cout<< filesize<<endl;
    long long last = filesize%part;
    char bufmod[last];
    //cout<<last<<endl;

    //char *ans = " "
    if(fd != -1){
        //these operations will be performed as basic file descriptors like 0 , 1,2 are
        //given to standaasrd input, output, error.
         int fd1;
    
         fd1 = open(p.c_str(), O_TRUNC | O_CREAT |O_RDWR | S_IRWXU , 00700);
          //you can read and write the file and directory and 
         //other users have no access to it.

         
         //lseek(fd1,0,SEEK_SET); //set output file pointer to 0 .
         //lseek(fd,-1,SEEK_END); // set input file pointer to last.
         long long written=0;

         if(last!=0){
         lseek(fd,-last,SEEK_CUR);
         read(fd,bufmod,last);
         reverse(bufmod,last);
         //   for(int i =0; i<last; i++){
         //    cout<< bufmod[i]<<" "; 
         // }
         written = last;
         percent = ((float)written/filesize)*100;
         cout<< percent <<endl;
         write(fd1,bufmod,last);
          lseek(fd,-last,SEEK_CUR);
          }

         //before loop
         long long loop = filesize/part;
         //cout<<loop<<endl;
        // int count =1;




         //int start = lseek(fd,0, SEEK_END);
         //int end = lseek(fd,0, SEEK_CUR);
         while(loop >0){
         lseek(fd,-part,SEEK_CUR);

         read(fd,buf1,part);// read from fd and put it in buf1, part is count of bytes to transfer
         reverse(buf1,part);
         // for(int i =0; i<part; i++){
         //    cout<< buf1[i]<<" "; 
         // }
         write(fd1,buf1,part);// write in fd1 from buf2 and last is count of bytes to write
         lseek(fd,-part,SEEK_CUR);
         loop--;
         written =written+part;
         percent = ((float)written/filesize)*100;
         cout<<"\033[2J\033[1;1H";
         cout<<percent <<endl;
        // count++;
          }
         close(fd);
         close(fd1);
        
}
         
return 0;
}