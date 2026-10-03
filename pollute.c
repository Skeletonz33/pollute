/*
 * Pollute - pollute.c
 * Created on 21st September 2026
 * 
 * Copyright (c) 2026 Skeletonz33
 */

#include <stdio.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_SIZE                                        1 << 24

const char template[] = "/*\n * %s - %s\n * Created on %s %s %s\n * \n * Copyright (c) %s %s\n */\n\n";

void readWholeFile(int fd, void* buffer) {
    struct stat stats;
    if(fstat(fd, &stats) == -1) {
        printf("pollute: error: failed to get stats\n");
        exit(errno);
    }

    int size = (int)stats.st_size;
    int bytesRead = 0;

    while(bytesRead < size) {
        int res = read(fd, (char*)buffer + bytesRead, size - bytesRead);
        if(res <= 0) {
            printf("pollute: error: failed to read whole file\n");
            exit(errno);
        }

        bytesRead += res;
    }
}

void prependFile(const char* path, const char* comment) {
    int fd = open(path, O_RDWR | O_BINARY);
    if(fd < 0) {
        return;
    }

    struct stat stats;
    if(fstat(fd, &stats) == -1) {
        printf("pollute: error: failed to get stats\n");
        exit(errno);
    }

    int size = (int)stats.st_size;
    if(size > MAX_SIZE) {
        printf("pollute: error: file size exceeds maximum of %u bytes\n", (unsigned int)MAX_SIZE);
        exit(0x1000);
    }

    void* buffer = malloc((int)size);
    if(buffer == NULL) {
        printf("pollute: error: failed to allocate buffer\n");
        exit(errno);
    }

    readWholeFile(fd, buffer);

    lseek(fd, 0, SEEK_SET);
    if(write(fd, comment, strlen(comment)) < strlen(comment)) {
        printf("pollute: error: failed to write comment\n");
        exit(errno);
    }

    if(write(fd, buffer, (unsigned int)size) < size) {
        printf("pollute: error: failed to write rest of file\n");
        exit(errno);
    }

    free(buffer);
    close(fd);
}

int main(int argc, char** argv) {
    if(argc < 7) {
        printf("pollute: usage: pollute file project author year month date\n");
        return 0;
    }

    int totalLength = sizeof(template);
    for(int i = 1;i < 7;i++) {
        totalLength += strlen(argv[i]);
    }

    const char* fileName = argv[1];
    const char* project = argv[2];
    const char* author = argv[3];
    const char* year = argv[4];
    const char* month = argv[5];
    const char* date = argv[6];

    char* buffer = malloc(totalLength + 1);
    if(buffer == NULL) {
        printf("pollute: error: failed to allocate buffer\n");
        return errno;
    }

    buffer[totalLength] = 0;

    snprintf(buffer, totalLength, template, project, fileName, date, month, year, year, author);
    prependFile(fileName, buffer);

    return 0;
}