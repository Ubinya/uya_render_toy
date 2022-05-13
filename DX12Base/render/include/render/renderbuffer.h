#include "core/math/math_headers.h"

#include "render/include/render/light.h"
#include "render/include/render/render_camera.h"

#include <atomic> // about multi thread
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace Pilot {
    enum class PILOT_PIXEL_FORMAT : size_t
    {
        PILOT_PIXEL_FORMAT_R8G8B8_UNORM = 1,
        PILOT_PIXEL_FORMAT_R8G8B8_SRGB,
        PILOT_PIXEL_FORMAT_R8G8B8A8_UNORM,
        PILOT_PIXEL_FORMAT_R8G8B8A8_SRGB,
        PILOT_PIXEL_FORMAT_R32G32_FLOAT,
        PILOT_PIXEL_FORMAT_R32G32B32_FLOAT,
        PILOT_PIXEL_FORMAT_R32G32B32A32_FLOAT
    };

    enum class PILOT_IMAGE_TYPE : size_t
    {
        PILOT_IMAGE_TYPE_2D
    };

    class SceneMemory
    {
    public:
        size_t m_size;
        char* m_data;
    };

    class SceneImage
    {
    public:
        size_t m_width;
        size_t m_height;
        size_t m_depth;

        size_t m_mip_levels;
        size_t m_array_layers;

        PILOT_PIXEL_FORMAT m_format;
        PILOT_IMAGE_TYPE   m_type;
        void* m_pixels;
    };

       
}