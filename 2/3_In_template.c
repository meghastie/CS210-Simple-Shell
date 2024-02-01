#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_INPUT_LENGTH 512
char* tokenArray[50];

// Function prototypes
void ExecuteExternalProcess();
void ReadingUserInput();
void display_prompt();

int main(){
    // Find the user home directory from the environment (3)
    char *home_directory = getenv("HOME");

    // Set current working directory to user home directory (3)
    chdir(home_directory);

    // Save the current path (3)
    char current_path[256];
    getcwd(current_path, sizeof(current_path));

    // Load history (6)
    // Load aliases (8)

    // Do while shell has not terminated
    while(1){
        // Display prompt (1)
        display_prompt();

        // Read and parse user input (1)
        ReadingUserInput();

        // While the command is a history invocation or alias then replace it 
        // with the appropriate command from history or the aliased command 
        // respectively (5 & 7)

        // If command is built-in invoke appropriate function (1-5, 7)

        // Else execute command as an external process (2)
    }
    // End while (okay yes this comment is pointless)

    // From here down *could* be a separate exit function, depends on your logic
    // Save history (6)
    // Save aliases (8)

    // Restore original path (3)
    chdir(current_path);

    // Exit
    return 0;
}

/*
    Displays the shell prompt. Should make prompt constant?
    inputs: n/a
    outputs: n/a
    side effects: prints prompt to stdout
    written by Andrew, Fred and John
*/
void display_prompt(){
    printf("This is the greatest and best shell in the world >>>>> ");
}

void ExecuteExternalProcess(){
    pid_t forkValue = fork();
    
    // Child process' behavior
    if(forkValue == 0){
        char* args[50];
        
        // Fill array with arguments
        for(int i = 0; i < 49 && tokenArray[i] != NULL; i++){
            args[i] = tokenArray[i+1];
        }
        args[50] = NULL;  // Ensure argument array ends with NULL      
        char* command = tokenArray[0];  // Store the first token

        // Search along the path for the command and execute with arguments, dealing with any errors
        int executionStatus = execvp(command, args);   
        if(executionStatus == -1){
            perror(command);
        }
        exit(1);
    }
    else if(forkValue > 0){  // Parent process waits for the child process to finish
        wait(NULL);
    }
    else{
        perror("fork has failed");
        exit(1);
    }
}

void ReadingUserInput(){
    char userInput[MAX_INPUT_LENGTH + 1];
    char* token;

    do{
        // Prompt user for input & read input
        printf(">");
        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            break;
        }

        // Removes newline caused by hitting <enter> 
        int len = strlen(userInput);
        if(len > 0 && userInput[len -1] == '\n'){
            userInput[len - 1] = '\0';
        }

        // Clear tokenArray    
        for(int i = 0; i < 50; i++ && tokenArray[i] != NULL){
            tokenArray[i] = NULL;
        } 

        // Parses a line of user input into tokens, storing each token in an array
        token = strtok(userInput, " \t|<>;&");

        // If no user input, ensure array is not NULL and jump to while condition
        if (token == NULL){
            tokenArray[0] = "";
            continue;
        }

        // Fill tokenArray with tokens printing each to the display
        tokenArray[0] = token;
        int i = 1;
        while(token != NULL){
            printf("\"%s\" \n", token);
            tokenArray[i] = token;
            i++; 
            token = strtok(NULL, " \t|<>;&");
        }

        ExecuteExternalProcess();

        // Repeats the above lines until the first 'token' in an inputted line is "exit"    
    } while(strcmp("exit", tokenArray[0]));

    printf("\nExiting...\n");
    exit(1);
}
