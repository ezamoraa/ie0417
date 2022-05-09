#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sensor_commands/version.h>
#include <sensor_commands/command.h>
#include <sensor_commands/sensor/sensor.h>
#include <sensor_commands/sensor/manager.h>

int main(int argc, char **argv) {
    int opt;
    int status = 0;
    int a = 0, b = 0;
    bool flag = false;
    struct Sensor *snr = NULL;
    const char *version = NULL;

    while((opt = getopt(argc, (char *const *)argv, "a:b:f")) != -1) {
        switch(opt) {
            case 'a':
                a = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
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

    snr = sensor_create(a);

    printf("snr->member: %d\n", snr->member);

    sensor_destroy(snr);

    return status;
}
