#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

// Clears the input buffer.
void clearInputBuffer();

// Gets a NON-EMPTY string from the user.
void getString(const char* prompt, char* buffer, int size);

// Gets a string that CAN BE EMPTY from the user. Ideal for optional fields.
void getOptionalString(const char* prompt, char* buffer, int size); // <-- ADD THIS LINE

// Gets an integer from the user within a specified range [min, max].
int getInt(const char* prompt, int min, int max);

// Gets a float from the user within a specified range [min, max].
float getFloat(const char* prompt, float min, float max);

// Gets and validates a date of birth from the user.
void getDate(const char* prompt, int* year, int* month, int* day);

void getGender(const char* prompt, char* buffer, int size);
float getOptionalFloat(const char* prompt, float min, float max, float defaultValue); 
void getHtmlFilePath(const char* prompt, char* buffer, int size);
int stricmp_portable(const char* s1, const char* s2);
#endif