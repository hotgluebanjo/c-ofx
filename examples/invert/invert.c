#include "ofx.h"

#define PLUGIN_GROUP       "C Example OFX Plugins"
#define PLUGIN_NAME        "Invert"
#define PLUGIN_VERSION     "0.1.0"
#define PLUGIN_DESCRIPTION "Inverts via 100% - pixel."

struct {
    OFX_Host *host;
    OFX_ImageEffectSuiteV1 *effect;
    OFX_PropertySuiteV1 *prop;
} ctx;

bool cstr_eq(char *a, char *b) {
    return strcmp(a, b) == 0;
}

typedef struct Image Image;
struct Image {
    OFX_Rect_i32 bounds;
    OFX_PropMapHandle props;
    void *ptr;
    int32_t row_bytes;
};

OFX_Status get_image(OFX_EffectHandle instance, OFX_ClipHandle clip, OFX_Time time, Image *img) {
    OFX_Status stat = ctx.effect->clip_get_image(clip, time, NULL, &img->props);
    if (stat != OFX_OK) {
        return stat;
    }

    ctx.prop->get_int_n(img->props, OFX_IMAGE_PROP_BOUNDS, 4, &img->bounds.x1);
    ctx.prop->get_pointer(img->props, OFX_IMAGE_PROP_DATA, 0, &img->ptr);
    ctx.prop->get_int(img->props, OFX_IMAGE_PROP_ROW_BYTES, 0, &img->row_bytes);

    return OFX_OK;
}

void release_image(Image img) {
    if (img.props != NULL) {
        ctx.effect->clip_release_image(img.props);
    }
}

OFX_Rgba_f32 *get_pixel_ptr(Image img, uint32_t x, uint32_t y) {
    if (x < img.bounds.x1 || x >= img.bounds.x2 ||
        y < img.bounds.y1 || y >= img.bounds.y2) {
        return NULL;
    }

    uint32_t y_offset = y - img.bounds.y1;
    uint32_t x_offset = x - img.bounds.x1;

    uint8_t *bytes = (uint8_t *)img.ptr;
    size_t start = y_offset * img.row_bytes + x_offset * sizeof(OFX_Rgba_f32);

    return (OFX_Rgba_f32 *)&bytes[start];
}

OFX_Status load() {
    if (ctx.host == NULL) {
        return OFX_ERR_MISSING_HOST_FEATURE;
    }

    ctx.effect = ctx.host->fetch_suite(ctx.host->host, OFX_IMAGE_EFFECT_SUITE, 1);
    ctx.prop   = ctx.host->fetch_suite(ctx.host->host, OFX_PROPERTY_SUITE, 1);

    if (ctx.effect == NULL || ctx.prop == NULL) {
        return OFX_ERR_MISSING_HOST_FEATURE;
    }

    return OFX_OK;
}

OFX_Status describe(OFX_EffectHandle descriptor) {
    OFX_PropMapHandle props;
    ctx.effect->get_prop_map(descriptor, &props);

    ctx.prop->set_string(props, OFX_PROP_LABEL, 0, PLUGIN_NAME);
    ctx.prop->set_string(props, OFX_IMAGE_EFFECT_PLUGIN_PROP_GROUPING, 0, PLUGIN_GROUP);
    ctx.prop->set_string(props, OFX_PROP_PLUGIN_DESCRIPTION, 0, PLUGIN_DESCRIPTION);
    ctx.prop->set_string(props, OFX_PROP_VERSION_LABEL, 0, PLUGIN_VERSION);

    ctx.prop->set_string(props, OFX_IMAGE_EFFECT_PROP_SUPPORTED_CONTEXTS, 0, OFX_IMAGE_EFFECT_CONTEXT_FILTER);

    ctx.prop->set_int(props, OFX_IMAGE_EFFECT_PROP_SUPPORTS_MULTIPLE_CLIP_DEPTHS, 0, 0);
    ctx.prop->set_string(props, OFX_IMAGE_EFFECT_PROP_SUPPORTED_PIXEL_DEPTHS, 0, OFX_BIT_DEPTH_FLOAT);

    return OFX_OK;
}

OFX_Status describe_in_context(OFX_EffectHandle descriptor) {
    OFX_PropMapHandle props;

    ctx.effect->clip_define(descriptor, OFX_IMAGE_EFFECT_SIMPLE_SOURCE_CLIP_NAME, &props);
    ctx.prop->set_string(props, OFX_IMAGE_EFFECT_PROP_SUPPORTED_COMPONENTS, 0, OFX_IMAGE_COMPONENT_RGBA);

    ctx.effect->clip_define(descriptor, OFX_IMAGE_EFFECT_OUTPUT_CLIP_NAME, &props);
    ctx.prop->set_string(props, OFX_IMAGE_EFFECT_PROP_SUPPORTED_COMPONENTS, 0, OFX_IMAGE_COMPONENT_RGBA);

    return OFX_OK;
}

OFX_Status render(OFX_EffectHandle instance, OFX_PropMapHandle in_args) {
    OFX_Time time;
    ctx.prop->get_double(in_args, OFX_PROP_TIME, 0, &time);

    OFX_Rect_i32 render_bounds;
    ctx.prop->get_int_n(in_args, OFX_IMAGE_EFFECT_PROP_RENDER_WINDOW, 4, &render_bounds.x1);

    OFX_ClipHandle src_clip, dst_clip;
    ctx.effect->clip_get_handle(instance, OFX_IMAGE_EFFECT_SIMPLE_SOURCE_CLIP_NAME, &src_clip, NULL);
    ctx.effect->clip_get_handle(instance, OFX_IMAGE_EFFECT_OUTPUT_CLIP_NAME, &dst_clip, NULL);

    Image src;
    OFX_Status src_stat = get_image(instance, src_clip, time, &src);
    if (src_stat != OFX_OK) {
        return src_stat;
    }

    Image dst;
    OFX_Status dst_stat = get_image(instance, dst_clip, time, &dst);
    if (dst_stat != OFX_OK) {
        return dst_stat;
    }

    for (uint32_t y = render_bounds.y1; y < render_bounds.y2; y += 1) {
        if (ctx.effect->abort(instance)) {
            break;
        }
        for (uint32_t x = render_bounds.x1; x < render_bounds.x2; x += 1) {
            OFX_Rgba_f32 *input = get_pixel_ptr(src, x, y);
            OFX_Rgba_f32 *output = get_pixel_ptr(dst, x, y);

            if (input != NULL) {
                *output = (OFX_Rgba_f32){
                    1.0f - input->r,
                    1.0f - input->g,
                    1.0f - input->b,
                    input->a
                };
            } else {
                *output = (OFX_Rgba_f32){0.0f, 0.0f, 0.0f, 0.0f};
            }
        }
    }

    release_image(src);
    release_image(dst);

    return OFX_OK;
}

OFX_Status plugin_main(
    char *action,
    OFX_EffectHandle handle,
    OFX_PropMapHandle in_args,
    OFX_PropMapHandle out_args
) {
    if (cstr_eq(action, OFX_ACTION_LOAD)) {
        return load();
    }

    if (cstr_eq(action, OFX_ACTION_UNLOAD) ||
        cstr_eq(action, OFX_ACTION_CREATE_INSTANCE) ||
        cstr_eq(action, OFX_ACTION_DESTROY_INSTANCE))
    {
        return OFX_OK;
    }

    if (cstr_eq(action, OFX_ACTION_DESCRIBE)) {
        return describe(handle);
    }

    if (cstr_eq(action, OFX_IMAGE_EFFECT_ACTION_DESCRIBE_IN_CONTEXT)) {
        return describe_in_context(handle);
    }

    if (cstr_eq(action, OFX_IMAGE_EFFECT_ACTION_RENDER)) {
        return render(handle, in_args);
    }

    return OFX_REPLY_DEFAULT;
}

void set_host(OFX_Host *host) {
    ctx.host = host;
}

OFX_Plugin PLUGIN = {
    .plugin_api           = OFX_IMAGE_EFFECT_PLUGIN_API,
    .api_version          = OFX_IMAGE_EFFECT_PLUGIN_API_VERSION,
    .plugin_identifier    = "ExampleOFX.InvertC",
    .plugin_version_major = 0,
    .plugin_version_minor = 1,
    .set_host             = set_host,
    .main_entry           = plugin_main,
};

ofx_export
OFX_Plugin *OfxGetPlugin(int32_t nth) {
    if (nth == 0) {
        return &PLUGIN;
    }
    return NULL;
}

ofx_export
int32_t OfxGetNumberOfPlugins() {
    return 1;
}
