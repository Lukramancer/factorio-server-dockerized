#include "../utils.h"

#define _GNU_SOURCE

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#define COPY_CHUNK_SIZE 8192
#define USERSPACE_COPY_BUFFER_SIZE COPY_CHUNK_SIZE
#define COPY_FILE_RANGE_SIZE COPY_CHUNK_SIZE

int copy_file_copy_file_range(int source_file_descriptor, int destination_file_descriptor) {
    ssize_t written_bytes_amount;
    do {
        written_bytes_amount = copy_file_range(
            source_file_descriptor, NULL,
            destination_file_descriptor, NULL,
            COPY_FILE_RANGE_SIZE, 0
        );

        if (written_bytes_amount < 0 && errno == EINTR) continue;
        else if (written_bytes_amount < 0) {
            return -1; // Could not copy file range, check errno
        }
    } while (written_bytes_amount != 0);

    return 0;
}

int copy_file_read_write(int source_file_descriptor, int destination_file_descriptor) {
    char copy_buffer[USERSPACE_COPY_BUFFER_SIZE];

    bool done = false;
    while (!done) {
        ssize_t read_bytes_amount = read(source_file_descriptor, copy_buffer, sizeof(copy_buffer));
        if (read_bytes_amount < 0 && errno == EINTR) continue;
        else if (read_bytes_amount < 0) {
            return -1;  // Could not read from source file
        }
        else if (read_bytes_amount == 0) {
            done = true;
        }
        
        ssize_t written_bytes_amount = 0;
        while (written_bytes_amount < read_bytes_amount) {
            ssize_t iteration_written_bytes_amount = write(
                destination_file_descriptor,
                copy_buffer + written_bytes_amount,
                read_bytes_amount - written_bytes_amount
            );
            if (iteration_written_bytes_amount < 0 && errno == EINTR) continue;
            else if (iteration_written_bytes_amount < 0) {
                return -1; // Could not write to destination file
            }

            written_bytes_amount += iteration_written_bytes_amount;
        }
    }

    return 0;
}

int copy_file(const char* destination_file_path, const char* source_file_path) {
    int source_file_descriptor = open(source_file_path, O_RDONLY);
    if (source_file_descriptor < 0) {
        return -1; // Could not open source file, check errno
    }

    int destination_file_descriptor = open(destination_file_path, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (destination_file_descriptor < 0) {
        close(source_file_descriptor);
        return -1; // Could not open destination file, check errno
    }

    int copy_operation_return_code;
    
    copy_operation_return_code = copy_file_copy_file_range(source_file_descriptor, destination_file_descriptor);
    
    if (copy_operation_return_code < 0 && errno == EXDEV) {
        copy_operation_return_code = copy_file_read_write(source_file_descriptor, destination_file_descriptor);
    }
    
    close(destination_file_descriptor);
    close(source_file_descriptor);
    
    return copy_operation_return_code;
}
