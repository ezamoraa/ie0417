#include <stdio.h>
#include <stdlib.h>

#include <sensor_commands/sensor/sensor.h>

struct Sensor *sensor_create(struct SensorInfo *info,
                             struct SensorOps *ops,
                             void *priv)
{
    struct Sensor *snr =
        (struct Sensor *)calloc(1, sizeof(struct Sensor));
    if (snr == NULL) {
        fprintf(stderr, "Failed to allocate sensor\n");
        return NULL;
    }
    snr->info = *info;
    snr->ops = ops;
    snr->priv = priv;
    return snr;
}

void sensor_destroy(struct Sensor *snr)
{
    free(snr);
}
