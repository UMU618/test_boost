//#include <sdkddkver.h>
//#define BOOST_USE_WINAPI_VERSION _WIN32_WINNT
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

enum class SeverityLevel { trace, debug, info, warning, error, fatal };
BOOST_LOG_ATTRIBUTE_KEYWORD(Severity, "Severity", SeverityLevel);

void SetSeverityFilter(logging::trivial::severity_level severity_level) {
  // boost::shared_ptr<logging::core> core = logging::core::get();
  logging::core::get()->set_filter(logging::trivial::severity >=
                                   severity_level);
}

int main() {
  src::logger lg;
  BOOST_LOG(lg) << "ST +";

  src::logger_mt lg_mt;
  BOOST_LOG(lg_mt) << "MT +";

  SetSeverityFilter(logging::trivial::info);

  BOOST_LOG(lg) << "ST -";
  BOOST_LOG(lg_mt) << "MT -";

  // src::severity_logger_mt
  src::severity_logger<SeverityLevel> slg;
  //logging::core::get()->set_filter(Severity >= SeverityLevel::debug);
  BOOST_LOG_SEV(slg, SeverityLevel::trace) << "A normal severity message";
  BOOST_LOG_SEV(slg, SeverityLevel::debug) << "A warning severity message";
  BOOST_LOG_SEV(slg, SeverityLevel::info) << "An error severity message";

  BOOST_LOG_TRIVIAL(trace) << "trace";
  BOOST_LOG_TRIVIAL(debug) << "debug";
  BOOST_LOG_TRIVIAL(info) << "info";
  BOOST_LOG_TRIVIAL(warning) << "warning";
  BOOST_LOG_TRIVIAL(error) << "error";
  BOOST_LOG_TRIVIAL(fatal) << "fatal";
}
