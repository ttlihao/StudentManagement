#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "StudentController.h"
#include "MajorController.h"
#include "SubjectController.h" 
#include "../Extensions/InputHelper.h"    

struct Student students[MAX_VIEWS];
struct StudentView views[MAX_VIEWS];
int viewCount = 0;
static int nextStudentID = 1; 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h> // For strcasecmp()
#include "StudentController.h"
#include "MajorController.h"
#include "SubjectController.h"
#include "../Extensions/InputHelper.h"

#define PAGE_SIZE 5

static void displayPaginatedViews(struct StudentView viewsToShow[], int count) {
    if (count == 0) {
        printf("No students to display.\n");
        return;
    }

    int totalPages = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    int currentPage = 1;
    char navInput[20];

    while (1) {
        system("cls"); // Use "clear" for Linux/macOS
        printf("\n--- Student List (Page %d of %d | Total: %d students) ---\n\n", currentPage, totalPages, count);

        int startIndex = (currentPage - 1) * PAGE_SIZE;
        int endIndex = startIndex + PAGE_SIZE;
        if (endIndex > count) {
            endIndex = count;
        }

        for (int i = startIndex; i < endIndex; i++) {
            displayStudentView(viewsToShow[i]);
            printf("------------------------------------\n");
        }

        // Updated navigation menu with the "Jump" option
        printf("\n[N]ext | [P]rev | [J]ump to Page | [Q]uit to Menu\n");
        getOptionalString("Enter navigation choice: ", navInput, sizeof(navInput));

        if (strcasecmp(navInput, "n") == 0) {
            if (currentPage < totalPages) currentPage++;
        } else if (strcasecmp(navInput, "p") == 0) {
            if (currentPage > 1) currentPage--;
        } else if (strcasecmp(navInput, "j") == 0) {
            // New logic to handle jumping to a specific page
            // We reuse our robust getInt helper for validated input!
            int pageToJump = getInt("Enter page number to jump to: ", 1, totalPages);
            currentPage = pageToJump;
        } else if (strcasecmp(navInput, "q") == 0) {
            break; // Exit the pagination loop
        }
    }
}


void updateNextStudentID() {
    if (viewCount == 0) {
        nextStudentID = 1;
        return;
    }
    int maxID = 0;
    for (int i = 0; i < viewCount; i++) {
        // Assuming ID format is "SVxxxx"
        int idNum = atoi(students[i].id + 2); // Skip "SV"
        if (idNum > maxID) {
            maxID = idNum;
        }
    }
    nextStudentID = maxID + 1;
}

// Calculates GPA based on scores and credit hours.
float calculateGPA(struct Score scores[], int scoreCount) {
    float totalPoints = 0;
    int totalCredits = 0;

    if (scoreCount == 0) return 0.0;

    for (int i = 0; i < scoreCount; i++) {

        totalPoints += (scores[i].value / 10.0 * 4.0) * scores[i].subject.creditHours;
        totalCredits += scores[i].subject.creditHours;
    }

    return (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0;
}


void addStudentView() {
    struct Student s;

    // 1. Auto-generate ID
    sprintf(s.id, "SV%04d", nextStudentID);
    printf("New Student ID: %s\n", s.id);

    // 2. Get validated inputs using InputHelper
    getString("Enter Name: ", s.name, 50);
    getGender("Enter Gender", s.gender, sizeof(s.gender));
    getDate("Enter Date of Birth", &s.birthYear, &s.birthMonth, &s.birthDay);

    // 3. Choose a major from a list
    printf("\n--- Select a Major ---\n");
    for (int i = 0; i < majorCount; i++) {
        printf("  %d. %s (%s)\n", i + 1, majors[i].name, majors[i].code);
    }
    int majorChoice = getInt("Enter your choice: ", 1, majorCount);
    s.major = majors[majorChoice - 1];

    // 4. Enter scores by choosing subjects from a list
    printf("\n--- Enter Scores ---\n");
    s.scoreCount = getInt("How many subjects to add? ", 0, 10);

    for (int i = 0; i < s.scoreCount; i++) {
        printf("\n--- Subject #%d ---\n", i + 1);
        for (int j = 0; j < subjectCount; j++) {
            printf("  %d. %s (%s)\n", j + 1, subjects[j].name, subjects[j].code);
        }
        int subjectChoice = getInt("Select subject: ", 1, subjectCount);
        s.scores[i].subject = subjects[subjectChoice - 1];

        char prompt[100];
        sprintf(prompt, "Enter score for %s (0-10): ", s.scores[i].subject.code);
        s.scores[i].value = getFloat(prompt, 0.0, 10.0);
    }

    // 5. Calculate GPA automatically (removes manual input)
    s.gpa = calculateGPA(s.scores, s.scoreCount);
    printf("Calculated GPA: %.2f\n", s.gpa);


    // Add to our lists
    students[viewCount] = s;
    views[viewCount].student = &students[viewCount];
    viewCount++;
    nextStudentID++; // Increment for the next student

    printf("\n✅ Student added successfully!\n");
}


void saveStudentViewsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < viewCount; i++) {
        struct Student* s = views[i].student;
        fprintf(file, "%s|%s|%d|%d|%d|%s|%s|%.2f|", s->id, s->name, s->birthYear, s->birthMonth, s->birthDay, s->gender, s->major.code, s->gpa); // <-- MODIFIED
        for (int j = 0; j < s->scoreCount; j++) {
            fprintf(file, "%s,%.2f", s->scores[j].subject.code, s->scores[j].value);
            if (j < s->scoreCount - 1) fprintf(file, ";");
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Student data saved to file.\n");
}


void loadStudentViewsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        // This is not an error if the file doesn't exist on the first run
        return;
    }

    char line[512];
    viewCount = 0;

    while (fgets(line, sizeof(line), file) && viewCount < MAX_VIEWS) {
        struct Student* s = &students[viewCount];
        line[strcspn(line, "\n")] = '\0'; // Remove newline

        // Use strtok for all tokenizing, which is part of the standard library
        char* token = strtok(line, "|");
        if (token) strcpy(s->id, token);

        token = strtok(NULL, "|");
        if (token) strcpy(s->name, token);

        token = strtok(NULL, "|");
        if (token) s->birthYear = atoi(token);

        token = strtok(NULL, "|");
        if (token) s->birthMonth = atoi(token);

        token = strtok(NULL, "|");
        if (token) s->birthDay = atoi(token);
        token = strtok(NULL, "|"); 
        if (token) strcpy(s->gender, token);

        // Load Major
        token = strtok(NULL, "|");
        if (token) {
            int majorIndex = findMajorIndex(token);
            if (majorIndex != -1) s->major = majors[majorIndex];
        }

        token = strtok(NULL, "|");
        if (token) s->gpa = atof(token);

        // Load Scores
        char* scoresData = strtok(NULL, "|");
        s->scoreCount = 0;
        if (scoresData) {
            // Use a different pointer for the inner strtok to avoid conflicts
            char* scoreToken = strtok(scoresData, ";");
            while (scoreToken && s->scoreCount < 10) {
                char subjectCode[10];
                float value;
                if (sscanf(scoreToken, "%[^,],%f", subjectCode, &value) == 2) {
                    int subjectIndex = findSubjectIndex(subjectCode);
                    if (subjectIndex != -1) {
                        s->scores[s->scoreCount].subject = subjects[subjectIndex];
                        s->scores[s->scoreCount].value = value;
                        s->scoreCount++;
                    }
                }
                scoreToken = strtok(NULL, ";");
            }
        }

        views[viewCount].student = s;
        viewCount++;
    }

    fclose(file);
    updateNextStudentID(); // Update the ID counter after loading
    printf("Student data loaded from file.\n");
}


void displayAllStudentViews() {
    displayPaginatedViews(views, viewCount);
}

void filterAndSortStudentViews(
    const char* idKeyword,
    const char* nameKeyword,
    const char* majorCode,
    float minGPA,
    float maxGPA,
    const char* sortByField,
    int sortOrder // New parameter
) {
    struct StudentView matched[MAX_VIEWS];
    int matchedCount = 0;

    // --- Filtering logic (remains the same) ---
    for (int i = 0; i < viewCount; i++) {
        struct Student* s = views[i].student;
        if ((idKeyword && strlen(idKeyword) > 0 && !strstr(s->id, idKeyword)) ||
            (nameKeyword && strlen(nameKeyword) > 0 && !strstr(s->name, nameKeyword)) ||
            (majorCode && strlen(majorCode) > 0 && strcmp(s->major.code, majorCode) != 0) ||
            (minGPA >= 0 && s->gpa < minGPA) ||
            (maxGPA >= 0 && s->gpa > maxGPA)) {
            continue;
        }
        matched[matchedCount++] = views[i];
    }

    // --- Sorting logic (updated for sort order) ---
    if (strlen(sortByField) > 0 && matchedCount > 1) {
        for (int i = 0; i < matchedCount - 1; i++) {
            for (int j = i + 1; j < matchedCount; j++) {
                int comparison = 0; // -1 if i<j, 0 if i==j, 1 if i>j

                if (strcmp(sortByField, "gpa") == 0) {
                    if (matched[i].student->gpa < matched[j].student->gpa) comparison = -1;
                    else if (matched[i].student->gpa > matched[j].student->gpa) comparison = 1;
                } else if (strcmp(sortByField, "name") == 0) {
                    comparison = strcmp(matched[i].student->name, matched[j].student->name);
                } else if (strcmp(sortByField, "id") == 0) {
                    comparison = strcmp(matched[i].student->id, matched[j].student->id);
                }

                // Elegant check: (1 * 1 > 0) -> swap. (-1 * -1 > 0) -> swap.
                if (comparison * sortOrder > 0) {
                    struct StudentView temp = matched[i];
                    matched[i] = matched[j];
                    matched[j] = temp;
                }
            }
        }
    }
    
    // --- Display logic (now calls pagination) ---
    printf("\nFound %d matching student(s).\n", matchedCount);
    printf("Press Enter to view results...");
    getchar();
    
    displayPaginatedViews(matched, matchedCount);
}
void deleteStudentView() {
    if (viewCount == 0) {
        printf(" No students available to delete.\n");
        return;
    }

    char idToDelete[20];
    getString("Enter Student ID to delete (e.g., SV0001): ", idToDelete, sizeof(idToDelete));

    int foundIndex = -1;
    for (int i = 0; i < viewCount; i++) {
        if (strcmp(students[i].id, idToDelete) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Error: Student ID not found.\n");
        return;
    }

    printf("\nStudent found:\n");
    printf("ID: %s | Name: %s | GPA: %.2f | Major: %s\n",
           students[foundIndex].id,
           students[foundIndex].name,
           students[foundIndex].gpa,
           students[foundIndex].major.name);

    char confirm[10];
    getOptionalString("Are you sure you want to delete this student? (Y/N): ", confirm, sizeof(confirm));

    if (strcasecmp(confirm, "y") != 0 && strcasecmp(confirm, "yes") != 0) {
        printf(" Delete operation cancelled by user.\n");
        return;
    }

    for (int i = foundIndex; i < viewCount - 1; i++) {
        students[i] = students[i + 1];
        views[i] = views[i + 1];
    }

    viewCount--;

    saveStudentViewsToFile("students.txt");

    printf("Student deleted successfully and file updated.\n");
}
void editStudentInfo()
{
    // Step 1: Check if there are any students in the list
    if (viewCount == 0) {
        printf("There are no students to edit.\n");
        return;
    }

    // Step 2: Prompt the user to enter the ID of the student to edit
    char idToEdit[20];
    getString("Enter Student ID to edit (e.g., SV0001): ", idToEdit, sizeof(idToEdit));

    // Step 3: Find the index of the student in the `students` array
    int foundIndex = -1;
    for (int i = 0; i < viewCount; i++) {
        // Compare the input ID with each student's ID
        if (strcmp(students[i].id, idToEdit) == 0) {
            foundIndex = i;
            break; // Found, break the loop
        }
    }

    // Step 4: Handle the case where the student is not found
    if (foundIndex == -1) {
        printf("Error: Student with ID '%s' not found.\n", idToEdit);
        return;
    }

    // Create a pointer `s` for easier access to the student's information
    struct Student* s = &students[foundIndex];
    int choice;
    int changesMade = 0; // Flag to check if any changes were made

    // Step 5: Display the edit menu in a `while` loop
    while (1) {
        system("cls"); // Clear the screen for a clean menu display
        printf("\n--- Editing Student Information: %s ---\n", s->name);
        printf("ID: %s (Cannot be changed)\n", s->id);
        printf("---------------------------------------------\n");
        // List all editable information from Student.h
        printf("1. Full Name      : %s\n", s->name);
        printf("2. Gender         : %s\n", s->gender);
        printf("3. Date of Birth  : %d-%02d-%02d\n", s->birthYear, s->birthMonth, s->birthDay);
        printf("4. Major          : %s (%s)\n", s->major.name, s->major.code);
        printf("5. Scores         : (%d subjects)\n", s->scoreCount);
        printf("---------------------------------------------\n");
        printf("0. Save Changes and Exit\n");
        printf("---------------------------------------------\n");

        choice = getInt("Select the information you want to edit: ", 0, 5);

        // If the user selects 0, break the loop
        if (choice == 0) {
            break;
        }

        changesMade = 1; // Mark that a change has been made

        // Use `switch` to handle the user's choice
        switch (choice) {
            case 1: // Edit Full Name
                printf("Current name: %s\n", s->name);
                getString("Enter new name: ", s->name, sizeof(s->name));
                printf("=> Name updated successfully!\n");
                break;
            
            case 2: // Edit Gender
                printf("Current gender: %s\n", s->gender);
                getGender("Enter new gender", s->gender, sizeof(s->gender));
                printf("=> Gender updated successfully!\n");
                break;

            case 3: // Edit Date of Birth
                printf("Current date of birth: %d-%02d-%02d\n", s->birthYear, s->birthMonth, s->birthDay);
                getDate("Enter new date of birth", &s->birthYear, &s->birthMonth, &s->birthDay);
                printf("=> Date of birth updated successfully!\n");
                break;

            case 4: // Edit Major
                printf("Current major: %s\n", s->major.name);
                printf("\n--- Select a new major ---\n");
                // Display the list of available majors
                for (int i = 0; i < majorCount; i++) {
                    printf("  %d. %s (%s)\n", i + 1, majors[i].name, majors[i].code);
                }
                int majorChoice = getInt("Enter your choice: ", 1, majorCount);
                s->major = majors[majorChoice - 1]; // Update the major
                printf("=> Major updated successfully!\n");
                break;

            case 5: // Edit Scores
                printf("\n--- Re-enter all scores ---\n");
                // Request re-entry of all scores to ensure consistency
                s->scoreCount = getInt("How many subjects do you want to enter scores for? ", 0, 10);

                for (int i = 0; i < s->scoreCount; i++) {
                    printf("\n--- Subject #%d ---\n", i + 1);
                    // Display the list of subjects
                    for (int j = 0; j < subjectCount; j++) {
                        printf("  %d. %s (%s)\n", j + 1, subjects[j].name, subjects[j].code);
                    }
                    int subjectChoice = getInt("Select a subject: ", 1, subjectCount);
                    s->scores[i].subject = subjects[subjectChoice - 1];

                    // Enter the score for the selected subject
                    char prompt[100];
                    sprintf(prompt, "Enter score for %s (0-10): ", s->scores[i].subject.code);
                    s->scores[i].value = getFloat(prompt, 0.0, 10.0);
                }
                printf("=> Scores updated successfully!\n");
                break;
        }
        printf("\nPress Enter to continue editing...");
        getchar(); // Pause for the user to read the message
    }

    // Step 6: Process after exiting the edit menu
    if (changesMade) {
        // Recalculate GPA as scores might have changed
        s->gpa = calculateGPA(s->scores, s->scoreCount);

        // Save the entire student list (including the edited student) to the file
        saveStudentViewsToFile("students.txt");

        printf("\n=> Student information has been updated and saved to the file.\n");
        printf("   The newly recalculated GPA is: %.2f\n", s->gpa);
    } else {
        printf("\n=> No changes were made.\n");
    }
}