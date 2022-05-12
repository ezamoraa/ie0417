#ifndef SENSOR_H_
#define SENSOR_H_

struct SensorOps;

struct SensorInfo {
    const char * type;
    const char * name;
    const char * unit;
};

struct Sensor {
    struct SensorInfo info;
    struct SensorOps *ops;
    void *priv;
};

struct Sensor *sensor_create(struct SensorInfo *info);

void sensor_destroy(struct Sensor *snr);

#endif // SENSOR_H_
