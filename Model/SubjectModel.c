#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Model/SubjectModel.h"

#define MAX_SUBJECTS 100
#define SUBJECT_FILE "Data/subjects.txt"

static struct Subject subjectList[MAX_SUBJECTS];
static int subjectCount = 0;

void loadSubjects() {
    FILE* file = fopen(SUBJECT_FILE, "r");
    if (!file) { /* File doesn't exist */ return; }

    subjectCount = 0;
    while (subjectCount < MAX_SUBJECTS &&
           fscanf(file, "%[^|]|%[^|]|%d|%[^\n]\n",
                  subjectList[subjectCount].code,
                  subjectList[subjectCount].name,
                  &subjectList[subjectCount].creditHours,
                  subjectList[subjectCount].majorCode) == 4) {
        subjectCount++;
    }
    fclose(file);
}

void saveSubjects() {
    FILE* file = fopen(SUBJECT_FILE, "w");
    if (!file) { perror("Could not save subjects"); return; }
    for (int i = 0; i < subjectCount; i++) {
        fprintf(file, "%s|%s|%d|%s\n", subjectList[i].code, subjectList[i].name, subjectList[i].creditHours, subjectList[i].majorCode);
    }
    fclose(file);
}

struct Subject* getAllSubjects() { return subjectList; }
int getSubjectCount() { return subjectCount; }

struct Subject* findSubjectByCode(const char* code) {
    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjectList[i].code, code) == 0) {
            return &subjectList[i];
        }
    }
    return NULL;
}