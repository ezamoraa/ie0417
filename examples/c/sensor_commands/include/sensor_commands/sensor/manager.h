#ifndef MANAGER_H_
#define MANAGER_H_

#include <cjson/cJSON.h>

struct SensorManager;

struct SensorManagerConfig {
    const char *cfg_filename;
};

struct SensorManager *sensor_manager_create(struct SensorManagerConfig *cfg);
void sensor_manager_destroy(struct SensorManager *smgr);

#endif // MANAGER_H_
