#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sensor_commands/external/uthash.h>
#include <sensor_commands/sensor/manager.h>
#include <sensor_commands/command.h>
#include "sensors/factory.h"

/** Entry structure to support adding sensors to an UT hash table */
struct SensorHashEntry {
    struct Sensor *ssr;
    UT_hash_handle hh;
};

/** Sensor manager structure declaration (not exposed) */
struct SensorManager {
    struct SensorManagerConfig cfg;
    cJSON *cfg_cjson;
    struct SensorFactory *sf;
    /** Head entry for the sensor hash table */
    struct SensorHashEntry *sensor_ht;
};

/** Creates a cJSON handle from the data read from a file */
static cJSON *cjson_handle_create(const char *filename)
{
    int ret;
    FILE *fp;
    int fsize;
    cJSON *cjson = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s [%s (%d)]\n",
                filename, strerror(errno), errno);
        return NULL;
    }

    // Get file size in bytes
    if (fseek(fp, 0L, SEEK_END) != 0) {
        fprintf(stderr, "fseek end failed for config file\n");
        fclose(fp);
        return NULL;
    }
    fsize = ftell(fp);

    // Rewind file to start
    if (fseek(fp, 0L, SEEK_SET) != 0) {
        fprintf(stderr, "fseek set failed for config file\n");
        fclose(fp);
        return NULL;
    }

    // Allocate buffer to store file data
    char *fbuf = malloc(fsize);
    if (fbuf == NULL) {
        fprintf(stderr, "Failed to allocate file buffer\n");
        fclose(fp);
        return NULL;
    }

    // Read file data into buffer
    ret = fread(fbuf, sizeof(char), fsize, fp);
    if (!ret) {
        fprintf(stderr, "Failed to read file into buffer [ret=%d][%s (%d)]\n",
                ret, strerror(errno), errno);
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    // Parse JSON from file data
    cjson = cJSON_Parse(fbuf);
    if (cjson == NULL) {
        fprintf(stderr, "Failed to parse json file\n");
        return NULL;
    }
    return cjson;
}

/** Destroys a cJSON handle */
static void cjson_handle_destroy(cJSON* cjson)
{
    cJSON_Delete(cjson);
}

/** Add sensor to the manager's sensor hash table */
static int sensor_ht_add(struct SensorManager *smgr, struct Sensor *ssr)
{
    struct SensorHashEntry *entry =
        malloc(sizeof(struct SensorHashEntry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate sensor hash entry\n");
        return -ENOMEM;
    }
    printf("%s: sensor name=%s, type=%s\n", __func__, ssr->info.name, ssr->info.type);
    entry->ssr = ssr;
    HASH_ADD_KEYPTR(hh, smgr->sensor_ht, ssr->info.name, strlen(ssr->info.name), entry);
    return 0;
}

/** Creates the manager's sensor hash table and populates it with cJSON */
static int sensor_ht_create(struct SensorManager *smgr)
{
    int ret;
    cJSON *sensors = NULL;
    int num_sensors = 0;

    sensors = cJSON_GetObjectItem(smgr->cfg_cjson, "sensors");
    if (sensors == NULL) {
        fprintf(stderr, "Failed to read sensors array: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    // Init head entry for sensor hash table
    smgr->sensor_ht = NULL;

    // Iterate over config array to create sensors
    num_sensors = cJSON_GetArraySize(sensors);
    for(int i = 0; i < num_sensors; i++)
    {
        struct Sensor *ssr = NULL;
        cJSON *sensor, *obj;
        char *name, *type;
        sensor = cJSON_GetArrayItem(sensors, i);

        // Read type and name from JSON
        obj = cJSON_GetObjectItem(sensor, "type");
        if (obj == NULL) {
            fprintf(stderr, "Failed to read sensor type: %s\n", cJSON_GetErrorPtr());
            return -1;
        }
        type = cJSON_GetStringValue(obj);

        obj = cJSON_GetObjectItem(sensor, "name");
        if (obj == NULL) {
            fprintf(stderr, "Failed to read sensor name: %s\n", cJSON_GetErrorPtr());
            return -1;
        }
        name = cJSON_GetStringValue(obj);

        // Create sensor and add it to hash table
        ssr = sensor_factory_sensor_create(smgr->sf, type, name);
        if (ssr == NULL) {
            fprintf(stderr, "Failed to create sensor with type: %s, name: %s\n",
                    type, name);
            return -1;
        }
        ret = sensor_ht_add(smgr, ssr);
        if (ret) {
            fprintf(stderr, "Failed to add sensor with type: %s, name: %s\n",
                    ssr->info.type, ssr->info.name);
            return ret;
        }
    }

    return 0;
}

/** Destroys the manager's sensor hash table */
static void sensor_ht_destroy(struct SensorManager *smgr)
{
    struct SensorHashEntry *entry, *tmp;
    HASH_ITER(hh, smgr->sensor_ht, entry, tmp) {
        HASH_DEL(smgr->sensor_ht, entry);
        free(entry);
    }
}

struct SensorManager *sensor_manager_create(struct SensorManagerConfig *cfg)
{
    int ret;
    cJSON *cjson = NULL;
    struct SensorManager *smgr =
        (struct SensorManager *)calloc(1, sizeof(struct SensorManager));
    if (smgr == NULL) {
        fprintf(stderr, "Failed to allocate sensor manager\n");
        return NULL;
    }
    smgr->cfg = *cfg;

    cjson = cjson_handle_create(smgr->cfg.cfg_filename);
    if (cjson == NULL) {
        fprintf(stderr, "Failed to create config JSON handle\n");
        return NULL;
    }
    smgr->cfg_cjson = cjson;

    smgr->sf = sensor_factory_create();
    if (smgr->sf == NULL) {
        fprintf(stderr, "Failed to create sensor factory\n");
        return NULL;
    }

    ret = sensor_ht_create(smgr);
    if (ret) {
        fprintf(stderr, "Failed to create sensor hash table with ret=%d\n", ret);
        return NULL;
    }

    return smgr;
}

struct Sensor *sensor_manager_sensor_get(struct SensorManager *smgr,
                                         const char *name)
{
    struct SensorHashEntry *entry = NULL;
    HASH_FIND(hh, smgr->sensor_ht, name, strlen(name), entry);
    if (entry == NULL) {
        fprintf(stderr, "Sensor entry not found for name: %s\n", name);
        return NULL;
    }
    return entry->ssr;
}

/** Sensor read command private data */
struct ssr_read_cmd_data {
    struct Sensor *ssr;
};

/** Sensor read command execute function */
static int ssr_read_cmd_exec_fn(void *data)
{
    struct ssr_read_cmd_data *cmd_data = data;
    struct Sensor *ssr = cmd_data->ssr;
    double val = sensor_read(ssr);
    printf("Sensor read command: [%s]: %s: %f %s\n",
           ssr->info.type, ssr->info.name, val, ssr->info.unit);
    return 0;
}

struct Command *sensor_manager_read_cmd_create(
    struct SensorManager *smgr,
    const char *name)
{
    struct ssr_read_cmd_data *cmd_data = malloc(sizeof(struct ssr_read_cmd_data));
    if (cmd_data == NULL) {
        fprintf(stderr, "Failed to allocate sensor read command data\n");
        return NULL;
    }
    struct Sensor * ssr = sensor_manager_sensor_get(smgr, name);
    if (ssr == NULL) return NULL;
    cmd_data->ssr = ssr;

    return command_create(cmd_data, ssr_read_cmd_exec_fn);
}

void sensor_manager_destroy(struct SensorManager *smgr)
{
    sensor_ht_destroy(smgr);
    sensor_factory_destroy(smgr->sf);
    cjson_handle_destroy(smgr->cfg_cjson);
    free(smgr);
}
