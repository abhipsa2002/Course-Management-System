#ifndef FACULTY_H
#define FACULTY_H
#include "courses.h"
struct faculty{
	int id;
	int course_count;
	char name[50];
	int age;
	char address[100];
	char email[50];
	struct course c[4];  // a faculty can take atmax 4 courses per semester
	char password[10];
	char status[20];
};

#endif
