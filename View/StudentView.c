#include <stdio.h>
#include "StudentView.h"

void displayStudentView(struct StudentView view) {
    struct Student* s = view.student;

    printf("ID    : %s\n", s->id);
    printf("Name  : %s\n", s->name);
    printf("Gender: %s\n", s->gender); // <-- ADD THIS LINE
    printf("Major : %s (%s)\n", s->major.name, s->major.code);
    printf("GPA   : %.2f\n", s->gpa);

    for (int i = 0; i < s->scoreCount; i++) {
        printf("  %s: %.2f\n", s->scores[i].subject.code, s->scores[i].value);
    }
}