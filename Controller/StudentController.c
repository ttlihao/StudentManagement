#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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
#include <strings.h> 
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
        system("cls");
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

        printf("\n[N]ext | [P]rev | [J]ump to Page | [Q]uit to Menu\n");
        getOptionalString("Enter navigation choice: ", navInput, sizeof(navInput));

        if (strcasecmp(navInput, "n") == 0) {
            if (currentPage < totalPages) currentPage++;
        } else if (strcasecmp(navInput, "p") == 0) {
            if (currentPage > 1) currentPage--;
        } else if (strcasecmp(navInput, "j") == 0) {

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
        // ID format is "SVxxxx"
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
    sprintf(s.id, "SV%04d", nextStudentID);
    printf("New Student ID: %s\n", s.id);
    getString("Enter Name: ", s.name, 50);
    getDate("Enter Date of Birth", &s.birthYear, &s.birthMonth, &s.birthDay);
    getGender("Enter Gender", s.gender, sizeof(s.gender));

    printf("\n--- Select a Major ---\n");
    for (int i = 0; i < majorCount; i++) {
        printf("  %d. %s (%s)\n", i + 1, majors[i].name, majors[i].code);
    }
    int majorChoice = getInt("Enter your choice: ", 1, majorCount);
    s.major = majors[majorChoice - 1];

    printf("\n--- Enter Scores ---\n");
    s.scoreCount = getInt("How many subjects to add? ", 0, 10);

    struct Subject availableSubjects[MAX_SUBJECTS];
    int availableCount = 0;

    for (int i = 0; i < subjectCount; i++) {
        if (strcmp(subjects[i].majorCode, s.major.code) == 0 ||
            strcmp(subjects[i].majorCode, "GEN") == 0) {
            availableSubjects[availableCount++] = subjects[i];
        }
    }

    for (int i = 0; i < s.scoreCount; i++) {
        printf("\n--- Subject #%d ---\n", i + 1);

        if (availableCount == 0) {
            printf("There are no subjects available for this major.\n");
            break;
        }

        // Display ONLY the available subjects.
        for (int j = 0; j < availableCount; j++) {
            printf("  %d. %s (%s)\n", j + 1, availableSubjects[j].name, availableSubjects[j].code);
        }
        
        int subjectChoice = getInt("Select subject: ", 1, availableCount);
        s.scores[i].subject = availableSubjects[subjectChoice - 1];

        char prompt[100];
        sprintf(prompt, "Enter score for %s (0-10): ", s.scores[i].subject.code);
        s.scores[i].value = getFloat(prompt, 0.0, 10.0);
    }
    s.gpa = calculateGPA(s.scores, s.scoreCount);
    printf("Calculated GPA: %.2f\n", s.gpa);

    students[viewCount] = s;
    views[viewCount].student = &students[viewCount];
    viewCount++;
    nextStudentID++;

    printf("\nStudent added successfully!\n");
}

void saveStudentViewsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < viewCount; i++) {
        struct Student* s = &students[i];


        fprintf(file, "%s|%s|%d|%d|%d|%s|%s|%.2f|",
                s->id, s->name, s->birthYear, s->birthMonth, s->birthDay,
                s->gender, s->major.code, s->gpa);

        for (int j = 0; j < s->scoreCount; j++) {

            fprintf(file, "%s,%.2f", s->scores[j].subject.code, s->scores[j].value);

            if (j < s->scoreCount - 1) {
                fprintf(file, ";");
            }
        }

        fprintf(file, "\n");
    }

    fclose(file);
}


void loadStudentViewsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return; 
    }

    char line[512];
    viewCount = 0;

    while (fgets(line, sizeof(line), file) && viewCount < MAX_VIEWS) {
        struct Student* s = &students[viewCount];
        line[strcspn(line, "\n")] = '\0';

        // We need a "save pointer" for strtok_s to store its context.
        char* context1 = NULL;

        // Use strtok_s for all outer tokenizing based on '|'
        char* token = strtok_s(line, "|", &context1);
        if (token) strcpy(s->id, token);

        token = strtok_s(NULL, "|", &context1);
        if (token) strcpy(s->name, token);

        token = strtok_s(NULL, "|", &context1);
        if (token) s->birthYear = atoi(token);

        token = strtok_s(NULL, "|", &context1);
        if (token) s->birthMonth = atoi(token);

        token = strtok_s(NULL, "|", &context1);
        if (token) s->birthDay = atoi(token);

        token = strtok_s(NULL, "|", &context1);
        if (token) strcpy(s->gender, token);


        token = strtok_s(NULL, "|", &context1);
        if (token) {
            int majorIndex = findMajorIndex(token);
            if (majorIndex != -1) s->major = majors[majorIndex];
        }

        token = strtok_s(NULL, "|", &context1);
        if (token) s->gpa = atof(token);

        // Load Scores
        char* scoresData = strtok_s(NULL, "|", &context1);
        s->scoreCount = 0;
        if (scoresData) {

            char* context2 = NULL;
            char* scoreToken = strtok_s(scoresData, ";", &context2);

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
                scoreToken = strtok_s(NULL, ";", &context2);
            }
        }

        views[viewCount].student = s;
        viewCount++;
    }

    fclose(file);
    updateNextStudentID();
    printf("Student data loaded from file.\n");
}

void displayAllStudentViews() {
    displayPaginatedViews(views, viewCount);
}

static int stricmp_portable(const char* s1, const char* s2) {
    while (*s1 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

static const char* stristr(const char* str, const char* find) {
    if (!find || !*find) return str; // If 'find' is empty, it's always a match

    while (*str) {
        const char* s1 = str;
        const char* s2 = find;
        // Compare characters one by one, ignoring case
        while (*s1 && *s2 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
            s1++;
            s2++;
        }
        if (!*s2) {
            return str;
        }
        str++;
    }
    return NULL; // No match found
}
void filterAndSortStudentViews(
    const char* idKeyword,
    const char* nameKeyword,
    const char* majorCode,
    float minGPA,
    float maxGPA,
    const char* sortByField,
    int sortOrder
) {
    struct StudentView matched[MAX_VIEWS];
    int matchedCount = 0;


    for (int i = 0; i < viewCount; i++) {
        struct Student* s = views[i].student;
        if ((idKeyword && strlen(idKeyword) > 0 && !stristr(s->id, idKeyword)) ||
            (nameKeyword && strlen(nameKeyword) > 0 && !stristr(s->name, nameKeyword)) ||
            (majorCode && strlen(majorCode) > 0 && stricmp_portable(s->major.code, majorCode) != 0) ||
            (minGPA >= 0 && s->gpa < minGPA) ||
            (maxGPA >= 0 && s->gpa > maxGPA)) {
            continue;
        }
        matched[matchedCount++] = views[i];
    }


    if (strlen(sortByField) > 0 && matchedCount > 1) {
        for (int i = 0; i < matchedCount - 1; i++) {
            for (int j = i + 1; j < matchedCount; j++) {
                int comparison = 0;
                if (stricmp_portable(sortByField, "gpa") == 0) {
                    if (matched[i].student->gpa < matched[j].student->gpa) comparison = -1;
                    else if (matched[i].student->gpa > matched[j].student->gpa) comparison = 1;
                } else if (stricmp_portable(sortByField, "name") == 0) {
                    comparison = stricmp_portable(matched[i].student->name, matched[j].student->name);
                } else if (stricmp_portable(sortByField, "id") == 0) {
                    comparison = stricmp_portable(matched[i].student->id, matched[j].student->id);
                }

                if (comparison * sortOrder > 0) {
                    struct StudentView temp = matched[i];
                    matched[i] = matched[j];
                    matched[j] = temp;
                }
            }
        }
    }

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

static int isSubjectAlreadyTaken(struct Student* s, const char* subjectCode) {
    for (int i = 0; i < s->scoreCount; i++) {
        if (strcmp(s->scores[i].subject.code, subjectCode) == 0) {
            return 1; // True, the student already has this subject
        }
    }
    return 0; // False
}


static void validateScoresAfterMajorChange(struct Student* s) {
    struct Score validScores[10];
    int validCount = 0;
    int changesMade = 0;

    for (int i = 0; i < s->scoreCount; i++) {
        struct Subject currentSubject = s->scores[i].subject;

        if (strcmp(currentSubject.majorCode, s->major.code) == 0 ||
            strcmp(currentSubject.majorCode, "GEN") == 0) {
            validScores[validCount++] = s->scores[i]; // Keep the score
        } else {

            printf("  -> Notice: Subject '%s' is not valid for the new major '%s' and has been removed.\n",
                   currentSubject.name, s->major.name);
            changesMade = 1;
        }
    }


    s->scoreCount = validCount;
    for (int i = 0; i < validCount; i++) {
        s->scores[i] = validScores[i];
    }
    
    if (changesMade) {
        printf("  -> Press Enter to continue...");
        getchar();
    }
}


static void handleEditScores(struct Student* s) {
    while (1) {
        system("cls");
        printf("\n--- Managing Scores for: %s ---\n", s->name);
        printf("Current Scores:\n");
        if (s->scoreCount == 0) {
            printf("  No scores entered.\n");
        } else {
            for (int i = 0; i < s->scoreCount; i++) {
                printf("  %d. %-30s Score: %.2f\n", i + 1, s->scores[i].subject.name, s->scores[i].value);
            }
        }
        printf("----------------------------------------\n");
        printf("1. Add a new score\n");
        printf("2. Edit an existing score\n");
        printf("3. Remove a score\n");
        printf("0. Back to previous menu\n");
        printf("----------------------------------------\n");

        int choice = getInt("Enter your choice: ", 0, 3);

        if (choice == 0) {
            return;
        }

        switch (choice) {
            case 1: { 
                if (s->scoreCount >= 10) {
                    printf("Cannot add more scores (maximum 10).\n");
                    break;
                }

                struct Subject availableSubjects[MAX_SUBJECTS];
                int availableCount = 0;
                for (int i = 0; i < subjectCount; i++) {

                    if ((strcmp(subjects[i].majorCode, s->major.code) == 0 || strcmp(subjects[i].majorCode, "GEN") == 0) &&
                        !isSubjectAlreadyTaken(s, subjects[i].code)) {
                        availableSubjects[availableCount++] = subjects[i];
                    }
                }

                if (availableCount == 0) {
                    printf("No new subjects available to add for this major.\n");
                } else {
                    printf("\n--- Select a Subject to Add ---\n");
                    for (int i = 0; i < availableCount; i++) {
                        printf("  %d. %s (%s)\n", i + 1, availableSubjects[i].name, availableSubjects[i].code);
                    }
                    int subjectChoice = getInt("Select a subject: ", 1, availableCount);
                    s->scores[s->scoreCount].subject = availableSubjects[subjectChoice - 1];
                    s->scores[s->scoreCount].value = getFloat("Enter score (0-10): ", 0.0, 10.0);
                    s->scoreCount++;
                    printf("=> Score added successfully!\n");
                }
                break;
            }
            case 2: { 
                if (s->scoreCount == 0) break; 
                int scoreChoice = getInt("Which score do you want to edit? ", 1, s->scoreCount);
                float newScore = getFloat("Enter the new score (0-10): ", 0.0, 10.0);
                s->scores[scoreChoice - 1].value = newScore;
                printf("=> Score updated successfully!\n");
                break;
            }
            case 3: { // Remove a score
                if (s->scoreCount == 0) break; 
                int scoreChoice = getInt("Which score do you want to remove? ", 1, s->scoreCount);
                // Shift all subsequent elements one position to the left
                for (int i = scoreChoice - 1; i < s->scoreCount - 1; i++) {
                    s->scores[i] = s->scores[i + 1];
                }
                s->scoreCount--;
                printf("=> Score removed successfully!\n");
                break;
            }
        }
        printf("Press Enter to continue...");
        getchar();
    }
}



void editStudentInfo() {
    if (viewCount == 0) {
        printf("There are no students to edit.\n");
        return;
    }

    char idToEdit[20];
    getString("Enter Student ID to edit (e.g., SV0001): ", idToEdit, sizeof(idToEdit));

    int foundIndex = -1;
    for (int i = 0; i < viewCount; i++) {
        if (strcmp(students[i].id, idToEdit) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Error: Student with ID '%s' not found.\n", idToEdit);
        return;
    }

    struct Student* s = &students[foundIndex];
    int choice;
    int changesMade = 0;

    while (1) {
        system("cls");
        printf("\n--- Editing Student Information: %s ---\n", s->name);
        printf("ID: %s (Cannot be changed)\n", s->id);
        printf("---------------------------------------------\n");
        printf("1. Full Name      : %s\n", s->name);
        printf("2. Gender         : %s\n", s->gender);
        printf("3. Date of Birth  : %d-%02d-%02d\n", s->birthYear, s->birthMonth, s->birthDay);
        printf("4. Major          : %s (%s)\n", s->major.name, s->major.code);
        printf("5. Manage Scores  : (%d subjects)\n", s->scoreCount);
        printf("---------------------------------------------\n");
        printf("0. Save Changes and Exit\n");
        printf("---------------------------------------------\n");

        choice = getInt("Select the information you want to edit: ", 0, 5);

        if (choice == 0) {
            break;
        }

        changesMade = 1;

        switch (choice) {
            case 1:
                getString("Enter new name: ", s->name, sizeof(s->name));
                break;
            case 2:
                getGender("Enter new gender", s->gender, sizeof(s->gender));
                break;
            case 3:
                getDate("Enter new date of birth", &s->birthYear, &s->birthMonth, &s->birthDay);
                break;
            case 4: {
                printf("\n--- Select a new major ---\n");
                for (int i = 0; i < majorCount; i++) {
                    printf("  %d. %s (%s)\n", i + 1, majors[i].name, majors[i].code);
                }
                int majorChoice = getInt("Enter your choice: ", 1, majorCount);
                s->major = majors[majorChoice - 1];
                // After changing major, validate the student's current scores.
                validateScoresAfterMajorChange(s);
                break;
            }
            case 5:
                // Call the new, dedicated function for score management.
                handleEditScores(s);
                break;
        }
        if(choice != 5) { // The score handler has its own pause
             printf("\n=> Information updated. Press Enter to continue editing...");
             getchar();
        }
    }

    if (changesMade) {
        s->gpa = calculateGPA(s->scores, s->scoreCount);
        saveStudentViewsToFile("students.txt");
        printf("\n=> Student information has been updated and saved to the file.\n");
        printf("     The newly recalculated GPA is: %.2f\n", s->gpa);
    } else {
        printf("\n=> No changes were made.\n");
    }
}