#ifndef FACULTY
#define FACULTY

#include "../structures/macros.h"
#include "../structures/courses.h"
#include "../structures/faculty.h"
#include "../structures/student.h"
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include <sys/file.h>
#include<unistd.h>
#include<stdbool.h>
#include<errno.h>

//#define defaultpass "abc123"

//view requested faculty 
//this can be implemented by setting a record lock at the desired faculty id (id is simply the serial number)
struct faculty viewFaculty(int id){
        int i=id;
        struct faculty curr;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/faculty.data",O_RDONLY);
        
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct faculty);   
        lock.l_len=sizeof(struct faculty);    
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock); 

        lseek(fd,(i)*sizeof(struct faculty),SEEK_SET);
        read(fd,&curr,sizeof(struct faculty));

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
        return curr;
}

bool verifyFaculty(struct faculty curr){
        int i=curr.id;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/faculty.data",O_RDONLY);
        bool res;
        struct faculty temp;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct faculty);  
        lock.l_len=sizeof(struct faculty);         
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock);

        lseek(fd,(i)*sizeof(struct faculty),SEEK_SET);
        read(fd,&temp,sizeof(struct faculty));
        printf("%s",temp.password);
        if(!strcmp(temp.password,curr.password) && !strcmp(temp.status,"ACTIVE"))   
        	res=true;
        else                                         
        	res=false;

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
        return res;
}

bool addFaculty(struct faculty newFaculty, int sd){
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/faculty.data",O_RDWR);
        struct faculty last;
        int flag=0;
        
        //check if its the first record then create the file and then write
        if (fd == -1 && errno == ENOENT) //ENOENT is  used to represent the "No such file or directory" error.
        {       
                fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/faculty.data",O_RDWR | O_CREAT | O_APPEND,0744);
                newFaculty.id = 0;
                newFaculty.course_count=0;
                strcpy(newFaculty.password, defaultpass);
                flag=1;
        }
        bool res;
        char buff[1000];
        bzero(buff, sizeof(buff));
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_END;
        lock.l_start=(-1)*sizeof(struct faculty); 
        lock.l_len=sizeof(struct faculty);         
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock); 

        lseek(fd,(-1)*sizeof(struct faculty),SEEK_END);
        read(fd,&last,sizeof(struct faculty));

        if(flag==0)
        {
                newFaculty.id=last.id+1;
                newFaculty.course_count=0;
                strcpy(newFaculty.password, defaultpass);
        }
        printf("User ID = %d\n", newFaculty.id);
       sprintf(buff, "%s %d %s \n","User ID and password of your new Account is ",newFaculty.id,newFaculty.password); //newFaculty.id has a value of 123, buff will contain the following string: "User ID of your new Account is 123\n".
        strcpy(newFaculty.status,"ACTIVE");

        int j=write(fd,&newFaculty,sizeof(struct faculty));
        if(j!=0)        
        	res=true;
        else    
        	res=false;

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
        write(sd, buff, sizeof(buff));
        return res;
}


bool deleteFaculty(int id){
        int i=id;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/faculty.data",O_RDWR,0744);
        bool res;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct faculty);
        lock.l_len=sizeof(struct faculty);     
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock);  

        struct faculty curr;
        lseek(fd,(i)*sizeof(struct faculty),SEEK_SET);
        read(fd,&curr,sizeof(struct faculty));

        if(strcmp(curr.status,"ACTIVE")==0){
                strcpy(curr.status,"DELETED");
                /*for(int i=0;i<4;i++)
                {
                	curr.c[i].availSeats++;
                }*/
        }
       	lseek(fd,(-1)*sizeof(struct faculty),SEEK_CUR);
        int j=write(fd,&curr,sizeof(struct faculty));
        if(j!=0)        
               	res=true;
        else            
                res=false;

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        return res;
}

#endif
