#ifndef FACTORY_H_
#define FACTORY_H_

#include <sensor_commands/sensor/sensor.h>

struct SensorFactory;

struct SensorFactory *sensor_factory_create(void);

struct Sensor *sensor_factory_sensor_create(struct SensorFactory *sf,
                                            const char *type, const char *name);

void sensor_factory_destroy(struct SensorFactory *sf);

#endif // FACTORY_H_
