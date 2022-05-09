#ifndef MANAGER_H_
#define MANAGER_H_

struct SensorManager {
    int member;
};

struct SensorManager *sensor_manager_create(int param);
void sensor_manager_destroy(struct SensorManager *smgr);

#endif // MANAGER_H_
