#include <stdlib.h>

#include "factory.h"
#include "level.h"
#include "temp.h"

struct SensorFactory {
};

struct SensorFactory *sensor_factory_create(void)
{
    struct SensorFactory *sf =
        (struct SensorFactory *)calloc(1, sizeof(struct SensorFactory));
    return sf;
}

void sensor_factory_destroy(struct SensorFactory *sf)
{
    free(sf);
}
