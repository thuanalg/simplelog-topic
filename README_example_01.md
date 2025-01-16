#include "simplelog.h"

int my_callback(SPL_CALLBACL_DATA* data) {
    // Example: Process callback data here
}

int main() {
    SPL_INPUT_ARG input = {
        .folder = "/var/logs/my_logger",  /* Specify the output folder */
        .id_name = "Logger1",            /* Set a unique identifier */
        .fn = my_callback,               /* Assign the callback function */
        .obj = NULL                      /* No additional data for the callback */
    };

    spl_init_log(&input);               /*  Initialize the logger */
    spllog(SPL_LOG_INFO, "Logger initialized!");      /*  Log a message */
    spl_finish_log();                   /*  Finalize the logger */
    return 0;
}
