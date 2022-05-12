#ifndef FACTORY_H_
#define FACTORY_H_

struct SensorFactory;

struct SensorFactory *sensor_factory_create(void);
void sensor_factory_destroy(struct SensorFactory *sf);

#endif // FACTORY_H_
