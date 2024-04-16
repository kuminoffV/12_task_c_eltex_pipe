#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>  // Объявление функции wait

int main() {
    int pipe_fds[2];  // Дескрипторы файловых потоков для канала

    // Создание неименованного канала
    if (pipe(pipe_fds) < 0) {
        perror("pipe");
        exit(1);
    }

    int pid = fork();  // Создание дочернего процесса
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс
        close(pipe_fds[1]);  // Закрыть дескриптор для записи

        char message[1024];
        if (read(pipe_fds[0], message, sizeof(message)) == -1) {
            perror("read");
            exit(1);
        }

        printf("Полученное сообщение: %s\n", message);

        close(pipe_fds[0]);  // Закрыть дескриптор для чтения
    } else {
        // Родительский процесс
        close(pipe_fds[0]);  // Закрыть дескриптор для чтения

        char message[] = "Hi!";  // Сообщение для дочернего процесса
        if (write(pipe_fds[1], message, strlen(message) + 1) == -1) {
            perror("write");
            exit(1);
        }

        wait(NULL);  // Ожидать завершения дочернего процесса

        close(pipe_fds[1]);  // Закрыть дескриптор для записи
    }

    return 0;
}

