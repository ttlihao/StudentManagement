#include <stdio.h>
#include <string.h>
#include "MajorController.h"

struct Major majors[MAX_MAJORS];
int majorCount = 0;

void loadMajorsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return;

    majorCount = 0;
    while (fscanf(file, "%[^|]|%[^\n]\n", majors[majorCount].code, majors[majorCount].name) == 2) {
        majorCount++;
    }

    fclose(file);
}

void saveMajorsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    for (int i = 0; i < majorCount; i++) {
        fprintf(file, "%s|%s\n", majors[i].code, majors[i].name);
    }

    fclose(file);
}

int findMajorIndex(const char* code) {
    for (int i = 0; i < majorCount; i++) {
        if (strcmp(majors[i].code, code) == 0) return i;
    }
    return -1;
}

struct Major getOrAddMajor(const char* code, const char* name, const char* filename) {
    int index = findMajorIndex(code);
    if (index != -1) return majors[index];

    struct Major newMajor;
    strcpy(newMajor.code, code);
    strcpy(newMajor.name, name);
    majors[majorCount++] = newMajor;
    saveMajorsToFile(filename);
    return newMajor;
}