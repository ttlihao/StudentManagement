/*ADD THIS CODE*/
 #include "MajorView.h"                   // Header for this view's declarations
 #include "../Controller/MajorController.h" // To call controller logic
 #include "../Model/MajorModel.h"         // To get major list
 #include "../Model/StudentModel.h"       // To check if student exists before removing
 #include "../Extensions/InputHelper.h"   // For user input functions
 #include "StudentView.h"          // To reuse showMessage and pressEnterToContinue
 #include <stdio.h>
 #include <stdlib.h>               // For system("cls")
 #include <string.h>               // For strcmp

 /**
  * @brief Implementation for displayMajorMenu. Clears screen and shows options.
  */
 int displayMajorMenu() {
     system("cls"); // Clear the console
     printf("\n--- Manage Majors ---\n");
     printf("1. Add New Major\n");
     printf("2. View All Majors\n");
     printf("3. Update Major Info (Add/Remove Student)\n");
     printf("4. Delete Major\n");
     printf("0. Back to Main Menu\n");
     // Get and return validated user choice between 0 and 4
     return getInt("Enter your choice: ", 0, 4);
 }

 /**
  * @brief Implementation for displayMajorList. Formats and prints the 3-column table.
  */
 void displayMajorList() {
     struct Major* majors = getAllMajors(); // Get majors from model
     int count = getMajorCount();          // Get count from model

     if (count == 0) {
         showMessage("No majors currently exist in the system.");
         return;
     }

     system("cls"); // Clear screen before showing the list
     printf("\n--- Major List ---\n");
     // Print the table header as specified
     printf("%-10s | %-20s | %s\n",
            "Major Code", "Student Count", "Student IDs");
     printf("-----------+----------------------+------------------------------------------\n"); // Separator line

     char studentIdBuffer[512]; // Buffer to hold the generated student ID string
     for (int i = 0; i < count; i++) {
         // Get calculated data from the controller
         int studentCount = getStudentCountForMajor(majors[i].code);
         getStudentIDsForMajor(majors[i].code, studentIdBuffer, sizeof(studentIdBuffer));

         // Print the table row
         printf("%-10s | %-20d | %s\n",
                majors[i].code,
                studentCount,
                // Display "(None)" if the ID buffer is empty, otherwise display the buffer content
                studentIdBuffer[0] == '\0' ? "(None)" : studentIdBuffer);
     }
 }

 /**
  * @brief Implementation for handleUpdateMajorStudents (Menu Option 3).
  */
 void handleUpdateMajorStudents() {
     char majorCodeInput[10];
     // Prompt for and get the major code
     getString("Enter Major Code to update students for: ", majorCodeInput, sizeof(majorCodeInput));
     // Optional: Convert input majorCodeInput to uppercase if needed

     // Find the major using the controller
     struct Major* targetMajor = handleFindMajorByCode(majorCodeInput);

     // If the major doesn't exist, show error and return
     if (!targetMajor) {
         showMessage("Error: Major code not found.");
         return;
     }

     // Show which major is selected and the available actions
     printf("\nSelected Major: %s (%s)\n", targetMajor->name, targetMajor->code);
     printf("1. Add Student TO this Major\n");
     printf("2. Remove Student FROM their current Major\n");
     printf("0. Cancel\n");
     int actionChoice = getInt("Select action: ", 0, 2); // Get user's action choice

     char studentIdInput[15];
     switch (actionChoice) {
         case 1: // Add Student to the selected Major
             getString("Enter Student ID to ADD to this major: ", studentIdInput, sizeof(studentIdInput));
             // Call controller function to handle the addition
             if (handleAddStudentToMajor(targetMajor, studentIdInput)) {
                 showMessage("Student successfully assigned to the major.");
             } else {
                 showMessage("Error: Student ID not found. Assignment failed.");
             }
             break;

         case 2: // Remove Student from whatever major they are in
             getString("Enter Student ID to REMOVE from their current major: ", studentIdInput, sizeof(studentIdInput));
             // Check if student exists first to provide better feedback
             struct Student* studentToCheck = findStudentById(studentIdInput); // Use model function for check
             if (!studentToCheck) {
                  showMessage("Error: Student ID not found.");
             } else if (strcmp(studentToCheck->major.code, "") == 0) { // Check if student already has no major
                 showMessage("This student is not currently assigned to any major.");
             } else {
                 // Proceed with removal only if student exists and has a major
                 if (handleRemoveStudentFromMajor(studentIdInput)) { // Call Controller function
                      showMessage("Student successfully removed from their major (Major set to N/A).");
                 } else {
                       // This specific error should be rare now due to the check above
                      showMessage("Error removing student from major.");
                 }
             }
             break;

         case 0: // Cancel
             showMessage("Update operation cancelled.");
             break;
     }
 }
 /*END NEW CODE*/