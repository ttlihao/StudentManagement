#ifndef STUDENT_VIEW_H
#define STUDENT_VIEW_H

#include "../Model/Student.h"

void displayStudentDetails(const struct Student* student, int index);


void displayPaginatedList(struct Student* students, int count);
int displayEditMenu(const struct Student* s);
int displayScoresMenu(const struct Student* s);
void getNewNameFromUser(char* buffer, int size);
void getNewGenderFromUser(char* buffer, int size);
void getNewDateFromUser(int* y, int* m, int* d);
struct Major getNewMajorFromUser();
void getNewScoreFromUser(struct Student* s);
void editExistingScore(struct Student* s);
void removeScore(struct Student* s);
#endif