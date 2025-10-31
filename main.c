#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Extensions/InputHelper.h"
#include "Model/StudentModel.h"
#include "Model/MajorModel.h"
#include "Model/SubjectModel.h"
#include "View/StudentView.h"
#include "View/DashboardView.h"
#include "View/SubjectView.h"
#include "View/MajorView.h" 
#include "Controller/StudentController.h"
#include "Controller/SubjectController.h"
#include "Controller/DashboardController.h"
#include "Controller/MajorController.h" 

static int getConfirmation(const char* prompt) {
    char buffer[10];
    while(1) {
        // We use getString from InputHelper to ensure non-empty input
        getString(prompt, buffer, sizeof(buffer)); 
        
        // stricmp_portable from InputHelper handles case-insensitivity (Y/y)
        if (stricmp_portable(buffer, "y") == 0 || stricmp_portable(buffer, "yes") == 0) {
            return 1; // User confirmed
        }
        if (stricmp_portable(buffer, "n") == 0 || stricmp_portable(buffer, "no") == 0) {
            return 0; // User denied
        }
        printf("Invalid input. Please enter 'y' or 'n'.\n");
    }
}

void showMessage(const char* message) {
    printf("\n>> %s <<\n", message);
}

void pressEnterToContinue() {
    printf("\nPress Enter to return to the menu...");
    getchar();
}

void handleAddNewStudent() {
    struct Student s;
    sprintf(s.id, "SV%04d", getNextStudentID());
    printf("New Student ID: %s\n", s.id);

    getString("Enter Name: ", s.name, 50);
    getDate("Enter Date of Birth", &s.birthYear, &s.birthMonth, &s.birthDay);
    getGender("Enter Gender", s.gender, sizeof(s.gender));

    printf("\n--- Select a Major ---\n");
    struct Major* majors = getAllMajors();
    int majorCount = getMajorCount();
    for (int i = 0; i < majorCount; i++) {
        printf("  %d. %s (%s)\n", i + 1, majors[i].name, majors[i].code);
    }
    int choice = getInt("Choose a major: ", 1, majorCount);
    s.major = majors[choice - 1];

    s.scoreCount = getInt("\nHow many subjects to add scores for? ", 0, MAX_STUDENT_SCORES);
    struct Subject* allSubjects = getAllSubjects();
    int totalSubjects = getSubjectCount();

    for (int i = 0; i < s.scoreCount; i++) {
        printf("\n--- Entering Score for Subject #%d ---\n", i + 1);
        for(int j=0; j<totalSubjects; j++) {
            if(strcmp(allSubjects[j].majorCode, s.major.code) == 0) {
                 printf("  -> Code: %s, Name: %s\n", allSubjects[j].code, allSubjects[j].name);
            }
        }
        char subCode[10];
        getString("Enter subject code from list above: ", subCode, sizeof(subCode));
        struct Subject* chosenSub = findSubjectByCode(subCode);
        if(chosenSub) {
            s.scores[i].subject = *chosenSub;
            s.scores[i].value = getFloat("Enter score (0-10): ", 0.0, 10.0);
        } else {
            showMessage("Invalid subject code, skipping.");
            i--; // Retry this score entry
        }
    }
    
    if (handleAddStudent(s)) {
        showMessage("Student added successfully!");
        saveStudents();
    } else {
        showMessage("Error: Could not add student.");
    }
}



// This entire function lives in main.c and handles all user input.
void handleFilterSort() {
    char id[15] = "", name[50] = "", major[10] = "", sortBy[10] = "";
    float minG = -1.0, maxG = -1.0;

    printf("\n--- Filter & Sort Options (Press Enter to skip) ---\n");
    
    // 1. All "scanf" (getOptionalString, etc.) calls are here in main.c
    getOptionalString("ID contains: ", id, sizeof(id));
    getOptionalString("Name contains: ", name, sizeof(name));
    getOptionalString("Major Code (exact): ", major, sizeof(major));
    minG = getOptionalFloat("Min GPA: ", 0.0, 4.0, -1.0);
    maxG = getOptionalFloat("Max GPA: ", 0.0, 4.0, -1.0);
    getOptionalString("\nSort by (id, name, gpa): ", sortBy, sizeof(sortBy));
    
    int sortOrder = 1;
    if(strlen(sortBy) > 0) {
        int orderChoice = getInt("Sort Order (1-Asc, 2-Desc): ", 1, 2);
        if(orderChoice == 2) sortOrder = -1;
    }

    int resultCount = 0;
    struct Student** results = handleFilterAndSort(id, name, major, minG, maxG, sortBy, sortOrder, &resultCount);
	if(results) {
        struct Student* resultStructs = malloc(resultCount * sizeof(struct Student));
        for(int i=0; i<resultCount; i++) {
            resultStructs[i] = *results[i];
        }
        displayPaginatedList(resultStructs, resultCount);
        free(resultStructs);
        free(results);
    } else {
        showMessage("No students match your criteria.");
    }
}        




void displayMenu() {
    system("cls");
    printf("====================================\n");
    printf("    Student Management System\n");
    printf("====================================\n");
    printf("1. Add New Student\n");
    printf("2. Display All Students\n");
    printf("3. Filter & Sort Students\n");
    printf("4. Delete Student\n");
    printf("5. Edit Student Info\n");
    printf("6. Dashboard & Statistics\n");
    printf("7. Manage Subjects\n"); 
    printf("8. Manage Majors\n"); 
    printf("9. Export Report to HTML File\n");
    printf("0. Exit\n"); 
    printf("------------------------------------\n");
}

static void runSubjectManagementSession() {
    int choice;
    do {
        choice = displaySubjectMenu(); 

        switch (choice) {
            case 1: {
                struct Subject newSubject;
                if (getNewSubjectInfo(&newSubject)) { 
                    int result = handleAddSubject(newSubject); 
                    if (result == 1) {
                        showMessage("Subject added successfully.");
                        saveSubjects(); // Model saves data
                    } else {
                        showMessage("Error: Subject code already exists.");
                    }
                }
                pressEnterToContinue();
                break;
            }
            case 2: // View All Subjects
                displaySubjectList(getAllSubjects(), getSubjectCount());
                pressEnterToContinue();
                break;
            case 3: { // Update Subject
                char code[10];
                getString("Enter subject code to update: ", code, sizeof(code));
                
                // 1. Find the subject via the Model (read-only is acceptable here)
                const struct Subject* originalSubject = findSubjectByCode(code);

                if (originalSubject) {
                    // 2. The View gets the new data into a temporary struct
                    struct Subject updatedInfo;
                    getUpdatedSubjectInfo(originalSubject, &updatedInfo);
                    
                    // 3. The Controller handles the update logic
                    if (handleUpdateSubject(code, updatedInfo)) {
                        showMessage("Subject updated successfully.");
                        saveSubjects();
                    } else {
                        // This case shouldn't be hit if find works, but it's safe to have
                        showMessage("Error: Failed to update subject.");
                    }
                } else {
                    showMessage("Subject not found.");
                }
                pressEnterToContinue();
                break;
            }
            case 4: { // Delete Subject
                char code[10];
                getString("Enter subject code to delete: ", code, sizeof(code));
                if (findSubjectByCode(code)) {
                     if (getConfirmation("Are you sure? (y/n): ")) {
                        if(handleDeleteSubject(code)) {
                            showMessage("Subject deleted successfully.");
                            saveSubjects();
                        }
                     } else {
                        showMessage("Deletion cancelled.");
                     }
                } else {
                    showMessage("Subject not found.");
                }
                pressEnterToContinue();
                break;
            }
            case 0:
                // Return to main menu
                break;
        }
    } while (choice != 0);
}
 static void runMajorManagementSession() {
     int choice;
     do {
         // Display the major-specific menu (function defined in MajorView.c)
         choice = displayMajorMenu();

         switch (choice) {
             case 1: { // Add New Major
                 struct Major newMajor;
                 // Prompt user for Code and Name using InputHelper functions
                 getString("Enter new Major Code (e.g., IT, CS - will be stored as entered): ", newMajor.code, sizeof(newMajor.code));
                 // Note: Uppercase conversion is not automatically handled here, add if required.
                 getString("Enter new Major Name: ", newMajor.name, sizeof(newMajor.name));

                 // Call the controller function to handle the addition logic (includes duplicate check)
                 if (handleAddMajor(newMajor)) {
                     saveMajors(); // Persist the changes to the majors file via Model
                     showMessage("Major added successfully.");
                 } else {
                     showMessage("Error: Major code already exists or the major list is full.");
                 }
                 pressEnterToContinue(); // Wait for user acknowledgment
                 break;
             }
             case 2: // View All Majors
                 // Call the view function to display the formatted list
                 displayMajorList(); // Function defined in MajorView.c
                 pressEnterToContinue(); // Wait for user acknowledgment
                 break;
             case 3: { // Delete Major
                 char codeToDelete[10];
                 // Prompt user for the code of the major to delete
                 getString("Enter Major Code to delete: ", codeToDelete, sizeof(codeToDelete));
                 // Optional: Convert codeToDelete to uppercase if codes are consistently uppercase

                 // First, check if the major actually exists using the controller
                 if (handleFindMajorByCode(codeToDelete) == NULL) {
                     showMessage("Error: Major code not found.");
                 } else {
                     // If it exists, ask for confirmation
                     if (getConfirmation("Are you sure? Deleting this major will remove it from all associated students (y/n): ")) {
                         // If confirmed, call the controller function to handle deletion (updates students too)
                         if (handleDeleteMajor(codeToDelete)) {
                             saveMajors(); // Persist the change to the majors file via Model
                             // saveStudents() is called inside the controller's handleDeleteMajor
                             showMessage("Major deleted successfully. Associated students have been updated.");
                         } else {
                             // This error is less likely if find worked, but possible
                             showMessage("An error occurred while deleting the major.");
                         }
                     } else {
                         // If user entered 'n' or 'no'
                         showMessage("Deletion cancelled.");
                     }
                 }
                 pressEnterToContinue(); // Wait for user acknowledgment
                 break;
             }
             case 0:
                 // Choice 0 means return to the main menu, so do nothing here, the loop will exit.
                 break;
         }
     } while (choice != 0); // Continue loop until user chooses 0
 }
int main() {
    printf("Loading data...\n");
    loadMajors();
    loadSubjects();
    loadStudents();
    printf("Load complete. Press Enter to start...");
    getchar();

    int choice;
    do {
        displayMenu();
        choice = getInt("Enter your choice: ", 0, 9);

        switch (choice) {
            case 1:
                handleAddNewStudent();
                pressEnterToContinue();
                break;
            case 2:
                displayPaginatedList(getAllStudents(), getStudentCount());
                break;
            case 3:
                handleFilterSort();
                pressEnterToContinue();
                break;
            case 4: {
                char id[15];
                getString("Enter student ID to delete: ", id, sizeof(id));
                if (handleDelStudent(id)) {
                    showMessage("Student deleted.");
                    saveStudents();
                } else {
                    showMessage("Student not found.");
                }
                pressEnterToContinue();
                break;
            }
            case 5: { // Edit Student
                char id[15];
                getString("Enter Student ID to edit: ", id, sizeof(id));
                // The controller now handles the entire interactive session
                handleEditStudentSession(id);
                pressEnterToContinue();
                break;
            }
            case 6: {
                struct DashboardStats stats = generateStatistics();
                displayDashboard(&stats);
                pressEnterToContinue();
                break;
            }
            case 7:
                runSubjectManagementSession();
                break;
            case 8: 
                runMajorManagementSession(); 
                break;
            case 9: {
                char path[512];
                getHtmlFilePath("Enter path to save HTML report: ", path, sizeof(path));
                exportReportToHtml(path);
                pressEnterToContinue();
                break;
            }
            case 0:
                printf("\nExiting program. Goodbye!\n");
                break;
        }
    } while (choice != 0);

    return 0;
}