#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "StudentController.h"

struct Student students[MAX_VIEWS];
struct StudentView views[MAX_VIEWS];
int viewCount = 0;

void addStudentView() {
    struct Student s;
    printf("Enter ID: "); scanf("%s", s.id);
    printf("Enter Name: "); getchar(); fgets(s.name, 50, stdin); s.name[strcspn(s.name, "\n")] = '\0';
    printf("Enter Birth Year, Month, Day: "); scanf("%d %d %d", &s.birthYear, &s.birthMonth, &s.birthDay);
    printf("Enter Major Code: "); scanf("%s", s.major.code);
    printf("Enter Major Name: "); getchar(); fgets(s.major.name, 50, stdin); s.major.name[strcspn(s.major.name, "\n")] = '\0';
    printf("Enter GPA: "); scanf("%f", &s.gpa);

    printf("How many subjects? "); scanf("%d", &s.scoreCount);
    for (int i = 0; i < s.scoreCount; i++) {
        printf("Subject #%d Code: ", i + 1); scanf("%s", s.scores[i].subject.code);
        printf("Subject #%d Name: ", i + 1); getchar(); fgets(s.scores[i].subject.name, 50, stdin); s.scores[i].subject.name[strcspn(s.scores[i].subject.name, "\n")] = '\0';
        printf("Score: "); scanf("%f", &s.scores[i].value);
    }

    students[viewCount] = s;
    views[viewCount].student = &students[viewCount];
    viewCount++;

    printf("Student added successfully!\n");
}

void saveStudentViewsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < viewCount; i++) {
        struct Student* s = views[i].student;
        fprintf(file, "%s|%s|%d|%d|%d|%s|%s|%.2f|", s->id, s->name, s->birthYear, s->birthMonth, s->birthDay, s->major.code, s->major.name, s->gpa);
        for (int j = 0; j < s->scoreCount; j++) {
            fprintf(file, "%s,%s,%.2f", s->scores[j].subject.code, s->scores[j].subject.name, s->scores[j].value);
            if (j < s->scoreCount - 1) fprintf(file, ";");
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Student data saved to file.\n");
}

void loadStudentViewsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    char line[512];
    viewCount = 0;

    while (fgets(line, sizeof(line), file)) {
        struct Student s;
        char* token = strtok(line, "|");
        strcpy(s.id, token);
        strcpy(s.name, strtok(NULL, "|"));
        s.birthYear = atoi(strtok(NULL, "|"));
        s.birthMonth = atoi(strtok(NULL, "|"));
        s.birthDay = atoi(strtok(NULL, "|"));
        strcpy(s.major.code, strtok(NULL, "|"));
        strcpy(s.major.name, strtok(NULL, "|"));
        s.gpa = atof(strtok(NULL, "|"));

        char* pointData = strtok(NULL, "\n");
        s.scoreCount = 0;
        char* pointToken = strtok(pointData, ";");
        while (pointToken && s.scoreCount < 10) {
            sscanf(pointToken, "%[^,],%[^,],%f", s.scores[s.scoreCount].subject.code, s.scores[s.scoreCount].subject.name, &s.scores[s.scoreCount].value);
            s.scoreCount++;
            pointToken = strtok(NULL, ";");
        }

        students[viewCount] = s;
        views[viewCount].student = &students[viewCount];
        viewCount++;
    }

    fclose(file);
    printf("Student data loaded from file.\n");
}

void displayAllStudentViews() {
    if (viewCount == 0) {
        printf("No students to display.\n");
        return;
    }

    for (int i = 0; i < viewCount; i++) {
        displayStudentView(views[i]);
    }
}

void filterAndSortStudentViews(
    const char* idKeyword,
    const char* nameKeyword,
    const char* majorCode,
    float minGPA,
    float maxGPA,
    const char* sortByField
) {
    struct StudentView matched[MAX_VIEWS];
    int matchedCount = 0;

    for (int i = 0; i < viewCount; i++) {
        struct Student* s = views[i].student;

        if (idKeyword && strlen(idKeyword) > 0 && !strstr(s->id, idKeyword)) continue;
        if (nameKeyword && strlen(nameKeyword) > 0 && !strstr(s->name, nameKeyword)) continue;
        if (majorCode && strlen(majorCode) > 0 && strcmp(s->major.code, majorCode) != 0) continue;
        if (minGPA >= 0 && s->gpa < minGPA) continue;
        if (maxGPA >= 0 && s->gpa > maxGPA) continue;

        matched[matchedCount++] = views[i];
    }

    if (matchedCount == 0) {
        printf("No matching students found.\n");
        return;
    }

    for (int i = 0; i < matchedCount - 1; i++) {
        for (int j = i + 1; j < matchedCount; j++) {
            int shouldSwap = 0;

            if (sortByField && strcmp(sortByField, "gpa") == 0) {
                shouldSwap = matched[i].student->gpa < matched[j].student->gpa;
            } else if (sortByField && strcmp(sortByField, "name") == 0) {
                shouldSwap = strcmp(matched[i].student->name, matched[j].student->name) > 0;
            } else if (sortByField && strcmp(sortByField, "id") == 0) {
                shouldSwap = strcmp(matched[i].student->id, matched[j].student->id) > 0;
            }

            if (shouldSwap) {
                struct StudentView temp = matched[i];
                matched[i] = matched[j];
                matched[j] = temp;
            }
        }
    }

    printf("\nFiltered and Sorted Students:\n");
    for (int i = 0; i < matchedCount; i++) {
        displayStudentView(matched[i]);
    }
}