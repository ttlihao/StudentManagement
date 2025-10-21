#ifndef SUBJECT_CONTROLLER_H
#define SUBJECT_CONTROLLER_H

#include "../Model/Subject.h"

// Logic to add a new subject
int handleAddSubject(struct Subject newSubject);

// Logic to delete a subject
int handleDeleteSubject(const char* code);
int handleUpdateSubject(const char* code, struct Subject updatedInfo);
#endif