#ifndef SENSOR_H_
#define SENSOR_H_

struct SensorInfo {
    const char * name;
    const char * type;
    const char * unit;
};

struct SensorOps {
    double (*read)(struct SensorInfo *info, void *priv);
};

struct Sensor {
    struct SensorInfo info;
    struct SensorOps *ops;
    void *priv;
};

struct Sensor *sensor_create(struct SensorInfo *info,
                             struct SensorOps *ops,
                             void *priv);

void sensor_destroy(struct Sensor *snr);

#endif // SENSOR_H_
