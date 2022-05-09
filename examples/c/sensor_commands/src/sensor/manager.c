#include <stdlib.h>

#include <sensor_commands/sensor/manager.h>

struct SensorManager *sensor_manager_create(int param)
{
    struct SensorManager *smgr =
        (struct SensorManager *)calloc(1, sizeof(struct SensorManager));
    smgr->member = param;
    return smgr;
}

void sensor_manager_destroy(struct SensorManager *smgr)
{
    free(smgr);
}
