#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT 100
#define MAX_ARGS 10

void parse_command(char *command, char **args) {
    int i = 0;
    args[i] = strtok(command, " ");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        args[++i] = strtok(NULL, " ");
    }
    args[i] = NULL;
}

int main() {
    char command[MAX_INPUT];
    char *args[MAX_ARGS];
    char *args_pipe[MAX_ARGS];
    pid_t pid;
    int fd[2];

    while (1) {
        printf("Введите команду: ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Ошибка чтения команды");
            continue;
        }
        command[strcspn(command, "\n")] = 0; // Убираем символ новой строки

        //проверка выхода из команды
        if (strcmp(command, "exit") == 0) {
            break;
        }

        //проверка команды на символ |
        char *pipe_pos = strchr(command, '|');
        if (pipe_pos != NULL) {
            // Разделяем команду на две части
            *pipe_pos = 0;
            pipe_pos++;
            while (*pipe_pos == ' ') pipe_pos++; // Убираем пробелы после |

            //парсим команды до и после |
            parse_command(command, args);
            parse_command(pipe_pos, args_pipe);

            //создание канала
            if (pipe(fd) == -1) {
                perror("Ошибка создания канала");
                continue;
            }

            //создание первого дочернего для первой команды
            if ((pid = fork()) == 0) {
                close(fd[0]); //закрытие неиспользуемого конеца канала
                dup2(fd[1], STDOUT_FILENO); //перенаправление stdout в канал
                close(fd[1]);
                execvp(args[0], args);
                perror("Ошибка выполнения команды");
                exit(EXIT_FAILURE);
            } else if (pid < 0) {
                perror("Ошибка создания процесса");
                continue;
            }

            //создается второй дочерний для второй команды
            if ((pid = fork()) == 0) {
                close(fd[1]); //закрытие неиспользуемого конеца канала
                dup2(fd[0], STDIN_FILENO); //перенаправление stdin из канала
                close(fd[0]);
                execvp(args_pipe[0], args_pipe);
                perror("Ошибка выполнения команды");
                exit(EXIT_FAILURE);
            } else if (pid < 0) {
                perror("Ошибка создания процесса");
                continue;
            }

            //закрытие обоих сторон канала в родителе
            close(fd[0]);
            close(fd[1]);

            //ожидание завершение обоих дочерних
            wait(NULL);
            wait(NULL);
        } else {
            //парсим обычную команду без |
            parse_command(command, args);

            pid = fork();
            if (pid == 0) {
                //если дочерний, выполним введенную программу
                execvp(args[0], args);
                perror("Ошибка выполнения команды");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                //если родительский, ожижаем завершения дочернего
                wait(NULL);
            } else {
                perror("Ошибка создания процесса");
            }
        }
    }

    return 0;
}
