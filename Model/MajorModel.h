#ifndef MAJOR_MODEL_H
#define MAJOR_MODEL_H

#include "Major.h"
void loadMajors();
void saveMajors();
struct Major* getAllMajors();
int getMajorCount();
/*ADD THIS CODE*/
 /**
  * @brief Adds a new major to the list in memory.
  * @param newMajor The major structure to add.
  * @return 1 on success, 0 if the list is full.
  */
 int addMajor(struct Major newMajor);

 /**
  * @brief Deletes a major from the list in memory using its code.
  * @param code The unique code of the major to delete.
  * @return 1 on success, 0 if the major code is not found.
  */
 int deleteMajor(const char* code);

 /**
  * @brief Finds a major in the list by its unique code.
  * @param code The code to search for.
  * @return A pointer to the found Major struct, or NULL if not found.
  */
 struct Major* findMajorByCode(const char* code);
 /*END NEW CODE*/
#endif