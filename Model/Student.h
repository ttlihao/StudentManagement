#ifndef STUDENT_H
#define STUDENT_H

#include "Major.h"
#include "Score.h"

struct Student {
    char id[15];
    char name[50];
    int birthYear;
    int birthDay;
    int birthMonth;
    struct Major major;
    float gpa;
    struct Score scores[10];
    int scoreCount;
};

#endif