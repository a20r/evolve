#ifdef OPENCL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAC
  #include <OpenCL/cl.h>
#else
  #include <CL/cl.h>
#endif

#include "parallel.h"
#include "utils.h"


int opencl_init(cl_platform_id p_id, cl_device_id d_id)
{
    p_id = NULL;
    d_id = NULL;

    return 0;
}

void opencl_info(void)
{
    printf("OpenCL Information\n");
    printf("--------------------\n");
    opencl_print_all_platforms();
}

void opencl_print_all_devices(cl_platform_id id)
{
    cl_uint i;
    cl_int err;

    cl_uint n_devs;
    cl_device_id *devs = NULL;
    char info[100];

    /* query number of devices in plaform */
    err = clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, 0, NULL, &n_devs);
    check(err >= 0, OPENCL_NO_DEVICES);
    check(n_devs > 0, OPENCL_NO_DEVICES);

    /* get all devices on platform */
    devs = malloc(sizeof(cl_device_id *) * n_devs);
    clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, n_devs, devs, NULL);

    /* print out the devices */
    for (i = 0; i < n_devs; i++) {
        clGetDeviceInfo(devs[i], CL_DEVICE_NAME, sizeof(info), info, NULL);
        printf("--> device[%d]: %s", i, info);

        clGetDeviceInfo(devs[i], CL_DEVICE_VERSION, sizeof(info), info, NULL);
        printf(" [%s]\n", info);
    }

    free_mem(devs, free);
    return;
error:
    free_mem(devs, free);
    return;
}

void opencl_print_all_platforms(void)
{
    cl_uint i;
    cl_int err;

    cl_uint n_platforms;
    cl_platform_id *p_ids = NULL;
    char p_name[100];

    /* query number of opencl platforms */
    err = clGetPlatformIDs(0, NULL, &n_platforms);
    check(err >= 0, OPENCL_NO_PLATFORMS);
    check(n_platforms > 0, OPENCL_NO_PLATFORMS);

    /* get all opencl platforms */
    p_ids = malloc(sizeof(cl_platform_id *) * n_platforms);
    clGetPlatformIDs(n_platforms, p_ids, NULL);

    /* print out the platforms */
    for (i = 0; i < n_platforms; i++) {
        err = clGetPlatformInfo(
            p_ids[i],
            CL_PLATFORM_VENDOR,
            sizeof(p_name),
            p_name,
            NULL
        );
        printf("platform[%d]: %s\n", i, p_name);
        opencl_print_all_devices(p_ids[i]);

        printf("\n");
    }

    free_mem(p_ids, free);
    return;
error:
    free_mem(p_ids, free);
    return;
}

#endif
