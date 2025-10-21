#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Controller/StudentController.h"
#include "../Model/StudentModel.h"
#include "../Extensions/InputHelper.h"
#include "../View/StudentView.h"

int handleAddStudent(struct Student newStudent) {
    calculateStudentGPA(&newStudent); // Ensure GPA is calculated before adding
    return addStudent(newStudent);
}

struct Student* handleFindStudent(const char* id) {
    return findStudentById(id);
}

int handleDelStudent(const char* id) {
    return deleteStudent(id);
}

const char* stristr(const char* haystack, const char* needle) {
    if (!needle || !*needle) return haystack;
    while (*haystack) {
        const char* h = haystack;
        const char* n = needle;
        while (*h && *n && (tolower((unsigned char)*h) == tolower((unsigned char)*n))) {
            h++;
            n++;
        }
        if (!*n) return haystack;
        haystack++;
    }
    return NULL;
}


struct Student** handleFilterAndSort(
    const char* idKeyword, const char* nameKeyword, const char* majorCode,
    float minGPA, float maxGPA,
    const char* sortByField, int sortOrder,
    int* resultCount
) {
    struct Student* allStudents = getAllStudents();
    int totalCount = getStudentCount();
    struct Student* matched[MAX_STUDENTS];
    int matchedCount = 0;

    // --- Filtering ---
    for (int i = 0; i < totalCount; i++) {
        struct Student* s = &allStudents[i];
        if ((strlen(idKeyword) > 0 && !stristr(s->id, idKeyword)) ||
            (strlen(nameKeyword) > 0 && !stristr(s->name, nameKeyword)) ||
            (strlen(majorCode) > 0 && stricmp_portable(s->major.code, majorCode) != 0) ||
            (minGPA >= 0 && s->gpa < minGPA) ||
            (maxGPA >= 0 && s->gpa > maxGPA)) {
            continue;
        }
        matched[matchedCount++] = s;
    }

    // --- Sorting (Bubble Sort for simplicity) ---
    if (strlen(sortByField) > 0 && matchedCount > 1) {
        for (int i = 0; i < matchedCount - 1; i++) {
            for (int j = i + 1; j < matchedCount; j++) {
                int comparison = 0;
                if (stricmp_portable(sortByField, "gpa") == 0) {
                    if (matched[i]->gpa < matched[j]->gpa) comparison = -1;
                    else if (matched[i]->gpa > matched[j]->gpa) comparison = 1;
                } else if (stricmp_portable(sortByField, "name") == 0) {
                    comparison = stricmp_portable(matched[i]->name, matched[j]->name);
                } else { // Default to ID
                    comparison = stricmp_portable(matched[i]->id, matched[j]->id);
                }

                if (comparison * sortOrder > 0) { // Swap if needed
                    struct Student* temp = matched[i];
                    matched[i] = matched[j];
                    matched[j] = temp;
                }
            }
        }
    }

    // --- Prepare results for returning ---
    *resultCount = matchedCount;
    if (matchedCount == 0) return NULL;

    // Allocate memory for an array of POINTERS and copy them
    struct Student** results = malloc(matchedCount * sizeof(struct Student*));
    if (!results) {
        perror("Failed to allocate memory for filter results");
        *resultCount = 0;
        return NULL;
    }
    for (int i = 0; i < matchedCount; i++) {
        results[i] = matched[i];
    }
    return results;
}
static void validateScoresAfterMajorChange(struct Student* s) {
    struct Score validScores[MAX_STUDENT_SCORES];
    int validCount = 0;
    int changesMade = 0;

    for (int i = 0; i < s->scoreCount; i++) {
        struct Subject currentSubject = s->scores[i].subject;
        // Keep scores that match the new major code or are "General"
        if (strcmp(currentSubject.majorCode, s->major.code) == 0 || strcmp(currentSubject.majorCode, "GEN") == 0) {
            validScores[validCount++] = s->scores[i];
        } else {
            // This score is no longer valid, inform the user via the view.
            char message[256];
            sprintf(message, "Notice: Subject '%s' removed (invalid for new major '%s').", currentSubject.name, s->major.name);
            showMessage(message);
            changesMade = 1;
        }
    }

    // Update the student's scores array with only the valid ones
    s->scoreCount = validCount;
    for (int i = 0; i < validCount; i++) {
        s->scores[i] = validScores[i];
    }
    
    if (changesMade) {
        pressEnterToContinue();
    }
}

// Manages the sub-menu for adding, editing, or removing scores.
static void handleScoreManagementSession(struct Student* s) {
    while (1) {
        int choice = displayScoresMenu(s);
        if (choice == 0) break;

        switch (choice) {
            case 1: // Add Score
                if (s->scoreCount >= MAX_STUDENT_SCORES) {
                    showMessage("Cannot add more scores (maximum reached).");
                } else {
                    // The view will prompt user to select a subject and get the score
                    getNewScoreFromUser(s);
                }
                break;
            case 2: // Edit Score
                if (s->scoreCount > 0) {
                    editExistingScore(s); // View function to handle the process
                }
                break;
            case 3: // Remove Score
                 if (s->scoreCount > 0) {
                    removeScore(s); // View function to handle the process
                }
                break;
        }
    }
    
    
}


// --- Public Session Function ---

void handleEditStudentSession(const char* studentId) {
    struct Student* s = findStudentById(studentId);
    if (!s) {
        showMessage("Student not found.");
        return;
    }

    int changesMade = 0;
    while (1) {
        int choice = displayEditMenu(s);
        if (choice == 0) break;

        changesMade = 1;
        switch (choice) {
            case 1: // Edit Name
                getNewNameFromUser(s->name, sizeof(s->name));
                break;
            case 2: // Edit Gender
                getNewGenderFromUser(s->gender, sizeof(s->gender));
                break;
            case 3: // Edit DOB
                getNewDateFromUser(&s->birthYear, &s->birthMonth, &s->birthDay);
                break;
            case 4: { // Edit Major
                struct Major newMajor = getNewMajorFromUser();
                s->major = newMajor;
                validateScoresAfterMajorChange(s);
                break;
            }
            case 5: // Manage Scores
                handleScoreManagementSession(s);
                break;
        }
    }

    if (changesMade) {
        calculateStudentGPA(s);
        saveStudents();
        char message[100];
        sprintf(message, "Student info updated. New GPA: %.2f", s->gpa);
        showMessage(message);
    } else {
        showMessage("No changes were made.");
    }
}