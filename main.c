#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Controller/StudentController.h"
#include "Controller/MajorController.h"
#include "Controller/SubjectController.h"
#include "Extensions/InputHelper.h"
#include "Controller/DashboardController.h"

void displayMenu() {
    system("cls"); // Use "clear" for Linux/macOS, "cls" for Windows
    printf("====================================\n");
    printf("    Student Management System\n");
    printf("====================================\n");
    printf("1. Add New Student\n");
    printf("2. Display All Students\n");
    printf("3. Filter & Sort Students\n");
    printf("4. Delete Student\n");
    printf("5. Edit Student Info\n");
    printf("6. Dashboard\n");
    printf("7. Exit\n");
    printf("------------------------------------\n");
}

// Helper function to get input for filtering and sorting
void handleFilterSortInput() {
    char idKeyword[15] = "";
    char nameKeyword[50] = "";
    char majorCode[10] = "";
    char sortByField[10] = "";
    float minGPA = -1.0;
    float maxGPA = -1.0;
    const float MAX_SYSTEM_GPA = 4.0;

    printf("\n--- Filter & Sort Options ---\n");
    printf("Enter filter criteria (leave blank by pressing Enter to ignore).\n\n");

    getOptionalString("ID contains: ", idKeyword, sizeof(idKeyword));
    getOptionalString("Name contains: ", nameKeyword, sizeof(nameKeyword));
    getOptionalString("Major Code (exact match): ", majorCode, sizeof(majorCode));

    // --- New GPA Logic with Re-entry on Invalid Input ---
    printf("\nEnter GPA Range (0.0 - 4.0).\n");

    while (1) { // Loop until a valid GPA range is entered
        // 1. Get both inputs from the user.
        minGPA = getOptionalFloat("  Minimum GPA: ", 0.0, MAX_SYSTEM_GPA, -1.0);
        maxGPA = getOptionalFloat("  Maximum GPA: ", 0.0, MAX_SYSTEM_GPA, -1.0);

        // 2. Validate the logic.
        // This check only runs if BOTH values were actually entered.
        if (minGPA != -1.0 && maxGPA != -1.0 && minGPA > maxGPA) {
            // If the range is invalid, print an error and the loop will force re-entry.
            printf("\n Error: Minimum GPA cannot be greater than Maximum GPA.\n");
            printf("Please enter the range again.\n\n");
        } else {
            // The range is valid (or was skipped), so we can exit the loop.
            break;
        }
    }

    getOptionalString("\nSort by (id, name, or gpa): ", sortByField, sizeof(sortByField));
    int sortOrder = 1; 
    if (strlen(sortByField) > 0) { 
        printf("  Sort Order (1-Ascending, 2-Descending): ");
        int orderChoice = getInt("", 1, 2);
        if (orderChoice == 2) {
            sortOrder = -1; 
        }
    }
    filterAndSortStudentViews(idKeyword, nameKeyword, majorCode, minGPA, maxGPA, sortByField, sortOrder);
}

int main() {

    printf("Loading data from files...\n");
    loadMajorsFromFile("majors.txt");
    loadSubjectsFromFile("subjects.txt");
    loadStudentViewsFromFile("students.txt");
    printf("Data loading complete.\n\n");
    printf("Press Enter to start...");
    getchar();


    int choice;
    while (1) {
        displayMenu();
        choice = getInt("Enter your choice: ", 1, 7);

        switch (choice) {
            case 1: 
                printf("\n--- Add New Student ---\n");
                addStudentView();
                saveStudentViewsToFile("students.txt");

                printf("\nPress Enter to return to the menu...");
                getchar();
                break;

            case 2: // Display All Students
                displayAllStudentViews();
                printf("\nPress Enter to return to the menu...");
                getchar();
                break;

            case 3: // Filter & Sort Students
                handleFilterSortInput();
                printf("\nPress Enter to return to the menu...");
                getchar();
                break;
            case 4: //  Delete Student
             	deleteStudentView();
                saveStudentViewsToFile("students.txt");  
                printf("\nPress Enter to return to the menu...");
                getchar();
                break;
            case 5: //  Edit Student Info
             	editStudentInfo();  
                printf("\nPress Enter to return to the menu...");
                getchar();
                break;
            case 6: 
                showStatisticsReport();
                printf("\nPress Enter to return to the menu...");
                getchar();
                break;    
			case 7: // Exit
                printf("\nExiting program. Goodbye!\n");
                return 0; // Terminate the program

            default:
                // This case is unlikely due to the getInt validation, but is good practice.
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}