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
        printf("\n--- Student List (Page %d of %d) ---\n\n", currentPage, totalPages);

        int startIndex = (currentPage - 1) * PAGE_SIZE;
        int endIndex = startIndex + PAGE_SIZE;
        if (endIndex > count) {
            endIndex = count;
        }

        for (int i = startIndex; i < endIndex; i++) {
            displayStudentView(viewsToShow[i]);
            printf("------------------------------------\n");
        }

        printf("\n[N]ext Page | [P]revious Page | [Q]uit to Menu\n");
        getOptionalString("Enter navigation choice: ", navInput, sizeof(navInput));

        if (strcasecmp(navInput, "n") == 0) {
            if (currentPage < totalPages) currentPage++;
        } else if (strcasecmp(navInput, "p") == 0) {
            if (currentPage > 1) currentPage--;
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
