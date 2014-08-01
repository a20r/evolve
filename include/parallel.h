#ifndef _PARALLEL_H_
#define _PARALLEL_H_
#ifdef OPENCL

/* ERROR MSGS */
#define OPENCL_NO_PLATFORMS "Could not find any OpenCL platforms!"
#define OPENCL_NO_DEVICES "Could not find any OpenCL devices!"

/* FUNCTIONS */
int opencl_init(cl_platform_id p_id, cl_device_id d_id);
void opencl_info(void);
void opencl_print_all_devices(cl_platform_id id);
void opencl_print_all_platforms(void);

#endif
#endif
