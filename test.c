//
// Created by marc on 07/03/23.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#define NOP asm("nop");
#define CHECK(value, action) if(value < 0){action}

int fd;

void sighandler(int sig){
    printf("Alarm\n");
    close(fd);
}

int main(void){
    fd = open("/dev/fingerprint", O_RDONLY);
    CHECK(fd, perror("Erreur open"); exit(EXIT_FAILURE);)

    char buffer[20];
    ssize_t bytes_read = read(fd, buffer, 20);
    if(bytes_read > 0){
        printf("%s\n", buffer);
    }
    close(fd);

    return 0;
}