#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef int32_t OFX_Status;

typedef double OFX_Time;

typedef struct OFX_Rgba_u8  { uint8_t r, g, b, a; } OFX_Rgba_u8;
typedef struct OFX_Rgba_u16 { uint16_t r, g, b, a; } OFX_Rgba_u16;
typedef struct OFX_Rgba_f32 { float r, g, b, a; } OFX_Rgba_f32;
typedef struct OFX_Rgba_f64 { double r, g, b, a; } OFX_Rgba_f64;

typedef struct OFX_Rgb_u8  { uint8_t r, g, b; } OFX_Rgb_u8;
typedef struct OFX_Rgb_u16 { uint16_t r, g, b; } OFX_Rgb_u16;
typedef struct OFX_Rgb_f32 { float r, g, b; } OFX_Rgb_f32;
typedef struct OFX_Rgb_f64 { double r, g, b; } OFX_Rgb_f64;

typedef struct OFX_Point_i32 { int32_t x, y; } OFX_Point_i32;
typedef struct OFX_Point_f64 { double x, y; } OFX_Point_f64;

typedef struct OFX_Range_i32 { int32_t min, max; } OFX_Range_i32;
typedef struct OFX_Range_f64 { double min, max; } OFX_Range_f64;

typedef struct OFX_Rect_i32 { int32_t x1, y1, x2, y2; } OFX_Rect_i32;
typedef struct OFX_Rect_f64 { double x1, y1, x2, y2; } OFX_Rect_f64;

typedef void *OFX_PropertySetHandle;
typedef void *OFX_DrawContextHandle;
typedef void *OFX_ImageEffectHandle;
typedef void *OFX_ImageClipHandle;
typedef void *OFX_ImageMemoryHandle;
typedef void *OFX_InteractHandle;
typedef void *OFX_ParamHandle;
typedef void *OFX_ParamSetHandle;
typedef void *OFX_MutexHandle;

typedef void (*OFX_ThreadFunctionV1)(uint32_t thread_index, uint32_t thread_max, void *custom_arg);

typedef struct OFX_Bytes OFX_Bytes;
struct OFX_Bytes {
    uint8_t *data;
    size_t length;
};

typedef enum OFX_StandardColour OFX_StandardColour;
enum OFX_StandardColour {
    OFX_StandardColour_OverlayBackground,
    OFX_StandardColour_OverlayActive,
    OFX_StandardColour_OverlaySelected,
    OFX_StandardColour_OverlayDeselected,
    OFX_StandardColour_OverlayMarqueeFG,
    OFX_StandardColour_OverlayMarqueeBG,
    OFX_StandardColour_OverlayText,
};

typedef enum OFX_DrawLineStipplePattern OFX_DrawLineStipplePattern;
enum OFX_DrawLineStipplePattern {
    OFX_DrawLineStipplePattern_Solid,
    OFX_DrawLineStipplePattern_Dot,
    OFX_DrawLineStipplePattern_Dash,
    OFX_DrawLineStipplePattern_AltDash,
    OFX_DrawLineStipplePattern_DotDash,
};

typedef enum OFX_DrawPrimitive OFX_DrawPrimitive;
enum OFX_DrawPrimitive {
    OFX_DrawPrimitive_Lines,
    OFX_DrawPrimitive_LineStrip,
    OFX_DrawPrimitive_LineLoop,
    OFX_DrawPrimitive_Rectangle,
    OFX_DrawPrimitive_Polygon,
    OFX_DrawPrimitive_Ellipse,
};

typedef enum OFX_DrawTextAlignment OFX_DrawTextAlignment;
enum OFX_DrawTextAlignment {
    OFX_DrawTextAlignment_Left     = 1 << 0,
    OFX_DrawTextAlignment_Right    = 1 << 1,
    OFX_DrawTextAlignment_Top      = 1 << 2,
    OFX_DrawTextAlignment_Bottom   = 1 << 3,
    OFX_DrawTextAlignment_Baseline = 1 << 4,
    OFX_DrawTextAlignment_CenterH  = OFX_DrawTextAlignment_Left | OFX_DrawTextAlignment_Right,
    OFX_DrawTextAlignment_CenterV  = OFX_DrawTextAlignment_Top | OFX_DrawTextAlignment_Baseline,
};

typedef struct OFX_Host OFX_Host;
struct OFX_Host {
    OFX_PropertySetHandle host;
    void *(*fetch_suite)(OFX_PropertySetHandle host, char *suite_name, int32_t suite_version);
};

typedef struct OFX_Plugin OFX_Plugin;
struct OFX_Plugin {
    char *plugin_api;
    int32_t api_version;
    char *plugin_identifier;
    uint32_t plugin_version_major;
    uint32_t plugin_version_minor;

    void (*set_host)(OFX_Host *host);
    OFX_Status (*main_entry)(char *action, OFX_ImageEffectHandle handle, OFX_PropertySetHandle in_args, OFX_PropertySetHandle out_args);
};

typedef struct OFX_DialogSuiteV1 OFX_DialogSuiteV1;
struct OFX_DialogSuiteV1 {
    OFX_Status (*request_dialog)(void *user_data);
    OFX_Status (*notify_redraw_rending)();
};

typedef struct OFX_DrawSuiteV1 OFX_DrawSuiteV1;
struct OFX_DrawSuiteV1 {
    OFX_Status (*get_colour)(OFX_DrawContextHandle context, OFX_StandardColour std_colour, OFX_Rgba_f32 *colour);
    OFX_Status (*set_colour)(OFX_DrawContextHandle context, OFX_Rgba_f32 *colour);

    OFX_Status (*set_line_width)(OFX_DrawContextHandle context, float width);
    OFX_Status (*set_line_stipple)(OFX_DrawContextHandle context, OFX_DrawLineStipplePattern pattern);

    OFX_Status (*draw)(OFX_DrawContextHandle context, OFX_DrawPrimitive primitive, OFX_Point_f64 *points, int32_t point_count);
    OFX_Status (*draw_text)(OFX_DrawContextHandle context, char *text, OFX_Point_f64 *pos, OFX_DrawTextAlignment alignment);
};

typedef struct OFX_ImageEffectOpenGLRenderSuiteV1 OFX_ImageEffectOpenGLRenderSuiteV1;
struct OFX_ImageEffectOpenGLRenderSuiteV1 {
    OFX_Status (*clip_load_texture)(OFX_ImageClipHandle clip, OFX_Time time, char *format, OFX_Rect_f64 *region, OFX_PropertySetHandle *texture_handle);
    OFX_Status (*clip_free_texture)(OFX_PropertySetHandle texture_handle);
    OFX_Status (*flush_resources)();
};

typedef struct OFX_ImageEffectSuiteV1 OFX_ImageEffectSuiteV1;
struct OFX_ImageEffectSuiteV1 {
    OFX_Status (*get_property_set)(OFX_ImageEffectHandle image_effect, OFX_PropertySetHandle *prop_handle);
    OFX_Status (*get_param_set)(OFX_ImageEffectHandle image_effect, OFX_ParamSetHandle *param_set);

    OFX_Status (*clip_define)(OFX_ImageEffectHandle image_effect, char *name, OFX_PropertySetHandle *property_set);
    OFX_Status (*clip_get_handle)(OFX_ImageEffectHandle image_effect, char *name, OFX_ImageClipHandle *clip, OFX_PropertySetHandle *property_set);
    OFX_Status (*clip_get_property_set)(OFX_ImageClipHandle clip, OFX_PropertySetHandle *prop_handle);
    OFX_Status (*clip_get_image)(OFX_ImageClipHandle clip, OFX_Time time, OFX_Rect_f64 *region, OFX_PropertySetHandle *image_handle);
    OFX_Status (*clip_release_image)(OFX_PropertySetHandle image_handle);
    OFX_Status (*clip_get_region_of_definition)(OFX_ImageClipHandle clip, OFX_Time time, OFX_Rect_f64 *bounds);

    int32_t (*abort)(OFX_ImageEffectHandle image_effect);

    OFX_Status (*image_memory_alloc)(OFX_ImageEffectHandle instance_handle, size_t n_bytes, OFX_ImageMemoryHandle *memory_handle);
    OFX_Status (*image_memory_free)(OFX_ImageMemoryHandle memory_handle);
    OFX_Status (*image_memory_lock)(OFX_ImageMemoryHandle memory_handle, void **returned_ptr);
    OFX_Status (*image_memory_unlock)(OFX_ImageMemoryHandle memory_handle);
};

typedef struct OFX_InteractSuiteV1 OFX_InteractSuiteV1;
struct OFX_InteractSuiteV1 {
    OFX_Status (*interact_swap_buffers)(OFX_InteractHandle interact_instance);
    OFX_Status (*interact_redraw)(OFX_InteractHandle interact_instance);
    OFX_Status (*interact_get_propertyset)(OFX_InteractHandle interact_instance, OFX_PropertySetHandle *property);
};

typedef struct OFX_MemorySuiteV1 OFX_MemorySuiteV1;
struct OFX_MemorySuiteV1 {
    OFX_Status (*memory_alloc)(void *handle, size_t n_bytes, void **allocated_data);
    OFX_Status (*memory_free)(void *allocated_data);
};

typedef struct OFX_MessageSuiteV1 OFX_MessageSuiteV1;
struct OFX_MessageSuiteV1 {
    OFX_Status (*message)(OFX_ImageEffectHandle handle, char *message_type, char *message_id, char *format, ...);
};

typedef struct OFX_MessageSuiteV2 OFX_MessageSuiteV2;
struct OFX_MessageSuiteV2 {
    OFX_Status (*message)(OFX_ImageEffectHandle handle, char *message_type, char *message_id, char *format, ...);
    OFX_Status (*set_persistent_message)(OFX_ImageEffectHandle handle, char *message_type, char *message_id, char *format, ...);
    OFX_Status (*clear_persistent_message)(OFX_ImageEffectHandle handle);
};

typedef struct OFX_MultithreadSuiteV1 OFX_MultithreadSuiteV1;
struct OFX_MultithreadSuiteV1 {
    OFX_Status (*multithread)(OFX_ThreadFunctionV1 func, uint32_t n_threads, void *custom_arg);
    OFX_Status (*multithread_num_cpus)(uint32_t *n_cpus);

    OFX_Status (*multithread_index)(uint32_t *thread_index);
    int32_t (*multithread_is_spawned_thread)();

    OFX_Status (*mutex_create)(OFX_MutexHandle mutex, int32_t lock_count);
    OFX_Status (*mutex_destroy)(OFX_MutexHandle mutex);
    OFX_Status (*mutex_lock)(OFX_MutexHandle mutex);
    OFX_Status (*mutex_unlock)(OFX_MutexHandle mutex);
    OFX_Status (*mutex_try_lock)(OFX_MutexHandle mutex);
};

typedef struct OFX_OpenCLProgramSuiteV1 OFX_OpenCLProgramSuiteV1;
struct OFX_OpenCLProgramSuiteV1 {
    OFX_Status (*compile_program)(char *psz_program_source, int32_t f_optional, void *p_result);
};

typedef struct OFX_ParameterSuiteV1 OFX_ParameterSuiteV1;
struct OFX_ParameterSuiteV1 {
    OFX_Status (*param_define)(OFX_ParamSetHandle param_set, char *param_type, char *name, OFX_PropertySetHandle *property_set);
    OFX_Status (*param_get_handle)(OFX_ParamSetHandle param_set, char *name, OFX_ParamHandle *param, OFX_PropertySetHandle *property_set);
    
    OFX_Status (*param_set_get_property_set)(OFX_ParamSetHandle param_set, OFX_PropertySetHandle *prop_handle);
    OFX_Status (*param_get_property_set)(OFX_ParamHandle param, OFX_PropertySetHandle *prop_handle);

    OFX_Status (*param_get_value)(OFX_ParamHandle param_handle, ...);
    OFX_Status (*param_get_value_at_time)(OFX_ParamHandle param_handle, OFX_Time time, ...);
    OFX_Status (*param_get_derivative)(OFX_ParamHandle param_handle, OFX_Time time, ...);
    OFX_Status (*param_get_integral)(OFX_ParamHandle param_handle, OFX_Time time1, OFX_Time time2, ...);

    OFX_Status (*param_set_value)(OFX_ParamHandle param_handle, ...);
    OFX_Status (*param_set_value_at_time)(OFX_ParamHandle param_handle, OFX_Time time, ...);

    OFX_Status (*param_get_num_keys)(OFX_ParamHandle param_handle, uint32_t *number_of_keys);
    OFX_Status (*param_get_key_time)(OFX_ParamHandle param_handle, uint32_t nth_key, OFX_Time *time);
    OFX_Status (*param_get_key_index)(OFX_ParamHandle param_handle, OFX_Time time, int32_t direction, int32_t *index);

    OFX_Status (*param_delete_key)(OFX_ParamHandle param_handle, OFX_Time time);
    OFX_Status (*param_delete_all_keys)(OFX_ParamHandle param_handle);

    OFX_Status (*param_copy)(OFX_ParamHandle param_to, OFX_ParamHandle param_from, OFX_Time dst_offset, OFX_Range_f64 *frame_range);

    OFX_Status (*param_edit_begin)(OFX_ParamSetHandle param_set, char *name);
    OFX_Status (*param_edit_end)(OFX_ParamSetHandle param_set);
};

typedef struct OFX_ParametricParameterSuiteV1 OFX_ParametricParameterSuiteV1;
struct OFX_ParametricParameterSuiteV1 {
    OFX_Status (*parametric_param_get_value)           (OFX_ParamHandle param, int32_t curve_index, OFX_Time time, double parametric_position, double *return_value);
    OFX_Status (*parametric_param_get_n_control_points)(OFX_ParamHandle param, int32_t curve_index, OFX_Time time, int32_t *return_value);
    OFX_Status (*parametric_param_get_nth_control_point)(OFX_ParamHandle param, int32_t curve_index, OFX_Time time, int32_t nth_ctrl, double *key, double *value);

    OFX_Status (*parametric_param_set_nth_control_point)(OFX_ParamHandle param, int32_t curve_index, OFX_Time time, int32_t nth_ctrl, double key, double value, bool add_animation_key);

    OFX_Status (*parametric_param_add_control_point)(OFX_ParamHandle param, int32_t curve_index, OFX_Time time, double key, double value, bool add_animation_key);

    OFX_Status (*parametric_param_delete_control_point)(OFX_ParamHandle param, int32_t curve_index, int32_t nth_ctrl);
    OFX_Status (*parametric_param_delete_all_control_points)(OFX_ParamHandle param, int32_t curve_index);
};

typedef struct OFX_ProgressSuiteV1 OFX_ProgressSuiteV1;
struct OFX_ProgressSuiteV1 {
    OFX_Status (*progress_start)(OFX_ImageEffectHandle instance, char *label);
    OFX_Status (*progress_update)(OFX_ImageEffectHandle instance, double progress);
    OFX_Status (*progress_end)(OFX_ImageEffectHandle instance);
};

typedef struct OFX_ProgressSuiteV2 OFX_ProgressSuiteV2;
struct OFX_ProgressSuiteV2 {
    OFX_Status (*progress_start)(OFX_ImageEffectHandle instance, char *message, char *messageid);
    OFX_Status (*progress_update)(OFX_ImageEffectHandle instance, double progress);
    OFX_Status (*progress_end)(OFX_ImageEffectHandle instance);
};

typedef struct OFX_PropertySuiteV1 OFX_PropertySuiteV1;
struct OFX_PropertySuiteV1 {
    OFX_Status (*prop_set_pointer)(OFX_PropertySetHandle properties, char *property, int32_t index, void *value);
    OFX_Status (*prop_set_string) (OFX_PropertySetHandle properties, char *property, int32_t index, char *value);
    OFX_Status (*prop_set_double) (OFX_PropertySetHandle properties, char *property, int32_t index, double value);
    OFX_Status (*prop_set_int)    (OFX_PropertySetHandle properties, char *property, int32_t index, int32_t value);

    OFX_Status (*prop_set_pointer_n)(OFX_PropertySetHandle properties, char *property, int32_t count, void **value);
    OFX_Status (*prop_set_string_n) (OFX_PropertySetHandle properties, char *property, int32_t count, char **value);
    OFX_Status (*prop_set_double_n) (OFX_PropertySetHandle properties, char *property, int32_t count, double *value);
    OFX_Status (*prop_set_int_n)    (OFX_PropertySetHandle properties, char *property, int32_t count, int32_t *value);

    OFX_Status (*prop_get_pointer)(OFX_PropertySetHandle properties, char *property, int32_t index, void **value);
    OFX_Status (*prop_get_string) (OFX_PropertySetHandle properties, char *property, int32_t index, char **value);
    OFX_Status (*prop_get_double) (OFX_PropertySetHandle properties, char *property, int32_t index, double *value);
    OFX_Status (*prop_get_int)    (OFX_PropertySetHandle properties, char *property, int32_t index, int32_t *value);

    OFX_Status (*prop_get_pointer_n)(OFX_PropertySetHandle properties, char *property, int32_t count, void **value);
    OFX_Status (*prop_get_string_n) (OFX_PropertySetHandle properties, char *property, int32_t count, char **value);
    OFX_Status (*prop_get_double_n) (OFX_PropertySetHandle properties, char *property, int32_t count, double *value);
    OFX_Status (*prop_get_int_n)    (OFX_PropertySetHandle properties, char *property, int32_t count, int32_t *value);

    OFX_Status (*prop_reset)(OFX_PropertySetHandle properties, char *property);
    OFX_Status (*prop_get_dimension)(OFX_PropertySetHandle properties, char *property, int32_t *count);
};

typedef struct OFX_TimelineSuiteV1 OFX_TimelineSuiteV1;
struct OFX_TimelineSuiteV1 {
    OFX_Status (*get_time)(OFX_ImageEffectHandle instance, OFX_Time *time);
    OFX_Status (*go_to_time)(OFX_ImageEffectHandle instance, OFX_Time time);
    OFX_Status (*get_time_bounds)(OFX_ImageEffectHandle instance, OFX_Time *first_time, OFX_Time *last_time);
};

#if defined(__cplusplus)
    #define ofx_extern extern "C"
#else
    #define ofx_extern extern
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define ofx_export ofx_extern __declspec(dllexport)
#else
    #define ofx_export ofx_extern __attribute__((visibility("default")))
#endif

#define OFX_FLAG_INFINITE_MAX INT_MAX
#define OFX_FLAG_INFINITE_MIN INT_MIN

#define OFX_OK                       0
#define OFX_FAILED                   1
#define OFX_ERR_FATAL                2
#define OFX_ERR_UNKNOWN              3
#define OFX_ERR_MISSING_HOST_FEATURE 4
#define OFX_ERR_UNSUPPORTED          5
#define OFX_ERR_EXISTS               6
#define OFX_ERR_FORMAT               7
#define OFX_ERR_MEMORY               8
#define OFX_ERR_BADHANDLE            9
#define OFX_ERR_BADINDEX             10
#define OFX_ERR_VALUE                11
#define OFX_REPLY_YES                12
#define OFX_REPLY_NO                 13
#define OFX_REPLY_DEFAULT            14
#define OFX_ERR_IMAGE_FORMAT         1000
#define OFX_GPU_OUT_OF_MEMORY        1001
#define OFX_GL_OUT_OF_MEMORY         1001
#define OFX_GPU_RENDER_FAILED        1002
#define OFX_GL_RENDER_FAILED         1002

#define OFX_DIALOG_SUITE                                          "OfxDialogSuite"
#define OFX_DRAW_SUITE                                            "OfxDrawSuite"
#define OFX_INTERACT_SUITE                                        "OfxInteractSuite"
#define OFX_MEMORY_SUITE                                          "OfxMemorySuite"
#define OFX_MESSAGE_SUITE                                         "OfxMessageSuite"
#define OFX_MULTITHREAD_SUITE                                     "OfxMultiThreadSuite"
#define OFX_PROGRESS_SUITE                                        "OfxProgressSuite"
#define OFX_PROPERTY_SUITE                                        "OfxPropertySuite"
#define OFX_TIMELINE_SUITE                                        "OfxTimeLineSuite"
#define OFX_PARAMETER_SUITE                                       "OfxParameterSuite"
#define OFX_PARAMETRIC_PARAMETER_SUITE                            "OfxParametricParameterSuite"
#define OFX_OPENGL_RENDER_SUITE                                   "OfxImageEffectOpenGLRenderSuite"
#define OFX_OPENCL_PROGRAM_SUITE                                  "OfxOpenCLProgramSuite"
#define OFX_IMAGE_EFFECT_SUITE                                    "OfxImageEffectSuite"

#define OFX_PROP_API_VERSION                                      "OfxPropAPIVersion"
#define OFX_PROP_TIME                                             "OfxPropTime"
#define OFX_PROP_IS_INTERACTIVE                                   "OfxPropIsInteractive"
#define OFX_PROP_INSTANCE_DATA                                    "OfxPropInstanceData"
#define OFX_PROP_TYPE                                             "OfxPropType"
#define OFX_PROP_NAME                                             "OfxPropName"
#define OFX_PROP_VERSION                                          "OfxPropVersion"
#define OFX_PROP_VERSION_LABEL                                    "OfxPropVersionLabel"
#define OFX_PROP_PLUGIN_DESCRIPTION                               "OfxPropPluginDescription"
#define OFX_PROP_LABEL                                            "OfxPropLabel"
#define OFX_PROP_ICON                                             "OfxPropIcon"
#define OFX_PROP_SHORT_LABEL                                      "OfxPropShortLabel"
#define OFX_PROP_LONG_LABEL                                       "OfxPropLongLabel"
#define OFX_PROP_CHANGE_REASON                                    "OfxPropChangeReason"
#define OFX_PROP_EFFECT_INSTANCE                                  "OfxPropEffectInstance"
#define OFX_PROP_HOST_OS_HANDLE                                   "OfxPropHostOSHandle"
#define OFX_PROP_KEY_SYM                                          "kOfxPropKeySym"
#define OFX_PROP_KEY_STRING                                       "kOfxPropKeyString"
#define OFX_PROP_PARAM_SET_NEEDS_SYNCING                          "OfxPropParamSetNeedsSyncing"

#define OFX_HOST_NATIVE_ORIGIN_BOTTOM_LEFT                        "kOfxImageEffectHostPropNativeOriginBottomLeft"
#define OFX_HOST_NATIVE_ORIGIN_TOP_LEFT                           "kOfxImageEffectHostPropNativeOriginTopLeft"
#define OFX_HOST_NATIVE_ORIGIN_CENTER                             "kOfxImageEffectHostPropNativeOriginCenter"

#define OFX_PLUGIN_PROP_FILE_PATH                                 "OfxPluginPropFilePath"
#define OFX_PLUGIN_PROP_PARAM_PAGE_ORDER                          "OfxPluginPropParamPageOrder"

#define OFX_CHANGE_USER_EDITED                                    "OfxChangeUserEdited"
#define OFX_CHANGE_PLUGIN_EDITED                                  "OfxChangePluginEdited"
#define OFX_CHANGE_TIME                                           "OfxChangeTime"

#define OFX_TYPE_IMAGE_EFFECT_HOST                                "OfxTypeImageEffectHost"
#define OFX_TYPE_IMAGE_EFFECT                                     "OfxTypeImageEffect"
#define OFX_TYPE_IMAGE_EFFECT_INSTANCE                            "OfxTypeImageEffectInstance"
#define OFX_TYPE_CLIP                                             "OfxTypeClip"
#define OFX_TYPE_IMAGE                                            "OfxTypeImage"
#define OFX_TYPE_PARAMETER                                        "OfxTypeParameter"
#define OFX_TYPE_PARAMETER_INSTANCE                               "OfxTypeParameterInstance"

#define OFX_BIT_DEPTH_NONE                                        "OfxBitDepthNone"
#define OFX_BIT_DEPTH_BYTE                                        "OfxBitDepthByte"
#define OFX_BIT_DEPTH_SHORT                                       "OfxBitDepthShort"
#define OFX_BIT_DEPTH_HALF                                        "OfxBitDepthHalf"
#define OFX_BIT_DEPTH_FLOAT                                       "OfxBitDepthFloat"

#define OFX_IMAGE_COMPONENT_NONE                                  "OfxImageComponentNone"
#define OFX_IMAGE_COMPONENT_RGBA                                  "OfxImageComponentRGBA"
#define OFX_IMAGE_COMPONENT_RGB                                   "OfxImageComponentRGB"
#define OFX_IMAGE_COMPONENT_ALPHA                                 "OfxImageComponentAlpha"

#define OFX_IMAGE_OPAQUE                                          "OfxImageOpaque"
#define OFX_IMAGE_PREMULTIPLIED                                   "OfxImageAlphaPremultiplied"
#define OFX_IMAGE_UNPREMULTIPLIED                                 "OfxImageAlphaUnPremultiplied"

#define OFX_IMAGE_FIELD_NONE                                      "OfxFieldNone"
#define OFX_IMAGE_FIELD_LOWER                                     "OfxFieldLower"
#define OFX_IMAGE_FIELD_UPPER                                     "OfxFieldUpper"
#define OFX_IMAGE_FIELD_BOTH                                      "OfxFieldBoth"
#define OFX_IMAGE_FIELD_SINGLE                                    "OfxFieldSingle"
#define OFX_IMAGE_FIELD_DOUBLED                                   "OfxFieldDoubled"

#define OFX_IMAGE_PROP_UNIQUE_IDENTIFIER                          "OfxImagePropUniqueIdentifier"
#define OFX_IMAGE_PROP_PIXEL_ASPECT_RATIO                         "OfxImagePropPixelAspectRatio"
#define OFX_IMAGE_PROP_DATA                                       "OfxImagePropData"
#define OFX_IMAGE_PROP_BOUNDS                                     "OfxImagePropBounds"
#define OFX_IMAGE_PROP_REGION_OF_DEFINITION                       "OfxImagePropRegionOfDefinition"
#define OFX_IMAGE_PROP_ROW_BYTES                                  "OfxImagePropRowBytes"
#define OFX_IMAGE_PROP_FIELD                                      "OfxImagePropField"

#define OFX_IMAGE_CLIP_PROP_FIELD_EXTRACTION                      "OfxImageClipPropFieldExtraction"
#define OFX_IMAGE_CLIP_PROP_CONTINUOUS_SAMPLES                    "OfxImageClipPropContinuousSamples"
#define OFX_IMAGE_CLIP_PROP_UNMAPPED_PIXEL_DEPTH                  "OfxImageClipPropUnmappedPixelDepth"
#define OFX_IMAGE_CLIP_PROP_UNMAPPED_COMPONENTS                   "OfxImageClipPropUnmappedComponents"
#define OFX_IMAGE_CLIP_PROP_OPTIONAL                              "OfxImageClipPropOptional"
#define OFX_IMAGE_CLIP_PROP_IS_MASK                               "OfxImageClipPropIsMask"
#define OFX_IMAGE_CLIP_PROP_CONNECTED                             "OfxImageClipPropConnected"
#define OFX_IMAGE_CLIP_PROP_FIELD_ORDER                           "OfxImageClipPropFieldOrder"
#define OFX_IMAGE_CLIP_PROP_COLOURSPACE                           "OfxImageClipPropColourspace"
#define OFX_IMAGE_CLIP_PROP_PREFERRED_COLOURSPACES                "OfxImageClipPropPreferredColourspaces"

#define OFX_ACTION_LOAD                                           "OfxActionLoad"
#define OFX_ACTION_DESCRIBE                                       "OfxActionDescribe"
#define OFX_ACTION_UNLOAD                                         "OfxActionUnload"
#define OFX_ACTION_PURGE_CACHES                                   "OfxActionPurgeCaches"
#define OFX_ACTION_SYNC_PRIVATE_DATA                              "OfxActionSyncPrivateData"
#define OFX_ACTION_CREATE_INSTANCE                                "OfxActionCreateInstance"
#define OFX_ACTION_DESTROY_INSTANCE                               "OfxActionDestroyInstance"
#define OFX_ACTION_INSTANCE_CHANGED                               "OfxActionInstanceChanged"
#define OFX_ACTION_BEGIN_INSTANCE_CHANGED                         "OfxActionBeginInstanceChanged"
#define OFX_ACTION_END_INSTANCE_CHANGED                           "OfxActionEndInstanceChanged"
#define OFX_ACTION_BEGIN_INSTANCE_EDIT                            "OfxActionBeginInstanceEdit"
#define OFX_ACTION_END_INSTANCE_EDIT                              "OfxActionEndInstanceEdit"
#define OFX_ACTION_DIALOG                                         "OfxActionDialog"
#define OFX_ACTION_OPENGL_CONTEXT_ATTACHED                        "OfxActionOpenGLContextAttached"
#define OFX_ACTION_OPENGL_CONTEXT_DETACHED                        "kOfxActionOpenGLContextDetached"
#define OFX_ACTION_DESCRIBE_INTERACT                              OFX_ACTION_DESCRIBE
#define OFX_ACTION_CREATE_INSTANCE_INTERACT                       OFX_ACTION_CREATE_INSTANCE
#define OFX_ACTION_DESTROY_INSTANCE_INTERACT                      OFX_ACTION_DESTROY_INSTANCE

#define OFX_IMAGE_EFFECT_ACTION_GET_REGION_OF_DEFINITION          "OfxImageEffectActionGetRegionOfDefinition"
#define OFX_IMAGE_EFFECT_ACTION_GET_REGIONS_OF_INTEREST           "OfxImageEffectActionGetRegionsOfInterest"
#define OFX_IMAGE_EFFECT_ACTION_GET_TIME_DOMAIN                   "OfxImageEffectActionGetTimeDomain"
#define OFX_IMAGE_EFFECT_ACTION_GET_FRAMES_NEEDED                 "OfxImageEffectActionGetFramesNeeded"
#define OFX_IMAGE_EFFECT_ACTION_GET_CLIP_PREFERENCES              "OfxImageEffectActionGetClipPreferences"
#define OFX_IMAGE_EFFECT_ACTION_IS_IDENTITY                       "OfxImageEffectActionIsIdentity"
#define OFX_IMAGE_EFFECT_ACTION_RENDER                            "OfxImageEffectActionRender"
#define OFX_IMAGE_EFFECT_ACTION_BEGIN_SEQUENCE_RENDER             "OfxImageEffectActionBeginSequenceRender"
#define OFX_IMAGE_EFFECT_ACTION_END_SEQUENCE_RENDER               "OfxImageEffectActionEndSequenceRender"
#define OFX_IMAGE_EFFECT_ACTION_DESCRIBE_IN_CONTEXT               "OfxImageEffectActionDescribeInContext"
#define OFX_IMAGE_EFFECT_ACTION_GET_OUTPUT_COLOURSPACE            "OfxImageEffectActionGetOutputColourspace"

#define OFX_IMAGE_EFFECT_CONTEXT_GENERATOR                        "OfxImageEffectContextGenerator"
#define OFX_IMAGE_EFFECT_CONTEXT_FILTER                           "OfxImageEffectContextFilter"
#define OFX_IMAGE_EFFECT_CONTEXT_TRANSITION                       "OfxImageEffectContextTransition"
#define OFX_IMAGE_EFFECT_CONTEXT_PAINT                            "OfxImageEffectContextPaint"
#define OFX_IMAGE_EFFECT_CONTEXT_GENERAL                          "OfxImageEffectContextGeneral"
#define OFX_IMAGE_EFFECT_CONTEXT_RETIMER                          "OfxImageEffectContextRetimer"

#define OFX_IMAGE_EFFECT_HOST_PROP_IS_BACKGROUND                  "OfxImageEffectHostPropIsBackground"
#define OFX_IMAGE_EFFECT_HOST_PROP_NATIVE_ORIGIN                  "OfxImageEffectHostPropNativeOrigin"

#define OFX_IMAGE_EFFECT_RENDER_UNSAFE                            "OfxImageEffectRenderUnsafe"
#define OFX_IMAGE_EFFECT_RENDER_INSTANCE_SAFE                     "OfxImageEffectRenderInstanceSafe"
#define OFX_IMAGE_EFFECT_RENDER_FULLY_SAFE                        "OfxImageEffectRenderFullySafe"

#define OFX_IMAGE_EFFECT_FRAME_VARYING                            "OfxImageEffectFrameVarying"

#define OFX_IMAGE_EFFECT_PLUGIN_API                               "OfxImageEffectPluginAPI"
#define OFX_IMAGE_EFFECT_PLUGIN_API_VERSION                       1
#define OFX_IMAGE_EFFECT_PLUGIN_RENDER_THREAD_SAFETY              "OfxImageEffectPluginRenderThreadSafety"

#define OFX_IMAGE_EFFECT_PLUGIN_PROP_SINGLE_INSTANCE              "OfxImageEffectPluginPropSingleInstance"
#define OFX_IMAGE_EFFECT_PLUGIN_PROP_HOST_FRAME_THREADING         "OfxImageEffectPluginPropHostFrameThreading"
#define OFX_IMAGE_EFFECT_PLUGIN_PROP_GROUPING                     "OfxImageEffectPluginPropGrouping"
#define OFX_IMAGE_EFFECT_PLUGIN_PROP_OVERLAY_INTERACT_V1          "OfxImageEffectPluginPropOverlayInteractV1"
#define OFX_IMAGE_EFFECT_PLUGIN_PROP_OVERLAY_INTERACT_V2          "OfxImageEffectPluginPropOverlayInteractV2"
#define OFX_IMAGE_EFFECT_PLUGIN_PROP_FIELD_RENDER_TWICE_ALWAYS    "OfxImageEffectPluginPropFieldRenderTwiceAlways"

#define OFX_IMAGE_EFFECT_INSTANCE_PROP_SEQUENTIAL_RENDER          "OfxImageEffectInstancePropSequentialRender"
#define OFX_IMAGE_EFFECT_INSTANCE_PROP_EFFECT_DURATION            "OfxImageEffectInstancePropEffectDuration"

#define OFX_IMAGE_EFFECT_OUTPUT_CLIP_NAME                         "Output"
#define OFX_IMAGE_EFFECT_SIMPLE_SOURCE_CLIP_NAME                  "Source"
#define OFX_IMAGE_EFFECT_TRANSITION_SOURCE_FROM_CLIP_NAME         "SourceFrom"
#define OFX_IMAGE_EFFECT_TRANSITION_SOURCE_TO_CLIP_NAME           "SourceTo"
#define OFX_IMAGE_EFFECT_TRANSITION_PARAM_NAME                    "Transition"
#define OFX_IMAGE_EFFECT_RETIMER_PARAM_NAME                       "SourceTime"

#define OFX_IMAGE_EFFECT_COLOUR_MANAGEMENT_NONE                   "OfxImageEffectColourManagementNone"
#define OFX_IMAGE_EFFECT_COLOUR_MANAGEMENT_BASIC                  "OfxImageEffectColourManagementBasic"
#define OFX_IMAGE_EFFECT_COLOUR_MANAGEMENT_CORE                   "OfxImageEffectColourManagementCore"
#define OFX_IMAGE_EFFECT_COLOUR_MANAGEMENT_FULL                   "OfxImageEffectColourManagementFull"
#define OFX_IMAGE_EFFECT_COLOUR_MANAGEMENT_OCIO                   "OfxImageEffectColourManagementOCIO"

#define OFX_IMAGE_EFFECT_PROP_SUPPORTED_CONTEXTS                  "OfxImageEffectPropSupportedContexts"
#define OFX_IMAGE_EFFECT_PROP_PLUGIN_HANDLE                       "OfxImageEffectPropPluginHandle"
#define OFX_IMAGE_EFFECT_PROP_SUPPORTS_MULTIPLE_CLIP_DEPTHS       "OfxImageEffectPropMultipleClipDepths"
#define OFX_IMAGE_EFFECT_PROP_SUPPORTS_MULTIPLE_CLIP_PARS         "OfxImageEffectPropSupportsMultipleClipPARs"
#define OFX_IMAGE_EFFECT_PROP_CLIP_PREFERENCES_SLAVE_PARAM        "OfxImageEffectPropClipPreferencesSlaveParam"
#define OFX_IMAGE_EFFECT_PROP_SETABLE_FRAME_RATE                  "OfxImageEffectPropSetableFrameRate"
#define OFX_IMAGE_EFFECT_PROP_SETABLE_FIELDING                    "OfxImageEffectPropSetableFielding"
#define OFX_IMAGE_EFFECT_PROP_SEQUENTIAL_RENDER_STATUS            "OfxImageEffectPropSequentialRenderStatus"
#define OFX_IMAGE_EFFECT_PROP_INTERACTIVE_RENDER_STATUS           "OfxImageEffectPropInteractiveRenderStatus"
#define OFX_IMAGE_EFFECT_PROP_SUPPORTS_OVERLAYS                   "OfxImageEffectPropSupportsOverlays"
#define OFX_IMAGE_EFFECT_PROP_SUPPORTS_MULTI_RESOLUTION           "OfxImageEffectPropSupportsMultiResolution"
#define OFX_IMAGE_EFFECT_PROP_SUPPORTS_TILES                      "OfxImageEffectPropSupportsTiles"
#define OFX_IMAGE_EFFECT_PROP_TEMPORAL_CLIP_ACCESS                "OfxImageEffectPropTemporalClipAccess"
#define OFX_IMAGE_EFFECT_PROP_CONTEXT                             "OfxImageEffectPropContext"
#define OFX_IMAGE_EFFECT_PROP_PIXEL_DEPTH                         "OfxImageEffectPropPixelDepth"
#define OFX_IMAGE_EFFECT_PROP_COMPONENTS                          "OfxImageEffectPropComponents"
#define OFX_IMAGE_EFFECT_PROP_PREMULTIPLICATION                   "OfxImageEffectPropPreMultiplication"
#define OFX_IMAGE_EFFECT_PROP_SUPPORTED_PIXEL_DEPTHS              "OfxImageEffectPropSupportedPixelDepths"
#define OFX_IMAGE_EFFECT_PROP_SUPPORTED_COMPONENTS                "OfxImageEffectPropSupportedComponents"
#define OFX_IMAGE_EFFECT_PROP_FRAME_RATE                          "OfxImageEffectPropFrameRate"
#define OFX_IMAGE_EFFECT_PROP_UNMAPPED_FRAME_RATE                 "OfxImageEffectPropUnmappedFrameRate"
#define OFX_IMAGE_EFFECT_PROP_FRAME_STEP                          "OfxImageEffectPropFrameStep"
#define OFX_IMAGE_EFFECT_PROP_FRAME_RANGE                         "OfxImageEffectPropFrameRange"
#define OFX_IMAGE_EFFECT_PROP_UNMAPPED_FRAME_RANGE                "OfxImageEffectPropUnmappedFrameRange"
#define OFX_IMAGE_EFFECT_PROP_RENDER_SCALE                        "OfxImageEffectPropRenderScale"
#define OFX_IMAGE_EFFECT_PROP_RENDER_QUALITY_DRAFT                "OfxImageEffectPropRenderQualityDraft"
#define OFX_IMAGE_EFFECT_PROP_PROJECT_EXTENT                      "OfxImageEffectPropProjectExtent"
#define OFX_IMAGE_EFFECT_PROP_PROJECT_SIZE                        "OfxImageEffectPropProjectSize"
#define OFX_IMAGE_EFFECT_PROP_PROJECT_OFFSET                      "OfxImageEffectPropProjectOffset"
#define OFX_IMAGE_EFFECT_PROP_PROJECT_PIXEL_ASPECT_RATIO          "OfxImageEffectPropPixelAspectRatio"
#define OFX_IMAGE_EFFECT_PROP_FIELD_TO_RENDER                     "OfxImageEffectPropFieldToRender"
#define OFX_IMAGE_EFFECT_PROP_REGION_OF_DEFINITION                "OfxImageEffectPropRegionOfDefinition"
#define OFX_IMAGE_EFFECT_PROP_REGION_OF_INTEREST                  "OfxImageEffectPropRegionOfInterest"
#define OFX_IMAGE_EFFECT_PROP_RENDER_WINDOW                       "OfxImageEffectPropRenderWindow"
#define OFX_IMAGE_EFFECT_PROP_OPENGL_RENDER_SUPPORTED             "OfxImageEffectPropOpenGLRenderSupported"
#define OFX_IMAGE_EFFECT_PROP_OPENGL_ENABLED                      "OfxImageEffectPropOpenGLEnabled"
#define OFX_IMAGE_EFFECT_PROP_OPENGL_TEXTURE_INDEX                "OfxImageEffectPropOpenGLTextureIndex"
#define OFX_IMAGE_EFFECT_PROP_OPENGL_TEXTURE_TARGET               "OfxImageEffectPropOpenGLTextureTarget"
#define OFX_IMAGE_EFFECT_PROP_CUDA_RENDER_SUPPORTED               "OfxImageEffectPropCudaRenderSupported"
#define OFX_IMAGE_EFFECT_PROP_CUDA_ENABLED                        "OfxImageEffectPropCudaEnabled"
#define OFX_IMAGE_EFFECT_PROP_CUDA_STREAM_SUPPORTED               "OfxImageEffectPropCudaStreamSupported"
#define OFX_IMAGE_EFFECT_PROP_CUDA_STREAM                         "OfxImageEffectPropCudaStream"
#define OFX_IMAGE_EFFECT_PROP_METAL_RENDER_SUPPORTED              "OfxImageEffectPropMetalRenderSupported"
#define OFX_IMAGE_EFFECT_PROP_METAL_ENABLED                       "OfxImageEffectPropMetalEnabled"
#define OFX_IMAGE_EFFECT_PROP_METAL_COMMAND_QUEUE                 "OfxImageEffectPropMetalCommandQueue"
#define OFX_IMAGE_EFFECT_PROP_OPENCL_RENDER_SUPPORTED             "OfxImageEffectPropOpenCLRenderSupported"
#define OFX_IMAGE_EFFECT_PROP_OPENCL_SUPPORTED                    "OfxImageEffectPropOpenCLSupported"
#define OFX_IMAGE_EFFECT_PROP_OPENCL_ENABLED                      "OfxImageEffectPropOpenCLEnabled"
#define OFX_IMAGE_EFFECT_PROP_OPENCL_COMMAND_QUEUE                "OfxImageEffectPropOpenCLCommandQueue"
#define OFX_IMAGE_EFFECT_PROP_OPENCL_IMAGE                        "OfxImageEffectPropOpenCLImage"
#define OFX_IMAGE_EFFECT_PROP_COLOUR_MANAGEMENT_STYLE             "OfxImageEffectPropColourManagementStyle"
#define OFX_IMAGE_EFFECT_PROP_COLOUR_MANAGEMENT_AVAILABLE_CONFIGS "OfxImageEffectPropColourManagementAvailableConfigs"
#define OFX_IMAGE_EFFECT_PROP_COLOUR_MANAGEMENT_CONFIG            "OfxImageEffectPropColourManagementConfig"
#define OFX_IMAGE_EFFECT_PROP_OCIO_CONFIG                         "OfxImageEffectPropOCIOConfig"
#define OFX_IMAGE_EFFECT_PROP_DISPLAY_COLOURSPACE                 "OfxImageEffectPropDisplayColourspace"
#define OFX_IMAGE_EFFECT_PROP_OCIO_DISPLAY                        "OfxImageEffectPropOCIODisplay"
#define OFX_IMAGE_EFFECT_PROP_OCIO_VIEW                           "OfxImageEffectPropOCIOView"

#define OFX_OPENGL_PROP_PIXEL_DEPTH                               "OfxOpenGLPropPixelDepth"

#define OFX_INTERACT_ACTION_DRAW                                  "OfxInteractActionDraw"
#define OFX_INTERACT_ACTION_PEN_MOTION                            "OfxInteractActionPenMotion"
#define OFX_INTERACT_ACTION_PEN_DOWN                              "OfxInteractActionPenDown"
#define OFX_INTERACT_ACTION_PEN_UP                                "OfxInteractActionPenUp"
#define OFX_INTERACT_ACTION_KEY_DOWN                              "OfxInteractActionKeyDown"
#define OFX_INTERACT_ACTION_KEY_UP                                "OfxInteractActionKeyUp"
#define OFX_INTERACT_ACTION_KEY_REPEAT                            "OfxInteractActionKeyRepeat"
#define OFX_INTERACT_ACTION_GAIN_FOCUS                            "OfxInteractActionGainFocus"
#define OFX_INTERACT_ACTION_LOSE_FOCUS                            "OfxInteractActionLoseFocus"

#define OFX_INTERACT_PROP_SLAVE_TO_PARAM                          "OfxInteractPropSlaveToParam"
#define OFX_INTERACT_PROP_PIXEL_SCALE                             "OfxInteractPropPixelScale"
#define OFX_INTERACT_PROP_BACKGROUND_COLOUR                       "OfxInteractPropBackgroundColour"
#define OFX_INTERACT_PROP_SUGGESTED_COLOUR                        "OfxInteractPropSuggestedColour"
#define OFX_INTERACT_PROP_PEN_POSITION                            "OfxInteractPropPenPosition"
#define OFX_INTERACT_PROP_PEN_VIEWPORT_POSITION                   "OfxInteractPropPenViewportPosition"
#define OFX_INTERACT_PROP_PEN_PRESSURE                            "OfxInteractPropPenPressure"
#define OFX_INTERACT_PROP_BIT_DEPTH                               "OfxInteractPropBitDepth"
#define OFX_INTERACT_PROP_HAS_ALPHA                               "OfxInteractPropHasAlpha"
#define OFX_INTERACT_PROP_DRAW_CONTEXT                            "OfxInteractPropDrawContext"

#define OFX_MESSAGE_FATAL                                         "OfxMessageFatal"
#define OFX_MESSAGE_ERROR                                         "OfxMessageError"
#define OFX_MESSAGE_WARNING                                       "OfxMessageWarning"
#define OFX_MESSAGE_MESSAGE                                       "OfxMessageMessage"
#define OFX_MESSAGE_LOG                                           "OfxMessageLog"
#define OFX_MESSAGE_QUESTION                                      "OfxMessageQuestion"

#define OFX_PARAM_TYPE_INTEGER                                    "OfxParamTypeInteger"
#define OFX_PARAM_TYPE_DOUBLE                                     "OfxParamTypeDouble"
#define OFX_PARAM_TYPE_BOOLEAN                                    "OfxParamTypeBoolean"
#define OFX_PARAM_TYPE_CHOICE                                     "OfxParamTypeChoice"
#define OFX_PARAM_TYPE_STR_CHOICE                                 "OfxParamTypeStrChoice"
#define OFX_PARAM_TYPE_RGBA                                       "OfxParamTypeRGBA"
#define OFX_PARAM_TYPE_RGB                                        "OfxParamTypeRGB"
#define OFX_PARAM_TYPE_DOUBLE_2D                                  "OfxParamTypeDouble2D"
#define OFX_PARAM_TYPE_INTEGER_2D                                 "OfxParamTypeInteger2D"
#define OFX_PARAM_TYPE_DOUBLE_3D                                  "OfxParamTypeDouble3D"
#define OFX_PARAM_TYPE_INTEGER_3D                                 "OfxParamTypeInteger3D"
#define OFX_PARAM_TYPE_STRING                                     "OfxParamTypeString"
#define OFX_PARAM_TYPE_CUSTOM                                     "OfxParamTypeCustom"
#define OFX_PARAM_TYPE_BYTES                                      "OfxParamTypeBytes"
#define OFX_PARAM_TYPE_GROUP                                      "OfxParamTypeGroup"
#define OFX_PARAM_TYPE_PAGE                                       "OfxParamTypePage"
#define OFX_PARAM_TYPE_PUSH_BUTTON                                "OfxParamTypePushButton"
#define OFX_PARAM_TYPE_PARAMETRIC                                 "OfxParamTypeParametric"

#define OFX_PARAM_HOST_PROP_SUPPORTS_CUSTOM_ANIMATION             "OfxParamHostPropSupportsCustomAnimation"
#define OFX_PARAM_HOST_PROP_SUPPORTS_STRING_ANIMATION             "OfxParamHostPropSupportsStringAnimation"
#define OFX_PARAM_HOST_PROP_SUPPORTS_BOOLEAN_ANIMATION            "OfxParamHostPropSupportsBooleanAnimation"
#define OFX_PARAM_HOST_PROP_SUPPORTS_CHOICE_ANIMATION             "OfxParamHostPropSupportsChoiceAnimation"
#define OFX_PARAM_HOST_PROP_SUPPORTS_STR_CHOICE_ANIMATION         "OfxParamHostPropSupportsStrChoiceAnimation"
#define OFX_PARAM_HOST_PROP_SUPPORTS_STR_CHOICE                   "OfxParamHostPropSupportsStrChoice"
#define OFX_PARAM_HOST_PROP_SUPPORTS_CUSTOM_INTERACT              "OfxParamHostPropSupportsCustomInteract"
#define OFX_PARAM_HOST_PROP_SUPPORTS_PARAMETRIC_ANIMATION         "OfxParamHostPropSupportsParametricAnimation"

#define OFX_PARAM_HOST_PROP_MAX_PARAMETERS                        "OfxParamHostPropMaxParameters"
#define OFX_PARAM_HOST_PROP_MAX_PAGES                             "OfxParamHostPropMaxPages"
#define OFX_PARAM_HOST_PROP_PAGE_ROW_COLUMN_COUNT                 "OfxParamHostPropPageRowColumnCount"

#define OFX_PARAM_PAGE_SKIP_ROW                                   "OfxParamPageSkipRow"
#define OFX_PARAM_PAGE_SKIP_COLUMN                                "OfxParamPageSkipColumn"

#define OFX_PARAM_INVALIDATE_VALUE_CHANGE                         "OfxParamInvalidateValueChange"
#define OFX_PARAM_INVALIDATE_VALUE_CHANGE_TO_END                  "OfxParamInvalidateValueChangeToEnd"
#define OFX_PARAM_INVALIDATE_ALL                                  "OfxParamInvalidateAll"

#define OFX_PARAM_PROP_INTERACT_V1                                "OfxParamPropInteractV1"
#define OFX_PARAM_PROP_INTERACT_SIZE                              "OfxParamPropInteractSize"
#define OFX_PARAM_PROP_INTERACT_SIZE_ASPECT                       "OfxParamPropInteractSizeAspect"
#define OFX_PARAM_PROP_INTERACT_MINIMUM_SIZE                      "OfxParamPropInteractMinimumSize"
#define OFX_PARAM_PROP_INTERACT_PREFERED_SIZE                     "OfxParamPropInteractPreferedSize"

#define OFX_PARAM_PROP_TYPE                                       "OfxParamPropType"
#define OFX_PARAM_PROP_ANIMATES                                   "OfxParamPropAnimates"
#define OFX_PARAM_PROP_CAN_UNDO                                   "OfxParamPropCanUndo"
#define OFX_PARAM_PROP_IS_ANIMATING                               "OfxParamPropIsAnimating"
#define OFX_PARAM_PROP_PLUGIN_MAY_WRITE                           "OfxParamPropPluginMayWrite"
#define OFX_PARAM_PROP_PERSISTANT                                 "OfxParamPropPersistant"
#define OFX_PARAM_PROP_EVALUATE_ON_CHANGE                         "OfxParamPropEvaluateOnChange"
#define OFX_PARAM_PROP_SECRET                                     "OfxParamPropSecret"
#define OFX_PARAM_PROP_SCRIPT_NAME                                "OfxParamPropScriptName"
#define OFX_PARAM_PROP_CACHE_INVALIDATION                         "OfxParamPropCacheInvalidation"
#define OFX_PARAM_PROP_HINT                                       "OfxParamPropHint"
#define OFX_PARAM_PROP_DEFAULT                                    "OfxParamPropDefault"
#define OFX_PARAM_PROP_DOUBLE_TYPE                                "OfxParamPropDoubleType"
#define OFX_PARAM_PROP_DEFAULT_COORDINATE_SYSTEM                  "OfxParamPropDefaultCoordinateSystem"
#define OFX_PARAM_PROP_HAS_HOST_OVERLAY_HANDLE                    "OfxParamPropHasHostOverlayHandle"
#define OFX_PARAM_PROP_USE_HOST_OVERLAY_HANDLE                    "kOfxParamPropUseHostOverlayHandle"
#define OFX_PARAM_PROP_SHOW_TIME_MARKER                           "OfxParamPropShowTimeMarker"
#define OFX_PARAM_PROP_PAGE_CHILD                                 "OfxParamPropPageChild"
#define OFX_PARAM_PROP_PARENT                                     "OfxParamPropParent"
#define OFX_PARAM_PROP_GROUP_OPEN                                 "OfxParamPropGroupOpen"
#define OFX_PARAM_PROP_ENABLED                                    "OfxParamPropEnabled"
#define OFX_PARAM_PROP_DATA_PTR                                   "OfxParamPropDataPtr"
#define OFX_PARAM_PROP_CHOICE_OPTION                              "OfxParamPropChoiceOption"
#define OFX_PARAM_PROP_CHOICE_ORDER                               "OfxParamPropChoiceOrder"
#define OFX_PARAM_PROP_CHOICE_ENUM                                "OfxParamPropChoiceEnum"
#define OFX_PARAM_PROP_MIN                                        "OfxParamPropMin"
#define OFX_PARAM_PROP_MAX                                        "OfxParamPropMax"
#define OFX_PARAM_PROP_DISPLAY_MIN                                "OfxParamPropDisplayMin"
#define OFX_PARAM_PROP_DISPLAY_MAX                                "OfxParamPropDisplayMax"
#define OFX_PARAM_PROP_INCREMENT                                  "OfxParamPropIncrement"
#define OFX_PARAM_PROP_DIGITS                                     "OfxParamPropDigits"
#define OFX_PARAM_PROP_DIMENSION_LABEL                            "OfxParamPropDimensionLabel"
#define OFX_PARAM_PROP_IS_AUTO_KEYING                             "OfxParamPropIsAutoKeying"
#define OFX_PARAM_PROP_CUSTOM_INTERP_CALLBACK_V1                  "OfxParamPropCustomCallbackV1"
#define OFX_PARAM_PROP_STRING_MODE                                "OfxParamPropStringMode"
#define OFX_PARAM_PROP_STRING_FILE_PATH_EXISTS                    "OfxParamPropStringFilePathExists"
#define OFX_PARAM_PROP_CUSTOM_VALUE                               "OfxParamPropCustomValue"
#define OFX_PARAM_PROP_INTERPOLATION_TIME                         "OfxParamPropInterpolationTime"
#define OFX_PARAM_PROP_INTERPOLATION_AMOUNT                       "OfxParamPropInterpolationAmount"
#define OFX_PARAM_PROP_PARAMETRIC_DIMENSION                       "OfxParamPropParametricDimension"
#define OFX_PARAM_PROP_PARAMETRIC_UI_COLOUR                       "OfxParamPropParametricUIColour"
#define OFX_PARAM_PROP_PARAMETRIC_INTERACT_BACKGROUND             "OfxParamPropParametricInteractBackground"
#define OFX_PARAM_PROP_PARAMETRIC_RANGE                           "OfxParamPropParametricRange"

#define OFX_PARAM_DOUBLE_TYPE_PLAIN                               "OfxParamDoubleTypePlain"
#define OFX_PARAM_DOUBLE_TYPE_SCALE                               "OfxParamDoubleTypeScale"
#define OFX_PARAM_DOUBLE_TYPE_ANGLE                               "OfxParamDoubleTypeAngle"
#define OFX_PARAM_DOUBLE_TYPE_TIME                                "OfxParamDoubleTypeTime"
#define OFX_PARAM_DOUBLE_TYPE_ABSOLUTE_TIME                       "OfxParamDoubleTypeAbsoluteTime"
#define OFX_PARAM_DOUBLE_TYPE_X                                   "OfxParamDoubleTypeX"
#define OFX_PARAM_DOUBLE_TYPE_Y                                   "OfxParamDoubleTypeY"
#define OFX_PARAM_DOUBLE_TYPE_X_ABSOLUTE                          "OfxParamDoubleTypeXAbsolute"
#define OFX_PARAM_DOUBLE_TYPE_Y_ABSOLUTE                          "OfxParamDoubleTypeYAbsolute"
#define OFX_PARAM_DOUBLE_TYPE_XY                                  "OfxParamDoubleTypeXY"
#define OFX_PARAM_DOUBLE_TYPE_XY_ABSOLUTE                         "OfxParamDoubleTypeXYAbsolute"

#define OFX_PARAM_COORDINATES_CANONICAL                           "OfxParamCoordinatesCanonical"
#define OFX_PARAM_COORDINATES_NORMALISED                          "OfxParamCoordinatesNormalised"

#define OFX_PARAM_STRING_IS_SINGLE_LINE                           "OfxParamStringIsSingleLine"
#define OFX_PARAM_STRING_IS_MULTI_LINE                            "OfxParamStringIsMultiLine"
#define OFX_PARAM_STRING_IS_FILE_PATH                             "OfxParamStringIsFilePath"
#define OFX_PARAM_STRING_IS_DIRECTORY_PATH                        "OfxParamStringIsDirectoryPath"
#define OFX_PARAM_STRING_IS_LABEL                                 "OfxParamStringIsLabel"
#define OFX_PARAM_STRING_IS_RICH_TEXT_FORMAT                      "OfxParamStringIsRichTextFormat"

#if defined(__cplusplus)
}
#endif
