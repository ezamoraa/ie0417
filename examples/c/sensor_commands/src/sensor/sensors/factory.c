#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sensor_commands/external/uthash.h>
#include "factory.h"
#include "level.h"
#include "temp.h"

typedef struct Sensor * (*sensor_create_fn)(const char *name);

/** Sensor constructor information structure */
struct SensorCtorInfo {
    const char *type;
    sensor_create_fn create_fn;
};

/** Sensor constructor structure */
struct SensorConstructor {
    struct SensorCtorInfo info;
    UT_hash_handle hh;
};

/** Sensor factory structure */
struct SensorFactory {
    /** Head entry for the constructor hash table */
    struct SensorConstructor *ctor_ht;
};

/** Global array with the supported sensor constructors info */
static struct SensorCtorInfo ctors_info[] = {
  {"temperature", temp_sensor_create},
  {"level", level_sensor_create},
  {"", NULL},
};

/** Add constructor to the factory's hash table */
static int ctor_ht_add(struct SensorFactory *sf, struct SensorCtorInfo *info)
{
    struct SensorConstructor *ctor =
        malloc(sizeof(struct SensorConstructor));
    if (ctor == NULL) {
        fprintf(stderr, "Failed to allocate sensor ctor for type: %s\n", info->type);
        return -ENOMEM;
    }
    ctor->info = *info;
    HASH_ADD_KEYPTR(hh, sf->ctor_ht, info->type, strlen(info->type), ctor);
    return 0;
}

/** Creates the factory's hash table and populates it with the global array info */
static int ctor_ht_create(struct SensorFactory *sf)
{
    int ret;
    sf->ctor_ht = NULL;

    // Create ctors from info array and add them to hash table
    for (int i=0;; i++) {
        struct SensorCtorInfo *info = &ctors_info[i];
        // The last info struct should have type == ""
        if ((strlen(info->type) == 0) ||
            (info->create_fn == NULL)) break;

        ret = ctor_ht_add(sf, info);
        if (ret) {
            fprintf(stderr, "Failed to add sensor ctor for type: %s\n", info->type);
            return ret;
        }
    }
    return 0;
}

/** Destroys the factory's constructor hash table */
static void ctor_ht_destroy(struct SensorFactory *sf)
{
    struct SensorConstructor *ctor, *tmp;
    HASH_ITER(hh, sf->ctor_ht, ctor, tmp) {
        HASH_DEL(sf->ctor_ht, ctor);
        free(ctor);
    }
}

struct SensorFactory *sensor_factory_create(void)
{
    struct SensorFactory *sf =
        (struct SensorFactory *)calloc(1, sizeof(struct SensorFactory));
    if (sf == NULL) {
        fprintf(stderr, "Failed to allocate sensor factory\n");
        return NULL;
    }
    ctor_ht_create(sf);
    return sf;
}

struct Sensor *sensor_factory_sensor_create(struct SensorFactory *sf,
                                            const char *type, const char *name)
{
    struct SensorConstructor *ctor = NULL;
    HASH_FIND(hh, sf->ctor_ht, type, strlen(type), ctor);
    if (ctor == NULL) {
        fprintf(stderr, "Sensor ctor not found for type: %s\n", type);
        return NULL;
    }
    return ctor->info.create_fn(name);
}

void sensor_factory_destroy(struct SensorFactory *sf)
{
    ctor_ht_destroy(sf);
    free(sf);
}
