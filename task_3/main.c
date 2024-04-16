#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    char command[100];
    char *args[10]; // Массив для хранения аргументов команды
    pid_t pid;

    while (1) {
        printf("Введите имя программы и опции запуска: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        // Разделение введенной строки на аргументы
        int argc = 0;
        char *token = strtok(command, " ");
        while (token != NULL) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL; // Установка завершающего NULL-символа

        pid = fork();
        if (pid == 0) {
            // Если мы находимся в дочернем процессе, выполняем введенную программу
            if (argc > 1) {
                // Если есть аргументы, используем execvp()
                execvp(args[0], args);
            } else {
                // Если аргументов нет, используем execlp()
                execlp(args[0], args[0], NULL);
            }
            perror("exec");
            exit(1);
        } else {
            // Если мы находимся в родительском процессе, ожидаем завершения дочернего процесса
            wait(NULL);
        }

        // Проверяем, не ввели ли мы команду exit
        if (strcmp(command, "exit") == 0) {
            break;
        }
    }

    return 0;
}

