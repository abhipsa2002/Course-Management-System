#ifndef COURSES_H
#define COURSES_H

struct course{
        int id;
        int inst_id;
        char name[50];
        char dept[10];
        int credits;
        int maxSeats;
        int availSeats;
        char status[20];
        int *enrolled;
        int enrolled_count;
};

#endif
