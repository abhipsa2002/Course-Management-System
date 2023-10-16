#ifndef COURSES
#define COURSES

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

struct course viewCourse(int id,int sd)
{
	int i=id;
        struct course curr;
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/courses.data",O_RDONLY);
        
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct course);   
        lock.l_len=sizeof(struct course);    
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock); 

        lseek(fd,(i)*sizeof(struct course),SEEK_SET);
        read(fd,&curr,sizeof(struct course));

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
        return curr;
}




bool addCourse(int sd,int fac_id,struct course newCourse)
{
        int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/courses.data",O_RDWR);
        int fd1=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/faculty.data",O_RDWR);
        struct course last;
        int flag=0;
        struct faculty curr;
        int i = fac_id;
        
        
        bool res;
        char buff[1000];
        bzero(buff, sizeof(buff));
        
        //check if its the first record then create the file and then write
        if (fd == -1 && errno == ENOENT) //ENOENT is  used to represent the "No such file or directory" error.
        {       
                fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/courses.data",O_RDWR | O_CREAT | O_APPEND,0744);
                newCourse.id = 0;
                newCourse.enrolled_count=0;
                newCourse.inst_id=fac_id;
                curr.c[curr.course_count]=newCourse; //assign to faculty 
        	curr.course_count++; //incr course_count of faculty
                strcpy(newCourse.status,"ACTIVE");
                newCourse.availSeats=newCourse.maxSeats;
                flag=1;
        }
        
	//to obtain the faculty having fac_id as we've to increment the course_count there
	struct flock lock1;                
        lock1.l_type = F_WRLCK;
        lock1.l_whence=SEEK_SET;
        lock1.l_start=(i)*sizeof(struct faculty); 
        lock1.l_len=sizeof(struct faculty);         
        lock1.l_pid=getpid();

        fcntl(fd1,F_SETLKW,&lock1); 

        lseek(fd1,(-1)*sizeof(struct faculty),SEEK_CUR);
        read(fd1,&curr,sizeof(struct faculty));
        
        
        struct flock lock;                 //to obtain the last course id
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_END;
        lock.l_start=(-1)*sizeof(struct course); 
        lock.l_len=sizeof(struct course);         
        lock.l_pid=getpid();

        fcntl(fd,F_SETLKW,&lock); 

        lseek(fd,(-1)*sizeof(struct course),SEEK_END);
        read(fd,&last,sizeof(struct course));
	
        if(flag==0)
        {
                newCourse.id=last.id+1;
                newCourse.inst_id=fac_id;
                newCourse.enrolled_count=0;
                curr.c[curr.course_count]=newCourse;
        	curr.course_count++; //incr course_count of faculty
        	strcpy(newCourse.status,"ACTIVE");
        	newCourse.availSeats=newCourse.maxSeats;
               
        }
        //printf("Course ID = %d\n", newCourse.id);
        sprintf(buff, "%s%d\n"," ID of your new Course is ",newCourse.id); //newCourse.id has a value of 123, buff will contain the following string: "User ID of your new Account is 123\n".
        //write back and unlock faculty
	write(fd1,&curr,sizeof(struct faculty));
	lock1.l_type=F_UNLCK;
        fcntl(fd1,F_SETLK,&lock1);
        close(fd1);
        
        int j=write(fd,&newCourse,sizeof(struct course));
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

bool deleteCourse(int sd,int fac_id,struct course c) //to be donw later
{
	int i = c.id;
	int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/courses.data",O_RDWR);
	bool res;
	if(c.inst_id == fac_id && !strcmp(c.status,"ACTIVE"))
	{
		strcpy(c.status,"DELETED");
		
	
	}

}

bool enrollCourse(int studentid,int courseid)
{
	bool res;
	struct course curr;
	int i = courseid;
	int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/courses.data",O_RDWR);
	struct flock lock;
	
	//obtain lock for course
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct course);   
        lock.l_len=sizeof(struct course);    
        lock.l_pid=getpid();
        
        fcntl(fd,F_SETLKW,&lock); 
	
        lseek(fd,(i)*sizeof(struct course),SEEK_SET);
        read(fd,&curr,sizeof(struct course));
        
        int i1 = studentid;
        struct student s;
        int fd1=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDWR);
        //obtain lock for student
	struct flock lock1;
        lock1.l_type = F_WRLCK;
        lock1.l_whence=SEEK_SET;
        lock1.l_start=(i1)*sizeof(struct student);   
        lock1.l_len=sizeof(struct student);    
        lock1.l_pid=getpid();
        
        fcntl(fd1,F_SETLKW,&lock1); 
	
        lseek(fd1,(i1)*sizeof(struct student),SEEK_SET);
        read(fd1,&s,sizeof(struct student));
        
        if(!strcmp(curr.status,"ACTIVE") && curr.availSeats < curr.maxSeats && s.course_count<4)  //check if course is active and seats are available and course_count of student < 4 i.e. max number of courses that can be enrolled
        {
        	curr.enrolled[curr.maxSeats-curr.availSeats]=s.id;
        	curr.availSeats--;
        	s.c[s.course_count]=curr;
        	s.course_count++;  //incr course_count of student
        	res=true;
        	curr.enrolled_count++;
        }
	else
		res=false;
		
	//write and unlock student
	lseek(fd1,(-1)*sizeof(struct student),SEEK_CUR);
	write(fd1,&s,sizeof(struct student));
        lock1.l_type=F_UNLCK;
        fcntl(fd1,F_SETLK,&lock1);
        close(fd1);
	
	//write and unlock course
	
	lseek(fd,(-1)*sizeof(struct course),SEEK_CUR);
	write(fd,&curr,sizeof(struct course));
        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
	return res;
}

bool unenrollCourse(int studentid, int courseid)
{
	bool res=false;
	struct course curr;
	int i = courseid;
	int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/courses.data",O_RDWR);
	//obtain lock for course
	struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct course);   
        lock.l_len=sizeof(struct course);    
        lock.l_pid=getpid();
        
        fcntl(fd,F_SETLKW,&lock); 
	
        lseek(fd,(i)*sizeof(struct course),SEEK_SET);
        read(fd,&curr,sizeof(struct course));
        
        
        //obtain lock for student
        int i1 = studentid;
        struct student s;
        int fd1=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDWR);
	struct flock lock1;
        lock1.l_type = F_WRLCK;
        lock1.l_whence=SEEK_SET;
        lock1.l_start=(i1)*sizeof(struct student);   
        lock1.l_len=sizeof(struct student);    
        lock1.l_pid=getpid();
        
        fcntl(fd1,F_SETLKW,&lock1); 
	
        lseek(fd1,(i1)*sizeof(struct student),SEEK_SET);
        read(fd1,&s,sizeof(struct student));
        
        
        if(strcmp(curr.status,"ACTIVE")==0)            //check if the course is active and the student is enrolled in the course
        {
        	for(int k=0;k<curr.enrolled_count;k++)
        	{
        		if(curr.enrolled[k]==studentid)
        		{
        			--s.course_count;
        			curr.availSeats++;
        			curr.enrolled_count--;
        			res=true;
        			break;
        		}
        	} 
        	
        }
		
	//write and unlock student
	lseek(fd1,(-1)*sizeof(struct student),SEEK_CUR);
	write(fd1,&s,sizeof(struct student));
        lock1.l_type=F_UNLCK;
        fcntl(fd1,F_SETLK,&lock1);
        close(fd1);	
		
	//write and unlock course
	lseek(fd,(-1)*sizeof(struct course),SEEK_CUR);
	write(fd,&curr,sizeof(struct course));
        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
	return res;
}

void viewAllCourses(int sd)
{
	int fd = open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/courses.data", O_RDONLY);
	if(fd==-1) return;
	char buff[1000];
	bzero(buff,sizeof(buff));
	int i=0;
	while(1)
	{
		struct flock lock;
    		lock.l_type = F_RDLCK; // Read lock
    		lock.l_whence = SEEK_SET;
    		lock.l_start = i*sizeof(struct course);
    		lock.l_len = sizeof(struct course);
        	int result = fcntl(fd, F_SETLKW, &lock);
        	if (result == -1) {
            	perror("Locking failed");
            	break;
        	}
        	struct course c;
        	int j = read(fd, &c, sizeof(struct course));
        	if (j == 0) {
            	// End of file reached
            	break;
        	}

        // Process and display the course information (e.g., print it)
        	sprintf(buff,"%d \t, %s\t %d\n", c.id, c.name,c.availSeats);
		write(sd,buff,sizeof(buff));
        // Unlock the record
        	lock.l_type = F_UNLCK;
        	fcntl(fd, F_SETLK, &lock);
        	++i;
	
	}
}




#endif
