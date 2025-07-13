#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>

int main() {
    int pipe1[2]; // Parent to Child
    int pipe2[2]; // Child to Parent

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        // ----- Child process -----
        close(pipe1[1]); // Close write end (parent-to-child)
        close(pipe2[0]); // Close read end (child-to-parent)

        char buf[1];
        read(pipe1[0], buf, 1); // Receive signal from parent

        struct timespec end;
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Send end timestamp back to parent
        write(pipe2[1], &end, sizeof(end));

        close(pipe1[0]);
        close(pipe2[1]);
    } else {
        // ----- Parent process -----
        close(pipe1[0]); // Close read end
        close(pipe2[1]); // Close write end

        struct timespec start;
        clock_gettime(CLOCK_MONOTONIC, &start);

        write(pipe1[1], "X", 1); // Send to child

        struct timespec end;
        read(pipe2[0], &end, sizeof(end)); // Get child's timestamp

        long latency_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        printf("IPC latency (pipe, parent → child): %ld ns\n", latency_ns);

        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL); // Wait for child to finish
    }

    return 0;
}
