#ifndef FACTORY_H_
#define FACTORY_H_

#include <sensor_commands/sensor/sensor.h>

/**
 * Creates a sensor factory
 *
 * The sensor factory is in charge of abstracting the construction
 * of different types of sensors. The correct sensor type is going
 * to be instantiated internally according to the desired type
 * and the generic sensor structure is returned.
 *
 * @return Pointer to a sensor factory structure.
 */
struct SensorFactory *sensor_factory_create(void);

/**
 * Creates a sensor according to the factory logic
 *
 * @param sf   Sensor factory structure.
 * @param type Sensor type to be created.
 * @param name Name of the sensor to create.
 *
 * @return Pointer to a sensor factory structure.
 */
struct Sensor *sensor_factory_sensor_create(struct SensorFactory *sf,
                                            const char *type, const char *name);

/**
 * Destroys the sensor factory
 *
 * @param sf Sensor factory structure.
 */
void sensor_factory_destroy(struct SensorFactory *sf);

#endif // FACTORY_H_
