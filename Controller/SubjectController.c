#include "../Controller/SubjectController.h"
#include "../Model/SubjectModel.h"
#include<stddef.h>
// In Sources/SubjectController.c

int handleAddSubject(struct Subject newSubject) {
    if (findSubjectByCode(newSubject.code) != NULL) {
        return 0; // Already exists
    }
    return addSubject(newSubject);
}
int handleUpdateSubject(const char* code, struct Subject updatedInfo) {
    // The controller's job is to pass the command to the model.
    return updateSubject(code, updatedInfo);
}
int handleDeleteSubject(const char* code) {
    return deleteSubject(code);
}