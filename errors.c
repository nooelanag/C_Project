#include <errno.h>
#include <string.h>
#include "errors.h"
static const char *error_map_coll[NUM_COLLECTION_ERROR_MSGS + 1] = {
		"Success.",
        "END OF FILE.",
        "The input introduced is empty.",
        "The input introduced contains a letter.",
		"There are no processes in the schedule.",
		"There is a process with that data in the schedule.",
		"Process not found in the schedule.",
		"The current schedule is not empty.",
		"The file does not exist or cannot be read.",
		"The file cannot be written.",
		"Unrecognized option.",
		"Fork failed.",
		"Execution failed.",
		"Convertion from integer to string failed.",
		"Unknown error."
};
const char* get_error_msg_col(int error){
	if (error > 0)
		return strerror(error);
	if (-error < NUM_COLLECTION_ERROR_MSGS)
		return error_map_coll[-error];
	return error_map_coll[NUM_COLLECTION_ERROR_MSGS];
}
