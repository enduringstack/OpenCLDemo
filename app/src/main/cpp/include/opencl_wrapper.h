//
// Created by chen on 2022/9/14.
//

#ifndef OPENCLDEMO_OPENCL_WRAPPER_H
#define OPENCLDEMO_OPENCL_WRAPPER_H

#include "cl2.hpp"

//support opencl min version is 1.1
#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 200
#endif
#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_TARGET_OPENCL_VERSION 110
#endif
#ifndef CL_HPP_MINIMUM_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 110
#endif

#include <cassert>
#define ASSERT(x)                                                                                                      \
    {                                                                                                                  \
        int res = (x);                                                                                                 \
        if (!res) {                                                                                                    \
            LOGE("Error: assert failed\n");                                                                              \
            assert(res);                                                                                               \
        }                                                                                                              \
    }

#define CHECK_NOTNULL(X)                                                       \
    ASSERT(X != NULL)                                                          \
    if (X == NULL) {                                                           \
        LOGE("OpenCL API is null\n");                                          \
    }

namespace hiai_opencl {
    class OpenCLLib {
    public:
        OpenCLLib();
        static OpenCLLib *GetInstance();

        OpenCLLib(const OpenCLLib &) = delete;

        OpenCLLib &operator=(const OpenCLLib &) = delete;

        ~OpenCLLib();

        bool LoadOpenCLLibrary();

        bool UnloadOpenCLLibrary();

        //get platfrom id
        using clGetPlatformIDsFunc = cl_int (CL_API_CALL *)(cl_uint, cl_platform_id *,
                                                            cl_uint *);
        //get platform info
        using clGetPlatformInfoFunc = cl_int (CL_API_CALL *)(cl_platform_id, cl_platform_info,
                                                             size_t, void *, size_t *);
        // build program
        using clBuildProgramFunc =
        cl_int (CL_API_CALL *)(cl_program, cl_uint, const cl_device_id *, const char *,
                               void (CL_CALLBACK *pfn_notify)(cl_program, void *), void *);
        //enqueue run kernel
        using clEnqueueNDRangeKernelFunc = cl_int (CL_API_CALL *)(cl_command_queue, cl_kernel,
                                                                  cl_uint, const size_t *,
                                                                  const size_t *,
                                                                  const size_t *, cl_uint,
                                                                  const cl_event *, cl_event *);
        //set kernel parameter
        using clSetKernelArgFunc = cl_int (CL_API_CALL *)(cl_kernel, cl_uint, size_t,
                                                          const void *);
        using clRetainMemObjectFunc = cl_int (CL_API_CALL *)(cl_mem);
        using clReleaseMemObjectFunc = cl_int (CL_API_CALL *)(cl_mem);
        using clEnqueueUnmapMemObjectFunc = cl_int (CL_API_CALL *)(cl_command_queue, cl_mem,
                                                                   void *, cl_uint,
                                                                   const cl_event *,
                                                                   cl_event *);
        using clRetainCommandQueueFunc = cl_int (CL_API_CALL *)(cl_command_queue command_queue);
        //create context
        using clCreateContextFunc = cl_context (CL_API_CALL *)(
                const cl_context_properties *, cl_uint, const cl_device_id *,
                void(CL_CALLBACK *)(  // NOLINT(readability/casting)
                        const char *, const void *, size_t, void *),
                void *, cl_int *);
        using clEnqueueCopyImageFunc = cl_int (CL_API_CALL *)(cl_command_queue, cl_mem, cl_mem,
                                                              const size_t *, const size_t *,
                                                              const size_t *, cl_uint,
                                                              const cl_event *, cl_event *);

        using clCreateContextFromTypeFunc =
        cl_context (CL_API_CALL *)(const cl_context_properties *, cl_device_type,
                                   void(CL_CALLBACK *)(  // NOLINT(readability/casting)
                                           const char *, const void *, size_t, void *),
                                   void *, cl_int *);
        using clReleaseContextFunc = cl_int (CL_API_CALL *)(cl_context);
        using clWaitForEventsFunc = cl_int (CL_API_CALL *)(cl_uint, const cl_event *);
        using clReleaseEventFunc = cl_int (CL_API_CALL *)(cl_event);
        using clEnqueueWriteBufferFunc = cl_int (CL_API_CALL *)(cl_command_queue, cl_mem,
                                                                cl_bool, size_t, size_t,
                                                                const void *, cl_uint,
                                                                const cl_event *, cl_event *);
        using clEnqueueReadBufferFunc = cl_int (CL_API_CALL *)(cl_command_queue, cl_mem,
                                                               cl_bool, size_t, size_t, void *,
                                                               cl_uint, const cl_event *,
                                                               cl_event *);
        using clGetProgramBuildInfoFunc = cl_int (CL_API_CALL *)(cl_program, cl_device_id,
                                                                 cl_program_build_info, size_t,
                                                                 void *, size_t *);
        using clRetainProgramFunc = cl_int (CL_API_CALL *)(cl_program program);
        using clEnqueueMapBufferFunc = void *(CL_API_CALL *)(cl_command_queue, cl_mem, cl_bool,
                                                             cl_map_flags, size_t, size_t,
                                                             cl_uint, const cl_event *,
                                                             cl_event *, cl_int *);
        using clEnqueueMapImageFunc = void *(CL_API_CALL *)(cl_command_queue, cl_mem, cl_bool,
                                                            cl_map_flags, const size_t *,
                                                            const size_t *, size_t *, size_t *,
                                                            cl_uint, const cl_event *,
                                                            cl_event *, cl_int *);
        using clCreateCommandQueueFunc = cl_command_queue(CL_API_CALL *)(  // NOLINT
                cl_context, cl_device_id, cl_command_queue_properties, cl_int *);
        using clGetCommandQueueInfoFunc = cl_int (CL_API_CALL *)(cl_command_queue,
                                                                 cl_command_queue_info, size_t,
                                                                 void *, size_t *);
        using clReleaseCommandQueueFunc = cl_int (CL_API_CALL *)(cl_command_queue);
        using clCreateProgramWithBinaryFunc = cl_program (CL_API_CALL *)(cl_context, cl_uint,
                                                                         const cl_device_id *,
                                                                         const size_t *,
                                                                         const unsigned char **,
                                                                         cl_int *, cl_int *);
        using clRetainContextFunc = cl_int (CL_API_CALL *)(cl_context context);
        using clGetContextInfoFunc = cl_int (CL_API_CALL *)(cl_context, cl_context_info, size_t,
                                                            void *, size_t *);
        using clReleaseProgramFunc = cl_int (CL_API_CALL *)(cl_program program);
        //flush command queue to target device
        using clFlushFunc = cl_int (CL_API_CALL *)(cl_command_queue command_queue);
        //sync device command queue
        using clFinishFunc = cl_int (CL_API_CALL *)(cl_command_queue command_queue);
        using clGetProgramInfoFunc = cl_int (CL_API_CALL *)(cl_program, cl_program_info, size_t,
                                                            void *, size_t *);
        //create kernel with kernel name
        using clCreateKernelFunc = cl_kernel (CL_API_CALL *)(cl_program, const char *,
                                                             cl_int *);
        using clRetainKernelFunc = cl_int (CL_API_CALL *)(cl_kernel kernel);
        using clCreateBufferFunc = cl_mem (CL_API_CALL *)(cl_context, cl_mem_flags, size_t,
                                                          void *, cl_int *);
        //create image 2d
        using clCreateImage2DFunc = cl_mem(CL_API_CALL *)(cl_context,  // NOLINT
                                                          cl_mem_flags,
                                                          const cl_image_format *,
                                                          size_t, size_t, size_t,
                                                          void *, cl_int *);
        //create image 3d
        using clCreateImage3DFunc = cl_mem(CL_API_CALL *)(cl_context,  // NOLINT
                                                          cl_mem_flags,
                                                          const cl_image_format *,
                                                          size_t, size_t, size_t,
                                                          size_t, size_t, void *,
                                                          cl_int *);
        //crete program with source code
        using clCreateProgramWithSourceFunc = cl_program (CL_API_CALL *)(cl_context, cl_uint,
                                                                         const char **,
                                                                         const size_t *,
                                                                         cl_int *);
        using clReleaseKernelFunc = cl_int (CL_API_CALL *)(cl_kernel kernel);
        using clGetDeviceInfoFunc = cl_int (CL_API_CALL *)(cl_device_id, cl_device_info, size_t,
                                                           void *, size_t *);
        //get device id, two device have different device id
        using clGetDeviceIDsFunc = cl_int (CL_API_CALL *)(cl_platform_id, cl_device_type,
                                                          cl_uint, cl_device_id *, cl_uint *);
        using clRetainEventFunc = cl_int (CL_API_CALL *)(cl_event);
        using clGetKernelWorkGroupInfoFunc = cl_int (CL_API_CALL *)(cl_kernel, cl_device_id,
                                                                    cl_kernel_work_group_info,
                                                                    size_t, void *, size_t *);
        using clGetEventInfoFunc = cl_int (CL_API_CALL *)(cl_event event,
                                                          cl_event_info param_name,
                                                          size_t param_value_size,
                                                          void *param_value,
                                                          size_t *param_value_size_ret);
        using clGetEventProfilingInfoFunc = cl_int (CL_API_CALL *)(
                cl_event event, cl_profiling_info param_name, size_t param_value_size,
                void *param_value, size_t *param_value_size_ret);
        using clGetImageInfoFunc = cl_int (CL_API_CALL *)(cl_mem, cl_image_info, size_t, void *,
                                                          size_t *);
        using clEnqueueCopyBufferToImageFunc = cl_int(CL_API_CALL *)(
                cl_command_queue, cl_mem, cl_mem, size_t, const size_t *,
                const size_t *, cl_uint, const cl_event *, cl_event *);
        using clEnqueueCopyImageToBufferFunc = cl_int(CL_API_CALL *)(
                cl_command_queue, cl_mem, cl_mem, const size_t *, const size_t *,
                size_t, cl_uint, const cl_event *, cl_event *);
#if CL_HPP_TARGET_OPENCL_VERSION >= 120
        using clRetainDeviceFunc = cl_int (CL_API_CALL *)(cl_device_id);
        using clReleaseDeviceFunc = cl_int (CL_API_CALL *)(cl_device_id);
        using clCreateImageFunc = cl_mem (CL_API_CALL *)(cl_context, cl_mem_flags,
                                                         const cl_image_format *,
                                                         const cl_image_desc *, void *,
                                                         cl_int *);
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 200
        //opencl 2.0 can get sub group info and wave size.
        using clGetKernelSubGroupInfoKHRFunc = cl_int(CL_API_CALL *)(
                cl_kernel, cl_device_id, cl_kernel_sub_group_info, size_t, const void *, size_t,
                void *, size_t *);
        using clCreateCommandQueueWithPropertiesFunc = cl_command_queue(CL_API_CALL *)(
                cl_context, cl_device_id, const cl_queue_properties *, cl_int *);
        using clGetExtensionFunctionAddressFunc = void *(CL_API_CALL *)(const char *);
#endif

#define OPENCL_DEFINE_FUNC_PTR(func) func##Func func = nullptr

        OPENCL_DEFINE_FUNC_PTR(clGetPlatformIDs);
        OPENCL_DEFINE_FUNC_PTR(clGetPlatformInfo);
        OPENCL_DEFINE_FUNC_PTR(clBuildProgram);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueNDRangeKernel);
        OPENCL_DEFINE_FUNC_PTR(clSetKernelArg);
        OPENCL_DEFINE_FUNC_PTR(clReleaseKernel);
        OPENCL_DEFINE_FUNC_PTR(clCreateProgramWithSource);
        OPENCL_DEFINE_FUNC_PTR(clCreateBuffer);
        OPENCL_DEFINE_FUNC_PTR(clCreateImage2D);
        OPENCL_DEFINE_FUNC_PTR(clCreateImage3D);
        OPENCL_DEFINE_FUNC_PTR(clRetainKernel);
        OPENCL_DEFINE_FUNC_PTR(clCreateKernel);
        OPENCL_DEFINE_FUNC_PTR(clGetProgramInfo);
        OPENCL_DEFINE_FUNC_PTR(clFlush);
        OPENCL_DEFINE_FUNC_PTR(clFinish);
        OPENCL_DEFINE_FUNC_PTR(clReleaseProgram);
        OPENCL_DEFINE_FUNC_PTR(clRetainContext);
        OPENCL_DEFINE_FUNC_PTR(clGetContextInfo);
        OPENCL_DEFINE_FUNC_PTR(clCreateProgramWithBinary);
        OPENCL_DEFINE_FUNC_PTR(clCreateCommandQueue);
        OPENCL_DEFINE_FUNC_PTR(clGetCommandQueueInfo);
        OPENCL_DEFINE_FUNC_PTR(clReleaseCommandQueue);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueMapBuffer);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueMapImage);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueCopyImage);
        OPENCL_DEFINE_FUNC_PTR(clRetainProgram);
        OPENCL_DEFINE_FUNC_PTR(clGetProgramBuildInfo);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueReadBuffer);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueWriteBuffer);
        OPENCL_DEFINE_FUNC_PTR(clWaitForEvents);
        OPENCL_DEFINE_FUNC_PTR(clReleaseEvent);
        OPENCL_DEFINE_FUNC_PTR(clCreateContext);
        OPENCL_DEFINE_FUNC_PTR(clCreateContextFromType);
        OPENCL_DEFINE_FUNC_PTR(clReleaseContext);
        OPENCL_DEFINE_FUNC_PTR(clRetainCommandQueue);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueUnmapMemObject);
        OPENCL_DEFINE_FUNC_PTR(clRetainMemObject);
        OPENCL_DEFINE_FUNC_PTR(clReleaseMemObject);
        OPENCL_DEFINE_FUNC_PTR(clGetDeviceInfo);
        OPENCL_DEFINE_FUNC_PTR(clGetDeviceIDs);
        OPENCL_DEFINE_FUNC_PTR(clRetainEvent);
        OPENCL_DEFINE_FUNC_PTR(clGetKernelWorkGroupInfo);
        OPENCL_DEFINE_FUNC_PTR(clGetEventInfo);
        OPENCL_DEFINE_FUNC_PTR(clGetEventProfilingInfo);
        OPENCL_DEFINE_FUNC_PTR(clGetImageInfo);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueCopyBufferToImage);
        OPENCL_DEFINE_FUNC_PTR(clEnqueueCopyImageToBuffer);
#if CL_HPP_TARGET_OPENCL_VERSION >= 120
        OPENCL_DEFINE_FUNC_PTR(clRetainDevice);
        OPENCL_DEFINE_FUNC_PTR(clReleaseDevice);
        OPENCL_DEFINE_FUNC_PTR(clCreateImage);
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 200
        OPENCL_DEFINE_FUNC_PTR(clGetKernelSubGroupInfoKHR);
        OPENCL_DEFINE_FUNC_PTR(clCreateCommandQueueWithProperties);
        OPENCL_DEFINE_FUNC_PTR(clGetExtensionFunctionAddress);
#endif

#undef OPENCL_DEFINE_FUNC_PTR

    private:
//        static void OpenCLLibInit();
        bool LoadLibraryFromPath();
    private:
        static std::shared_ptr<OpenCLLib> openClLibInstance;
        void *handle = nullptr;
    };
}


#endif //OPENCLDEMO_OPENCL_WRAPPER_H
