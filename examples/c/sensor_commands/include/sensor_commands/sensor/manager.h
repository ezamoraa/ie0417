#ifndef MANAGER_H_
#define MANAGER_H_

#include <cjson/cJSON.h>
#include <sensor_commands/sensor/sensor.h>

struct SensorManager;

struct SensorManagerConfig {
    const char *cfg_filename;
};

struct SensorManager *sensor_manager_create(struct SensorManagerConfig *cfg);

struct Sensor *sensor_manager_sensor_get(struct SensorManager *smgr,
                                         const char *name);

void sensor_manager_destroy(struct SensorManager *smgr);

#endif // MANAGER_H_
