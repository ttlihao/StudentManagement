#include <stdio.h>
#include <string.h>
#include "../View/SubjectView.h"
#include "../Extensions/InputHelper.h"
#include "../Model/SubjectModel.h" // Needed for findSubjectByCode


int displaySubjectMenu() {
    system("cls");
    printf("\n--- Manage Subjects ---\n");
    printf("1. Add New Subject\n");
    printf("2. View All Subjects\n");
    printf("3. Update Subject Info\n");
    printf("4. Delete Subject\n");
    printf("0. Back to Main Menu\n");
    return getInt("Enter your choice: ", 0, 4);
}

void displaySubjectList(const struct Subject subjects[], int count) {
    if (count == 0) {
        printf("\n>> No subjects found. <<\n");
        return;
    }
    printf("\n%-10s | %-30s | %-8s | %s\n", "Code", "Name", "Credits", "Major");
    printf("-----------+--------------------------------+----------+------\n");
    for (int i = 0; i < count; i++) {
        printf("%-10s | %-30s | %-8d | %s\n",
               subjects[i].code,
               subjects[i].name,
               subjects[i].creditHours,
               subjects[i].majorCode);
    }
}

int getNewSubjectInfo(struct Subject* s) {
    getString("Enter Subject Code: ", s->code, sizeof(s->code));
    // Validate for duplicate code right away
    if (findSubjectByCode(s->code) != NULL) {
        return 0; // Failure: code already exists
    }
    getString("Enter Subject Name: ", s->name, sizeof(s->name));
    s->creditHours = getInt("Enter Credit Hours: ", 1, 10);
    getString("Enter associated Major Code (e.g., CS, BA, or GEN): ", s->majorCode, sizeof(s->majorCode));
    return 1; // Success
}

void getUpdatedSubjectInfo(const struct Subject* original, struct Subject* updated) {
    printf("Updating Subject Code: %s\n", original->code);
    printf("Current Name: %s\n", original->name);
    getString("Enter new Subject Name: ", updated->name, sizeof(updated->name));

    printf("Current Credit Hours: %d\n", original->creditHours);
    updated->creditHours = getInt("Enter new Credit Hours: ", 1, 10);
    
    printf("Current Major Code: %s\n", original->majorCode);
    getString("Enter new Major Code: ", updated->majorCode, sizeof(updated->majorCode));
    
    // The code itself doesn't change, so copy it over
    strcpy(updated->code, original->code);
}