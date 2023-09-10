#include <iostream>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/stat.h>
using namespace std;

void reverse(char buf1[], int a){
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
      long long fd=0;
       long long fd1=0;
    long long part = 5000;
    char buf1[part];
    char buf2[part];
    fd = open(argv[1] , O_RDWR);
    //string p ="Assignment1_1/"; 
    string op = argv[2];
    fd1= open(op.c_str(),O_RDWR);
    off_t filesize = lseek(fd, 0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    off_t outputsize = lseek(fd1,0,SEEK_END);
    //cout<<filesize<<endl;
    const char * filethere = argv[3];
    if(access(filethere,F_OK)!=0){
        cout<<"Directory is created: NO"<<endl;
    }
    else {
        cout<<"Directory is created: Yes"<<endl;
    }


    if(filesize != outputsize){
      cout<< "whether the file is reversed : NO"<<endl;
      goto jump;
      
    }
    else{

    long long last = filesize%part;
    char bufmod1[last];
    char bufmod2[last]; 
   // cout<<last<<endl;
    float percent = 0;
    long long written=0;

    if(last!=0){
         lseek(fd1,-last,SEEK_CUR);
         read(fd1,bufmod2,last);
         read(fd,bufmod1,last);
         reverse(bufmod2,last);
         for(int i=0; i<last;i++){
           if( bufmod1[i] != bufmod2[i]){
                  cout<<"whether the file is reversed : NO"<<endl;
                  goto jump;
                              }
         }
         lseek(fd1,-last,SEEK_CUR);
          }

          long long loop = filesize/part;


         while(loop >0){
         lseek(fd1,-part,SEEK_CUR);

         read(fd1,buf2,part);
         read(fd,buf1,part);
         reverse(buf2,part);

          for(int i=0; i<part;i++){
           if( buf1[i] != buf2[i]){
                  cout<<"whether the file is reversed : NO"<<endl;
                  goto jump;
            }
         }
         lseek(fd1,-part,SEEK_CUR);
         loop--;
                   } 
             }
             cout<<"whether the file is reversed : yes"<<endl;

             //permissions
             jump :
    const char *filename = argv[1]; // Replace with your file's name
    struct stat file_stats;

    if (stat(filename, &file_stats) == 0) {
        cout << "User has read permissions on " << filename << ": " << ((file_stats.st_mode & S_IRUSR) ? "Yes" : "No") << endl;
        cout << "User has write permission on " << filename << ": " << ((file_stats.st_mode & S_IWUSR) ? "Yes" : "No") << endl;
        cout << "User has execute permission on " << filename << ": " << ((file_stats.st_mode & S_IXUSR) ? "Yes" : "No") << endl;
        cout << "Group has read permissions on " << filename << ": " << ((file_stats.st_mode & S_IRGRP) ? "Yes" : "No") << endl;
        cout << "Group has write permission on " << filename << ": " << ((file_stats.st_mode & S_IWGRP) ? "Yes" : "No") << endl;
        cout << "Group has execute permission on " << filename << ": " << ((file_stats.st_mode & S_IXGRP) ? "Yes" : "No") << endl;
        cout << "Others has read permissions on " << filename << ": " << ((file_stats.st_mode & S_IROTH) ? "Yes" : "No") << endl;
        cout << "Others has write permission on " << filename << ": " << ((file_stats.st_mode & S_IWOTH) ? "Yes" : "No") << endl;
        cout << "Others has execute permission on " << filename << ": " << ((file_stats.st_mode & S_IXOTH) ? "Yes" : "No") << endl;
    } else {
        perror("stat");
    }
    const char *filename2 = argv[2]; // Replace with your file's name
   

    if (stat(filename2, &file_stats) == 0) {
        cout << "User has read permissions on " << filename2 << ": " << ((file_stats.st_mode & S_IRUSR) ? "Yes" : "No") << endl;
        cout << "User has write permission on " << filename2 << ": " << ((file_stats.st_mode & S_IWUSR) ? "Yes" : "No") << endl;
        cout << "User has execute permission on " << filename2 << ": " << ((file_stats.st_mode & S_IXUSR) ? "Yes" : "No") << endl;
        cout << "Group has read permissions on " << filename2 << ": " << ((file_stats.st_mode & S_IRGRP) ? "Yes" : "No") << endl;
        cout << "Group has write permission on " << filename2 << ": " << ((file_stats.st_mode & S_IWGRP) ? "Yes" : "No") << endl;
        cout << "Group has execute permission on " << filename2 << ": " << ((file_stats.st_mode & S_IXGRP) ? "Yes" : "No") << endl;
        cout << "Others has read permissions on " << filename2 << ": " << ((file_stats.st_mode & S_IROTH) ? "Yes" : "No") << endl;
        cout << "Others has write permission on " << filename2 << ": " << ((file_stats.st_mode & S_IWOTH) ? "Yes" : "No") << endl;
        cout << "Others has execute permission on " << filename2 << ": " << ((file_stats.st_mode & S_IXOTH) ? "Yes" : "No") << endl;
    } else {
        perror("stat");
    }
    const char *filename3 = argv[3]; // Replace with your file's name
    

    if (stat(filename3, &file_stats) == 0) {
        cout << "User has read permissions on " << filename3 << ": " << ((file_stats.st_mode & S_IRUSR) ? "Yes" : "No") << endl;
        cout << "User has write permission on " << filename3 << ": " << ((file_stats.st_mode & S_IWUSR) ? "Yes" : "No") << endl;
        cout << "User has execute permission on " << filename3 << ": " << ((file_stats.st_mode & S_IXUSR) ? "Yes" : "No") << endl;
        cout << "Group has read permissions on " << filename3 << ": " << ((file_stats.st_mode & S_IRGRP) ? "Yes" : "No") << endl;
        cout << "Group has write permission on " << filename3 << ": " << ((file_stats.st_mode & S_IWGRP) ? "Yes" : "No") << endl;
        cout << "Group has execute permission on " << filename3 << ": " << ((file_stats.st_mode & S_IXGRP) ? "Yes" : "No") << endl;
        cout << "Others has read permissions on " << filename3 << ": " << ((file_stats.st_mode & S_IROTH) ? "Yes" : "No") << endl;
        cout << "Others has write permission on " << filename3 << ": " << ((file_stats.st_mode & S_IWOTH) ? "Yes" : "No") << endl;
        cout << "Others has execute permission on " << filename3 << ": " << ((file_stats.st_mode & S_IXOTH) ? "Yes" : "No") << endl;
    } else {
        perror("stat");
    }
    close(fd);
    close(fd1);





    return 0;
}