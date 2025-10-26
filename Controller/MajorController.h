/*ADD THIS CODE*/
 #ifndef MAJOR_CONTROLLER_H
 #define MAJOR_CONTROLLER_H

 #include "../Model/Major.h" // Include the Major struct definition

 /**
  * @brief Returns the struct representing the "No Major" state.
  */
 struct Major getNoMajor();

 /**
  * @brief Business logic to add a new major. Checks for duplicates.
  * @param newMajor The Major struct containing the new major's details.
  * @return 1 if successfully added, 0 if code already exists or list is full.
  */
 int handleAddMajor(struct Major newMajor);

 /**
  * @brief Business logic to delete a major. Updates associated students.
  * @param code The code of the major to delete.
  * @return 1 if successfully deleted, 0 if the major was not found.
  */
 int handleDeleteMajor(const char* code);

 /**
  * @brief Business logic to find a major by its code.
  * @param code The major code to search for.
  * @return Pointer to the found Major struct, or NULL if not found.
  */
 struct Major* handleFindMajorByCode(const char* code);

 /**
  * @brief Calculates the number of students currently assigned to a specific major.
  * @param majorCode The code of the major.
  * @return The count of students in that major.
  */
 int getStudentCountForMajor(const char* majorCode);

 /**
  * @brief Creates a comma-separated string listing the IDs of students in a major.
  * @param majorCode The code of the major.
  * @param idBuffer Output buffer to store the comma-separated IDs.
  * @param bufferSize The size of the idBuffer.
  */
 void getStudentIDsForMajor(const char* majorCode, char* idBuffer, int bufferSize);

 /**
  * @brief Business logic to assign a student to a major.
  * @param major Pointer to the Major struct to assign.
  * @param studentId ID of the student to update.
  * @return 1 if successful, 0 if student not found.
  */
 int handleAddStudentToMajor(struct Major* major, const char* studentId);

 /**
  * @brief Business logic to remove a student from their current major.
  * Sets the student's major field to the "No Major" state.
  * @param studentId ID of the student to update.
  * @return 1 if successful, 0 if student not found.
  */
 int handleRemoveStudentFromMajor(const char* studentId);

 #endif // MAJOR_CONTROLLER_H
 /*END NEW CODE*/