#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for(unsigned int i = 0; i < N; i++) {
		if(strcmp(cmd, allowed[i]) == 0) return 1;
	}
	
	return 0;
}

int findCMD(const char* cmd) {
	for (unsigned int i = 0; i < N; i++) {
		if(strcmp(cmd, allowed[i]) == 0) return i;
	}
	return -1;
}

int main() {

    // TODO
    // Add variables as needed
	char* argv[21];
	char cmd[10];
	char* token;
    char line[256];

	int index;

	pid_t pid;
	int status;
    posix_spawnattr_t attr;

	// Initialize spawn attributes
	posix_spawnattr_init(&attr);
	
    while (1) {
		//set all argv to NULL
		for (unsigned int i = 0; i < 21; i++) {
			argv[i] = NULL;
		}

		fprintf(stderr,"rsh>");
		
		if (fgets(line,256,stdin)==NULL) continue;
		
		if (strcmp(line,"\n")==0) continue;
		
		line[strlen(line)-1]='\0';
		
		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		// Use the example provided in myspawn.c
		
		//tokenize!!!
		token = strtok(line, " ");
		strcpy(cmd, token);
		token = strtok(NULL, " ");
		for (unsigned int i = 0; i < 20; i++) {
			if(token == NULL) break;
			argv[i] = malloc(strlen(token) + 1);
			strcpy(argv[i], token);
			token = strtok(NULL, " ");
		}
		
		//if allowed
		if(!isAllowed(cmd)) {
			printf("NOT ALLOWED!\n");
			continue;
		}

		
		
		//run cmd
		index = findCMD(cmd);
		switch(index){
			// first 9 commands
			case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
				// spawn new process
				if (posix_spawnp(&pid, cmd, NULL, &attr, argv, environ) != 0) {
					perror("spawn failed");
					exit(EXIT_FAILURE);
				}
			
				// Wait for the spawned process to terminate
				if (waitpid(pid, &status, 0) == -1) {
					perror("waitpid failed");
					exit(EXIT_FAILURE);
				}
				break;
			//cd
            case 9: 
				if (argv[1] != NULL) {
					printf("-rsh: cd: too many arguments\n");
					continue;
				}
				chdir(argv[0]);
				break;
			//exit
			case 10: 
				// Destroy spawn attributes
				posix_spawnattr_destroy(&attr);
				return 0;
			//help
			case 11:
				printf("The allowed commands are:\n"); 
				for (unsigned int i = 0; i < N; i++) {
					printf("%u: %s\n", i, allowed[i]);
				}
				break;
            default: printf("ummm uhhh umm what T.T\n");
        }

    }
    return 0;
}

