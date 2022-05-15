#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

#include <sensor_commands/version.h>
#include <sensor_commands/command.h>
#include <sensor_commands/command_runner.h>
#include <sensor_commands/sensor/sensor.h>
#include <sensor_commands/sensor/manager.h>

static int basic_command_experiment(void)
{
    int ret;

    struct CommandRunnerConfig cmd_runner_cfg = {
      .q_max_size = 100,
    };
    struct CommandRunner *cmd_runner = command_runner_create(&cmd_runner_cfg);
    if (cmd_runner == NULL) {
        fprintf(stderr, "Failed to create command runner\n");
        return -1;
    }
    struct Command *msg_cmd = msg_command_create("This is a command test message!\n");
    if (msg_cmd == NULL) {
        fprintf(stderr, "Failed to create message command\n");
        return -1;
    }

    printf("++++++++++ CommandRunner experiment ++++++++++\n");

    ret = command_runner_start(cmd_runner);
    if (ret) {
        fprintf(stderr, "Failed to start command runner with ret=%d\n", ret);
        return -1;
    }

    for (int i=0; i < 10; i++) {
        ret = command_runner_send(cmd_runner, msg_cmd);
        if (ret) {
            fprintf(stderr, "Failed to send command to command runner with ret=%d\n", ret);
            return -1;
        }
    }

    ret = command_runner_stop(cmd_runner);
    if (ret) {
        fprintf(stderr, "Failed to stop command runner with ret=%d\n", ret);
        return -1;
    }

    command_destroy(msg_cmd);
    command_runner_destroy(cmd_runner);

    return 0;
}

static int sensor_command_experiment(const char *cfg_filename)
{
    int ret;
    struct SensorManagerConfig smgr_cfg = {};
    struct SensorManager *smgr = NULL;
    struct Command *cmd = NULL;

    #define NUM_SENSORS 5

    const char *sensor_names[NUM_SENSORS] = {
      "level-eie202",
      "temp-eie206",
      "temp-eie204",
      "level-eie301",
      "temp-eie208",
    };
    struct Command *commands[NUM_SENSORS] = {};

    struct CommandRunnerConfig cmd_runner_cfg = {
      .q_max_size = 100,
    };
    struct CommandRunner *cmd_runner = command_runner_create(&cmd_runner_cfg);
    if (cmd_runner == NULL) {
        fprintf(stderr, "Failed to create command runner\n");
        return -1;
    }

    printf("++++++++++ SensorManager experiment ++++++++++\n");

    ret = command_runner_start(cmd_runner);
    if (ret) {
        fprintf(stderr, "Failed to start command runner with ret=%d\n", ret);
        return -1;
    }

    smgr_cfg.cfg_filename = cfg_filename;
    smgr = sensor_manager_create(&smgr_cfg);
    if (smgr == NULL) {
        fprintf(stderr, "Failed to create sensor manager\n");
        return -1;
    }

    for (int i=0; i < NUM_SENSORS; i++) {
        const char *name = sensor_names[i];
        if (strlen(name) == 0) break;

        cmd = sensor_manager_read_cmd_create(smgr, name);
        if (cmd != NULL) {
            commands[i] = cmd;
            ret = command_runner_send(cmd_runner, cmd);
            if (ret) {
                fprintf(stderr, "Failed to send command to command runner with ret=%d\n", ret);
                return -1;
            }
        } else {
            printf("Failed to get read command for sensor with name %s\n", name);
        }
    }

    ret = command_runner_stop(cmd_runner);
    if (ret) {
        fprintf(stderr, "Failed to stop command runner with ret=%d\n", ret);
        return -1;
    }

    for (int i=0; i < NUM_SENSORS; i++) {
        command_destroy(commands[i]);
        commands[i] = NULL;
    }

    sensor_manager_destroy(smgr);
    command_runner_destroy(cmd_runner);

    return 0;
}

int main(int argc, char **argv) {
    int opt;
    int ret = 0;
    const char *version = NULL;
    char cfg_filename[PATH_MAX + 1];

    // Parse cmdline options
    while((opt = getopt(argc, (char *const *)argv, "c:")) != -1) {
        switch(opt) {
            case 'c':
                strncpy(cfg_filename, optarg, PATH_MAX);
                break;
            default :
                (void)fprintf(stderr, "Unknown option -%c\n", opt);
                return -EINVAL;
        }
    }

    // Check library version
    ret = sensor_commands_version(&version);
    if (ret) return ret;
    printf("lib version: %s\n", version);

    // Experiment with basic commands
    basic_command_experiment();

    // Experiment with sensor commands
    sensor_command_experiment(cfg_filename);

    return ret;
}
