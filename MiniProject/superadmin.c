#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>

#include "./structures/admin.h"

int getid(){
	int fd=open("./Data/admin.data",O_RDONLY);
	if (fd == -1 && errno == ENOENT)  //check if 1st admin
    	{
        	return 0;
    	}
	struct admin prev;
	lseek(fd,(-1)*sizeof(struct admin),SEEK_END);
	read(fd,&prev,sizeof(struct admin));
	close(fd);
	return prev.id+1;
}

int main(){
	struct admin nadmin;
	nadmin.id=getid();
	int fd=open("./Data/admin.data",O_RDWR | O_CREAT | O_APPEND,0744);
	printf("Enter the password(max 10 characters):");
	scanf("%[^\n]",nadmin.password);
	printf("Your id is : %d\n",nadmin.id);
	write(fd,&nadmin,sizeof(struct admin));
	int choice;
	printf("Do you want to add more admins(Enter 1 for yes and 0 for no): \n");
	scanf("%d",&choice);
	while(choice==1){
		nadmin.id=getid();
		printf("Enter the password(max 10 characters): ");
		scanf(" %[^\n]",nadmin.password);
		printf("Your id is : %d\n",nadmin.id);
		write(fd,&nadmin,sizeof(struct admin));
		printf("Do you want to add more admins(Enter 1 for yes and 0 for no): ");
		scanf("%d",&choice);
	}
	close(fd);
	return 0;
}
