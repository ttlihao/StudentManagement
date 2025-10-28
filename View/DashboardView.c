#include <stdio.h>
#include <stdlib.h>
#include "../View/DashboardView.h"

static void displayScoreDistributionDiagram(const struct SubjectStats* subjectStat) {
    if (subjectStat->studentCount == 0) return;

    printf("\n  Score Distribution for %s (%s):\n", subjectStat->subjectName, subjectStat->subjectCode);
    printf("  --------------------------------------------------\n");

    const char* labels[] = {
        "9.0 - 10.0", "8.0 -  8.9", "7.0 -  7.9",
        "5.5 -  6.9", "4.0 -  5.4", "0.0 -  3.9"
    };

    for (int i = 0; i < 6; i++) {
        printf("  %-10s | ", labels[i]);
        for (int j = 0; j < subjectStat->scoreBins[i]; j++) {
            printf("#");
        }
        printf(" (%d)\n", subjectStat->scoreBins[i]);
    }
}

void displayDashboard(const struct DashboardStats* stats) {
    system("cls"); // "clear"
    printf("========================================\n");
    printf("           ACADEMIC STATISTICS\n");
    printf("========================================\n");

    if (stats->totalStudents == 0) {
        printf("No student data available to generate statistics.\n");
        return;
    }

    printf("\n--- Overall Summary ---\n");
    printf("Total Number of Students: %d\n", stats->totalStudents);
    printf("Total Number of Majors:   %d\n", stats->totalMajors);
    printf("Total Number of Subjects: %d\n", stats->totalSubjects);
    printf("Average Student GPA:      %.2f\n", stats->averageGpa);

    printf("\n--- Subject Performance ---\n");
    printf("%-10s | %-40s | %-8s | %-13s | %-10s\n", "Code", "Subject Name", "Students", "Average Score", "Score Range");
    printf("-----------+------------------------------------------+----------+---------------+------------\n");
    for (int i = 0; i < stats->totalSubjects; i++) {
        const struct SubjectStats* s = &stats->subjectStats[i];
        if (s->studentCount > 0) {
            double average = s->totalScore / s->studentCount;
            printf("%-10s | %-40s | %-8d | %-13.2f | %.2f - %.2f\n", s->subjectCode, s->subjectName, s->studentCount, average, s->minScore, s->maxScore);
        } else {
            printf("%-10s | %-40s | %-8d | %-13s | %s\n", s->subjectCode, s->subjectName, 0, "N/A", "N/A");
        }
    }

    printf("\n--- Score Distribution Diagrams ---\n");
    for (int i = 0; i < stats->totalSubjects; i++) {
        displayScoreDistributionDiagram(&stats->subjectStats[i]);
    }
}