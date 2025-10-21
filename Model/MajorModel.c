#include <stdio.h>
#include <stdlib.h>
#include "../Model/MajorModel.h"

#define MAX_MAJORS 50
#define MAJOR_FILE "Data/majors.txt"

static struct Major majorList[MAX_MAJORS];
static int majorCount = 0;

void loadMajors() {
    FILE* file = fopen(MAJOR_FILE, "r");
    if (!file) { /* File doesn't exist, will be created on save */ return; }

    majorCount = 0;
    while (majorCount < MAX_MAJORS &&
           fscanf(file, "%[^|]|%[^\n]\n", majorList[majorCount].code, majorList[majorCount].name) == 2) {
        majorCount++;
    }
    fclose(file);
}

void saveMajors() {
    FILE* file = fopen(MAJOR_FILE, "w");
    if (!file) { perror("Could not save majors"); return; }
    for (int i = 0; i < majorCount; i++) {
        fprintf(file, "%s|%s\n", majorList[i].code, majorList[i].name);
    }
    fclose(file);
}

struct Major* getAllMajors() { return majorList; }
int getMajorCount() { return majorCount; }