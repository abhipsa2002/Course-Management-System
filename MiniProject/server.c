#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include<unistd.h>
#include<stdbool.h>
#include<strings.h>
#include<string.h>

#include "./structures/student.h"
#include "./structures/faculty.h"
#include "./structures/courses.h"
#include "./structures/macros.h"
#include "./structures/admin.h"
#include "./temp/password.h"
#include "./temp/student_func.h"
#include "./temp/faculty_func.h"
#include "./temp/courses_func.h"


bool verifyAdmin(struct admin);
void viewEnrolledCourses(int nsd,int studentid);
bool viewEnrolledStudents(int nsd,int courseid, int facultyid);
bool verifyAdmin(struct admin curr);
bool change_pass(int usr, int id,char newpass[10], char oldpass[10]);


void serWork(int nsd)
{
	int select,type,opt,usr,currid;
	bool res;
	while(1){
		read(nsd,&opt,sizeof(opt));
		
		if(opt==1){
			struct faculty curr;
			read(nsd,&curr,sizeof(struct faculty));
			usr=1;
			currid=curr.id;
			res=verifyFaculty(curr);
			write(nsd,&res,sizeof(res));
		}
		if(opt==2){
			struct student curr;
			read(nsd,&curr,sizeof(struct student));
			usr=2;
			currid=curr.id;
			res=verifyStudent(curr);
			write(nsd,&res,sizeof(res));
		}
		else if(opt==3){
			struct admin curr;
			read(nsd,&curr,sizeof(struct admin));
			usr=3;
			currid=curr.id;
			res=verifyAdmin(curr);
			write(nsd,&res,sizeof(res));
		}
		else{
			res=false;
			write(nsd,&res,sizeof(res));
		}
		if(res)	break;
	}
	while(1){
		read(nsd,&select,sizeof(int));
		if(opt==1){
			if(select==1){   //add course
				struct course c;
				read(nsd,&c,sizeof(struct course));
				res=addCourse(nsd,currid,c);
				write(nsd,&res,sizeof(res));
			}
			else if(select==2){
				struct course c;
				read(nsd,&c,sizeof(struct course));
				//res=deleteCourse(nsd,currid,c);
				write(nsd,&res,sizeof(res));
			}
			else if(select==3){
				int courseid;
				read(nsd,&courseid,sizeof(int));
				res=viewEnrolledStudents(nsd,courseid,currid);
				write(nsd,&res,sizeof(res));
			}
			
			else if(select==4){
				char oldpass[10];
				char newpass[10];
				read(nsd,oldpass,sizeof(oldpass));
				read(nsd,newpass,sizeof(newpass));
				res=change_pass(usr,currid,newpass,oldpass);
				write(nsd,&res,sizeof(res));
			}
			else if(select==5)	break;
		}
		else if(opt==2)
		{
			if(select==1)  //view all courses
			{
				viewAllCourses(nsd);
			
			}
			
			if(select==2)  //enroll course
			{
				int courseid;
				read(nsd,&courseid,sizeof(int));
				res=enrollCourse(currid,courseid);
				write(nsd,&res,sizeof(res));
			}
			else if(select==3)  //unenroll
			{
				int courseid;
				read(nsd,&courseid,sizeof(int));
				res=unenrollCourse(currid,courseid);
				write(nsd,&res,sizeof(res));
			}
			else if(select==4)   //view enrolled courses
			{
				
				viewEnrolledCourses(nsd,currid);
			}
			else if(select==5)
			{
				char oldpass[10];
				char newpass[10];
				read(nsd,oldpass,sizeof(oldpass));
				read(nsd,newpass,sizeof(newpass));
				res=change_pass(usr,currid,newpass,oldpass);
				write(nsd,&res,sizeof(res));
			
			
			}
		}
		else if(opt==3){
			
			if(select==1){
			read(nsd,&type,sizeof(int));
				if(type==1){
					struct faculty newfac;
					read(nsd,&newfac,sizeof(struct faculty));
					res=addFaculty(newfac, nsd); //in faculty_func.h
					write(nsd,&res,sizeof(res));
				}
				else if(type==2){
					struct student newstud;
					read(nsd,&newstud,sizeof(struct student));
					res=addStudent(newstud, nsd); //in student_func.h
					write(nsd,&res,sizeof(res));
				}
			}
			else if(select==2){
					int id;
					read(nsd,&id,sizeof(int));
					res=deleteStudent(id); //in student_func.h
					write(nsd,&res,sizeof(res));
				
			}
			else if(select==3){
					int id;
					read(nsd,&id,sizeof(int));
					res=activateStudent(id);    //in student_func.h
					write(nsd,&res,sizeof(res));
			}
			else if(select==4){
					int id;
					read(nsd,&id,sizeof(int));
					res=deactivateStudent(id);   //in student_func.h
					write(nsd,&res,sizeof(res));
			}
			
			else if(select==5){
				read(nsd,&type,sizeof(int));
				if(type==1){
					struct faculty fac;
					int id;
					read(nsd,&id,sizeof(int));
					fac=viewFaculty(id);     //in faculty_func.h
					write(nsd,&fac,sizeof(struct faculty));
				}
				else if(type==2){
					struct student stud;
					int id;
					read(nsd,&id,sizeof(int));
					stud=viewStudent(id);   //in student_func.h
					write(nsd,&stud,sizeof(struct student));
				}
			}
			
			else if(select==6)	break;
		}
	}
	close(nsd);
	write(1,"Client session Ended\n",sizeof("Client session Ended\n"));
	return;
	
	//to add the course (pass the faculty struct also) 
}


void viewEnrolledCourses(int nsd,int studentid)
{
	int fd = open("home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDONLY);
	char buff[1000];
	int i = studentid;
	struct student curr;
	
	struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct course);   
        lock.l_len=sizeof(struct course);    
        lock.l_pid=getpid();
        fcntl(fd,F_SETLKW,&lock); 	
        lseek(fd,(i)*sizeof(struct course),SEEK_SET);
        read(fd,&curr,sizeof(struct course));
        for(int i=0;i<curr.course_count;i++)
        {
        	//snprintf(buff, sizeof(int),curr.c[i].id);
        	sprintf(buff, "%s %d %s \n","Enrolled course ID and name of your new Account is ",curr.c[i].id,curr.c[i].name);
        	write(nsd,&buff,sizeof(int));
        }

        write(nsd,&buff,sizeof(int));
        
        //unlock student
        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        
        
}

bool viewEnrolledStudents(int nsd,int courseid, int facultyid)
{
	int fd = open("home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/courses.data",O_RDONLY);
	int fd1 = open("home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/faculty.data",O_RDONLY);
	int i = courseid;
	bool res=false;
	struct course curr;
	struct faculty fac;
	
	//for course
	struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence=SEEK_SET;
        lock.l_start=(i)*sizeof(struct course);   
        lock.l_len=sizeof(struct course);    
        lock.l_pid=getpid();
        fcntl(fd,F_SETLKW,&lock); 	
        lseek(fd,(i)*sizeof(struct course),SEEK_SET);
        read(fd,&curr,sizeof(struct course));
        
        //for faculty
        int i1 = facultyid;
        struct flock lock1;
        lock1.l_type = F_RDLCK;
        lock1.l_whence=SEEK_SET;
        lock1.l_start=(i1)*sizeof(struct faculty);   
        lock1.l_len=sizeof(struct faculty);    
        lock1.l_pid=getpid();
        fcntl(fd1,F_SETLKW,&lock1); 	
        lseek(fd1,(i1)*sizeof(struct faculty),SEEK_SET);
        read(fd1,&fac,sizeof(struct faculty));
	
	char buff[1000];
	for(int k = 0; k < fac.course_count; k++)
	{
		if(fac.c[k].id==curr.id && strcmp(curr.status,"ACTIVE")==0) //curr.id=courseid
		{	res=true;
			for(int j=0;j<curr.enrolled_count;j++)
			{
				//snprintf(buff, sizeof(int),curr.enrolled[j]);
				sprintf(buff, "%s %d \n","Enrolled student ID of student is ",curr.enrolled[j]);
        			write(nsd,&buff,sizeof(int));
			}
		}
	}
	
	
	//unlock faculty
        lock1.l_type=F_UNLCK;
        fcntl(fd1,F_SETLK,&lock1);
        close(fd1);
	
	//unlock course
        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        
        return res;
}




bool verifyAdmin(struct admin curr){
	int i=curr.id;
	int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/admin.data",O_RDONLY);
	
	bool res;
	struct admin temp;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(struct admin);    	    
	lock.l_len=sizeof(struct admin);	           
	lock.l_pid=getpid();

	fcntl(fd,F_SETLKW,&lock);

	lseek(fd,(i)*sizeof(struct admin),SEEK_SET); 
	read(fd,&temp,sizeof(struct admin));
	printf("%s",temp.password);
	if(strcmp(temp.password,curr.password)==0)	
		res=true;
	else						
		res=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return res;
}


bool change_pass(int usr, int id,char newpass[10], char oldpass[10]){
	int i=id;
	if(usr==1){
		int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/faculty.data",O_RDWR);
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

		if(!strcmp(curr.status,"ACTIVE") && (strcmp(curr.password, oldpass)==0)){ //check for active user and if entered pw matches old pw
			strcpy(curr.password,newpass);
			lseek(fd,sizeof(struct faculty)*(-1),SEEK_CUR);
			write(fd,&curr,sizeof(struct faculty));
			res=true;
		}
		else	
			res=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return res;
	}
	else if(usr==2){
		int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/student.data",O_RDWR);
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

		if(!strcmp(curr.status,"ACTIVE") && strcmp(curr.password, oldpass)==0){//check for active user and if entered pw matches old pw
			strcpy(curr.password,newpass);
			lseek(fd,sizeof(struct student)*(-1),SEEK_CUR);
			write(fd,&curr,sizeof(struct student));
			res=true;
		}
		else	
			res=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return res;
	}
	/*else if(usr==3){
		int fd=open("/home/abhipsa/Desktop/ss/LinuxShellScripting/MiniProject/Data/admin.data",O_RDWR,0744);
		bool res;
		
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(struct admin);   
		lock.l_len=sizeof(struct admin);	    
		lock.l_pid=getpid();

		fcntl(fd,F_SETLKW,&lock);

		struct admin curr;
		lseek(fd,(i)*sizeof(struct admin),SEEK_SET);
		read(fd,&curr,sizeof(struct admin));

		if(strcmp(curr.password, oldpass)==0){		//check if entered pw matches old pw
			strcpy(curr.password,newpass);
			lseek(fd,sizeof(struct admin)*(-1),SEEK_CUR);
			write(fd,&curr,sizeof(struct admin));
			res=true;
		}
		else	
			res=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return res;
	}*/
	return false;
}




int main(){
	struct sockaddr_in server,client;
	int sd,nsd,clientLen;
	bool result;
	sd=socket(AF_UNIX,SOCK_STREAM,0);
	server.sin_family=AF_UNIX;    
	server.sin_addr.s_addr=INADDR_ANY; 
	server.sin_port=htons(5003);
	int bnd = bind(sd,(struct sockaddr *)&server,sizeof(server));
	if(bnd==-1)
	{
		perror("error in bind()");
		return -1;
	}
	int ls = listen(sd,5);	
	if(ls==-1)
	{
		perror("error in listen()");
		return -1;
	}     
   	printf("Course Management System \n");
	printf("Waiting for Client to connect\n");
	while(1){
		clientLen = sizeof(client);
		nsd=accept(sd,(struct sockaddr *)&client,&clientLen);
		write(1,"Connected to the client\n",sizeof("Connected to the client\n"));
		if(!fork()){
			close(sd);
			serWork(nsd);
			exit(0);
		}
		else{
			close(nsd);
		}
	}
	return 0;
}
