#ifndef SENSOR_H_
#define SENSOR_H_

struct Sensor {
    int member;
};

struct Sensor *sensor_create(int param);
void sensor_destroy(struct Sensor *snr);

#endif // SENSOR_H_
