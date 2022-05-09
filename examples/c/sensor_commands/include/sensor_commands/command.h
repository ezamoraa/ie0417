#ifndef COMMAND_H_
#define COMMAND_H_

struct Command {
    int member;
};

struct Command *command_create(int param);
void command_destroy(struct Command *cmd);

#endif // COMMAND_H_
