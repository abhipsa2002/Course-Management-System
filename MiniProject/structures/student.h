#ifndef STUDENT_H
#define STUDENT_H
#include "courses.h"
struct student{
	int id; //like record number, it'll be useful while record locking
        char name[50];
        int age;
        //add degree and sem;
        char email[50];
        char address[100];
        char password[10];
        char status[20];
        struct course c[4]; //A student is allowed to take 4 courses per semester
        int course_count;
};

#endif
