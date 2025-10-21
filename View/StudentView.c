#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../View/StudentView.h"
#include "../Model/SubjectModel.h"
#include "../Model/MajorModel.h"
#include "../Extensions/InputHelper.h"

#define PAGE_SIZE 7 


void displayStudentDetails(const struct Student* student, int index) {
    // --- Print the main, aligned row for the student ---
    printf("%-5d | %-15s | %-28s | %-25s | %.2f\n",
           index,
           student->id,
           student->name,
           student->major.name,
           student->gpa);
           
    // --- Check if there are scores and list them underneath, indented ---
    if (student->scoreCount > 0) {
        for (int i = 0; i < student->scoreCount; i++) {
            // Indent with spaces to align under the name column
            printf("      | %-15s   -> %-28s: %.2f\n",
                   student->scores[i].subject.code,
                   student->scores[i].subject.name,
                   student->scores[i].value);
        }
    }
}

/**
 * @brief Displays a paginated list using the new hybrid table format.
 */
void displayPaginatedList(struct Student* students, int count) {
    if (count == 0) {
        printf("\n>> No students to display. <<\n");
        return;
    }

    int totalPages = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    int currentPage = 1;
    char navInput[20];

    while (1) {
        system("cls"); // Use "clear" for Linux/macOS
        printf("\n--- Student List (Page %d of %d | Total: %d students) ---\n\n", currentPage, totalPages, count);

        // --- Table Header ---
        printf("%-5s | %-15s | %-28s | %-25s | %s\n", "Index", "Student ID", "Full Name", "Major", "GPA");
        printf("------+-----------------+------------------------------+---------------------------+------\n");

        int startIndex = (currentPage - 1) * PAGE_SIZE;
        int endIndex = (startIndex + PAGE_SIZE > count) ? count : startIndex + PAGE_SIZE;

        for (int i = startIndex; i < endIndex; i++) {
            displayStudentDetails(&students[i], startIndex + i + 1);
            // Add a light separator between student entries for readability
            if (i < endIndex - 1) {
                printf("      |....................................................................................\n");
            }
        }
        
        printf("------+-----------------+------------------------------+---------------------------+------\n");

        printf("\n[N]ext | [P]rev | [J]ump to Page | [Q]uit to Menu\n");
        getOptionalString("Enter navigation choice: ", navInput, sizeof(navInput));

        if (stricmp_portable(navInput, "n") == 0) {
            if (currentPage < totalPages) currentPage++;
        } else if (stricmp_portable(navInput, "p") == 0) {
            if (currentPage > 1) currentPage--;
        } else if (stricmp_portable(navInput, "j") == 0) {
            currentPage = getInt("Enter page number: ", 1, totalPages);
        } else if (stricmp_portable(navInput, "q") == 0) {
            break;
        }
    }
}
int displayEditMenu(const struct Student* s) {
    system("cls");
    printf("\n--- Editing Student: %s ---\n", s->name);
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
    return getInt("Select field to edit: ", 0, 5);
}

int displayScoresMenu(const struct Student* s) {
    system("cls");
    printf("\n--- Managing Scores for: %s ---\n", s->name);
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
    return getInt("Enter your choice: ", 0, 3);
}

void getNewNameFromUser(char* buffer, int size) {
    getString("Enter new name: ", buffer, size);
}

void getNewGenderFromUser(char* buffer, int size) {
    getGender("Enter new gender", buffer, size);
}

void getNewDateFromUser(int* y, int* m, int* d) {
    getDate("Enter new date of birth", y, m, d);
}

struct Major getNewMajorFromUser() {
    printf("\n--- Select a new major ---\n");
    struct Major* majors = getAllMajors();
    int count = getMajorCount();
    for (int i = 0; i < count; i++) {
        printf("  %d. %s (%s)\n", i + 1, majors[i].name, majors[i].code);
    }
    int choice = getInt("Enter your choice: ", 1, count);
    return majors[choice - 1];
}
static int isSubjectAlreadyTaken(const struct Student* s, const char* subjectCode) {
    for (int i = 0; i < s->scoreCount; i++) {
        if (strcmp(s->scores[i].subject.code, subjectCode) == 0) {
            return 1; // True, subject found in the student's scores
        }
    }
    return 0; // False
}
void getNewScoreFromUser(struct Student* s) {
    printf("\n--- Select an Available Subject to Add ---\n");
    struct Subject* allSubjects = getAllSubjects();
    int subjectCount = getSubjectCount();
    int availableCount = 0;

    for (int i = 0; i < subjectCount; i++) {
        // --- THIS IS THE UPDATED LOGIC ---
        // Condition 1: Subject is for the student's major OR is a general subject.
        int isRelevantMajor = (strcmp(allSubjects[i].majorCode, s->major.code) == 0 || strcmp(allSubjects[i].majorCode, "GEN") == 0);
        
        // Condition 2: Student has NOT already taken the subject.
        int isNotTaken = !isSubjectAlreadyTaken(s, allSubjects[i].code);

        if (isRelevantMajor && isNotTaken) {
            printf("  -> Code: %s, Name: %s\n", allSubjects[i].code, allSubjects[i].name);
            availableCount++;
        }
    }
    
    // Check if there are any subjects left to add
    if (availableCount == 0) {
        printf("\nNo new subjects are available for this student.\n");
        return;
    }

    char code[10];
    getString("Enter subject code: ", code, sizeof(code));
    struct Subject* subject = findSubjectByCode(code);

    // Final validation to ensure the entered code is valid and not already taken
    if (subject && !isSubjectAlreadyTaken(s, subject->code)) {
        s->scores[s->scoreCount].subject = *subject;
        s->scores[s->scoreCount].value = getFloat("Enter score (0-10): ", 0.0, 10.0);
        s->scoreCount++;
        printf("\nScore added successfully.\n");
    } else {
        printf("\nInvalid or already taken subject code.\n");
    }
}

void editExistingScore(struct Student* s) {
    int choice = getInt("Which score to edit? (enter number): ", 1, s->scoreCount);
    float newScore = getFloat("Enter the new score (0-10): ", 0.0, 10.0);
    s->scores[choice - 1].value = newScore;
    printf("\nScore updated.\n");
}

void removeScore(struct Student* s) {
    int choice = getInt("Which score to remove? (enter number): ", 1, s->scoreCount);
    for (int i = choice - 1; i < s->scoreCount - 1; i++) {
        s->scores[i] = s->scores[i + 1];
    }
    s->scoreCount--;
    printf("\nScore removed.\n");
}