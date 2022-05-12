#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sensor_commands/external/uthash.h>
#include <sensor_commands/sensor/manager.h>
#include "sensors/factory.h"

struct SensorHashEntry {
    struct Sensor *snr;
    UT_hash_handle hh;
};

struct SensorManager {
    struct SensorManagerConfig cfg;
    cJSON *cfg_cjson;
    struct SensorFactory *sf;
    struct SensorHashEntry *sensor_ht;
};

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

static void cjson_handle_destroy(cJSON* cjson)
{
    cJSON_Delete(cjson);
}

static int sensor_ht_add(struct SensorManager *smgr, struct Sensor *snr)
{
    struct SensorHashEntry *entry =
        malloc(sizeof(struct SensorHashEntry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate sensor hash entry\n");
        return -ENOMEM;
    }
    printf("%s: sensor name=%s, type=%s\n", __func__, snr->info.name, snr->info.type);
    entry->snr = snr;
    HASH_ADD_KEYPTR(hh, smgr->sensor_ht, snr->info.name, strlen(snr->info.name), entry);
    return 0;
}

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
        struct Sensor *snr = NULL;
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
        snr = sensor_factory_sensor_create(smgr->sf, type, name);
        if (snr == NULL) {
            fprintf(stderr, "Failed to create sensor with type: %s, name: %s\n",
                    type, name);
            return -1;
        }
        ret = sensor_ht_add(smgr, snr);
        if (ret) {
            fprintf(stderr, "Failed to add sensor with type: %s, name: %s\n",
                    snr->info.type, snr->info.name);
            return ret;
        }
    }

    return 0;
}

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

void sensor_manager_destroy(struct SensorManager *smgr)
{
    sensor_ht_destroy(smgr);
    sensor_factory_destroy(smgr->sf);
    cjson_handle_destroy(smgr->cfg_cjson);
    free(smgr);
}
