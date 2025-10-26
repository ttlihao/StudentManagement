#include <stdio.h>
#include <stdlib.h>
/*ADD THIS CODE*/
 #include <string.h> // Needed for strcmp
 /*END NEW CODE*/
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
/*ADD THIS CODE*/
 /**
  * @brief Implementation to find a major by code in the static list.
  */
 struct Major* findMajorByCode(const char* code) {
     for (int i = 0; i < majorCount; i++) {
         // Compare the input code with the code of each major in the list
         if (strcmp(majorList[i].code, code) == 0) {
             return &majorList[i]; // Return pointer to the found major
         }
     }
     return NULL; // Return NULL if no match is found
 }

 /**
  * @brief Implementation to add a major to the static list.
  * Assumes duplicate code check is done by the controller.
  */
 int addMajor(struct Major newMajor) {
     // Check if the major list is already full
     if (majorCount >= MAX_MAJORS) {
         return 0; // Indicate failure: list is full
     }
     // Add the new major to the next available slot and increment the count
     majorList[majorCount++] = newMajor;
     return 1; // Indicate success
 }

 /**
  * @brief Implementation to delete a major from the static list.
  * Shifts subsequent elements to fill the gap.
  */
 int deleteMajor(const char* code) {
     int index = -1;
     // Find the index of the major to be deleted
     for (int i = 0; i < majorCount; i++) {
         if (strcmp(majorList[i].code, code) == 0) {
             index = i;
             break; // Stop searching once found
         }
     }

     // If the major code was not found in the list
     if (index == -1) {
         return 0; // Indicate failure: not found
     }

     // Shift all elements after the found index one position to the left
     for (int i = index; i < majorCount - 1; i++) {
         majorList[i] = majorList[i + 1];
     }
     majorCount--; // Decrement the total count of majors
     return 1; // Indicate success
 }
 /*END NEW CODE*/