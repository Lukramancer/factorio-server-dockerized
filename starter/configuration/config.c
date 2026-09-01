#include "../paths.h"


#include <fcntl.h>
#include <unistd.h>


#define DEFAULT_CONFIG "[path]\nread-data=" FACTORIO_DATA_DIRECTORY_PATH "\nwrite-data=" FACTORIO_SERVER_DATA_DIRECTORY_PATH

int generate_default_config(const char* config_path) {
    int config_file_descriptor = open(config_path, O_CREAT | O_WRONLY | O_TRUNC);
    if (config_file_descriptor < 0) {
        return -1; // Could not config file, check errno
    }

    const char *data_position = DEFAULT_CONFIG;
    size_t written_bytes_amount = 0;
    while (written_bytes_amount < sizeof(DEFAULT_CONFIG) - 1) {
        int iteration_written_bytes_amount = write(
            config_file_descriptor,
            data_position,
            sizeof(DEFAULT_CONFIG) - 1 - written_bytes_amount
        );

        if (iteration_written_bytes_amount < 0) {
            return -1; // Could not write to config file, check errno
        }

        written_bytes_amount += iteration_written_bytes_amount;
        data_position += iteration_written_bytes_amount;
    }

    return 0;
}