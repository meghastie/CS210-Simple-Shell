#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_INPUT_LENGTH 512
char* tokenArray[50];



void ExecuteExternalProcess(){
    pid_t forkValue = fork();
    
    
    //Child process' behaviour
    if(forkValue == 0){
        char* args[50];
       //fill array with arguments
       for(int i = 0; i < 49 && tokenArray[i] != NULL; i++){
        args[i] = tokenArray[i+1];
       }
       args[50] = NULL;    //Ensure argument array ends with NULL      
       char* command = tokenArray[0];   //store first token


       //Search along path for command and execute with arguments, dealing with any errors
       int executionStatus = execvp(command, args);   
       if(executionStatus == -1){
        perror(command);
        }
        exit(1);
    }
    
    
    else if(forkValue > 0){   //Parent process waits for child process to finish
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
        //Prompt user for input & read input
        printf(">");
        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            break;
        }


        //removes newline caused by hitting <enter> 
        int len = strlen(userInput);
        if(len > 0 && userInput[len -1] == '\n'){
            userInput[len - 1] = '\0';
        }


        //Clear tokenArray    
        for(int i = 0; i < 50; i++ && tokenArray[i] != NULL){
            tokenArray[i] = NULL;
        } 


        //parses a line of user input into tokens, storing each token in an array
        token = strtok(userInput, " \t|<>;&");
    
    
        //If no user input, ensure array is not NULL and jump to while condition
        if (token == NULL){
            tokenArray[0] = "";
            continue;
        }

    
        //fill tokenArray with tokens printing each to the display
        tokenArray[0] = token;
        int i = 1;
        while(token != NULL){
            printf("\"%s\" \n", token);
            tokenArray[i] = token;
            i++; 
            token = strtok(NULL, " \t|<>;&");
        }
    
    
        ExecuteExternalProcess();
    
    
    //repeats the above lines until the first 'token' in an inputted line is exit    
    } while(strcmp("exit", tokenArray[0]));
    
    
    printf("\nExiting...\n");
}
    
    
int main(){
    ReadingUserInput();
    
    return 0;
}
