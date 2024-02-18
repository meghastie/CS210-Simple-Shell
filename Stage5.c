#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_INPUT_LENGTH 512
char* tokenArray[50];
char current_path[256];
char original_path[256];
char* commandHistory[20];
int count = 0; //keeps track of next avalible position in circular array

// Function prototypes
void ExecuteExternalProcess();
void ReadingUserInput();
void display_prompt();
void getPath();
void setPath(char* newPath);
void cd(char *path);
void addHistory(char* command);



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
        args[49] = NULL;  // Ensure argument array ends with NULL      
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

void cd(char *path) {
    if(path == NULL) {
        fprintf(stderr, "Usage: cd <directory>\n");
    } else {
        if(chdir(path) != 0) {
            perror("cd");
        }
    }
}

void getPath() {
 printf("CURRENT PATH : %s\n", getenv("PATH"));
}

void setPath(char* newPath) { 
 if (newPath != NULL) {
    setenv("PATH",newPath, 1);
    printf("PATH SET TO : %s\n", getenv("PATH"));
 } 
 
}

void addHistory(char* command) {
    if (count == 20) {
      for (int i = 1; i < 20; i++){
        commandHistory[i-1] = commandHistory[i]; //when array is full shifts elements left by one position
      } 
      count--; //count decremented to ensure it always stays in range   
    }
    commandHistory[count] = strdup(command); 
    count++;
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

        addHistory(tokenArray[0]);
        //for (int i = 0; i < 20; i++) {
        //printf("%s\n", commandHistory[i]);
        //}
        //above used to test that circular history array works correctly

        if ((strcmp("getpath", tokenArray[0]) == 0)) {
            getPath();
        }
        else if ((strcmp("setpath", tokenArray[0])) == 0) {
            char* newPath = tokenArray[2];
            setPath(newPath);
        }
        else if(strcmp("cd", tokenArray[0]) == 0) {
            char* newDirectory = tokenArray[2];
            cd(newDirectory);
        }
        else {
            ExecuteExternalProcess(); 
            }
        // Repeats the above lines until the first 'token' in an inputted line is "exit"    
    } while(strcmp("exit", tokenArray[0]));
    setPath(original_path);
    printf("\nExiting...\n");
    exit(1);
}

int main(){
    // Find the user home directory from the environment (3)
    char *home_directory = getenv("HOME");

    // Set current working directory to user home directory (3)
    chdir(home_directory);

    // Save the current path (3)
    getcwd(current_path, sizeof(current_path));
    strncpy(original_path, current_path, sizeof(current_path) - 1);
    
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
    home_directory = getenv("HOME");
    chdir(home_directory);

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
