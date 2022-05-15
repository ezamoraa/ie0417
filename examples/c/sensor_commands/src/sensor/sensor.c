#include <stdio.h>
#include <stdlib.h>

#include <sensor_commands/sensor/sensor.h>

struct Sensor *sensor_create(struct SensorInfo *info,
                             struct SensorOps *ops,
                             void *priv)
{
    struct Sensor *ssr =
        (struct Sensor *)calloc(1, sizeof(struct Sensor));
    if (ssr == NULL) {
        fprintf(stderr, "Failed to allocate sensor\n");
        return NULL;
    }
    ssr->info = *info;
    ssr->ops = ops;
    ssr->priv = priv;
    return ssr;
}

double sensor_read(struct Sensor *ssr)
{
    double val = 0;
    if (ssr->ops && ssr->ops->read) {
        val = ssr->ops->read(&ssr->info, ssr->priv);
    }
    return val;
}

void sensor_destroy(struct Sensor *ssr)
{
    free(ssr);
}
