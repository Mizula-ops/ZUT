//SO IS1 211B LAB09
//Artur Mizula
//ma55831@zut.edu.pl
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 2) exit(201);

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        while (*arg) {
            if (!isdigit(*arg)) exit(202);
            arg++;
        }
        int num = atoi(argv[i]);
        if (num < 0 || num > 100) exit(202);
    }

    if (argc == 2) exit(atoi(argv[1]));

    if (argc == 3) {
        if (atoi(argv[1]) > atoi(argv[2])) exit(atoi(argv[1]));
        else exit(atoi(argv[2]));
    }

    int n = (argc - 1) / 2 + 1;
    char left_command[1024] = "";
    char right_command[1024] = "";

    strcat(left_command, argv[0]);
    strcat(right_command, argv[0]);

    for (int i = 1; i < n; i++) {
        strcat(left_command, " ");
        strcat(left_command, argv[i]);
    }

    for (int i = n; i < argc; i++) {
        strcat(right_command, " ");
        strcat(right_command, argv[i]);
    }

    PROCESS_INFORMATION pi1 = {0}, pi2 = {0};
    STARTUPINFO si1 = {0}, si2 = {0};
    si1.cb = sizeof(si1);
    si2.cb = sizeof(si2);

    if (!CreateProcess(NULL, left_command, NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1)) {
        fprintf(stderr, "CreateProcess for left command failed. Error: %d\n", GetLastError());
        exit(203);
    }
    if (!CreateProcess(NULL, right_command, NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2)) {
        fprintf(stderr, "CreateProcess for right command failed. Error: %d\n", GetLastError());
        exit(204);
    }

    HANDLE handles[2] = {pi1.hProcess, pi2.hProcess};
    WaitForMultipleObjects(2, handles, TRUE, INFINITE);

    DWORD exit_code1, exit_code2;
    GetExitCodeProcess(pi1.hProcess, &exit_code1);
    GetExitCodeProcess(pi2.hProcess, &exit_code2);

    printf("%d %d %d |", GetCurrentProcessId(), pi1.dwProcessId, exit_code1);
    for (int i = 1; i < n; i++) {
        printf(" %s", argv[i]);
    }
    printf("\n");

    printf("%d %d %d |", GetCurrentProcessId(), pi2.dwProcessId, exit_code2);
    for (int i = n; i < argc; i++) {
        printf(" %s", argv[i]);
    }
    printf("\n");

    printf("%d %d\n", GetCurrentProcessId(), exit_code1 > exit_code2 ? exit_code1 : exit_code2);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    int final_exit = exit_code1 > exit_code2 ? exit_code1 : exit_code2;
    exit(final_exit);
}
