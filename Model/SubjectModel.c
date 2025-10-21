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

int updateSubject(const char* code, struct Subject updatedInfo) {
    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjectList[i].code, code) == 0) {
            // Found the subject, now update its fields
            strcpy(subjectList[i].name, updatedInfo.name);
            subjectList[i].creditHours = updatedInfo.creditHours;
            strcpy(subjectList[i].majorCode, updatedInfo.majorCode);
            return 1; // Success
        }
    }
    return 0; // Not found
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

// Add these new functions to Sources/SubjectModel.c

int addSubject(struct Subject newSubject) {
    if (subjectCount >= MAX_SUBJECTS) {
        return 0; // List is full
    }
    subjectList[subjectCount++] = newSubject;
    return 1; // Success
}

int deleteSubject(const char* code) {
    int index = -1;
    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjectList[i].code, code) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return 0; // Not found
    }

    // Shift elements to fill the gap
    for (int i = index; i < subjectCount - 1; i++) {
        subjectList[i] = subjectList[i + 1];
    }
    subjectCount--;
    return 1; // Success
}