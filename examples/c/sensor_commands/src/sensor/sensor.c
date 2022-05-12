#include <stdlib.h>

#include <sensor_commands/sensor/sensor.h>

struct Sensor *sensor_create(struct SensorInfo *info)
{
    struct Sensor *snr =
        (struct Sensor *)calloc(1, sizeof(struct Sensor));
    snr->info = *info;
    return snr;
}

void sensor_destroy(struct Sensor *snr)
{
    free(snr);
}
