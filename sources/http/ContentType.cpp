#include "http/ContentType.hpp"
#include <algorithm>
#include <cctype>

CONTENT_TYPE HttpContentType::getContentType(const std::string &type)
{
    std::string TYPE = std::string(type.size(), '\0');
    std::transform(type.begin(), type.end(), TYPE.begin(), ::tolower);

    if (TYPE == TEXT_PLAIN)
        return CONTENT_TYPE::TEXT_PLAIN;
    else if (TYPE == TEXT_HTML)
        return CONTENT_TYPE::TEXT_HTML;
    else if (TYPE == TEXT_CSS)
        return CONTENT_TYPE::TEXT_CSS;
    else if (TYPE == TEXT_JAVASCRIPT)
        return CONTENT_TYPE::TEXT_JAVASCRIPT;
    else if (TYPE == IMAGE_JPEG)
        return CONTENT_TYPE::IMAGE_JPEG;
    else if (TYPE == IMAGE_PNG)
        return CONTENT_TYPE::IMAGE_PNG;
    else if (TYPE == IMAGE_GIF)
        return CONTENT_TYPE::IMAGE_GIF;
    else if (TYPE == IMAGE_SVG_XML)
        return CONTENT_TYPE::IMAGE_SVG_XML;
    else if (TYPE == AUDIO_MPEG)
        return CONTENT_TYPE::AUDIO_MPEG;
    else if (TYPE == AUDIO_OGG)
        return CONTENT_TYPE::AUDIO_OGG;
    else if (TYPE == AUDIO_WAV)
        return CONTENT_TYPE::AUDIO_WAV;
    else if (TYPE == VIDEO_MPEG)
        return CONTENT_TYPE::VIDEO_MPEG;
    else if (TYPE == VIDEO_OGG)
        return CONTENT_TYPE::VIDEO_OGG;
    else if (TYPE == VIDEO_WEBM)
        return CONTENT_TYPE::VIDEO_WEBM;
    else if (TYPE == APP_JSON)
        return CONTENT_TYPE::APP_JSON;
    else if (TYPE == APP_XML)
        return CONTENT_TYPE::APP_XML;
    else if (TYPE == APP_PDF)
        return CONTENT_TYPE::APP_PDF;
    else if (TYPE == APP_ZIP)
        return CONTENT_TYPE::APP_ZIP;
    else if (TYPE == MULTIPART_FORM_DATA)
        return CONTENT_TYPE::MULTIPART_FORM_DATA;
    else
        return CONTENT_TYPE::MULTIPART_MIXED;
}
std::string HttpContentType::getValue(const CONTENT_TYPE &type)
{
    switch (type)
    {
    case CONTENT_TYPE::TEXT_PLAIN:
        return TEXT_PLAIN;
    case CONTENT_TYPE::TEXT_HTML:
        return TEXT_HTML;
    case CONTENT_TYPE::TEXT_CSS:
        return TEXT_CSS;
    case CONTENT_TYPE::TEXT_JAVASCRIPT:
        return TEXT_JAVASCRIPT;
    case CONTENT_TYPE::IMAGE_JPEG:
        return IMAGE_JPEG;
    case CONTENT_TYPE::IMAGE_PNG:
        return IMAGE_PNG;
    case CONTENT_TYPE::IMAGE_GIF:
        return IMAGE_GIF;
    case CONTENT_TYPE::IMAGE_SVG_XML:
        return IMAGE_SVG_XML;
    case CONTENT_TYPE::AUDIO_MPEG:
        return AUDIO_MPEG;
    case CONTENT_TYPE::AUDIO_OGG:
        return AUDIO_OGG;
    case CONTENT_TYPE::AUDIO_WAV:
        return AUDIO_WAV;
    case CONTENT_TYPE::VIDEO_MPEG:
        return VIDEO_MPEG;
    case CONTENT_TYPE::VIDEO_OGG:
        return VIDEO_OGG;
    case CONTENT_TYPE::VIDEO_WEBM:
        return VIDEO_WEBM;
    case CONTENT_TYPE::APP_JSON:
        return APP_JSON;
    case CONTENT_TYPE::APP_XML:
        return APP_XML;
    case CONTENT_TYPE::APP_PDF:
        return APP_PDF;
    case CONTENT_TYPE::APP_ZIP:
        return APP_ZIP;
    case CONTENT_TYPE::MULTIPART_FORM_DATA:
        return MULTIPART_FORM_DATA;
    default:
        return MULTIPART_MIXED;
    }
}