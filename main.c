#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Controller/StudentController.h"

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void waitForEnter() {
    printf("\nPress Enter to return to menu...");
    getchar(); // consume leftover newline
    getchar(); // wait for Enter
}

int main() {
    int choice;
    char filename[] = "students.txt";
    loadStudentViewsFromFile(filename);

    do {
        system(CLEAR);
        printf("===== Student Management Menu =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Save Students to File\n");
        printf("4. Load Students from File\n");
        printf("5. Search and Sort Students\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        system(CLEAR);

        switch (choice) {
            case 1: {
                char confirm;
                do {
                    addStudentView();
                    printf("Add another student? (y/n): ");
                    getchar(); // consume newline
                    confirm = getchar();
                    system(CLEAR);
                } while (confirm == 'y' || confirm == 'Y');
                break;
            }

            case 2: {
                displayAllStudentViews();
                waitForEnter();
                break;
            }

            case 3: {
                saveStudentViewsToFile(filename);
                waitForEnter();
                break;
            }

            case 4: {
                loadStudentViewsFromFile(filename);
                waitForEnter();
                break;
            }

            case 5: {
                char id[50], name[50], major[10], sortField[10];
                float minGPA, maxGPA;

                printf("Enter ID keyword (or leave blank): ");
                getchar(); fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = '\0';

                printf("Enter name keyword (or leave blank): ");
                fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = '\0';

                printf("Enter major code (or leave blank): ");
                fgets(major, sizeof(major), stdin); major[strcspn(major, "\n")] = '\0';

                printf("Enter minimum GPA (-1 to skip): ");
                scanf("%f", &minGPA);
                printf("Enter maximum GPA (-1 to skip): ");
                scanf("%f", &maxGPA);
                getchar(); // consume newline

                printf("Sort by field (id, name, gpa): ");
                fgets(sortField, sizeof(sortField), stdin); sortField[strcspn(sortField, "\n")] = '\0';

                system(CLEAR);
                filterAndSortStudentViews(
                    strlen(id) ? id : NULL,
                    strlen(name) ? name : NULL,
                    strlen(major) ? major : NULL,
                    minGPA,
                    maxGPA,
                    strlen(sortField) ? sortField : NULL
                );
                waitForEnter();
                break;
            }

            case 0:
                printf("Exiting program. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
                waitForEnter();
        }

    } while (choice != 0);

    return 0;
}