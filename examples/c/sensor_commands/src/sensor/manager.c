#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sensor_commands/sensor/manager.h>

struct SensorManager {
    struct SensorManagerConfig cfg;
    cJSON *cfg_cjson;
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

static int sensors_init(struct SensorManager *smgr)
{
    cJSON *sensors = NULL;
    int num_sensors = 0;

    sensors = cJSON_GetObjectItem(smgr->cfg_cjson, "sensors");
    if (sensors == NULL) {
        fprintf(stderr, "Failed to read sensors array: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    // Iterate over sensor array
    num_sensors = cJSON_GetArraySize(sensors);
    for(int i = 0; i < num_sensors; i++)
    {
        cJSON *sensor, *obj;
        char *name, *type;
        sensor = cJSON_GetArrayItem(sensors, i);

        obj = cJSON_GetObjectItem(sensor, "name");
        if (obj == NULL) {
            fprintf(stderr, "Failed to read sensor name: %s\n", cJSON_GetErrorPtr());
            return -1;
        }
        name = cJSON_GetStringValue(obj);

        obj = cJSON_GetObjectItem(sensor, "type");
        if (obj == NULL) {
            fprintf(stderr, "Failed to read sensor type: %s\n", cJSON_GetErrorPtr());
            return -1;
        }
        type = cJSON_GetStringValue(obj);

        printf("sensor name=%s, type=%s\n", name, type);
    }

    return 0;
}

struct SensorManager *sensor_manager_create(struct SensorManagerConfig *cfg)
{
    int ret;
    cJSON *cjson = NULL;
    struct SensorManager *smgr =
        (struct SensorManager *)calloc(1, sizeof(struct SensorManager));
    smgr->cfg = *cfg;

    cjson = cjson_handle_create(smgr->cfg.cfg_filename);
    if (cjson == NULL) {
        fprintf(stderr, "Failed to create config JSON handle\n");
        return NULL;
    }
    smgr->cfg_cjson = cjson;

    ret = sensors_init(smgr);
    if (ret) {
        fprintf(stderr, "Failed to init sensors with ret=%d\n", ret);
        return NULL;
    }

    return smgr;
}

void sensor_manager_destroy(struct SensorManager *smgr)
{
    cjson_handle_destroy(smgr->cfg_cjson);
    free(smgr);
}
