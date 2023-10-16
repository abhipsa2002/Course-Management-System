#ifndef STUDENT
#define STUDENT

#include "../structures/macros.h"
#include "../structures/student.h"
#include "../structures/courses.h"
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include <sys/file.h>
#include<unistd.h>
#include<stdbool.h>
#include<errno.h>

#define defaultpass "abc123"

//view requested student 
//this can be implemented by setting a record lock at the desired student id (id is simply the serial number)
struct student viewStudent(int id){
        int i=id;
        struct student curr;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDONLY);
        
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct student);   
        lock.l_len=sizeof(struct student);    
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock); 

        lseek(fd,(i)*sizeof(struct student),SEEK_SET);
        read(fd,&curr,sizeof(struct student));

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
        return curr;
}

bool verifyStudent(struct student curr){
        int i=curr.id;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDONLY);
        bool res;
        struct student temp;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct student);  
        lock.l_len=sizeof(struct student);         
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock);

        lseek(fd,(i)*sizeof(struct student),SEEK_SET);
        read(fd,&temp,sizeof(struct student));
        printf("%s",temp.password);
        if((strcmp(temp.password,curr.password)==0) && (strcmp(temp.status,"ACTIVE")==0))   
        	res=true;
        else                                         
        	res=false;

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
        return res;
}

bool addStudent(struct student newStudent, int sd){
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDWR);
        struct student last;
        int flag=0;
        //char username[10]="MT2023";//
        
        //check if its the first record then create the file and then write
        if (fd == -1 && errno == ENOENT) //ENOENT is  used to represent the "No such file or directory" error.
        {       
                fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDWR | O_CREAT | O_APPEND,0744);
                newStudent.id = 0;
                newStudent.course_count=0;
                strcpy(newStudent.password, defaultpass);
                flag=1;
        }
        bool res;
        char buff[1000];
        bzero(buff, sizeof(buff));
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_END;
        lock.l_start=(-1)*sizeof(struct student); 
        lock.l_len=sizeof(struct student);         
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock); 

        lseek(fd,(-1)*sizeof(struct student),SEEK_END);
        read(fd,&last,sizeof(struct student));

        if(flag==0)
        {
                newStudent.id=last.id+1;
                newStudent.course_count=0;
                strcpy(newStudent.password,defaultpass);
        }
       // printf("User ID = %d\n", newStudent.id);
        sprintf(buff, "%s %d %s \n","User ID and password of your new Account is ",newStudent.id,newStudent.password); //newStudent.id has a value of 123, buff will contain the following string: "User ID of your new Account is 123\n".
        
        strcpy(newStudent.status,"ACTIVE");
	
        int j=write(fd,&newStudent,sizeof(struct student));
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

bool deactivateStudent(int id){
        int i=id;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDWR,0744);
        bool res;

       
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct student);
        lock.l_len=sizeof(struct student);     
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock);  

        struct student curr;
        lseek(fd,(i)*sizeof(struct student),SEEK_SET);
        read(fd,&curr,sizeof(struct student));

        if(!strcmp(curr.status,"ACTIVE")){
                strcpy(curr.status,"DEACTIVATED");
                for(int i=0;i<4;i++)
                {
                	curr.c[i].availSeats++;
                }
                lseek(fd,(-1)*sizeof(struct student),SEEK_CUR);
                int j=write(fd,&curr,sizeof(struct student));
                if(j!=0)        
                	res=true;
                else            
                	res=false;
        }

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        return res;
}

bool activateStudent(int id){
        int i=id;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDWR,0744);
        bool res;

       
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct student);
        lock.l_len=sizeof(struct student);     
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock);  

        struct student curr;
        lseek(fd,(i)*sizeof(struct student),SEEK_SET);
        read(fd,&curr,sizeof(struct student));

        if(!strcmp(curr.status,"DEACTIVATED")){
                strcpy(curr.status,"ACTIVE");
                for(int i=0;i<curr.course_count;i++)
                {
                	curr.c[i].availSeats--;
                }
                lseek(fd,(-1)*sizeof(struct student),SEEK_CUR);
                int j=write(fd,&curr,sizeof(struct student));
                if(j!=0)        
                	res=true;
                else            
                	res=false;
        }

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        return res;
}

bool deleteStudent(int id){
        int i=id;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDWR,0744);
        bool res;

       
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct student);
        lock.l_len=sizeof(struct student);     
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock);  

        struct student curr;
        lseek(fd,(i)*sizeof(struct student),SEEK_SET);
        read(fd,&curr,sizeof(struct student));

        if(!strcmp(curr.status,"ACTIVE")){
                strcpy(curr.status,"DELETED");
                for(int i=0;i<curr.course_count;i++)
                {
                	curr.c[i].availSeats++;
                }
        }
        else if(!strcmp(curr.status,"DEACTIVATED"))
        	strcpy(curr.status,"DELETED");
       	lseek(fd,(-1)*sizeof(struct student),SEEK_CUR);
        int j=write(fd,&curr,sizeof(struct student));
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
