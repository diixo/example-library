
#pragma once

#include <string_view>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/logger.h>

static std::string formattedTraceText(const char* pFormat, ...)
{
    va_list arg;

    va_start(arg, pFormat);

    std::string formattedText;
    int size = vsnprintf(nullptr, 0, pFormat, arg);
    va_end(arg);

    formattedText.resize(size);

    va_start(arg, pFormat);
    vsnprintf(&formattedText[0], size + 1, pFormat, arg);
    va_end(arg);
    return formattedText;
}

#define EXT_LOGGER log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("exiv"))

#undef LOG_VERBOSE
#define LOG_VERBOSE(format, args...)                                                               \
    do {                                                                                           \
        LOG4CPLUS_TRACE(EXT_LOGGER, LOG4CPLUS_TEXT(formattedTraceText(format, ##args)));          \
    } while (false)

#undef LOG_DEBUG
#define LOG_DEBUG(format, args...)                                                                 \
    do {                                                                                           \
        LOG4CPLUS_DEBUG(EXT_LOGGER, LOG4CPLUS_TEXT(formattedTraceText(format, ##args)));          \
    } while (false)

#undef LOG_INFO
#define LOG_INFO(format, args...)                                                                  \
    do {                                                                                           \
        LOG4CPLUS_INFO(EXT_LOGGER, LOG4CPLUS_TEXT(formattedTraceText(format, ##args)));           \
    } while (false)

#undef LOG_ERROR
#define LOG_ERROR(format, args...)                                                                 \
    do {                                                                                           \
        LOG4CPLUS_ERROR(EXT_LOGGER, LOG4CPLUS_TEXT(formattedTraceText(format, ##args)));          \
    } while (false)

#undef LOG_WARN
#define LOG_WARN(format, args...)                                                                  \
    do {                                                                                           \
        LOG4CPLUS_WARN(EXT_LOGGER, LOG4CPLUS_TEXT(formattedTraceText(format, ##args)));           \
    } while (false)

#undef LOG_FATAL
#define LOG_FATAL(format, args...)                                                                 \
    do {                                                                                           \
        LOG4CPLUS_FATAL(EXT_LOGGER, LOG4CPLUS_TEXT(formattedTraceText(format, ##args)));          \
    } while (false)

#define DBG_MSG(parenArgsList) LOG_INFO parenArgsList

#define DBG_WARNING(parenArgsList) LOG_WARN parenArgsList

#define DBG_ERROR(parenArgsList) LOG_ERROR parenArgsList

#undef LOG_TRC_SCOPE
#define LOG_TRC_SCOPE(argList)                                                                     \
    do {                                                                                           \
        std::string argScopeList = #argList;                                                       \
        LOG4CPLUS_DEBUG(EXT_LOGGER, argScopeList);                                                \
    } while (false)

#undef TRC_SCOPE_DEF
#define TRC_SCOPE_DEF(pathName, className, methodName) /* Empty */

#undef TRC_SCOPE
#define TRC_SCOPE(pathName, className, methodName)                                                 \
    LOG_TRC_SCOPE(pathName##_##className##_##methodName)
