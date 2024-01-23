void display_prompt();

int main(){
	
	//Find the user home directory from the environment (3)
	
	//Set current working directory to user home directory (3)
	
	//Save the current path (3)
	
	//Load history (6)
	
	//Load aliases (8)
	
	//Do while shell has not terminated
	while(1){
		//Display prompt (1)
		display_prompt();
		//Read and parse user input (1)
		
		//While the command is a history invocation or alias then replace it 
		//with the appropriate command from history or the aliased command 
		//respectively (5 & 7)
		
		//If command is built-in invoke appropriate function (1-5, 7)
		
		//Else execute command as an external process (2)
	}
	// End while (okay yes this comment is pointless)
	
	//From here down *could* be a separate exit function, depends on your logic
	//Save history (6)
	
	//Save aliases (8)
	
	//Restore original path (3)
	
	//Exit
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
