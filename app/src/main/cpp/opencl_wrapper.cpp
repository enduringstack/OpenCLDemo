#include <jni.h>
#include <memory>
#include <string>
#include <android/log.h>
#include <mutex>
#include <dlfcn.h>
#include <memory>
#include "opencl_wrapper.h"

using namespace hiai_opencl;

#define LOG_TAG "HiAI_OpenCL"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

std::shared_ptr<OpenCLLib> OpenCLLib::openClLibInstance = nullptr;

//void OpenCLLib::OpenCLLibInit() {
//    openClLibInstance = std::make_shared<OpenCLLib>();
//}

OpenCLLib::OpenCLLib() {
    LOGD("OpenCLLib()");
}

OpenCLLib *OpenCLLib::GetInstance() {
    static std::once_flag openclFunctionsOnce;
    std::call_once(openclFunctionsOnce, []() {openClLibInstance = std::make_shared<OpenCLLib>();});
//    std::call_once(openclFunctionsOnce, OpenCLLibInit);
    return openClLibInstance.get();
}

OpenCLLib::~OpenCLLib() {
    LOGI("~OpenCLLib begin.");
    if (openClLibInstance == nullptr) {
        return;
    }

    openClLibInstance->UnloadOpenCLLibrary();
    LOGI("~OpenCLLib end.");
}

bool OpenCLLib::LoadOpenCLLibrary() {
    return LoadLibraryFromPath();
}

bool OpenCLLib::UnloadOpenCLLibrary() {
    if (handle != nullptr) {
        if (dlclose(handle) != 0) {
            return false;
        }
        handle = nullptr;
        return true;
    }
    return true;
}

bool OpenCLLib::LoadLibraryFromPath() {
    if (handle != nullptr) {
        return true;
    }
    handle = dlopen("/vendor/lib64/libOpenCL.so", RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
        LOGE("dlopen libOpenCL fail.");
        return false;
    } else {
        LOGI("dlopen libOpenCL success.");
    }

#define LOAD_OPENCL_FUNCTION_PTR(functionName) \
    functionName = reinterpret_cast<functionName##Func>(dlsym(handle, #functionName)); \
    if (functionName == nullptr) { \
        LOGE("load func (%s) from libOpenCL failed!\n", #functionName); \
        return false; \
    } \

    LOAD_OPENCL_FUNCTION_PTR(clGetPlatformIDs);
    LOAD_OPENCL_FUNCTION_PTR(clGetPlatformInfo);
    LOAD_OPENCL_FUNCTION_PTR(clBuildProgram);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueNDRangeKernel);
    LOAD_OPENCL_FUNCTION_PTR(clSetKernelArg);
    LOAD_OPENCL_FUNCTION_PTR(clReleaseKernel);
    LOAD_OPENCL_FUNCTION_PTR(clCreateProgramWithSource);
    LOAD_OPENCL_FUNCTION_PTR(clCreateBuffer);
    LOAD_OPENCL_FUNCTION_PTR(clCreateImage2D);
    LOAD_OPENCL_FUNCTION_PTR(clCreateImage3D);
    LOAD_OPENCL_FUNCTION_PTR(clRetainKernel);
    LOAD_OPENCL_FUNCTION_PTR(clCreateKernel);
    LOAD_OPENCL_FUNCTION_PTR(clGetProgramInfo);
    LOAD_OPENCL_FUNCTION_PTR(clFlush);
    LOAD_OPENCL_FUNCTION_PTR(clFinish);
    LOAD_OPENCL_FUNCTION_PTR(clReleaseProgram);
    LOAD_OPENCL_FUNCTION_PTR(clRetainContext);
    LOAD_OPENCL_FUNCTION_PTR(clGetContextInfo);
    LOAD_OPENCL_FUNCTION_PTR(clCreateProgramWithBinary);
    LOAD_OPENCL_FUNCTION_PTR(clCreateCommandQueue);
    LOAD_OPENCL_FUNCTION_PTR(clGetCommandQueueInfo);
    LOAD_OPENCL_FUNCTION_PTR(clReleaseCommandQueue);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueMapBuffer);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueMapImage);
    LOAD_OPENCL_FUNCTION_PTR(clRetainProgram);
    LOAD_OPENCL_FUNCTION_PTR(clGetProgramBuildInfo);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueReadBuffer);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueWriteBuffer);
    LOAD_OPENCL_FUNCTION_PTR(clWaitForEvents);
    LOAD_OPENCL_FUNCTION_PTR(clReleaseEvent);
    LOAD_OPENCL_FUNCTION_PTR(clCreateContext);
    LOAD_OPENCL_FUNCTION_PTR(clCreateContextFromType);
    LOAD_OPENCL_FUNCTION_PTR(clReleaseContext);
    LOAD_OPENCL_FUNCTION_PTR(clRetainCommandQueue);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueUnmapMemObject);
    LOAD_OPENCL_FUNCTION_PTR(clRetainMemObject);
    LOAD_OPENCL_FUNCTION_PTR(clReleaseMemObject);
    LOAD_OPENCL_FUNCTION_PTR(clGetDeviceInfo);
    LOAD_OPENCL_FUNCTION_PTR(clGetDeviceIDs);
    LOAD_OPENCL_FUNCTION_PTR(clRetainEvent);
    LOAD_OPENCL_FUNCTION_PTR(clGetKernelWorkGroupInfo);
    LOAD_OPENCL_FUNCTION_PTR(clGetEventInfo);
    LOAD_OPENCL_FUNCTION_PTR(clGetEventProfilingInfo);
    LOAD_OPENCL_FUNCTION_PTR(clGetImageInfo);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueCopyImage);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueCopyBufferToImage);
    LOAD_OPENCL_FUNCTION_PTR(clEnqueueCopyImageToBuffer);
#if CL_HPP_TARGET_OPENCL_VERSION >= 120
    LOAD_OPENCL_FUNCTION_PTR(clRetainDevice);
    LOAD_OPENCL_FUNCTION_PTR(clReleaseDevice);
    LOAD_OPENCL_FUNCTION_PTR(clCreateImage);
#endif
#if CL_HPP_TARGET_OPENCL_VERSION >= 200
    LOAD_OPENCL_FUNCTION_PTR(clGetKernelSubGroupInfoKHR);
    LOAD_OPENCL_FUNCTION_PTR(clCreateCommandQueueWithProperties);
    LOAD_OPENCL_FUNCTION_PTR(clGetExtensionFunctionAddress);
#endif

#undef LOAD_OPENCL_FUNCTION_PTR

    return true;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_optimize_opencldemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    OpenCLLib::GetInstance()->LoadOpenCLLibrary();

    cl_int ret;
    cl_uint numPlatform;
    cl_platform_id *platform;
    ret = OpenCLLib::GetInstance()->clGetPlatformIDs(0, nullptr, &numPlatform);
    LOGI("num platform: %d", numPlatform);
    platform = (cl_platform_id*) malloc(sizeof(cl_platform_id) * numPlatform);
    ret = OpenCLLib::GetInstance()->clGetPlatformIDs(numPlatform, platform, nullptr);

    size_t size;
    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_NAME, 0, nullptr, &size);
    char* pName = (char*) malloc(size);
    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_NAME, size, pName, nullptr);
    LOGI("platform name: %s", pName);

    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_VENDOR, 0, nullptr, &size);
    char* pVendor = (char*) malloc(size);
    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_VENDOR, size, pVendor, nullptr);
    LOGI("vendor name: %s", pVendor);

    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_VERSION, 0, nullptr, &size);
    char* pVersion = (char*) malloc(size);
    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_VERSION, size, pVersion, nullptr);
    LOGI("platform version: %s", pVersion);

    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_PROFILE, 0, nullptr, &size);
    char* pProfile = (char*) malloc(size);
    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_PROFILE, size, pProfile, nullptr);
    LOGI("platform profile: %s", pProfile);

    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_EXTENSIONS, 0, nullptr, &size);
    char* pExtensions = (char*) malloc(size);
    ret = OpenCLLib::GetInstance()->clGetPlatformInfo(*platform, CL_PLATFORM_EXTENSIONS, size, pExtensions, nullptr);
    LOGI("platform extensions: %s", pExtensions);

    free(pName);
    free(pVendor);
    free(pProfile);
    free(pExtensions);

    return env->NewStringUTF(hello.c_str());
}

// clGetPlatformIDs wrapper, use OpenCLSymbols function. use OpenCLSymbols function.
cl_int CL_API_CALL clGetPlatformIDs(cl_uint num_entries, cl_platform_id *platforms, cl_uint *num_platforms) {
    auto func = OpenCLLib::GetInstance()->clGetPlatformIDs;
    CHECK_NOTNULL(func);
    return func(num_entries, platforms, num_platforms);
}

//clGetPlatformInfo wrapper, use OpenCLSymbols function. use OpenCLSymbols function.
cl_int CL_API_CALL clGetPlatformInfo(cl_platform_id platform, cl_platform_info param_name, size_t param_value_size,
                                     void *param_value, size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetPlatformInfo;
    CHECK_NOTNULL(func);
    return func(platform, param_name, param_value_size, param_value, param_value_size_ret);
}

//clGetDeviceIDs wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetDeviceIDs(cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id *devices,
                                  cl_uint *num_devices) {
    auto func = OpenCLLib::GetInstance()->clGetDeviceIDs;
    CHECK_NOTNULL(func);
    return func(platform, device_type, num_entries, devices, num_devices);
}

//clGetDeviceInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetDeviceInfo(cl_device_id device, cl_device_info param_name, size_t param_value_size, void *param_value,
                                   size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetDeviceInfo;
    CHECK_NOTNULL(func);
    return func(device, param_name, param_value_size, param_value, param_value_size_ret);
}

//clCreateContext wrapper, use OpenCLSymbols function.
cl_context CL_API_CALL clCreateContext(const cl_context_properties *properties, cl_uint num_devices, const cl_device_id *devices,
                                       void(CL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *), void *user_data,
                                       cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateContext;
    CHECK_NOTNULL(func);
    return func(properties, num_devices, devices, pfn_notify, user_data, errcode_ret);
}

//clCreateContextFromType wrapper, use OpenCLSymbols function.
cl_context CL_API_CALL clCreateContextFromType(const cl_context_properties *properties, cl_device_type device_type,
                                               void(CL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *),
                                               void *user_data, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateContextFromType;
    CHECK_NOTNULL(func);
    return func(properties, device_type, pfn_notify, user_data, errcode_ret);
}

//clRetainContext wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clRetainContext(cl_context context) {
    auto func = OpenCLLib::GetInstance()->clRetainContext;
    CHECK_NOTNULL(func);
    return func(context);
}

//clReleaseContext wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clReleaseContext(cl_context context) {
    auto func = OpenCLLib::GetInstance()->clReleaseContext;
    CHECK_NOTNULL(func);
    return func(context);
}

//clGetContextInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetContextInfo(cl_context context, cl_context_info param_name, size_t param_value_size, void *param_value,
                                    size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetContextInfo;
    CHECK_NOTNULL(func);
    return func(context, param_name, param_value_size, param_value, param_value_size_ret);
}

//clCreateProgramWithSource wrapper, use OpenCLSymbols function.
cl_program CL_API_CALL clCreateProgramWithSource(cl_context context, cl_uint count, const char **strings, const size_t *lengths,
                                                 cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateProgramWithSource;
    CHECK_NOTNULL(func);
    return func(context, count, strings, lengths, errcode_ret);
}

//clCreateProgramWithBinary wrapper, use OpenCLSymbols function.
cl_program CL_API_CALL clCreateProgramWithBinary(cl_context context, cl_uint count, const cl_device_id *device_list,
                                                 const size_t *length, const unsigned char **buffer,
                                                 cl_int *binary_status, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateProgramWithBinary;
    CHECK_NOTNULL(func);
    return func(context, count, device_list, length, buffer, binary_status, errcode_ret);
}

//clGetProgramInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetProgramInfo(cl_program program, cl_program_info param_name, size_t param_value_size, void *param_value,
                                    size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetProgramInfo;
    CHECK_NOTNULL(func);
    return func(program, param_name, param_value_size, param_value, param_value_size_ret);
}

//clGetProgramBuildInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetProgramBuildInfo(cl_program program, cl_device_id device, cl_program_build_info param_name,
                                         size_t param_value_size, void *param_value, size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetProgramBuildInfo;
    CHECK_NOTNULL(func);
    return func(program, device, param_name, param_value_size, param_value, param_value_size_ret);
}

//clRetainProgram wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clRetainProgram(cl_program program) {
    auto func = OpenCLLib::GetInstance()->clRetainProgram;
    CHECK_NOTNULL(func);
    return func(program);
}

//clReleaseProgram wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clReleaseProgram(cl_program program) {
    auto func = OpenCLLib::GetInstance()->clReleaseProgram;
    CHECK_NOTNULL(func);
    return func(program);
}

//clBuildProgram wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clBuildProgram(cl_program program, cl_uint num_devices, const cl_device_id *device_list, const char *options,
                                  void(CL_CALLBACK *pfn_notify)(cl_program program, void *user_data), void *user_data) {
    auto func = OpenCLLib::GetInstance()->clBuildProgram;
    CHECK_NOTNULL(func);
    return func(program, num_devices, device_list, options, pfn_notify, user_data);
}

//clCreateKernel wrapper, use OpenCLSymbols function.
cl_kernel CL_API_CALL clCreateKernel(cl_program program, const char *kernelName, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateKernel;
    CHECK_NOTNULL(func);
    return func(program, kernelName, errcode_ret);
}

//clRetainKernel wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clRetainKernel(cl_kernel kernel) {
    auto func = OpenCLLib::GetInstance()->clRetainKernel;
    CHECK_NOTNULL(func);
    return func(kernel);
}

//clReleaseKernel wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clReleaseKernel(cl_kernel kernel) {
    auto func = OpenCLLib::GetInstance()->clReleaseKernel;
    CHECK_NOTNULL(func);
    return func(kernel);
}

//clSetKernelArg wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clSetKernelArg(cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value) {
    auto func = OpenCLLib::GetInstance()->clSetKernelArg;
    CHECK_NOTNULL(func);
    return func(kernel, arg_index, arg_size, arg_value);
}

//clCreateBuffer wrapper, use OpenCLSymbols function.
cl_mem CL_API_CALL clCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateBuffer;
    CHECK_NOTNULL(func);
    return func(context, flags, size, host_ptr, errcode_ret);
}

//clRetainMemObject wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clRetainMemObject(cl_mem memobj) {
    auto func = OpenCLLib::GetInstance()->clRetainMemObject;
    CHECK_NOTNULL(func);
    return func(memobj);
}

//clReleaseMemObject wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clReleaseMemObject(cl_mem memobj) {
    auto func = OpenCLLib::GetInstance()->clReleaseMemObject;
    CHECK_NOTNULL(func);
    return func(memobj);
}

//clGetImageInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetImageInfo(cl_mem image, cl_image_info param_name, size_t param_value_size, void *param_value,
                                  size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetImageInfo;
    CHECK_NOTNULL(func);
    return func(image, param_name, param_value_size, param_value, param_value_size_ret);
}

//clRetainCommandQueue wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clRetainCommandQueue(cl_command_queue command_queue) {
    auto func = OpenCLLib::GetInstance()->clRetainCommandQueue;
    CHECK_NOTNULL(func);
    return func(command_queue);
}

//clReleaseCommandQueue wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clReleaseCommandQueue(cl_command_queue command_queue) {
    auto func = OpenCLLib::GetInstance()->clReleaseCommandQueue;
    CHECK_NOTNULL(func);
    return func(command_queue);
}

//clEnqueueReadBuffer wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clEnqueueReadBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset,
                                       size_t size, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                                       cl_event *event) {
    auto func = OpenCLLib::GetInstance()->clEnqueueReadBuffer;
    CHECK_NOTNULL(func);
    return func(command_queue, buffer, blocking_read, offset, size, ptr, num_events_in_wait_list, event_wait_list,
                event);
}

//clEnqueueWriteBuffer wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clEnqueueWriteBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset,
                                        size_t size, const void *ptr, cl_uint num_events_in_wait_list,
                                        const cl_event *event_wait_list, cl_event *event) {
    auto func = OpenCLLib::GetInstance()->clEnqueueWriteBuffer;
    CHECK_NOTNULL(func);
    return func(command_queue, buffer, blocking_write, offset, size, ptr, num_events_in_wait_list, event_wait_list,
                event);
}

//clEnqueueMapBuffer wrapper, use OpenCLSymbols function.
void *CL_API_CALL clEnqueueMapBuffer(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_map, cl_map_flags map_flags,
                                     size_t offset, size_t size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
                                     cl_event *event, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clEnqueueMapBuffer;
    CHECK_NOTNULL(func);
    return func(command_queue, buffer, blocking_map, map_flags, offset, size, num_events_in_wait_list, event_wait_list,
                event, errcode_ret);
}

//clEnqueueMapImage wrapper, use OpenCLSymbols function.
void *CL_API_CALL clEnqueueMapImage(cl_command_queue command_queue, cl_mem image, cl_bool blocking_map, cl_map_flags map_flags,
                                    const size_t *origin, const size_t *region, size_t *image_row_pitch, size_t *image_slice_pitch,
                                    cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event,
                                    cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clEnqueueMapImage;
    CHECK_NOTNULL(func);
    return func(command_queue, image, blocking_map, map_flags, origin, region, image_row_pitch, image_slice_pitch,
                num_events_in_wait_list, event_wait_list, event, errcode_ret);
}

//clEnqueueUnmapMemObject wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clEnqueueUnmapMemObject(cl_command_queue command_queue, cl_mem memobj, void *mapped_ptr,
                                           cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    auto func = OpenCLLib::GetInstance()->clEnqueueUnmapMemObject;
    CHECK_NOTNULL(func);
    return func(command_queue, memobj, mapped_ptr, num_events_in_wait_list, event_wait_list, event);
}

//clGetKernelWorkGroupInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetKernelWorkGroupInfo(cl_kernel kernel, cl_device_id device, cl_kernel_work_group_info param_name,
                                            size_t param_value_size, void *param_value, size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetKernelWorkGroupInfo;
    CHECK_NOTNULL(func);
    return func(kernel, device, param_name, param_value_size, param_value, param_value_size_ret);
}

//clGetEventProfilingInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetEventProfilingInfo(cl_event event, cl_profiling_info param_name, size_t param_value_size, void *param_value,
                                           size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetEventProfilingInfo;
    CHECK_NOTNULL(func);
    return func(event, param_name, param_value_size, param_value, param_value_size_ret);
}

//clEnqueueNDRangeKernel wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clEnqueueNDRangeKernel(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim,
                                          const size_t *global_work_offset, const size_t *global_work_size,
                                          const size_t *local_work_size, cl_uint num_events_in_wait_list,
                                          const cl_event *event_wait_list, cl_event *event) {
    auto func = OpenCLLib::GetInstance()->clEnqueueNDRangeKernel;
    CHECK_NOTNULL(func);
    return func(command_queue, kernel, work_dim, global_work_offset, global_work_size, local_work_size,
                num_events_in_wait_list, event_wait_list, event);
}

//clWaitForEvents wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clWaitForEvents(cl_uint num_events, const cl_event *event_list) {
    auto func = OpenCLLib::GetInstance()->clWaitForEvents;
    CHECK_NOTNULL(func);
    return func(num_events, event_list);
}

//clRetainEvent wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clRetainEvent(cl_event event) {
    auto func = OpenCLLib::GetInstance()->clRetainEvent;
    CHECK_NOTNULL(func);
    return func(event);
}

//clReleaseEvent wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clReleaseEvent(cl_event event) {
    auto func = OpenCLLib::GetInstance()->clReleaseEvent;
    CHECK_NOTNULL(func);
    return func(event);
}

//clGetEventInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetEventInfo(cl_event event, cl_event_info param_name, size_t param_value_size, void *param_value,
                                  size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetEventInfo;
    CHECK_NOTNULL(func);
    return func(event, param_name, param_value_size, param_value, param_value_size_ret);
}

//clFlush wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clFlush(cl_command_queue command_queue) {
    auto func = OpenCLLib::GetInstance()->clFlush;
    CHECK_NOTNULL(func);
    return func(command_queue);
}

//clFinish wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clFinish(cl_command_queue command_queue) {
    auto func = OpenCLLib::GetInstance()->clFinish;
    CHECK_NOTNULL(func);
    return func(command_queue);
}

//clCreateImage2D wrapper, use OpenCLSymbols function.
cl_mem CL_API_CALL clCreateImage2D(cl_context context, cl_mem_flags flags, const cl_image_format *image_format, size_t imageWidth,
                                   size_t imageHeight, size_t image_row_pitch, void *host_ptr, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateImage2D;
    CHECK_NOTNULL(func);
    return func(context, flags, image_format, imageWidth, imageHeight, image_row_pitch, host_ptr, errcode_ret);
}

//clCreateImage3D wrapper, use OpenCLSymbols function.
cl_mem CL_API_CALL clCreateImage3D(cl_context context, cl_mem_flags flags, const cl_image_format *image_format, size_t imageWidth,
                                   size_t imageHeight, size_t imageDepth, size_t image_row_pitch, size_t image_slice_pitch,
                                   void *host_ptr, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateImage3D;
    CHECK_NOTNULL(func);
    return func(context, flags, image_format, imageWidth, imageHeight, imageDepth, image_row_pitch, image_slice_pitch,
                host_ptr, errcode_ret);
}

//clCreateCommandQueue wrapper, use OpenCLSymbols function.
cl_command_queue CL_API_CALL clCreateCommandQueue(cl_context context, cl_device_id device, cl_command_queue_properties properties,
                                                  cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateCommandQueue;
    CHECK_NOTNULL(func);
    return func(context, device, properties, errcode_ret);
}

//clGetCommandQueueInfo wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetCommandQueueInfo(cl_command_queue command_queue, cl_command_queue_info param_name, size_t param_value_size,
                                         void *param_value, size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetCommandQueueInfo;
    CHECK_NOTNULL(func);
    return func(command_queue, param_name, param_value_size, param_value, param_value_size_ret);
}

//clEnqueueCopyImage wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clEnqueueCopyImage(cl_command_queue queue, cl_mem src_image, cl_mem dst_image, const size_t *src_origin,
                                      const size_t *dst_origin, const size_t *region, cl_uint num_events_in_wait_list,
                                      const cl_event *event_wait_list, cl_event *event) {
    auto func = OpenCLLib::GetInstance()->clEnqueueCopyImage;
    CHECK_NOTNULL(func);
    return func(queue, src_image, dst_image, src_origin, dst_origin, region, num_events_in_wait_list, event_wait_list,
                event);
}

//clEnqueueCopyBufferToImage wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clEnqueueCopyBufferToImage(cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_image,
                                              size_t src_offset, const size_t *dst_origin, const size_t *region,
                                              cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    auto func = OpenCLLib::GetInstance()->clEnqueueCopyBufferToImage;
    CHECK_NOTNULL(func);
    return func(command_queue, src_buffer, dst_image, src_offset, dst_origin, region, num_events_in_wait_list,
                event_wait_list, event);
}

//clEnqueueCopyImageToBuffer wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clEnqueueCopyImageToBuffer(cl_command_queue command_queue, cl_mem src_image, cl_mem dst_buffer,
                                              const size_t *src_origin, const size_t *region, size_t dst_offset,
                                              cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) {
    auto func = OpenCLLib::GetInstance()->clEnqueueCopyImageToBuffer;
    CHECK_NOTNULL(func);
    return func(command_queue, src_image, dst_buffer, src_origin, region, dst_offset, num_events_in_wait_list,
                event_wait_list, event);
}

#if CL_HPP_TARGET_OPENCL_VERSION >= 120

//clRetainDevice wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clRetainDevice(cl_device_id device) {
    auto func = OpenCLLib::GetInstance()->clRetainDevice;
    CHECK_NOTNULL(func);
    return func(device);
}

//clReleaseDevice wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clReleaseDevice(cl_device_id device) {
    auto func = OpenCLLib::GetInstance()->clReleaseDevice;
    CHECK_NOTNULL(func);
    return func(device);
}

//clCreateImage wrapper, use OpenCLSymbols function.
cl_mem CL_API_CALL clCreateImage(cl_context context, cl_mem_flags flags, const cl_image_format *image_format,
                     const cl_image_desc *image_desc, void *host_ptr, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateImage;
    CHECK_NOTNULL(func);
    return func(context, flags, image_format, image_desc, host_ptr, errcode_ret);
}

#endif

#if CL_HPP_TARGET_OPENCL_VERSION >= 200

//clGetKernelSubGroupInfoKHR wrapper, use OpenCLSymbols function.
cl_int CL_API_CALL clGetKernelSubGroupInfoKHR(cl_kernel kernel, cl_device_id device, cl_kernel_sub_group_info param_name,
                                  size_t input_value_size, const void *input_value, size_t param_value_size,
                                  void *param_value, size_t *param_value_size_ret) {
    auto func = OpenCLLib::GetInstance()->clGetKernelSubGroupInfoKHR;
    CHECK_NOTNULL(func);
    return func(kernel, device, param_name, input_value_size, input_value, param_value_size, param_value,
                param_value_size_ret);
}

//clCreateCommandQueueWithProperties wrapper, use OpenCLSymbols function.
cl_command_queue CL_API_CALL clCreateCommandQueueWithProperties(cl_context context, cl_device_id device,
                                                    const cl_queue_properties *properties, cl_int *errcode_ret) {
    auto func = OpenCLLib::GetInstance()->clCreateCommandQueueWithProperties;
    CHECK_NOTNULL(func);
    return func(context, device, properties, errcode_ret);
}

//clGetExtensionFunctionAddress wrapper, use OpenCLSymbols function.
void *CL_API_CALL clGetExtensionFunctionAddress(const char *func_name) {
    auto func = OpenCLLib::GetInstance()->clGetExtensionFunctionAddress;
    CHECK_NOTNULL(func);
    return func(func_name);
}
#endif

