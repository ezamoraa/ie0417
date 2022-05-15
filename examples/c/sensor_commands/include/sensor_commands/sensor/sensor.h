#ifndef SENSOR_H_
#define SENSOR_H_

/** General information related to a sensor */
struct SensorInfo {
    const char * name;
    const char * type;
    const char * unit;
};

/** Sensor operations to implement specific sensor types */
struct SensorOps {
    /** Read a sample from the sensor */
    double (*read)(struct SensorInfo *info, void *priv);
};

/** Sensor structure */
struct Sensor {
    /** Sensor information */
    struct SensorInfo info;
    /** Sensor operations */
    struct SensorOps *ops;
    /** Sensor private data passed to ops */
    void *priv;
};

/**
 * Creates a sensor
 *
 * A sensor can be read according to a custom operation
 * and holds generic information about its name, type and
 * measurement unit.
 *
 * @param info Sensor information structure.
 * @param ops  Sensor operations.
 * @param priv Sensor private data.
 *
 * @return Pointer to a sensor manager structure.
 */
struct Sensor *sensor_create(struct SensorInfo *info,
                             struct SensorOps *ops,
                             void *priv);

/**
 * Reads the sensor
 *
 * @param ssr Sensor structure.
 *
 * @return Value representing the sensor measurement.
 */
double sensor_read(struct Sensor *ssr);

/**
 * Destroys the sensor
 *
 * @param ssr Sensor structure.
 */
void sensor_destroy(struct Sensor *ssr);

#endif // SENSOR_H_
