#include <stdio.h>
#include <string.h>

#define MAX_INPUT_LENGTH 512

void ReadingUserInput(){
    char UserInput[MAX_INPUT_LENGTH + 1];
    char* token;
    char* tokenArray[256];

do {
    printf(">");
    if (fgets(UserInput, sizeof(UserInput), stdin) == NULL) {
        printf("\nExiting...\n");
        break;
    }
    
    //removes newline caused by hitting <enter> 
    int len = strlen(UserInput);
    if(len > 0 && UserInput[len -1] == '\n'){
        UserInput[len - 1] = '\0';
    }

    

    //parses a line of user input
    token = strtok(UserInput, " \t|<>;&");
    if (token == NULL){
        tokenArray[0] = "";
        continue;
        }
    tokenArray[0] = token;
     int i = 1;
    while(token != NULL){
    printf("\"%s\" \n", token);
    tokenArray[i] = token;
    i++; 
    token = strtok(NULL, " \t|<>;&");
}    



//repeats the above lines until the first 'token' in an inputted line is exit    
} while(strcmp("exit", tokenArray[0]) != 0);
 printf("\nExiting...\n");
}


int main(){
    ReadingUserInput();
    return 0;
}
