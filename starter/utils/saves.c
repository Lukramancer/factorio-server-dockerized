#include "../utils.h"

#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>


#define SAVE_FILE_PREFIX "save"
#define SAVE_FILE_SUFFIX ".zip"


bool has_save_file(const char *directory_path) {
    DIR* directory_ptr = opendir(directory_path);

    if (directory_ptr == NULL) {
        // TODO: report error
        return false;
    }
    
    bool result = false;
    struct dirent* directory_entry_ptr;
    while ((directory_entry_ptr = readdir(directory_ptr)) != NULL && !result) {
        const char* directory_entry_name_ptr = directory_entry_ptr->d_name;
        
        unsigned long directory_entry_name_length = strlen(directory_entry_name_ptr);
        if (directory_entry_name_length <= strlen(SAVE_FILE_SUFFIX)) continue;
        
        if (strcmp(directory_entry_name_ptr + directory_entry_name_length - strlen(SAVE_FILE_SUFFIX), SAVE_FILE_SUFFIX) == 0) {
            result = true;
        }
    }
    
    closedir(directory_ptr);
    
    return result;
}

int generate_save_file_path(const char* saves_directory_path, char* save_file_path) {
    strcpy(save_file_path, saves_directory_path);
    save_file_path[strlen(saves_directory_path)] = '/';

    char *filename_start_ptr = save_file_path + strlen(saves_directory_path) + 1;
    unsigned long long number = 1;
    while (number != 0) {
        sprintf(filename_start_ptr, SAVE_FILE_PREFIX "%llu" SAVE_FILE_SUFFIX, number);
        
        if (access(save_file_path, F_OK) == 0) ++number;
        else if (errno == ENOENT) return 0;
        else return -1; // Could not access file
    }

    return -1; // Could not find any free file
}