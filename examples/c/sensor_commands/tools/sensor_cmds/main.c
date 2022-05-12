#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

#include <sensor_commands/version.h>
#include <sensor_commands/command.h>
#include <sensor_commands/sensor/sensor.h>
#include <sensor_commands/sensor/manager.h>

int main(int argc, char **argv) {
    int opt;
    int status = 0;
    int a = 0, b = 0;
    bool flag = false;
    struct SensorManagerConfig smgr_cfg = {};
    struct SensorManager *smgr = NULL;
    const char *version = NULL;
    char cfg_filename[PATH_MAX + 1];

    while((opt = getopt(argc, (char *const *)argv, "b:c:f")) != -1) {
        switch(opt) {
            case 'b':
                b = atoi(optarg);
                break;
            case 'c':
                strncpy(cfg_filename, optarg, PATH_MAX);
                break;
            case 'f':
                flag = true;
                break;
            default :
                (void)fprintf(stderr, "Unknown option -%c\n", opt);
                return -EINVAL;
        }
    }
    printf("a: %d, b: %d, flag: %d\n", a, b, (int)flag);

    status = sensor_commands_version(&version);
    if (status) return status;

    printf("lib version: %s\n", version);

    // Setup sensor manager
    smgr_cfg.cfg_filename = cfg_filename;
    smgr = sensor_manager_create(&smgr_cfg);
    if (smgr == NULL) {
        fprintf(stderr, "Failed to create sensor manager\n");
        return -1;
    }

    sensor_manager_destroy(smgr);

    return status;
}
