#include <stdio.h>
#include <stdlib.h>

#include "temp.h"

struct TempSensorState {
    int value;
};

static double temp_sensor_read(struct SensorInfo *info, void *priv)
{
    struct TempSensorState *state = priv;
    printf("Reading temp sensor with name: %s\n", info->name);
    return state->value;
}

struct SensorOps temp_sensor_ops = {
  .read = temp_sensor_read,
};

struct Sensor * temp_sensor_create(const char *name)
{
    struct SensorInfo info = {};
    struct TempSensorState *state =
        calloc(1, sizeof(struct TempSensorState));
    if (state == NULL) {
        fprintf(stderr, "Failed to allocate temp sensor state\n");
        return NULL;
    }
    info.type = "temperature";
    info.unit = "celsius";
    info.name = name;
    state->value = 42;

    return sensor_create(&info, &temp_sensor_ops, state);
}
