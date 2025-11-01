 #ifndef MAJOR_CONTROLLER_H
 #define MAJOR_CONTROLLER_H

 #include "../Model/Major.h" // Include the Major struct definition
 struct Major getNoMajor();
 int handleAddMajor(struct Major newMajor);
 int handleDeleteMajor(const char* code);
 struct Major* handleFindMajorByCode(const char* code);
 int getStudentCountForMajor(const char* majorCode);
 void getStudentIDsForMajor(const char* majorCode, char* idBuffer, int bufferSize);
 int handleAddStudentToMajor(struct Major* major, const char* studentId);
 int handleRemoveStudentFromMajor(const char* studentId);

 #endif // MAJOR_CONTROLLER_H