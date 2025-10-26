/*ADD THIS CODE*/
 #ifndef MAJOR_VIEW_H
 #define MAJOR_VIEW_H

 // Forward declaration needed if Major struct is used, but it isn't directly needed here
 // #include "../Model/Major.h"

 /**
  * @brief Displays the menu options for Major Management.
  * @return The user's validated integer choice (0-4).
  */
 int displayMajorMenu();

 /**
  * @brief Displays the list of all majors in a formatted table (3 columns).
  * Requires data fetched by controller functions (getStudentCountForMajor, getStudentIDsForMajor).
  */
 void displayMajorList();

 /**
  * @brief Handles the user interaction for option 3: Add/Remove Student from Major.
  * Prompts for Major Code, action (add/remove), and Student ID.
  */
 void handleUpdateMajorStudents();

 // Note: The main loop function runMajorManagementSession() is kept in main.c
 // following the pattern of runSubjectManagementSession().

 #endif // MAJOR_VIEW_H
 /*END NEW CODE*/