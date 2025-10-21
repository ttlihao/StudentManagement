#ifndef DASHBOARD_CONTROLLER_H
#define DASHBOARD_CONTROLLER_H

#define MAX_SUBJECTS_IN_STATS 100

struct SubjectStats {
    char subjectCode[10];
    char subjectName[50];
    int studentCount;
    double totalScore;
    float minScore;
    float maxScore;
    int scoreBins[6]; // For the histogram
};

struct DashboardStats {
    int totalStudents;
    int totalMajors;
    int totalSubjects;
    double averageGpa;
    struct SubjectStats subjectStats[MAX_SUBJECTS_IN_STATS];
};

// Generates the full statistics report
struct DashboardStats generateStatistics();

// Handles the logic for exporting the database to an HTML file
void exportReportToHtml(const char* filePath);

#endif