#include <stdio.h>
#include "menu.h"
void main_menu() {
	printf("Program that emulates a process scheduler.\n\n"
	"Select one of the following options:\n\n"
	"1. Add a new process to the schedule\n"
	"2. Delete a certain process from the schedule\n"
	"3. Information about a process\n"
	"4. Show the entire schedule\n"
	"5. Delete the current schedule\n"
	"6. Sort the schedule according to a criteria\n"
	"7. Load from file\n"
	"8. Save to file\n"
	"9. Execute schedule\n"
	"10. Help\n"
	"0. Exit\n\n"
	"If you enter CTRL+D:\n"
	"- In this menu, the program will terminate in a controlled manner.\n"
	"- In a submenu, the program will return to this menu.\n\n");
}
