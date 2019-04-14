#ifndef json_png_convert_h
#define json_png_convert_h

#include <string>
#include <fstream>

#include "xtl/xbase64.hpp"
#include "nlohmann/json.hpp"

namespace im
{
    struct image
    {
        inline image(){}
        inline image(const std::vector<uint8_t> &png)
        {
            m_buffer.assign(png.begin(), png.end());
        }

        std::string m_buffer;
    };

    nlohmann::json mime_bundle_repr(const image& i)
    {
        auto bundle = nlohmann::json::object();
        bundle["image/png"] = xtl::base64encode(i.m_buffer);
        return bundle;
    }
}

#endif