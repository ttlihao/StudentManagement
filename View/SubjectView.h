#ifndef SUBJECT_VIEW_H
#define SUBJECT_VIEW_H

#include "../Model/Subject.h"

// Displays the sub-menu for subject management
int displaySubjectMenu();

// Displays a formatted list of all subjects
void displaySubjectList(const struct Subject subjects[], int count);

// Gets input from the user for a new subject
// Returns 1 if valid, 0 if user enters a duplicate code
int getNewSubjectInfo(struct Subject* s);


void getUpdatedSubjectInfo(const struct Subject* original, struct Subject* updated);
#endif