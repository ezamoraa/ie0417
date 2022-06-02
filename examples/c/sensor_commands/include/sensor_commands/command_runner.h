#ifndef COMMAND_RUNNER_H_
#define COMMAND_RUNNER_H_

/** Structure with the CommandRunner configuration */
struct CommandRunnerConfig {
    /** Maximum size of the command queue */
    int q_max_size;
};

/**
 * Creates a command runner
 *
 * A command runner executes commands coming from a queue in a consumer thread
 *
 * @param cfg  Command runner configuration
 *
 * @return Pointer to a command runner structure.
 */
struct CommandRunner * command_runner_create(struct CommandRunnerConfig *cfg);

/**
 * Starts the command runner
 *
 * Once the commmand runner is started it can begin to process commands.
 *
 * @param cmd_runner command runner structure.
 *
 * @return 0 on success, negative number on error
 */
int command_runner_start(struct CommandRunner *cmd_runner);

/**
 * Stops the command runner
 *
 * @param cmd_runner command runner structure.
 *
 * @return 0 on success, negative number on error
 */
int command_runner_stop(struct CommandRunner *cmd_runner);

/**
 * Sends a command to the command runner
 *
 * @param cmd_runner command runner structure.
 *
 * @return 0 on success, negative number on error
 */
int command_runner_send(struct CommandRunner *cmd_runner,
                        struct Command *cmd);

/**
 * Destroys the command runner
 *
 * @param cmd_runner command runner structure.
 *
 * @return 0 on success, negative number on error
 */
int command_runner_destroy(struct CommandRunner *cmd_runner);

#endif // COMMAND_RUNNER_H_
