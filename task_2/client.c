#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    // Создание именованного канала
    const char *fifo_name = "/tmp/my_fifo";

    // Открытие канала только для чтения
    int fd = open(fifo_name, O_RDONLY);

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Чтение сообщения из канала
    char message[1024];

    if (read(fd, message, sizeof(message)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Вывод сообщения на экран
    printf("Полученное сообщение: %s\n", message);

    // Закрытие канала
    close(fd);

    return 0;
}

