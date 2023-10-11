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
#include "input_usr.h"
#include "errors.h"
#include "execute_schedule.h"
#define EXIT 0
#define ADDING_PROCESS 1
#define DELETING_PROCESS 2
#define INFO_PROCESS 3
#define INFO_ALL_PROCESSES 4
#define DELETING_SCHEDULE 5
#define SORTING 6
#define UPLOAD_FILE 7
#define SAVE_FILE 8
#define EXECUTE_SCHEDULE 9
#define HELP 10
int main(int argc, char *argv[]) {
	int menu_option, error, priority, execution_time, pid, index, yes_no_number, sorting_option, elements_read, counter;
	char* yes_no;
	char* file_name;
	char* file_name_2;
	char* owner;
	char* aux_owner;
	char character;
	FILE* file;
	signal(SIGINT, SIG_DFL);
	struct collection col = initialize_collection(&error);
	main_menu();
	do {
		printf("Please enter an option (help: 10): ");
		menu_option = get_integer(&error);
		switch(menu_option) {
		case ERR_CTRL_D: case EXIT:
			clean_collection(col);
			printf("\n");
			exit(EXIT_SUCCESS);
		case ADDING_PROCESS:
			printf("Adding a new process.\n");
			do {
				printf("Enter the name of the owner: ");
		    	owner = get_input(&error);
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				printf("\n");
				main_menu();				
				break;			
			}
			do {
				printf("Enter the priority of the process: ");
		    	priority = get_integer(&error);
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				free(owner);
				printf("\n");
				main_menu();				
				break;			
			}
			do {
				printf("Enter the execution time (C): ");
		    	execution_time = get_integer(&error);
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				free(owner);
				printf("\n");
				main_menu();				
				break;			
			}
		    if(find_item(col, owner, priority, execution_time, &error) == ERR_ELEMENT_ALREADY_COL){
		    	fprintf(stderr, "%s\n", get_error_msg_col(error));
				do {
					printf("Do you want to add it anyway? Answer Y or N: ");
					yes_no = get_input(&error);
					yes_no_number = check_yes_no(yes_no, &error);
					free(yes_no);
				} while(error != SUCCESS && error != ERR_CTRL_D);
				if(error == ERR_CTRL_D) {
					free(owner);
					printf("\n");
					main_menu();				
					break;			
				}	
		    	if(yes_no_number == 1) {
					free(owner);
		    		printf("Process not added.\n");
					break;
				}
		    }
		    col = add_item(col, owner,priority, execution_time, &error);
			if(error != SUCCESS) {
				free(owner);
				fprintf(stderr, "%s\n", get_error_msg_col(error));
				break;
			}
			printf("Process added to the schedule at position %d \n", col.number_occupied);
		    break;
		case DELETING_PROCESS:
			printf("Deleting a process.\n");
			do {
				printf("Enter the pid of the process to be deleted: ");
				pid = get_integer(&error);
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				printf("\n");
				main_menu();				
				break;			
			}
			index = find_item_pid(col, pid, &error);
			if(error != SUCCESS)
				fprintf(stderr, "%s\n", get_error_msg_col(error));
			else {
				col = delete_item(col, index, &error);
				printf("Process successfully deleted.\n");
			}
			break;
		case INFO_PROCESS:
			printf("Information of a process.\n");
			do {
				printf("Enter the pid of the process to be displayed: ");
				pid = get_integer(&error);
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				printf("\n");
				main_menu();				
				break;			
			}
			index = find_item_pid(col, pid, &error);
			if(error != SUCCESS) {
				fprintf(stderr, "%s\n", get_error_msg_col(error));
				break;
			}
			print_header();
			print_item(col.schedule[index]);
			break;
		case INFO_ALL_PROCESSES:
			printf("Showing all scheduling.\n");
			print_all(col, &error);
			if(error != SUCCESS)
				fprintf(stderr, "%s\n", get_error_msg_col(error));
			break;
		case DELETING_SCHEDULE:
			do {
				printf("Do you want to delete all information? [Y/N]: ");
				yes_no = get_input(&error);
				yes_no_number = check_yes_no(yes_no, &error);
				free(yes_no);
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				printf("\n");
				main_menu();				
				break;			
			}	
			if(yes_no_number == 0){
				col = clean_collection(col);
				printf("Deleting all the scheduling.\n");
			}
			else
				printf("Deletion aborted.\n");
			break;
		case SORTING:
			do {
				printf("Sorting the collection by PID(1), P(2), C(3), OWNER(4): ");
				sorting_option = get_integer(&error);
				check_sorting_option(sorting_option, &error);
				if(error != SUCCESS && error != ERR_CTRL_D) {
					fprintf(stderr, "%s\n", get_error_msg_col(error));
				}
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				printf("\n");
				main_menu();	
				break;
			}
			switch(sorting_option) {
				case 1:
					sort_collection(col, compare_pid, &error);
					if(error != SUCCESS)
						fprintf(stderr, "%s\n", get_error_msg_col(error));
					break;
				case 2:
					sort_collection(col, compare_P, &error);
					if(error != SUCCESS)
						fprintf(stderr, "%s\n", get_error_msg_col(error));
					break;
				case 3:
					sort_collection(col, compare_C, &error);
					if(error != SUCCESS)
						fprintf(stderr, "%s\n", get_error_msg_col(error));
					break;
				case 4:
					sort_collection(col, compare_user, &error);
					if(error != SUCCESS)
						fprintf(stderr, "%s\n", get_error_msg_col(error));
					break;
			}
			break;
		case UPLOAD_FILE:
			printf("Uploading a file to the schedule.\n");
			do {
				printf("Indicate the file name: ");
				file_name_2 = get_input(&error);
				if(error == ERR_CTRL_D) {
					break;			
				}
				file_name = strndup(file_name_2, strlen(file_name_2) - 1);
				free(file_name_2);
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				printf("\n");
				main_menu();				
				break;			
			}
			else if(access(file_name, F_OK) != 0) {
				free(file_name);
				error = ERR_FILE_NOT_EXIST_OR_NOT_READ;
				fprintf(stderr, "%s\n", get_error_msg_col(error));
				break;
			}
			else {
				if(access(file_name, R_OK) != 0) {
					free(file_name);
					error = ERR_FILE_NOT_EXIST_OR_NOT_READ;
					fprintf(stderr, "%s\n", get_error_msg_col(error));
					break;
				}
			}
			if(col.number_occupied != 0) {
				printf("The current schedule is not empty.\n");
				do {
					printf("Do you want to continue? Answer yes to delete it (Y/N): ");
					yes_no = get_input(&error);
					yes_no_number = check_yes_no(yes_no, &error);
					free(yes_no);
				} while(error != SUCCESS && error != ERR_CTRL_D);
				if(error == ERR_CTRL_D) {
					free(file_name);
					printf("\n");
					main_menu();				
					break;			
				}
				if(yes_no_number == 0) {
					col = clean_collection(col);
					pid = 1;
					printf("Current schedule deleted.\n");
				}
				else {
					free(file_name);
					break;
				}
			}
			col = initialize_collection(&error);
			file = fopen(file_name, "r+");
			free(file_name);
			if (file == NULL) {
				perror("Error when opening file for reading.\n");
				break;
			}
			while(feof(file) == 0) {
				owner = (char*) calloc(1, sizeof(char));
				counter = 0;
				elements_read = fread(&priority, sizeof(int), 1, file);
				if(feof(file) != 0)
					break;
				if(elements_read != 1) {
					perror("Error when writing the file");
					break;
				}
				elements_read = fread(&execution_time, sizeof(int), 1, file);
				if(elements_read != 1) {
					perror("Error when writing the file");
					break;
				}
				do {
					elements_read = fread(&character, sizeof(char), 1, file);
					if(elements_read != 1) {
						perror("Error when writing the file");
						break;
					}
					aux_owner = (char*) calloc(counter + 2, sizeof(char));
					strcpy(aux_owner, owner);
					free(owner);
					owner = aux_owner;
					owner[counter] = character;
					counter++;
				} while(character != '\n');
				col = add_item(col, owner, priority, execution_time, &error);
			}
			free(owner);
			if (fclose(file) != 0) {
				fprintf(stderr, "Error when closing file.\n");
				break;
			}
			break;
		case SAVE_FILE:
			printf("Saving the schedule to file.\n");
			do {
				printf("Indicate the file name: ");
				file_name_2 = get_input(&error);
				if(error == ERR_CTRL_D) {
					break;			
				}
				file_name = strndup(file_name_2, strlen(file_name_2) - 1);
				free(file_name_2);
			} while(error != SUCCESS && error != ERR_CTRL_D);
			if(error == ERR_CTRL_D) {
				printf("\n");
				main_menu();				
				break;			
			}
			if(access(file_name, F_OK) == 0) {
				if(access(file_name, W_OK) != 0) {
					error = ERR_FILE_NOT_WRITABLE;
					fprintf(stderr, "%s\n", get_error_msg_col(error));
					free(file_name);
					break;
				}
				else {
					do {
						printf("The file exists, do you want to overwrite it? (Y/N): ");
						yes_no = get_input(&error);
						yes_no_number = check_yes_no(yes_no, &error);
						free(yes_no);
					} while(error != SUCCESS && error != ERR_CTRL_D);
					if(error == ERR_CTRL_D) {
						free(file_name);
						printf("\n");
						main_menu();				
						break;			
					}
					if(yes_no_number == 1) {
						free(file_name);
						break;
					}
				}
			}
			file = fopen(file_name, "w+");
			free(file_name);
			if (file == NULL) {
				perror("Error when opening file for writing");
				break;
			}
			for(int i = 0; i < col.number_occupied; i++) {
				elements_read = fwrite(&col.schedule[i].data.p, sizeof(int), 1, file);
				if(elements_read != 1) {
					perror("Error when writing the file");
					break;
				}
				elements_read = fwrite(&col.schedule[i].data.C, sizeof(int), 1, file);
				if(elements_read != 1) {
					perror("Error when writing the file");
					break;
				}
				elements_read = fwrite(col.schedule[i].data.user, strlen(col.schedule[i].data.user), 1, file);
				if(elements_read != 1) {
					break;
				}
			}
			if(fclose(file) != 0) {
				fprintf(stderr, "Error when closing file.\n");
				break;
			}
			break;
		case EXECUTE_SCHEDULE:
			if(col.number_occupied == 0) {
				error = ERR_NO_ITEMS;
				fprintf(stderr, "The current schedule is empty. It cannot be executed.\n");
			}
			else {
				execute_schedule(col, &error);
				if(error != SUCCESS) {
					fprintf(stderr, "%s\n", get_error_msg_col(error));
					clean_collection(col);
					exit(EXIT_FAILURE);
				}
			}
			break;
		case HELP:
			main_menu();
			break;
		default:
			if(error != SUCCESS)
				fprintf(stderr, "%s\n", get_error_msg_col(error));
			else
				printf("Unrecognized option.\n");
			main_menu();
			break;
		}
	} while(1);
	return 0;
}
