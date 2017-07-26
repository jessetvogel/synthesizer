#include "interface.hpp"
#include <unistd.h>
#include <stdio.h>
#include <iostream>

#define READ_FD  (0)
#define WRITE_FD (1)

#define PARENT_WRITE_PIPE  (0)
#define PARENT_READ_PIPE   (1)

#define PARENT_READ_FD  ( pipes[PARENT_READ_PIPE][READ_FD]   )
#define PARENT_WRITE_FD ( pipes[PARENT_WRITE_PIPE][WRITE_FD] )

#define CHILD_READ_FD   ( pipes[PARENT_WRITE_PIPE][READ_FD]  )
#define CHILD_WRITE_FD  ( pipes[PARENT_READ_PIPE][WRITE_FD]  )

#define INTERFACE_BUFFER_SIZE (1024)

Interface::Interface() {

}

bool Interface::start() {
    // pipes for parent to write and read
    pipe(pipes[PARENT_READ_PIPE]);
    pipe(pipes[PARENT_WRITE_PIPE]);
    
    if(!fork()) {
        child = true;
        
        // Child process
        dup2(CHILD_READ_FD, STDIN_FILENO);
        dup2(CHILD_WRITE_FD, STDOUT_FILENO);
        
        close(CHILD_READ_FD);
        close(CHILD_WRITE_FD);
        close(PARENT_READ_FD);
        close(PARENT_WRITE_FD);
        
        char command[128];
        char argument[128];
        strncpy(command, "./synthesizer", 128);
        strncpy(argument, "../settings/settings", 128);
        
        char* argv[3];
        argv[0] = command;
        argv[1] = argument;
        argv[2] = NULL;
        
        execv(argv[0], argv);
    } else {
        // Parent process
        child = false;
        
        // close file descriptors not required by parent
        close(CHILD_READ_FD);
        close(CHILD_WRITE_FD);
        
        // Read from child's stdout
        std::string line;
        line = readLine();
//        std::cout << line << std::endl; TODO wat doen we hiermee?
    }
    return true;
}

bool Interface::stop() {
    command("exit"); // TODO triggers SIGPIPE when already closed sometimes
    return true;
}

std::string Interface::command(std::string command) {
    mutex.lock();
    ::write(PARENT_WRITE_FD, command.c_str(), command.length());
    ::write(PARENT_WRITE_FD, "\n", 1);
    std::string line = readLine();
    mutex.unlock();
    return line;
}

std::string Interface::readLine() {
    char buffer[INTERFACE_BUFFER_SIZE];
    char* ptr = buffer;
    ssize_t bytes;
    do bytes = ::read(PARENT_READ_FD, ptr, 1);
    while(bytes > 0 && *(ptr++) != '\n');
    long length = ptr - buffer - 1;
    if(length < 0) length = 0;
    return std::string(buffer, length);
}
