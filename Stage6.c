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
char original_path[256];
char* commandHistory[20];
int count = 0; //keeps track of next avalible position in circular array

// Function prototypes
void ExecuteExternalProcess();
char* ReadUserInput();
void display_prompt();
void ParseString(char* userInput);
void getPath();
void setPath(char* newPath);
void cd(char *path);
void addHistory(char* command);
int isValidInteger(char *str, int max);
void saveHist();
void loadHist();



void display_prompt(){
    printf("This is the greatest and best shell in the world >>>>> ");
}


char* ReadUserInput(){
char userInput[MAX_INPUT_LENGTH + 1];  

    // Prompt user for input & read input
    if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
        return NULL;
    }

    // Removes newline caused by hitting <enter> 
    int len = strlen(userInput);
    if(len > 0 && userInput[len -1] == '\n'){
        userInput[len - 1] = '\0';
    }
    
    // Returns a copy of the user's input
    char* userInputCopy = strdup(userInput);  
    return userInputCopy;
}


void ParseString(char* userInput){
    
    // Clears tokenArray    
    for(int i = 0; i < 50 && tokenArray[i] != NULL; i++){
        tokenArray[i] = NULL;
    } 
         
    // Parses a line of user input into tokens, storing each token in an array
    char* token = strtok(userInput, " \t|<>;&");
    
    // If no user input, skip parsing
    if (token == NULL){
        return;
    }

    // Fill tokenArray with tokens printing each to the display
    int i = 0;
    while(token != NULL){
        printf("\"%s\" \n", token);
        tokenArray[i] = token;
        i++; 
        token = strtok(NULL, " \t|<>;&");
    }
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
        exit(0);
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
    free(commandHistory[0]);
      for (int i = 1; i < 20; i++){
        commandHistory[i-1] = commandHistory[i]; //when array is full shifts elements left by one position
      } 
       count--; //count decremented to ensure it always stays in range  
    }
    
    commandHistory[count] = strdup(command); 
    count++;
}


int isValidInteger(char *str, int max) {
    char *endptr;
    long value = strtol(str, &endptr, 10);
   
    // Checks that conversion is successful and within range
    if (*str != '\0' && *endptr == '\0' && value >= 1 && value <= max) {
        return 1;  
    } 
    else {
        return 0;  
    }
}

void saveHist() {
    FILE* fptr = fopen(".hist_list.txt", "w");
    
    if (fptr == NULL ) {
        perror("Unable to find file");
        return;
    }
    else {

    for (int i = 0; i < count; i++) {
        fprintf(fptr, "%s\n", commandHistory[i]);
        }
    }

    fclose(fptr);
}
    
void loadHist() {

    char line[512];
    char* token;

    FILE* fptr = fopen(".hist_list.txt", "r");
    if (fptr == NULL ) {
        perror("Unable to find file");
        return;
    }
    else {
        while (fgets(line, sizeof(line), fptr) != NULL) {
            token = strtok(line, " ");
            if (token != NULL) {
                token = strtok(NULL, " \n");
                addHistory(token);
            }
        }
    }

fclose(fptr);
}

int main(){
    // Find the user home directory from the environment (3)
    // Set current working directory to user home directory (3)
    char *home_directory = getenv("HOME");
    chdir(home_directory);

    char current_path[256];
    getcwd(current_path, sizeof(current_path));
    strncpy(original_path, current_path, sizeof(current_path) - 1);
    
    // Load history (6)
    loadHist();
    // Load aliases (8)

    // Do while shell has not terminated
    do{
        // Display prompt (1)
        display_prompt();
        
        // Read user's input, if <ctrl> D terminate shell
        char* userInput = ReadUserInput();
        if(userInput == NULL){
            break;
        }

        char tokenisedUserInput[MAX_INPUT_LENGTH + 1];
        strcpy(tokenisedUserInput, userInput);  

        // Parse user's input, if no input (i.e. <enter>) jump to end of do while loop
        ParseString(tokenisedUserInput);
        if(tokenArray[0] == NULL){
            tokenArray[0] = "";
            continue;
        }

        // If not a history invocation add user's input to history 
         if(strncmp(tokenArray[0], "!", 1) != 0){
            addHistory(userInput);
         }
        
        // Execute previous command again
        if(strncmp(tokenArray[0], "!!", 2) == 0){
            ParseString(commandHistory[count-1]);
        }
        else if (strncmp(tokenArray[0], "!-", 2) == 0 && tokenArray[1] != NULL){
            if(isValidInteger(tokenArray[1], count)){
                int historyNumber = atoi(tokenArray[1]);
                ParseString(commandHistory[count-historyNumber]);
            }
            else{
                if(count>0){
                printf("Invalid history number. Please enter an integer within range 1-%d. \n", count);
                continue;
                }
                else{
                    printf("Invalid history number. History is empty.\n");
                    continue;
                }
            }           
            
        }
        else if (strncmp(tokenArray[0], "!", 1) == 0 && tokenArray[1] != NULL){
            if(isValidInteger(tokenArray[1], count)){
                int historyNumber = atoi(tokenArray[1]);
                ParseString(commandHistory[historyNumber-1]);
            } 
            else{
                if(count>0){
                printf("Invalid history number. Please enter an integer within range 1-%d. \n", count);
                continue;
                }
                else{
                    printf("Invalid history number. History is empty.\n");
                    continue;
                }
            }   
        }

        
        
            
    
        // While the command is a history invocation or alias then replace it 
        // with the appropriate command from history or the aliased command 
        // respectively (5 & 7)

        // If command is built-in invoke appropriate function (1-5, 7)
        if ((strcmp("getpath", tokenArray[0]) == 0)) {
        	if (tokenArray[1] != NULL) {
        		printf("Error: getpath does not accept any parameters\n");
                } else {
            		getPath();
            }
        }
        else if ((strcmp("setpath", tokenArray[0])) == 0) {
            if (tokenArray[1] == NULL) {
            		printf("Error: setpath needs a parameter\n");
                } else {
            		char* newPath = tokenArray[1];
            		setPath(newPath);
        }
        }
        else if(strcmp("cd", tokenArray[0]) == 0) {
            char* newDirectory = tokenArray[1];
            cd(newDirectory);
        }
        else if(strcmp("history", tokenArray[0]) == 0){
            for(int i = 0; i < count; i++){
                if(i == count-1 && strcmp(userInput, commandHistory[i]) == 0){
                    continue;
                }
                printf("%d %s\n", i+1, commandHistory[i]);
            } 
        }
        
        // Else execute command as an external process (2)
        else {
            ExecuteExternalProcess(); 
            }
    
    // Repeats the above lines until the first 'token' in an inputted line is "exit"
    }while(strcmp("exit", tokenArray[0]));
    setPath(original_path);
    printf("\nExiting...\n");

    // From here down *could* be a separate exit function, depends on your logic
    // Save history (6)
    saveHist();
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













