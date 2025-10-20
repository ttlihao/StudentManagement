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
    // File format: CODE|NAME|CREDITS
    while (subjectCount < MAX_SUBJECTS &&
           fscanf(file, "%[^|]|%[^|]|%d\n",
                  subjects[subjectCount].code,
                  subjects[subjectCount].name,
                  &subjects[subjectCount].creditHours) == 3) {
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
    void addSubject() {
    if (subjectCount >= MAX_SUBJECTS) {
        printf("Cannot add more subjects. List is full.\n");
        return;
    }

    struct Subject newSubject;
    printf("Enter subject code: ");
    scanf("%s", newSubject.code);

    if (findSubjectIndex(newSubject.code) != -1) {
        printf("Subject code already exists!\n");
        return;
    }

    printf("Enter subject name: ");
    getchar(); // Clear buffer
    fgets(newSubject.name, sizeof(newSubject.name), stdin);
    newSubject.name[strcspn(newSubject.name, "\n")] = '\0'; // remove newline

    printf("Enter credit hours: ");
    scanf("%d", &newSubject.creditHours);

    subjects[subjectCount++] = newSubject;
    printf("Subject added successfully.\n");
}

// ------------------ READ (Display) ------------------
void displayAllSubjects() {
    if (subjectCount == 0) {
        printf("No subjects found.\n");
        return;
    }

    printf("\n%-10s | %-30s | %-6s\n", "Code", "Name", "Credits");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < subjectCount; i++) {
        printf("%-10s | %-30s | %-6d\n",
               subjects[i].code,
               subjects[i].name,
               subjects[i].creditHours);
    }
}

// ------------------ UPDATE ------------------
void updateSubject() {
    char code[20];
    printf("Enter subject code to update: ");
    scanf("%s", code);

    int index = findSubjectIndex(code);
    if (index == -1) {
        printf("Subject not found.\n");
        return;
    }

    printf("Enter new subject name: ");
    getchar();
    fgets(subjects[index].name, sizeof(subjects[index].name), stdin);
    subjects[index].name[strcspn(subjects[index].name, "\n")] = '\0';

    printf("Enter new credit hours: ");
    scanf("%d", &subjects[index].creditHours);

    printf("Subject updated successfully.\n");
}

// ------------------ DELETE ------------------
void deleteSubject() {
    char code[20];
    printf("Enter subject code to delete: ");
    scanf("%s", code);

    int index = findSubjectIndex(code);
    if (index == -1) {
        printf("Subject not found.\n");
        return;
    }

    for (int i = index; i < subjectCount - 1; i++) {
        subjects[i] = subjects[i + 1];
    }
    subjectCount--;

    printf("Subject deleted successfully.\n");
}
}