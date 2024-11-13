#ifndef __HTTP_CONTENT_TYPE__
#define __HTTP_CONTENT_TYPE__

#include <string>

enum CONTENT_TYPE {
    TEXT_PLAIN,
    TEXT_HTML,
    TEXT_CSS,
    TEXT_JAVASCRIPT,
    IMAGE_JPEG,
    IMAGE_PNG,
    IMAGE_GIF,
    IMAGE_SVG_XML,
    AUDIO_MPEG,
    AUDIO_OGG,
    AUDIO_WAV,
    VIDEO_MPEG,
    VIDEO_OGG,
    VIDEO_WEBM,
    APP_JSON,
    APP_XML,
    APP_PDF,
    APP_ZIP,
    MULTIPART_FORM_DATA,
    MULTIPART_MIXED
};

class HttpContentType 
{
public:
    inline static std::string TEXT_PLAIN = "text/plain";
    inline static std::string TEXT_HTML = "text/html";
    inline static std::string TEXT_CSS = "text/css";
    inline static std::string TEXT_JAVASCRIPT = "text/javascript";
    inline static std::string IMAGE_JPEG = "image/jpeg";
    inline static std::string IMAGE_PNG = "image/png";
    inline static std::string IMAGE_GIF = "image/gif";
    inline static std::string IMAGE_SVG_XML = "image/svg+xml";
    inline static std::string AUDIO_MPEG = "audio/mpeg";
    inline static std::string AUDIO_OGG = "audio/ogg";
    inline static std::string AUDIO_WAV = "audio/wav";
    inline static std::string VIDEO_MPEG = "video/mpeg";
    inline static std::string VIDEO_OGG = "video/ogg";
    inline static std::string VIDEO_WEBM = "video/webm";
    inline static std::string APP_JSON = "application/json";
    inline static std::string APP_XML = "application/json";
    inline static std::string APP_PDF = "application/pdf";
    inline static std::string APP_ZIP = "application/zip";
    inline static std::string MULTIPART_FORM_DATA = "multipart/form-data";
    inline static std::string MULTIPART_MIXED = "multipart/mixed";

    static CONTENT_TYPE getContentType(const std::string& type);

    static std::string getValue(const CONTENT_TYPE& type);
};

#endif