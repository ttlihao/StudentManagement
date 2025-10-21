#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Model/StudentModel.h"
#include "../Model/MajorModel.h"   
#include "../Model/SubjectModel.h" 
#define STUDENT_FILE "Data/students.txt"

static struct Student studentList[MAX_STUDENTS];
static int studentCount = 0;
static int nextStudentID = 1;

void updateNextStudentID() {
    if (studentCount == 0) {
        nextStudentID = 1;
        return;
    }
    int maxID = 0;
    for (int i = 0; i < studentCount; i++) {
        int idNum = atoi(studentList[i].id + 2); // Skip "SV"
        if (idNum > maxID) maxID = idNum;
    }
    nextStudentID = maxID + 1;
}

void loadStudents() {
    FILE* file = fopen(STUDENT_FILE, "r");
    if (!file) { return; }

    char line[1024];
    studentCount = 0;
    struct Major* allMajors = getAllMajors();
    int totalMajors = getMajorCount();

    while (fgets(line, sizeof(line), file) && studentCount < MAX_STUDENTS) {
        struct Student* s = &studentList[studentCount];
        line[strcspn(line, "\n")] = 0;

        char* parts[10] = {0}; // Pointers to parts of the string
        int partCount = 0;
        char* token = strtok(line, "|");
        while(token != NULL && partCount < 9) {
            parts[partCount++] = token;
            token = strtok(NULL, "|");
        }

        if (partCount < 8) continue; // Invalid line

        strcpy(s->id, parts[0]);
        strcpy(s->name, parts[1]);
        s->birthYear = atoi(parts[2]);
        s->birthMonth = atoi(parts[3]);
        s->birthDay = atoi(parts[4]);
        strcpy(s->gender, parts[5]);
        
        // Find and assign major
        for(int i=0; i<totalMajors; i++){
            if(strcmp(allMajors[i].code, parts[6]) == 0){
                s->major = allMajors[i];
                break;
            }
        }
        
        s->gpa = atof(parts[7]);

        // Load Scores
        s->scoreCount = 0;
        if (partCount > 8 && parts[8]) {
            char* scoreToken = strtok(parts[8], ";");
            while (scoreToken && s->scoreCount < MAX_STUDENT_SCORES) {
                char subjectCode[10];
                float value;
                if (sscanf(scoreToken, "%[^,],%f", subjectCode, &value) == 2) {
                    struct Subject* foundSubject = findSubjectByCode(subjectCode);
                    if (foundSubject) {
                        s->scores[s->scoreCount].subject = *foundSubject;
                        s->scores[s->scoreCount].value = value;
                        s->scoreCount++;
                    }
                }
                scoreToken = strtok(NULL, ";");
            }
        }
        studentCount++;
    }
    fclose(file);
    updateNextStudentID();
}

void saveStudents() {
    FILE* file = fopen(STUDENT_FILE, "w");
    if (!file) { perror("Could not save students"); return; }
    for (int i = 0; i < studentCount; i++) {
        struct Student* s = &studentList[i];
        fprintf(file, "%s|%s|%d|%d|%d|%s|%s|%.2f|",
                s->id, s->name, s->birthYear, s->birthMonth, s->birthDay,
                s->gender, s->major.code, s->gpa);
        for (int j = 0; j < s->scoreCount; j++) {
            fprintf(file, "%s,%.2f%s", s->scores[j].subject.code, s->scores[j].value, (j < s->scoreCount - 1) ? ";" : "");
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

// GPA based on 10-point score scale converted to 4-point GPA scale
void calculateStudentGPA(struct Student* student) {
    if (!student || student->scoreCount == 0) {
        student->gpa = 0.0f;
        return;
    }
    float totalPoints = 0;
    int totalCredits = 0;
    for (int i = 0; i < student->scoreCount; i++) {
        totalPoints += (student->scores[i].value / 10.0f * 4.0f) * student->scores[i].subject.creditHours;
        totalCredits += student->scores[i].subject.creditHours;
    }
    student->gpa = (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0f;
}

int getNextStudentID() { return nextStudentID; }
struct Student* getAllStudents() { return studentList; }
int getStudentCount() { return studentCount; }

int addStudent(struct Student newStudent) {
    if (studentCount >= MAX_STUDENTS) return 0;
    studentList[studentCount++] = newStudent;
    nextStudentID++;
    return 1;
}

struct Student* findStudentById(const char* id) {
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(studentList[i].id, id) == 0) {
            return &studentList[i];
        }
    }
    return NULL;
}

int deleteStudent(const char* id) {
    int foundIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(studentList[i].id, id) == 0) {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex == -1) return 0;

    for (int i = foundIndex; i < studentCount - 1; i++) {
        studentList[i] = studentList[i + 1];
    }
    studentCount--;
    return 1;
}
