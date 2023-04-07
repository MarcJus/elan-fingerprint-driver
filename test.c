//
// Created by marc on 07/03/23.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define NOP asm("nop");
#define CHECK(value, action) if(value < 0){action}

int main(void){
    int fd = open("/dev/fingerprint", O_RDONLY);
    CHECK(fd, perror("Erreur open"); exit(EXIT_FAILURE);)

    char *buffer = malloc(64);
    read(fd, buffer, 64);

    NOP

    free(buffer);
    close(fd);

    return 0;
}