#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sensor_commands/command.h>

#define MSG_CMD_MAX_SIZE 256

struct Command *command_create(void *data, cmd_exec_fn execute)
{
    struct Command *cmd =
        (struct Command *)calloc(1, sizeof(struct Command));
    if (cmd == NULL) {
        fprintf(stderr, "Failed to allocate command\n");
        return NULL;
    }
    if (execute == NULL) {
        fprintf(stderr, "Command execute function cannot be NULL\n");
        free(cmd);
        return NULL;
    }

    cmd->data = data;
    cmd->execute = execute;
    return cmd;
}

int command_execute(struct Command *cmd)
{
    if (cmd == NULL) {
        fprintf(stderr, "Command is NULL\n");
        return -1;
    }
    return cmd->execute(cmd->data);
}

int command_destroy(struct Command *cmd)
{
    if (cmd == NULL) {
        fprintf(stderr, "Command is NULL\n");
        return -1;
    }
    free(cmd);
    return 0;
}

/** Message command private data */
struct msg_cmd_data {
    char msg[MSG_CMD_MAX_SIZE + 1];
};

/** Message command execute function */
static int msg_cmd_exec_fn(void *data)
{
    struct msg_cmd_data *cmd_data = data;
    printf("%s", cmd_data->msg);
    return 0;
}

struct Command *msg_command_create(const char *msg)
{
    struct msg_cmd_data *cmd_data = malloc(sizeof(struct msg_cmd_data));
    if (cmd_data == NULL) {
        fprintf(stderr, "Failed to msg command data\n");
        return NULL;
    }
    strncpy(cmd_data->msg, msg, MSG_CMD_MAX_SIZE);

    return command_create(cmd_data, msg_cmd_exec_fn);
}
