#pragma once

#define _LOG_SEV(level) BOOST_LOG_SEV(umutech::log::g_logger, level) << '(' << __STEM__ << ':' << __LINE__ << "): "

#define _LOG_TRACE() _LOG_SEV(dry::log::SeverityLevel::kTrace)
#define _LOG_DEBUG() _LOG_SEV(dry::log::SeverityLevel::kDebug)
#define _LOG_INFO() _LOG_SEV(dry::log::SeverityLevel::kInfo)
#define _LOG_WARNING() _LOG_SEV(dry::log::SeverityLevel::kWarning)
#define _LOG_ERROR() _LOG_SEV(dry::log::SeverityLevel::kError)
#define _LOG_FATAL() _LOG_SEV(dry::log::SeverityLevel::kFatal)

#define LOGF_TRACE(...) _LOG_TRACE() << std::format(__VA_ARGS__)
#define LOGF_DEBUG(...) _LOG_DEBUG() << std::format(__VA_ARGS__)
#define LOGF_INFO(...) _LOG_INFO() << std::format(__VA_ARGS__)
#define LOGF_WARNING(...) _LOG_WARNING() << std::format(__VA_ARGS__)
#define LOGF_ERROR(...) _LOG_ERROR() << std::format(__VA_ARGS__)
#define LOGF_FATAL(...) _LOG_FATAL() << std::format(__VA_ARGS__)

// Using the following macro when issues should be solved during the development/debugging phase
#if _DEBUG
#define DBG_TRACE(x) _LOG_TRACE() << (x)
#define DBG_DEBUG(x) _LOG_DEBUG() << (x)
#define DBG_INFO(x) _LOG_INFO() << (x)
#define DBG_WARNING(x) _LOG_WARNING() << (x)
#define DBG_ERROR(x) _LOG_ERROR() << (x)
// Should be bug if DBG_FATAL outputs
#define DBG_FATAL(x) _LOG_FATAL() << (x)

#define DBGF_TRACE(...) _LOG_TRACE() << std::format(__VA_ARGS__)
#define DBGF_DEBUG(...) _LOG_DEBUG() << std::format(__VA_ARGS__)
#define DBGF_INFO(...) _LOG_INFO() << std::format(__VA_ARGS__)
#define DBGF_WARNING(...) _LOG_WARNING() << std::format(__VA_ARGS__)
#define DBGF_ERROR(...) _LOG_ERROR() << std::format(__VA_ARGS__)
// Should be bug if DBG_FATAL outputs
#define DBGF_FATAL(...) _LOG_FATAL() << std::format(__VA_ARGS__)
#else
#define DBG_TRACE(x)
#define DBG_DEBUG(x)
#define DBG_INFO(x)
#define DBG_WARNING(x)
#define DBG_ERROR(x)
#define DBG_FATAL(x)

#define DBGF_TRACE(...)
#define DBGF_DEBUG(...)
#define DBGF_INFO(...)
#define DBGF_WARNING(...)
#define DBGF_ERROR(...)
#define DBGF_FATAL(...)
#endif
