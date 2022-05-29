#ifndef DEMO_API_H_
#define DEMO_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Demo APIs result codes used for API return values */
typedef enum demo_api_result {
    /** The API executed succesfully */
    DEMO_API_OK = 0,
    /** At least one of the passed parameters is invalid */
    DEMO_API_PARM = -10,
    /** Invalid range found when attempting operation */
    DEMO_API_RANGE = -11,
    /** Timed out when waiting for event */
    DEMO_API_TIMEOUT = -12,
    /** Resource not found */
    DEMO_API_NOT_FOUND = -13,
} demo_api_result_t;

/**
 * Demo API function to add two integer values
 *
 * @param a[in]    First number to add
 * @param b[in]    Second number to add
 * @param b[out]   Pointer to store the result
 *
 * @return An integer with the demo_api_result_t return code
 */
int demo_api_add(int a, int b, int *out);

/**
 * Demo API function to multiply two integer values
 *
 * @param a[in]    First number to multiply
 * @param b[in]    Second number to multiply
 * @param b[out]   Pointer to store the result
 *
 * @return An integer with the demo_api_result_t return code
 */
int demo_api_mult(int a, int b, int *out);

/**
 * Demo API function to double a positive integer number. If the number is
 * negative, return an error.
 *
 * @param a[in]    Positive number to double
 * @param b[out]   Pointer to store the result
 *
 * @return An integer with the demo_api_result_t return code
 */
int demo_api_double_if_pos(int a, int *out);

#ifdef __cplusplus
}
#endif

#endif // DEMO_API_H_
