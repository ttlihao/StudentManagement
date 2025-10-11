#include <stdio.h>
#include <string.h>
#include <time.h>
#include "InputHelper.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getString(const char* prompt, char* buffer, int size) {
    while (1) {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline
        if (strlen(buffer) > 0) {
            break;
        }
        printf("Input cannot be empty. Please try again.\n");
    }
}
void getOptionalString(const char* prompt, char* buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline
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

void getDate(const char* prompt, int* year, int* month, int* day) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int currentYear = tm.tm_year + 1900;

    printf("%s", prompt);
    while (1) {
        printf("\nEnter Birth Year, Month, Day (YYYY MM DD): ");
        if (scanf("%d %d %d", year, month, day) == 3) {
            if (*year > 1900 && *year <= currentYear && *month >= 1 && *month <= 12 && *day >= 1 && *day <= 31) {
                 clearInputBuffer();
                 return;
            }
        }
        printf("Invalid date. Please enter a valid date.\n");
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

        // Case 1: User pressed Enter to skip
        if (buffer[0] == '\n') {
            return defaultValue;
        }

        // Case 2: User entered something, try to convert it
        if (sscanf(buffer, "%f", &value) == 1) {
            // Conversion successful, now check the range
            if (value >= min && value <= max) {
                return value; // Valid input, return it
            } else {
                // Input is a number but out of range
                printf("Error: Please enter a value between %.2f and %.2f.\n", min, max);
            }
        } else {
            // Input was not a valid number
            printf("Error: Invalid input. Please enter a number.\n");
        }
    }
}