#include <stdio.h>
#include <string.h>
#include "SubjectController.h"

struct Subject subjects[MAX_SUBJECTS];
int subjectCount = 0;

void loadSubjectsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Warning: Could not open %s. No subjects loaded.\n", filename);
        return;
    }

    subjectCount = 0;
    while (subjectCount < MAX_SUBJECTS &&
           fscanf(file, "%[^|]|%[^|]|%d|%[^\n]\n",
                  subjects[subjectCount].code,
                  subjects[subjectCount].name,
                  &subjects[subjectCount].creditHours,
                  subjects[subjectCount].majorCode) == 4) { 
        subjectCount++;
    }

    fclose(file);
}

int findSubjectIndex(const char* code) {
    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjects[i].code, code) == 0) {
            return i;
        }
    }
    return -1;
}