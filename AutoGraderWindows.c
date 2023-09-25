#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COMMAND_LENGTH 1000




void logError(char* message, char* studentid, char* assignmentid, char* weekid) {
    FILE* logFile = fopen("error_log.txt", "a");
    if (logFile) {
        fprintf(logFile, "Error: %s %s %s %s\n", message, studentid, weekid, assignmentid);
        fclose(logFile);
    }
}

int grade(char* studentid, char* assignmentid, int questionCount, char* weekid) {
    //define paths
    char c_program_path[32];
    char input_file_path[32];
    char answer_file_path[32];
    char compile_command[MAX_COMMAND_LENGTH];
    sprintf(c_program_path, "%s\\%s\\%s_%s.c", weekid, studentid, studentid, assignmentid);
    sprintf(compile_command, "gcc %s -o compiled_program.exe", c_program_path);
    //compile Program
    if (system(compile_command) != 0) {
        logError("Compilation failed.", studentid, assignmentid, weekid);
        return 1;
    } else {
        printf("Compilation successful.\n");
        //go through each test case
        for (int i = 0; i < questionCount; i++){
            //define input and answer paths
            sprintf(input_file_path, "%s\\input%d.txt\0", weekid, i);
            sprintf(answer_file_path, "%s\\answer%d.txt\0", weekid, i);
            //open files
            FILE* input_file = fopen(input_file_path, "r");
            FILE* answer_file = fopen(answer_file_path, "r");

            if (!input_file) {
                logError("Error opening input file.", studentid, assignmentid, weekid);
                return 1;
            }

            if (!answer_file) {
                logError("Error creating/opening answer file.", studentid, assignmentid, weekid);
                return 1;
            }
            //define variables for program execution
            char compiled_program_path[MAX_COMMAND_LENGTH];
            snprintf(compiled_program_path, sizeof(compiled_program_path), "%s", "compiled_program.exe");

            char execution_command[MAX_COMMAND_LENGTH];
            snprintf(execution_command, sizeof(execution_command), "%s < %s", compiled_program_path, input_file_path);
            //execute program
            FILE* program_output = popen(execution_command, "r");
            if (!program_output) {
                logError("Error executing the compiled program.", studentid, assignmentid, weekid);
                return 1;
            }
            //compare output to answer
            char bufferAnswer[1000];
            char bufferOutput[1000];
            while (fgets(bufferOutput, sizeof(bufferOutput), program_output) != NULL && fgets(bufferAnswer, sizeof(bufferAnswer), answer_file) != NULL) {
                if (strcmp(bufferOutput, bufferAnswer) != 0) {
                    logError("Incorrect output.", studentid, assignmentid, weekid);
                    return 1;
                }
            }

            // close files and cleanup
            fclose(input_file);
            fclose(answer_file);
            pclose(program_output);

            // clean up by removing the compiled program
            remove("compiled_program.exe");
            printf("Execution successful.\n");
        }

        return 0;
    }
}

int main(){
    //define students, week, assignment, question count, grading file
    char gradesPath[32] = {"gradestest.csv"};
    char* studentid[3] = {"std1\0", "std2\0", "std3\0"};
    char* assignmentid[1] = {"test\0"};
    char* weekid = "test\0";
    int questionCount = 2;
    //open grading file
    FILE* gradesFile = fopen(gradesPath, "w");
    if (!gradesFile) {
        printf("error opening gradesFile");
        return 1;
    }
    //write header
    fprintf(gradesFile, "StudentID,");
    for (int i = 0; i < 1; i++){
        fprintf(gradesFile, "%s,", assignmentid[i]);
    }
    //grade each student
    for (int i = 0; i < 3; i++){
        fprintf(gradesFile, "\n%s,", studentid[i]);
        for (int j = 0; j < 1; j++){
            if (grade(studentid[i], assignmentid[j], questionCount, weekid)){
                fprintf(gradesFile, "0,");
            }else{
                fprintf(gradesFile, "1,");
            }
        }
    }
    //close files and cleanup   
    fclose(gradesFile);
    printf("fin");
    return 0;
}