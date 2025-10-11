#ifndef MAJOR_CONTROLLER_H
#define MAJOR_CONTROLLER_H

#include "../Model/Major.h"

#define MAX_MAJORS 50

extern struct Major majors[MAX_MAJORS];
extern int majorCount;

void loadMajorsFromFile(const char* filename);
void saveMajorsToFile(const char* filename);
int findMajorIndex(const char* code);
struct Major getOrAddMajor(const char* code, const char* name, const char* filename);

#endif