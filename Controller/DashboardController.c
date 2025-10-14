#include <stdio.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include "DashboardController.h"
#include "StudentController.h" 
#include "MajorController.h"  
#include "SubjectController.h" 


struct SubjectStats {
    char subjectCode[10];
    char subjectName[50];
    int studentCount;
    double totalScore;
    float minScore;
    float maxScore;
};


static void displayScoreDistributionDiagram(const char* subjectCode, const char* subjectName) {
    // Bins for score ranges:
    // [0] = 9.0-10.0, [1] = 8.0-8.9, [2] = 7.0-7.9, [3] = 5.5-6.9, [4] = 4.0-5.4, [5] = <4.0
    int scoreBins[6] = {0};
    int studentCountForSubject = 0;


    for (int i = 0; i < viewCount; i++) {
        for (int j = 0; j < students[i].scoreCount; j++) {
            if (strcmp(students[i].scores[j].subject.code, subjectCode) == 0) {
                studentCountForSubject++;
                float score = students[i].scores[j].value;
                if (score >= 9.0) scoreBins[0]++;
                else if (score >= 8.0) scoreBins[1]++;
                else if (score >= 7.0) scoreBins[2]++;
                else if (score >= 5.5) scoreBins[3]++;
                else if (score >= 4.0) scoreBins[4]++;
                else scoreBins[5]++;
            }
        }
    }

    if (studentCountForSubject == 0) {
        return; 
    }

    printf("\n  Score Distribution for %s (%s):\n", subjectName, subjectCode);
    printf("  --------------------------------------------------\n");

    const char* labels[] = {
        "9.0 - 10.0", "8.0 -  8.9", "7.0 -  7.9",
        "5.5 -  6.9", "4.0 -  5.4", "0.0 -  3.9"
    };

    for (int i = 0; i < 6; i++) {
        printf("  %-10s | ", labels[i]);

        for (int j = 0; j < scoreBins[i]; j++) {
            printf("#");
        }

        printf(" (%d)\n", scoreBins[i]);
    }
}


void showStatisticsReport() {
    system("cls");
    printf("========================================\n");
    printf("           ACADEMIC STATISTICS\n");
    printf("========================================\n");

    if (viewCount == 0) {
        printf("No student data available to generate statistics.\n");
        return;
    }
    // --- 1. General Statistics (remains the same) ---
    printf("\n--- Overall Summary ---\n");
    printf("Total Number of Students: %d\n", viewCount);
    printf("Total Number of Majors:   %d\n", majorCount);
    printf("Total Number of Subjects: %d\n", subjectCount);
    double totalGpa = 0;
    for (int i = 0; i < viewCount; i++) {
        totalGpa += students[i].gpa;
    }
    printf("Average Student GPA:      %.2f\n", (totalGpa / viewCount));


    printf("\n--- Subject Performance ---\n");
    if (subjectCount > 0) {
        struct SubjectStats stats[MAX_SUBJECTS];

        for (int i = 0; i < subjectCount; i++) {
            strcpy(stats[i].subjectCode, subjects[i].code);
            strcpy(stats[i].subjectName, subjects[i].name);
            stats[i].studentCount = 0;
            stats[i].totalScore = 0.0;
            stats[i].minScore = 10.1;
            stats[i].maxScore = -0.1;
        }

        for (int i = 0; i < viewCount; i++) {
            for (int j = 0; j < students[i].scoreCount; j++) {
                for (int k = 0; k < subjectCount; k++) {
                    if (strcmp(students[i].scores[j].subject.code, stats[k].subjectCode) == 0) {
                        stats[k].studentCount++;
                        stats[k].totalScore += students[i].scores[j].value;
                        if (students[i].scores[j].value < stats[k].minScore) stats[k].minScore = students[i].scores[j].value;
                        if (students[i].scores[j].value > stats[k].maxScore) stats[k].maxScore = students[i].scores[j].value;
                        break;
                    }
                }
            }
        }

        printf("%-10s | %-25s | %-8s | %-12s | %-10s\n", "Code", "Subject Name", "Students", "Average Score", "Score Range");
        printf("-----------+---------------------------+----------+---------------+------------\n");
        for (int i = 0; i < subjectCount; i++) {
            if (stats[i].studentCount > 0) {
                double average = stats[i].totalScore / stats[i].studentCount;
                printf("%-10s | %-25s | %-8d | %-12.2f | %.2f - %.2f\n", stats[i].subjectCode, stats[i].subjectName, stats[i].studentCount, average, stats[i].minScore, stats[i].maxScore);
            } else {
                printf("%-10s | %-25s | %-8d | %-12s | %s\n", stats[i].subjectCode, stats[i].subjectName, 0, "N/A", "N/A");
            }
        }
    } else {
        printf("No subjects available to analyze.\n");
    }


    printf("\n--- Score Distribution Diagrams ---\n");
    if (subjectCount > 0) {
        for (int i = 0; i < subjectCount; i++) {
            // Call our new helper function for each subject
            displayScoreDistributionDiagram(subjects[i].code, subjects[i].name);
        }
    }
}