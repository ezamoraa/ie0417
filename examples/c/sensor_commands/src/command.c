#include <stdlib.h>

#include <sensor_commands/command.h>

struct Command *command_create(int param)
{
    struct Command *cmd =
        (struct Command *)calloc(1, sizeof(struct Command));
    cmd->member = param;
    return cmd;
}

void command_destroy(struct Command *cmd)
{
    free(cmd);
}
