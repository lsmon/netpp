#ifndef __STATUS_HPP__
#define __STATUS_HPP__

#include <sstream>
#include <functional>

enum CODES
{
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCESSING = 102,
    EARLY_HINTS = 103,
    OK = 200,
    CREATED = 201,
    ACCPETED = 202,
    NON_AUTHORATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    ALREADY_REPORT = 208,
    IM_USED = 226,
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    UNUSED = 306,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEAST = 400,
    UNAUTHORIZE = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    PAYLOAD_TOO_LARGE = 413,
    URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    RANGE_NOT_STIFIABLE = 416,
    EXPECATION_FAILED = 417,
    IM_A_TEAPOT = 418,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_CONTENT = 422,
    LOCKED = 423,
    FAILED_DEPENDENCY = 424,
    TOO_EARLY = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS = 429,
    REQUEST_HEADER_FILEDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSUFICIENT_STORAGE = 507,
    LOOP_DETECTED = 508,
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511
};

struct Status
{
    std::stringstream ss;
    Status(int c)
    {
        switch (c)
        {
        case CONTINUE:
            ss << "Code: [" << c << "] Message: continue";
            break;
        case SWITCHING_PROTOCOLS:
            ss << "Code: [" << c << "] Message: switching protocols";
            break;
        case PROCESSING:
            ss << "Code: [" << c << "] Message: processing";
            break;
        case EARLY_HINTS:
            ss << "Code: [" << c << "] Message: early hints";
            break;
        case OK:
            ss << "Code: [" << c << "] Message: ok";
            break;
        case CREATED:
            ss << "Code: [" << c << "] Message: created";
            break;
        case ACCPETED:
            ss << "Code: [" << c << "] Message: accpeted";
            break;
        case NON_AUTHORATIVE_INFORMATION:
            ss << "Code: [" << c << "] Message: non authorative information";
            break;
        case NO_CONTENT:
            ss << "Code: [" << c << "] Message: no content";
            break;
        case RESET_CONTENT:
            ss << "Code: [" << c << "] Message: reset content";
            break;
        case PARTIAL_CONTENT:
            ss << "Code: [" << c << "] Message: partial content";
            break;
        case MULTI_STATUS:
            ss << "Code: [" << c << "] Message: multi status";
            break;
        case ALREADY_REPORT:
            ss << "Code: [" << c << "] Message: already report";
            break;
        case IM_USED:
            ss << "Code: [" << c << "] Message: im used";
            break;
        case MULTIPLE_CHOICES:
            ss << "Code: [" << c << "] Message: multiple choices";
            break;
        case MOVED_PERMANENTLY:
            ss << "Code: [" << c << "] Message: moved permanently";
            break;
        case FOUND:
            ss << "Code: [" << c << "] Message: found";
            break;
        case SEE_OTHER:
            ss << "Code: [" << c << "] Message: see other";
            break;
        case NOT_MODIFIED:
            ss << "Code: [" << c << "] Message: not modified";
            break;
        case USE_PROXY:
            ss << "Code: [" << c << "] Message: use proxy";
            break;
        case UNUSED:
            ss << "Code: [" << c << "] Message: unused";
            break;
        case TEMPORARY_REDIRECT:
            ss << "Code: [" << c << "] Message: temporary redirect";
            break;
        case PERMANENT_REDIRECT:
            ss << "Code: [" << c << "] Message: permanent redirect";
            break;
        case BAD_REQUEAST:
            ss << "Code: [" << c << "] Message: bad requeast";
            break;
        case UNAUTHORIZE:
            ss << "Code: [" << c << "] Message: unauthorize";
            break;
        case PAYMENT_REQUIRED:
            ss << "Code: [" << c << "] Message: payment required";
            break;
        case FORBIDDEN:
            ss << "Code: [" << c << "] Message: forbidden";
            break;
        case NOT_FOUND:
            ss << "Code: [" << c << "] Message: not found";
            break;
        case METHOD_NOT_ALLOWED:
            ss << "Code: [" << c << "] Message: method not allowed";
            break;
        case NOT_ACCEPTABLE:
            ss << "Code: [" << c << "] Message: not acceptable";
            break;
        case PROXY_AUTHENTICATION_REQUIRED:
            ss << "Code: [" << c << "] Message: proxy authentication required";
            break;
        case REQUEST_TIMEOUT:
            ss << "Code: [" << c << "] Message: request timeout";
            break;
        case CONFLICT:
            ss << "Code: [" << c << "] Message: conflict";
            break;
        case GONE:
            ss << "Code: [" << c << "] Message: gone";
            break;
        case LENGTH_REQUIRED:
            ss << "Code: [" << c << "] Message: length required";
            break;
        case PRECONDITION_FAILED:
            ss << "Code: [" << c << "] Message: precondition failed";
            break;
        case PAYLOAD_TOO_LARGE:
            ss << "Code: [" << c << "] Message: payload too large";
            break;
        case URI_TOO_LONG:
            ss << "Code: [" << c << "] Message: uri too long";
            break;
        case UNSUPPORTED_MEDIA_TYPE:
            ss << "Code: [" << c << "] Message: unsupported media type";
            break;
        case RANGE_NOT_STIFIABLE:
            ss << "Code: [" << c << "] Message: range not stifiable";
            break;
        case EXPECATION_FAILED:
            ss << "Code: [" << c << "] Message: expecation failed";
            break;
        case IM_A_TEAPOT:
            ss << "Code: [" << c << "] Message: im a teapot";
            break;
        case MISDIRECTED_REQUEST:
            ss << "Code: [" << c << "] Message: misdirected request";
            break;
        case UNPROCESSABLE_CONTENT:
            ss << "Code: [" << c << "] Message: unprocessable content";
            break;
        case LOCKED:
            ss << "Code: [" << c << "] Message: locked";
            break;
        case FAILED_DEPENDENCY:
            ss << "Code: [" << c << "] Message: failed dependency";
            break;
        case TOO_EARLY:
            ss << "Code: [" << c << "] Message: too early";
            break;
        case UPGRADE_REQUIRED:
            ss << "Code: [" << c << "] Message: upgrade required";
            break;
        case PRECONDITION_REQUIRED:
            ss << "Code: [" << c << "] Message: precondition required";
            break;
        case TOO_MANY_REQUESTS:
            ss << "Code: [" << c << "] Message: too many requests";
            break;
        case REQUEST_HEADER_FILEDS_TOO_LARGE:
            ss << "Code: [" << c << "] Message: request header fileds too large";
            break;
        case UNAVAILABLE_FOR_LEGAL_REASONS:
            ss << "Code: [" << c << "] Message: unavailable for legal reasons";
            break;
        case INTERNAL_SERVER_ERROR:
            ss << "Code: [" << c << "] Message: internal server error";
            break;
        case NOT_IMPLEMENTED:
            ss << "Code: [" << c << "] Message: not implemented";
            break;
        case BAD_GATEWAY:
            ss << "Code: [" << c << "] Message: bad gateway";
            break;
        case SERVICE_UNAVAILABLE:
            ss << "Code: [" << c << "] Message: service unavailable";
            break;
        case GATEWAY_TIMEOUT:
            ss << "Code: [" << c << "] Message: gateway timeout";
            break;
        case HTTP_VERSION_NOT_SUPPORTED:
            ss << "Code: [" << c << "] Message: http version not supported";
            break;
        case VARIANT_ALSO_NEGOTIATES:
            ss << "Code: [" << c << "] Message: variant also negotiates";
            break;
        case INSUFICIENT_STORAGE:
            ss << "Code: [" << c << "] Message: insuficient storage";
            break;
        case LOOP_DETECTED:
            ss << "Code: [" << c << "] Message: loop detected";
            break;
        case NOT_EXTENDED:
            ss << "Code: [" << c << "] Message: not extended";
            break;
        case NETWORK_AUTHENTICATION_REQUIRED:
            ss << "Code: [" << c << "] Message: network authentication required";
            break;
        default:
            ss << "Code: [" << c << "] Message: unknown status";
            break;
        }
    }
};

#endif