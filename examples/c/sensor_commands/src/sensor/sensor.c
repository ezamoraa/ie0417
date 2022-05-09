#include <stdlib.h>

#include <sensor_commands/sensor/sensor.h>

struct Sensor *sensor_create(int param)
{
    struct Sensor *snr =
        (struct Sensor *)calloc(1, sizeof(struct Sensor));
    snr->member = param;
    return snr;
}

void sensor_destroy(struct Sensor *snr)
{
    free(snr);
}
