#include <stdio.h>
#include <stdlib.h>

#include "level.h"

struct LevelSensorState {
    int value;
};

static double level_sensor_read(struct SensorInfo *info, void *priv)
{
    struct LevelSensorState *state = priv;
    printf("Reading level sensor with name %s\n", info->name);
    return state->value;
}

struct SensorOps level_sensor_ops = {
  .read = level_sensor_read,
};

struct Sensor * level_sensor_create(const char *name)
{
    struct SensorInfo info = {};
    struct LevelSensorState *state =
        calloc(1, sizeof(struct LevelSensorState));
    if (state == NULL) {
        fprintf(stderr, "Failed to allocate level sensor state\n");
        return NULL;
    }
    info.type = "level";
    info.unit = "meters";
    info.name = name;
    state->value = 100;

    return sensor_create(&info, &level_sensor_ops, state);
}
