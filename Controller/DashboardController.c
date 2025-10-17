#include <stdio.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include<time.h>
#include "../Extensions/InputHelper.h"    
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

static void writeHtmlHeader(FILE* file, const char* title) {
    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html lang=\"en\">\n<head>\n");
    fprintf(file, "    <meta charset=\"UTF-8\">\n");
    fprintf(file, "    <title>%s</title>\n", title);
    fprintf(file, "    <style>\n");
    fprintf(file, "        body { font-family: Arial, sans-serif; margin: 20px; background-color: #f4f4f9; }\n");
    fprintf(file, "        h1 { color: #333; }\n");
    fprintf(file, "        h2 { color: #555; border-bottom: 2px solid #ddd; padding-bottom: 10px; margin-top: 40px; }\n");
    fprintf(file, "        table { border-collapse: collapse; width: 100%%; margin-bottom: 30px; box-shadow: 0 2px 3px rgba(0,0,0,0.1); }\n");
    fprintf(file, "        th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }\n");
    fprintf(file, "        th { background-color: #4CAF50; color: white; font-weight: bold; }\n");
    fprintf(file, "        tr:nth-child(even) { background-color: #f9f9f9; }\n");
    fprintf(file, "        tr:hover { background-color: #f1f1f1; }\n");
    fprintf(file, "        .container { max-width: 1200px; margin: auto; background: white; padding: 20px; border-radius: 8px; }\n");
    fprintf(file, "    </style>\n</head>\n<body>\n<div class=\"container\">\n");
    fprintf(file, "<h1>%s</h1>\n", title);
}
static void writeHtmlFooter(FILE* file) {
    fprintf(file, "</div>\n</body>\n</html>\n");
}

static void exportMajorsToHtml(FILE* file) {
    fprintf(file, "<h2>Majors</h2>\n<table>\n");
    fprintf(file, "  <tr><th>Code</th><th>Name</th></tr>\n");
    for (int i = 0; i < majorCount; i++) {
        fprintf(file, "  <tr><td>%s</td><td>%s</td></tr>\n", majors[i].code, majors[i].name);
    }
    fprintf(file, "</table>\n");
}

static void exportSubjectsToHtml(FILE* file) {
    fprintf(file, "<h2>Subjects</h2>\n<table>\n");
    fprintf(file, "  <tr><th>Code</th><th>Name</th><th>Credit Hours</th><th>Major Code</th></tr>\n");
    for (int i = 0; i < subjectCount; i++) {
        fprintf(file, "  <tr><td>%s</td><td>%s</td><td>%d</td><td>%s</td></tr>\n", subjects[i].code, subjects[i].name, subjects[i].creditHours, subjects[i].majorCode);
    }
    fprintf(file, "</table>\n");
}

static void exportStudentsToHtml(FILE* file) {
    fprintf(file, "<h2>Students</h2>\n<table>\n");
    fprintf(file, "  <tr><th>ID</th><th>Name</th><th>Gender</th><th>Birthdate</th><th>Major</th><th>GPA</th></tr>\n");
    for (int i = 0; i < viewCount; i++) {
        fprintf(file, "  <tr><td>%s</td><td>%s</td><td>%s</td><td>%d-%02d-%02d</td><td>%s (%s)</td><td>%.2f</td></tr>\n",
                students[i].id, students[i].name, students[i].gender,
                students[i].birthYear, students[i].birthMonth, students[i].birthDay,
                students[i].major.name, students[i].major.code, students[i].gpa);
    }
    fprintf(file, "</table>\n");
}

static void exportStatisticsToHtml(FILE* file) {
    fprintf(file, "<h2>Statistics Report</h2>\n");
    // --- General Stats Table ---
    fprintf(file, "<h3>Overall Summary</h3>\n<table>\n");
    double totalGpa = 0;
    if (viewCount > 0) {
        for (int i = 0; i < viewCount; i++) totalGpa += students[i].gpa;
    }
    fprintf(file, "  <tr><td>Total Students</td><td>%d</td></tr>\n", viewCount);
    fprintf(file, "  <tr><td>Total Majors</td><td>%d</td></tr>\n", majorCount);
    fprintf(file, "  <tr><td>Total Subjects</td><td>%d</td></tr>\n", subjectCount);
    fprintf(file, "  <tr><td>Average Student GPA</td><td>%.2f</td></tr>\n", viewCount > 0 ? (totalGpa / viewCount) : 0.0);
    fprintf(file, "</table>\n");

    // --- Per-Subject Stats Table ---
    if (subjectCount > 0) {
        fprintf(file, "<h3>Subject Performance</h3>\n<table>\n");
        fprintf(file, "  <tr><th>Subject Name</th><th>Enrolled Students</th><th>Average Score</th><th>Score Range</th></tr>\n");
        // ... (Logic to calculate stats is the same as the statistics controller) ...
        for (int i = 0; i < subjectCount; i++) {
            int enrolled = 0;
            double totalScore = 0;
            float minScore = 10.1, maxScore = -0.1;
            for(int j=0; j < viewCount; j++) {
                for(int k=0; k < students[j].scoreCount; k++) {
                    if(strcmp(students[j].scores[k].subject.code, subjects[i].code) == 0) {
                        enrolled++;
                        totalScore += students[j].scores[k].value;
                        if(students[j].scores[k].value < minScore) minScore = students[j].scores[k].value;
                        if(students[j].scores[k].value > maxScore) maxScore = students[j].scores[k].value;
                    }
                }
            }
            if(enrolled > 0) {
                 fprintf(file, "  <tr><td>%s (%s)</td><td>%d</td><td>%.2f</td><td>%.2f - %.2f</td></tr>\n", subjects[i].name, subjects[i].code, enrolled, totalScore/enrolled, minScore, maxScore);
            } else {
                 fprintf(file, "  <tr><td>%s (%s)</td><td>0</td><td>N/A</td><td>N/A</td></tr>\n", subjects[i].name, subjects[i].code);
            }
        }
        fprintf(file, "</table>\n");
    }
}


void exportDatabaseToHtml() {

    char filePath[512]; 
    printf("\nPlease enter the full path where you want to save the HTML report.\n");
    printf("Examples:\n");
    printf("  - On Windows: C:\\Users\\YourName\\Desktop\\student_report.html\n");
    printf("  - On Linux/macOS: /home/yourname/Documents/student_report.html\n");
    

    getHtmlFilePath("Enter save path: ", filePath, sizeof(filePath));


    FILE* file = fopen(filePath, "w");
    if (!file) {

        printf("\n❌ Error: Could not create or open the file at the specified path.\n");
        printf("Please ensure the directory exists and you have permission to write to it.\n");
        return;
    }

    writeHtmlHeader(file, "Student Database Report");
    exportStudentsToHtml(file);
    exportMajorsToHtml(file);
    exportSubjectsToHtml(file);
    exportStatisticsToHtml(file);
    writeHtmlFooter(file);

    fclose(file);
    printf("\nData successfully exported to:\n%s\n", filePath);
}