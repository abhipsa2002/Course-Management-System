#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
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


void FacultyLogin(int);
void StudentLogin(int);
void AdminLogin(int);
void change_password(int);
void add_id(int);
void delete_id(int);
void deactivate_student(int);
void activate_student(int);
void view_details(int);
void add_course(int);
void delete_course(int);
void viewEnrolledCourses(int);
void viewEnrolledStudents(int);
void viewAllCourses(int);
void enroll_course(int);
void unenroll_course(int);
void view_course(int);

int opt,currid;
struct faculty fac;
void selectopt(int sd){
	printf("1 : FACULTY  \n");
	printf("2 : STUDENT  \n");
	printf("3 : ADMIN  \n");
	printf("Choose an option: ");
	scanf("%d",&opt);
	//printf("Option selected: %d\n",opt);

	switch(opt){
		case 1 :
			FacultyLogin(sd);
			break;
		case 2 :
			StudentLogin(sd);
			break;
		case 3 :
			AdminLogin(sd);
			break;
		default :
			printf("Invalid!!! Choose a valid option \n");
			selectopt(sd);
			break;
	}
}

void disp_menu(int sd){
	int d;
	if(opt==1){
		printf("1 : Add a new course \n");
		printf("2 : Remove an existing course \n");
		printf("3 : View enrolled students in course \n");
		printf("4 : Password Change\n");
		printf("5 : Exit \n");
		printf("Choose the task : ");
		scanf("%d",&d);
		//printf("Option : %d\n",d);

		switch(d){
		case 1 :
			add_course(sd); //done ig 
			break;
		case 2 :
			delete_course(sd); //to be done later
			break;
		case 3 :
			viewEnrolledStudents(sd);  //done
			break;
		case 4 :
			change_password(sd); //done
			break;
		case 5 :
			write(sd,&d,sizeof(int));
			printf("Adios! \n");
			exit(0);
		default :
			printf("Invalid!!! Choose a valid option \n");
			disp_menu(sd);
			break;
		}
	}
	if(opt==2){
		printf("1 : View all courses \n"); //done ig
		printf("2 : Enroll to a new course \n"); //done ig
		printf("3 : Unenroll from an existing course \n");  // done ig
		printf("4 : View enrolled courses \n"); //done ig
		printf("5 : Password Change\n"); //done
		printf("6 : Exit \n"); //done 
		printf("Choose the task : ");
		scanf("%d",&d);
		//printf("Option : %d\n",d);

		switch(d){
		case 1:
			viewAllCourses(sd); 
			break;
		case 2 :
			enroll_course(sd);
			break;
		case 3 :
			unenroll_course(sd);
			break;
		case 4 :
			viewEnrolledCourses(sd);
			break;
		case 5 :
			change_password(sd); //done c
			break;
		case 6 :
			write(sd,&d,sizeof(int));
			printf("Adios! \n");
			exit(0);
		default :
			printf("Invalid!!! Choose a valid option \n");
			disp_menu(sd);
			break;
		}
	}
	else if(opt==3){
		printf("1 : Add ID \n"); //done c
		printf("2 : Delete ID \n");	//done c
		printf("3 : Activate Student \n");  //done c
		printf("4 : Deactivate Student \n");  //done c  
		printf("5 : View Details \n"); //done c
		printf("6 : Exit \n");	//done c
		printf("Choose the task: ");
		scanf("%d",&d);
		//printf("Option : %d\n",d);

		switch(d){
		case 1 :
			add_id(sd);
			break;
		case 2 :
			delete_id(sd);
			break;
		case 3 :
			activate_student(sd);
			break;
		case 4 :
			deactivate_student(sd);
			break;
		case 5:
			view_details(sd);
			break;	
		case 6:
			write(sd,&d,sizeof(int));
			printf("Adios !\n");
			exit(0);
		default:
			printf("Invalid!!! Choose a valid option \n");
			disp_menu(sd);
			break;
		}
	}
}


//add course
void add_course(int sd)
{
		int select=1;
		write(sd,&select,sizeof(select));
		bool res;
		struct course c;
		char buff[1000];
		printf("Name of the course : ");
		scanf(" %[^\n]",c.name);
		printf("Enter dept of course: ");
        	scanf(" %[^\n]",c.dept);
		printf("Enter course credits: ");
        	scanf(" %d",&c.credits);
        	printf("Enter max seats in the course: ");
        	scanf("%d",&c.maxSeats); //change availSeats in func file
		write(sd,&c, sizeof(struct course));
		read(sd,buff,sizeof(buff));
		read(sd,&res,sizeof(res));
		if(!res){
		printf("Error adding the course!\n\n");
		}
		else{
		printf("\n Succesfully added the course!");
		printf("%s \n",buff);
		}
		disp_menu(sd);
		return;
}

//delete course
void delete_course(int sd)
{
	bool res;
	int select=2;
		write(sd,&select,sizeof(select));
	struct course c;
	int id;
	printf("\n Enter id of the course you want to delete \n");
	scanf("%d",&id);
	write(sd,&c, sizeof(struct course));
	read(sd,&res,sizeof(res));
	if(!res){
	printf("Error deleting the course!\n\n");
	}
	else{
	printf("Succesfully deleted the course! \n");
	}
	disp_menu(sd);
	return;


}


//FACULTY LOGIN
void FacultyLogin(int sd){
	bool res;
	struct faculty curr;
	printf("User ID : ");
	scanf("%d",&curr.id);
	currid=curr.id;
	printf("Password : ");
	char* pass = getpass("");
	strcpy(curr.password, pass);
	printf("%s",pass);
	getchar(); 
	write(sd,&opt,sizeof(int)); //pass to server
	write(sd,&curr,sizeof(struct faculty)); //pass to server

	read(sd,&res,sizeof(res)); //wait for server to do the password matching and save the return value in 'res' variable

	if(!res){			
		printf("Invalid password or userid \n");
		printf("TRY AGAIN !!! \n");
		selectopt(sd);
	}
	else{
		printf("Logged in!!!\n");
		fac=curr;
	}
}

//STUDENT LOGIN

void StudentLogin(int sd){
	bool res;
	struct student curr;
	printf("User ID : ");
	scanf("%d",&curr.id);
	currid=curr.id;
	printf("\n Password : ");
	char* pass = getpass("");
	strcpy(curr.password, pass);
	getchar(); //wait till user presses enter
	write(sd,&opt,sizeof(int));  //pass to server
	write(sd,&curr,sizeof(struct student)); //pass to server
	read(sd,&res,sizeof(res)); //wait for server to do the password matching and save the return value in 'res' variable
	
	if(!res){
		printf("Invalid password or userid \n");
		printf("TRY AGAIN !!! \n");
		selectopt(sd);
	}
	else{
		printf("Logged in!!!\n");
		
	}
}

//ADMIN LOGIN

void AdminLogin(int sd){
	bool res;
	struct admin curr;
	printf("User ID : ");
	scanf("%d",&curr.id);
	currid=curr.id;
	printf("Password : ");
	char* pass = getpass("");
	strcpy(curr.password, pass);
	getchar(); //wait till user presses enter
	write(sd,&opt,sizeof(int));  //pass to server
	write(sd,&curr,sizeof(struct admin)); //pass to server

	read(sd,&res,sizeof(res)); //wait for server to do the password matching and save the return value in 'res' variable

	if(!res){
		printf("Invalid password or userid \n");
		printf("TRY AGAIN !!! \n");
		selectopt(sd);
	}
	else{
		printf("Logged in!!!\n");
	}
}

void change_password(int sd){
	int select;				/*check at the end*/
	if(opt==1) select=4;
	else if(opt==2) select=5;

	char old_pass[10];
	char new_pass[10];
	bool res;

	printf("Enter your current password : ");
	char* pass = getpass("");
        strcpy(old_pass, pass);

	//to the server
	write(sd,&select,sizeof(int));
	write(sd,old_pass,sizeof(old_pass));
	printf("Enter the new password(max 10 characters) : ");
	pass = getpass("");
        strcpy(new_pass,pass);
	write(sd,new_pass,sizeof(new_pass));
	read(sd,&res,sizeof(res));
	if(!res){
		printf("Error changing your password! \n");
	}
	else{
		printf("Succesfully changed your password! \n");
	}
	disp_menu(sd);
	return;
}


void add_id(int sd){     //add password
	int select=1;
	int usr;
	bool res;
	char buff[1000]; //for id and password
	bzero(buff, sizeof(buff));
	//bzero(buff2, sizeof(buff2));
	write(sd,&select,sizeof(int));

	printf("Enter the id type you want to add \n 1: Faculty \n 2: Student \n");
	scanf("%d",&usr);

	write(sd,&usr,sizeof(int));

	if(usr==1){
		struct faculty fac;
		printf("Name of the faculty : ");
		scanf(" %[^\n]",fac.name);
		/*printf("Password(max 10 characters) : ");  //password creation at faculty_func.h
		char* pass = getpass("");
        	strcpy(fac.password, pass);*/ 
		printf("Age of the faculty : ");
		scanf("%d",&fac.age);
		printf("Email of the faculty : ");
		scanf(" %[^\n]",fac.email);
		
		printf("Address of the faculty : ");
		scanf(" %[^\n]",fac.address);
		write(sd,&fac, sizeof(struct faculty));
		read(sd, buff, sizeof(buff));		
	}

	if(usr==2){
		struct student s;
		printf("Name of the student : ");
		scanf(" %[^\n]",s.name);
		//strcat(s.name,'\0');
		printf("Age of the student : ");
		scanf("%d",&s.age);
		printf("Email of the student : ");
		scanf(" %[^\n]",s.email);
		printf("Address of the student : ");
		scanf(" %[^\n]",s.address);
		write(sd,&s, sizeof(struct student));
		read(sd, buff, sizeof(buff));
		//printf("%s \n", buff);
		//read(sd,buff2,sizeof(buff2));
	}

	read(sd,&res,sizeof(res));

	if(!res){
		printf("Error adding the ID \n");
	}
	else{
		printf("Succesfully added the ID \n");
		printf("%s \n", buff);
	}
	//printf("%s \n",buff1);
	//printf("%s \n",buff2);
	disp_menu(sd);
	return;
}

void delete_id(int sd){
	int select=2;
	int usr,id;
	bool res;

	write(sd,&select,sizeof(int));

	/*printf("Enter the id type you want to delete \n 1: Faculty \n 2: Student \n");
	scanf("%d",&usr);*/
	

	write(sd,&usr,sizeof(int));

	printf("Enter student ID you want to delete : ");
	scanf("%d",&id);
	write(sd,&id,sizeof(int));

	read(sd,&res,sizeof(res));
	
	if(!res){
		printf("Error deleting the account \n Check your id again \n");
	}
	else{
		printf("Succesfully deleted the account \n");
	}
	disp_menu(sd);
	return;
}

void activate_student(int sd)
{
	int select=3;
	int usr,id;
	bool res;
	write(sd,&select,sizeof(int));
	printf("Enter ID you want to activate : ");
	scanf("%d",&id);
	write(sd,&id,sizeof(int));

	read(sd,&res,sizeof(res));
	if(!res){
		printf("Error in activating the account \n Check your id again \n");
	}
	else{
		printf("Succesfully activated the account \n");
	}
	disp_menu(sd);
	return;
}

void deactivate_student(int sd)
{
	int select=4;
	int usr,id;
	bool res;
	write(sd,&select,sizeof(int));
	printf("Enter ID you want to deactivate : ");
	scanf("%d",&id);
	write(sd,&id,sizeof(int));

	read(sd,&res,sizeof(res));
	
	if(!res){
		printf("Error in deactivatingthe account \n Check your id again \n");
	}
	else{
		printf("Succesfully deactivated the account \n");
	}
	disp_menu(sd);
	return;
}

void view_details(int sd){
	int select=5;
	int usr,i;
	bool result;

	write(sd,&select,sizeof(int));

	printf("Enter the id type you want to view \n 1: Faculty \n 2: Student \n");
	scanf("%d",&usr);
	write(sd,&usr,sizeof(int));

	if(usr==1){
		struct faculty fac;
		int id;
		printf("Enter User ID of faculty to view details : ");
		scanf("%d",&id);
		write(sd,&id,sizeof(int));
		i=read(sd,&fac,sizeof(struct faculty));
		if(i==0){
			printf("Entered id doesn't exist \n");
		}
		else{
			
			printf("User ID : %d\n",fac.id);
			printf("Name : %s\n",fac.name);
			printf("Age : %d\n",fac.age);
			printf("Email : %s\n",fac.email);
			printf("Address : %s\n",fac.address);
			printf("Status : %s\n",fac.status);
			printf("List of courses taught by the faculty: \n");
			for(int i=0;i<fac.course_count;i++)
			{
				printf("Course id: %d",fac.c[i].id);
				printf("Course name : %s",fac.c[i].name);
				
			}
			
		}
	}

	if(usr==2){
		struct student s;
		int id;
		printf("Enter User ID of student to view details : ");
		scanf("%d",&id);
		write(sd,&id,sizeof(int));
		i=read(sd,&s,sizeof(struct student));
		if(i==0){
			printf("Entered id doesn't exist \n");
		}
		else{
			
			printf("User ID : %d\n",s.id);
			printf("Name : %s\n",s.name);
			printf("Age : %d\n",s.age);
			printf("Email : %s\n",s.email);
			printf("Address : %s\n",s.address);
			printf("Status : %s\n",s.status);
			printf("List of courses taken by the student: \n");
			for(int i=0;i<s.course_count;i++)
			{
				printf("Course id: %d",s.c[i].id);
				printf("Course name : %s",s.c[i].name);
				
			}
			
		}
	}
	disp_menu(sd);
	return;
}

//enroll course

void enroll_course(int sd)
{
	bool res;
	int id;
	int select=2;
	write(sd,&select,sizeof(int));
	printf("Enter courseID you want to enroll : ");
	scanf("%d",&id);
	write(sd,&id,sizeof(int));
	read(sd,&res,sizeof(res));
	if(!res){
		printf("Error in enrolling the course \n Check your id again \n");
	}
	else{
		printf("Succesfully enrolled in the course \n");
	}
	disp_menu(sd);
	return;
}

void unenroll_course(int sd)
{
	bool res;
	int id;
	int select=3;
	write(sd,&select,sizeof(int));
	printf("Enter courseID you want to unenroll : ");
	scanf("%d",&id);
	write(sd,&id,sizeof(int));
	read(sd,&res,sizeof(res));
	if(!res){
		printf("Error in unenrolling the course \n Check your course id again \n");
	}
	else{
		printf("Succesfully unenrolled in the course \n");
	}
	disp_menu(sd);
	return;
}


void viewEnrolledCourses(int sd)
{
	int select = 4;
	write(sd,&select,sizeof(int));
	char buff[1000];
	printf("The list of enrolled courses id: \n ");
	while(1)
	{	//int i=1;
		int j = read(sd,&buff,sizeof(buff));
		if(j==0) break; //no bytes read from buffer i.e. EOF
		printf("%s",buff);
	}
	disp_menu(sd);
	return;
}

void viewEnrolledStudents(int sd)
{
	int select = 3;
	write(sd,&select,sizeof(int));
	printf("Enter the course id for which you want to view students :");
	int id;
	scanf("%d", &id);
	char buff[1000];
	bool res;
	while(1)
	{	
		int j  = read(sd,buff,sizeof(buff));
		if(j==0) break;
		printf("%s",buff);
	}
	read(sd,&res,sizeof(res));
	if(!res)
		printf("Either the course is deactivated or you don't have access to the course \n Check course id again \n");
	disp_menu(sd);
	return;
}

void viewAllCourses(int sd)  //display avail seats also
{
	int select = 1;
	write(sd,&select,sizeof(int));
	printf("LIST OF AVAILABLE COURSES \n COURSE ID \t COURSE NAME \t AVAILABLE SEATS \n");
	char buff[1000];
	while(1)
	{
		int j = read(sd,buff,sizeof(buff));
		if(j==0 || j==-1)
			break;
		printf("%s",buff);
	
	}
	disp_menu(sd);
	return;

}

int main(){
	printf("==========WELCOME TO COURSE MANAGEMENT SYSTEM============== \n ");
	struct sockaddr_in server;    
	int sd;
	char buff[1000];
	char result;

	sd=socket(AF_UNIX,SOCK_STREAM,0);
	server.sin_family=AF_UNIX;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(5003);

	connect(sd,(struct sockaddr *)&server,sizeof(server));
	selectopt(sd);
	disp_menu(sd);
	close(sd);
	return 0;
} 


//deactivate faculty is not handled in that case we've to delete the subsequent courses and unenroll the corresponding students (later)
