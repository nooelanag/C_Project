#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "menu.h"
#include "item.h"
#include "collection.h"
#include "errors.h"
pid_t* pid_child;
int pos_pid;
int finish_while_loop = 0;
void handler(int signal) {
	if(signal == SIGALRM)
 		kill(pid_child[pos_pid], SIGSTOP);
	if(signal == SIGINT)
		finish_while_loop = 1;
}
void print_execution_summary(struct collection col, int* pid_child, int* num_exec, int* child_status) {
	printf("Execution summary: \n");
	printf("%5s\t%5s\t%10s\t%5s\t%5s\n", "PID", "real PID", "STAT", "NUM-EXEC", "END-STATUS");
	for(int i = 0; i < col.number_occupied; i++) {
		printf("%5d\t%8d\t", col.schedule[i].pid, pid_child[i]);
		if(col.schedule[i].data.which == READY)
			printf("%10s\t", "READY");
		else if(col.schedule[i].data.which == RUNNING)
			printf("%10s\t", "RUNNING");
		else
			printf("%10s\t", "TERMINATED");
		printf("%8d\t%10d\n", num_exec[i], child_status[i]);
	}
}
void execute_schedule(struct collection col, int* error) {
	int status;
	int* num_exec;
	int* child_status;
	char* arg_1;
	char* arg_2;
	int elements_read;
	pid_child = (pid_t*) malloc(sizeof(pid_t) * col.number_occupied);
	num_exec = (int*) calloc(col.number_occupied, sizeof(int));
	child_status = (int*) calloc(col.number_occupied, sizeof(int));
	print_all(col, error);
	for(int i = 0; i < col.number_occupied; i++) {
		if((pid_child[i] = fork()) < 0) {
			*error = ERR_FORK_FAILED;
			for(int j = 0; j < i; j++) {
				kill(pid_child[j], SIGKILL);
			}
			return;				
		}
		else if(pid_child[i] == 0) {
			arg_1 = (char*) calloc(1, sizeof(int));
			arg_2 = (char*) calloc(1, sizeof(int));
			elements_read = sprintf(arg_1, "%d", col.schedule[i].pid);
			if(elements_read < 0) {
				*error = ERR_CONVERTION_INTEGER_TO_STRING_FAILED;
				free(arg_1);
				free(arg_2);
				for(int j = 0; j < i; j++) {
					kill(pid_child[j], SIGKILL);
				}
				return;
			}
			elements_read = sprintf(arg_2, "%d", col.schedule[i].data.p);
			if(elements_read < 0) {
				*error = ERR_CONVERTION_INTEGER_TO_STRING_FAILED;
				free(arg_1);
				free(arg_2);
				for(int j = 0; j < i; j++) {
					kill(pid_child[j], SIGKILL);
				}
				return;
			}
			*error = execl("./process", "./process", arg_1, arg_2, (char*) NULL);
			if(*error < 0) {
				*error = ERR_EXECUTION_FAILED;
				free(arg_1);
				free(arg_2);
				for(int j = 0; j < i; j++) {
					kill(pid_child[j], SIGKILL);
				}
				return;
			}
			free(arg_1);
			free(arg_2);
		}
	}
	printf("I launched the child processes.\n");
	signal(SIGINT, handler);
	signal(SIGALRM, handler);
	for(pos_pid = 0; pos_pid < col.number_occupied; pos_pid++) {
		kill(pid_child[pos_pid], SIGSTOP);
	}
	for(; finish_while_loop == 0;) {
		for(pos_pid = 0; pos_pid < col.number_occupied; pos_pid++) {
			kill(pid_child[pos_pid], SIGCONT);
			col.schedule[pos_pid].data.which = RUNNING;
			num_exec[pos_pid]++;
			alarm(1);
			pause();
			if(finish_while_loop == 1)
				break;
			col.schedule[pos_pid].data.which = READY;
		}
	}
	for(pos_pid = 0; pos_pid < col.number_occupied; pos_pid++) {
		kill(pid_child[pos_pid], SIGKILL);
		wait(&status);
		child_status[pos_pid] = status;
	}
	printf("\nI have suspended all my children.\n");
	signal(SIGALRM, SIG_IGN);
	print_execution_summary(col, pid_child, num_exec, child_status);
	free(num_exec);
	free(child_status);
	free(pid_child);
}
