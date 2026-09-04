#include "../configuration.h"


#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "jq.h"


const char update_server_setting_from_env_jq_expression[] = {
#embed "update-server-settings-from-env.jq"
, 0
};


int produce_server_settings_from_env(
    const char* base_server_settings_file_path,
    const char* destination_file_path
) {
    // NOTE: probably should be remade with posix_spawn
    int fork_return_code = fork();
    if (fork_return_code < 0) {
        return -1; // Could not fork process
    }
    else if (fork_return_code == 0) {
        int destination_file_descriptor = open(destination_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (destination_file_descriptor < 0) {
            exit(-1); // Could not open destination file
        }
        
        int dup_return_code = dup2(destination_file_descriptor, STDOUT_FILENO);
        if (dup_return_code < 0) {
            exit(-1); // Could not duplicate fd
        }

        close(destination_file_descriptor);
        
        int exec_return_code = execl(
            JQ_BINARY_PATH,
            "jq",
            update_server_setting_from_env_jq_expression,
            base_server_settings_file_path,
            NULL
        );
        
        if (exec_return_code == -1) exit(-1); // Could not exec jq
        else exit(0);
    }
    
    int status;
    waitpid(fork_return_code, &status, 0);
    return WEXITSTATUS(status);
}