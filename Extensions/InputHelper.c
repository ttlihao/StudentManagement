#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "InputHelper.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getString(const char* prompt, char* buffer, int size) {
    while (1) {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {
            break;
        }
        printf("Input cannot be empty. Please try again.\n");
    }
}
void getOptionalString(const char* prompt, char* buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}
int getInt(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1 && value >= min && value <= max) {
            clearInputBuffer();
            return value;
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
        clearInputBuffer();
    }
}

float getFloat(const char* prompt, float min, float max) {
    float value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &value) == 1 && value >= min && value <= max) {
            clearInputBuffer();
            return value;
        }
        printf("Invalid input. Please enter a number between %.2f and %.2f.\n", min, max);
        clearInputBuffer();
    }
}

static int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int isValidDate(int day, int month, int year, int currentYear) {
    if (year < 1900 || year > currentYear || month < 1 || month > 12 || day < 1) {
        return 0; 
    }

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return 0; 
    }


    if (month == 2) {
        if (isLeapYear(year)) {
            if (day > 29) return 0; 
        } else {
            if (day > 28) return 0; 
        }
    } else { 
        if (day > 31) return 0;
    }


    return 1; 
}


void getDate(const char* prompt, int* year, int* month, int* day) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int currentYear = tm.tm_year + 1900;

    printf("%s", prompt);
    while (1) {
        printf(" (DD MM YYYY): "); 
        if (scanf("%d %d %d", day, month, year) == 3) {
            if (isValidDate(*day, *month, *year, currentYear)) {
                clearInputBuffer();
                return; 
            }
        }

        printf("Invalid date. Please enter a valid date (e.g., 25 12 2004).\n");
        clearInputBuffer();
    }
}
void getGender(const char* prompt, char* buffer, int size) {
    char input[20];
    while (1) {
        printf("%s", prompt);
        printf(" (1-Male, 2-Female, 3-Other): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "1") == 0 || strcasecmp(input, "male") == 0) {
            strncpy(buffer, "Male", size -1);
            buffer[size-1] = '\0';
            return;
        }
        if (strcmp(input, "2") == 0 || strcasecmp(input, "female") == 0) {
            strncpy(buffer, "Female", size -1);
            buffer[size-1] = '\0';
            return;
        }
        if (strcmp(input, "3") == 0 || strcasecmp(input, "other") == 0) {
            strncpy(buffer, "Other", size -1);
            buffer[size-1] = '\0';
            return;
        }
        printf("Invalid input. Please enter 1, 2, or 3.\n");
    }
}
float getOptionalFloat(const char* prompt, float min, float max, float defaultValue) {
    char buffer[100];
    float value;

    while (1) {
        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);

        if (buffer[0] == '\n') {
            return defaultValue;
        }

        if (sscanf(buffer, "%f", &value) == 1) {
            if (value >= min && value <= max) {
                return value; 
            } else {
                printf("Error: Please enter a value between %.2f and %.2f.\n", min, max);
            }
        } else {
            printf("Error: Invalid input. Please enter a number.\n");
        }
    }
}
int stricmp_portable(const char* s1, const char* s2) {
    while (*s1 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}
void getHtmlFilePath(const char* prompt, char* buffer, int size) {
    // 1. Get a non-empty string from the user first.
    getString(prompt, buffer, size);

    int len = strlen(buffer);

    // 2. Check if the string already ends with ".html" (case-insensitive).
    //    We only check if the length is sufficient.
    if (len > 5 && stricmp_portable(buffer + len - 5, ".html") == 0) {
        // The path is already valid, so we do nothing.
        return;
    }

    // 3. If it doesn't end with .html, append it.
    //    We use strncat for safety to prevent buffer overflows.
    strncat(buffer, ".html", size - len - 1);

    printf("  -> Notice: Appended '.html' extension to the filename.\n");
}