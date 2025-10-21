#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Controller/DashboardController.h"
#include "../Model/StudentModel.h"
#include "../Model/MajorModel.h"
#include "../Model/SubjectModel.h"

struct DashboardStats generateStatistics() {
    struct DashboardStats stats = {0};
    struct Student* allStudents = getAllStudents();
    struct Subject* allSubjects = getAllSubjects();

    stats.totalStudents = getStudentCount();
    stats.totalMajors = getMajorCount();
    stats.totalSubjects = getSubjectCount();

    if (stats.totalStudents == 0) return stats;

    double totalGpa = 0;
    for (int i = 0; i < stats.totalStudents; i++) {
        totalGpa += allStudents[i].gpa;
    }
    stats.averageGpa = totalGpa / stats.totalStudents;

    for (int i = 0; i < stats.totalSubjects; i++) {
        strcpy(stats.subjectStats[i].subjectCode, allSubjects[i].code);
        strcpy(stats.subjectStats[i].subjectName, allSubjects[i].name);
        stats.subjectStats[i].minScore = 10.1;
        stats.subjectStats[i].maxScore = -0.1;
    }

    for (int i = 0; i < stats.totalStudents; i++) {
        for (int j = 0; j < allStudents[i].scoreCount; j++) {
            for (int k = 0; k < stats.totalSubjects; k++) {
                if (strcmp(allStudents[i].scores[j].subject.code, stats.subjectStats[k].subjectCode) == 0) {
                    struct SubjectStats* s = &stats.subjectStats[k];
                    float score = allStudents[i].scores[j].value;
                    s->studentCount++;
                    s->totalScore += score;
                    if (score < s->minScore) s->minScore = score;
                    if (score > s->maxScore) s->maxScore = score;

                    // Binning for histogram
                    if (score >= 9.0) s->scoreBins[0]++;
                    else if (score >= 8.0) s->scoreBins[1]++;
                    else if (score >= 7.0) s->scoreBins[2]++;
                    else if (score >= 5.5) s->scoreBins[3]++;
                    else if (score >= 4.0) s->scoreBins[4]++;
                    else s->scoreBins[5]++;
                    break;
                }
            }
        }
    }
    return stats;
}


// --- HTML Export Logic ---
static void writeHtmlHeader(FILE* file, const char* title) {
    fprintf(file, "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><title>%s</title>", title);
    fprintf(file, "<style>body{font-family:Arial,sans-serif;margin:20px;background-color:#f4f4f9;}h1,h2{color:#333;}table{border-collapse:collapse;width:100%%;margin-bottom:30px;}th,td{border:1px solid #ddd;padding:12px;text-align:left;}th{background-color:#4CAF50;color:white;}.container{max-width:1200px;margin:auto;background:white;padding:20px;border-radius:8px;}</style>");
    fprintf(file, "</head><body><div class='container'><h1>%s</h1>", title);
}
static void writeHtmlFooter(FILE* file) { fprintf(file, "</div></body></html>"); }

void exportReportToHtml(const char* filePath) {
    FILE* file = fopen(filePath, "w");
    if (!file) {
        perror("Error creating HTML file");
        return;
    }
    
    writeHtmlHeader(file, "Student Database Report");

    struct Student* students = getAllStudents();
    int studentCount = getStudentCount();
    fprintf(file, "<h2>Students (%d)</h2><table><tr><th>ID</th><th>Name</th><th>Gender</th><th>Birthdate</th><th>Major</th><th>GPA</th></tr>", studentCount);
    for (int i = 0; i < studentCount; i++) {
        fprintf(file, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%d-%02d-%02d</td><td>%s (%s)</td><td>%.2f</td></tr>",
                students[i].id, students[i].name, students[i].gender, students[i].birthYear, students[i].birthMonth, students[i].birthDay,
                students[i].major.name, students[i].major.code, students[i].gpa);
    }
    fprintf(file, "</table>");

    // Export majors, subjects, and stats similarly...
    struct DashboardStats stats = generateStatistics();
    fprintf(file, "<h2>Statistics Report</h2><h3>Overall Summary</h3><table>");
    fprintf(file, "<tr><td>Total Students</td><td>%d</td></tr>", stats.totalStudents);
    fprintf(file, "<tr><td>Average Student GPA</td><td>%.2f</td></tr></table>", stats.averageGpa);
    
    fprintf(file, "<h3>Subject Performance</h3><table><tr><th>Subject Name</th><th>Enrolled</th><th>Avg Score</th><th>Score Range</th></tr>");
    for(int i=0; i<stats.totalSubjects; i++) {
        struct SubjectStats* s = &stats.subjectStats[i];
         if(s->studentCount > 0) {
             fprintf(file, "<tr><td>%s (%s)</td><td>%d</td><td>%.2f</td><td>%.2f - %.2f</td></tr>", s->subjectName, s->subjectCode, s->studentCount, s->totalScore/s->studentCount, s->minScore, s->maxScore);
        }
    }
    fprintf(file, "</table>");

    writeHtmlFooter(file);
    fclose(file);
    printf("\nData successfully exported to: %s\n", filePath);
}