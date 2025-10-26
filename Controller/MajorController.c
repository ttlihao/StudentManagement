/*ADD THIS CODE*/
 #include "MajorController.h"       // Header for this controller
 #include "../Model/MajorModel.h"   // To interact with major data
 #include "../Model/StudentModel.h" // To interact with student data
 #include <string.h>               // For strcmp, strlen, strncat
 #include <stdio.h>                // For sprintf (although not used here, good practice)

 /**
  * @brief Constant representing the "No Major" state.
  */
 static const struct Major NO_MAJOR = {"", "N/A"};

 /**
  * @brief Returns the NO_MAJOR constant.
  */
 struct Major getNoMajor() {
     return NO_MAJOR;
 }

 /**
  * @brief Implements handleAddMajor: Checks duplicates, then calls model.
  */
 int handleAddMajor(struct Major newMajor) {
     // Check if a major with the same code already exists using the model's find function
     if (findMajorByCode(newMajor.code) != NULL) {
         return 0; // Failure: Duplicate code
     }
     // If no duplicate, call the model's add function
     return addMajor(newMajor);
 }

 /**
  * @brief Implements handleDeleteMajor: Updates students, then calls model.
  */
 int handleDeleteMajor(const char* code) {
     int studentsWereUpdated = 0; // Flag to check if saveStudents is needed
     // Step 1: Iterate through all students
     struct Student* students = getAllStudents();
     int studentCount = getStudentCount();
     for (int i = 0; i < studentCount; i++) {
         // If a student belongs to the major being deleted...
         if (strcmp(students[i].major.code, code) == 0) {
             students[i].major = NO_MAJOR; // ...set their major to the "No Major" state
             studentsWereUpdated = 1;      // Mark that at least one student was changed
         }
     }

     // Step 2: Save the student list *only* if changes were made
     if (studentsWereUpdated) {
         saveStudents();
     }

     // Step 3: Delete the major itself from the major list
     return deleteMajor(code); // Call model function
 }

 /**
  * @brief Implements handleFindMajorByCode: Simple pass-through to the model.
  */
 struct Major* handleFindMajorByCode(const char* code) {
     return findMajorByCode(code); // Call model function
 }

 /**
  * @brief Implements getStudentCountForMajor: Counts students matching the major code.
  */
 int getStudentCountForMajor(const char* majorCode) {
     int count = 0;
     struct Student* students = getAllStudents();
     int studentCount = getStudentCount();
     for (int i = 0; i < studentCount; i++) {
         // Check if the student's major code matches the provided code
         if (strcmp(students[i].major.code, majorCode) == 0) {
             count++; // Increment count if it matches
         }
     }
     return count;
 }

 /**
  * @brief Implements getStudentIDsForMajor: Builds the ID string safely.
  */
 void getStudentIDsForMajor(const char* majorCode, char* idBuffer, int bufferSize) {
     if (bufferSize <= 0) return; // Prevent writing to zero-sized buffer
     idBuffer[0] = '\0'; // Ensure the buffer starts as an empty C-string

     struct Student* students = getAllStudents();
     int studentCount = getStudentCount();
     int isFirstId = 1; // Flag to avoid leading comma

     for (int i = 0; i < studentCount; i++) {
         // Check if the student belongs to the specified major
         if (strcmp(students[i].major.code, majorCode) == 0) {
             const char* separator = isFirstId ? "" : ", "; // Add ", " before subsequent IDs
             size_t currentLen = strlen(idBuffer);
             size_t separatorLen = strlen(separator);
             size_t idLen = strlen(students[i].id);
             size_t neededSpace = separatorLen + idLen + 1; // +1 for null terminator

             // Check if there is enough space in the buffer for the separator, ID, and null terminator
             if (currentLen + neededSpace <= bufferSize) {
                 strncat(idBuffer, separator, bufferSize - currentLen - 1); // Append separator
                 strncat(idBuffer, students[i].id, bufferSize - strlen(idBuffer) - 1); // Append ID
                 isFirstId = 0; // Next ID will need a comma
             } else {
                 // Not enough space, try to append "..." if possible
                 size_t ellipsisLen = 3;
                 if (currentLen + ellipsisLen + 1 <= bufferSize) {
                     strncat(idBuffer, "...", bufferSize - currentLen - 1);
                 }
                 break; // Stop adding IDs as buffer is full
             }
         }
     }
      // Ensure null termination again in case loop finishes exactly at buffer capacity
      idBuffer[bufferSize - 1] = '\0';
 }


 /**
  * @brief Implements handleAddStudentToMajor: Finds student, updates, saves.
  */
 int handleAddStudentToMajor(struct Major* major, const char* studentId) {
     struct Student* student = findStudentById(studentId); // Find student via model
     if (student) {
         student->major = *major; // Update the student's major field
         saveStudents();         // Persist the change to the student file
         return 1; // Success
     }
     return 0; // Failure: Student not found
 }

 /**
  * @brief Implements handleRemoveStudentFromMajor: Finds student, sets to NO_MAJOR, saves.
  */
 int handleRemoveStudentFromMajor(const char* studentId) {
     struct Student* student = findStudentById(studentId); // Find student via model
     if (student) {
         student->major = NO_MAJOR; // Update student's major to the "No Major" state
         saveStudents();           // Persist the change to the student file
         return 1; // Success
     }
     return 0; // Failure: Student not found
 }
 /*END NEW CODE*/