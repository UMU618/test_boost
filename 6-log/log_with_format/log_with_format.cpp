#include <cstdlib>
#include <string>

#include <boost/log/attributes.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/make_shared.hpp>
#include <boost/phoenix.hpp>
#include <boost/shared_ptr.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

enum severity_level { info, warning, error, fatal, trace };

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id,
                            "ThreadID",
                            attrs::current_thread_id::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger,
                                       src::severity_logger_mt<severity_level>);

attrs::current_thread_id::value_type::native_type get_native_id1(
    logging::value_ref<attrs::current_thread_id::value_type,
                       tag::thread_id> const& tid) {
  if (tid)
    return tid->native_id();
  return 0;
}

struct get_native_id_impl {
  typedef attrs::current_thread_id::value_type::native_type result_type;

  get_native_id_impl() {}
  result_type operator()(
      logging::value_ref<attrs::current_thread_id::value_type,
                         tag::thread_id> const& tid) const {
    if (tid)
      return tid->native_id();
    return 0;
  }
};
boost::phoenix::function<get_native_id_impl> get_native_id2;

void init_logging() {
  logging::add_common_attributes();

  typedef logging::sinks::synchronous_sink<logging::sinks::text_ostream_backend>
      text_sink;

  // console
  boost::shared_ptr<text_sink> sinkConsole = boost::make_shared<text_sink>();
  sinkConsole->locked_backend()->add_stream(
      boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
  sinkConsole->locked_backend()->auto_flush(true);
  logging::core::get()->add_sink(sinkConsole);

  sinkConsole->set_formatter(
      expr::stream << "[" << severity << " " << thread_id << " "
                   << boost::phoenix::bind(&get_native_id1, thread_id.or_none())
                   << " " << get_native_id2(thread_id.or_none()) << "] "
                   << expr::smessage);
}

int main(int, char*[]) {
  init_logging();

  BOOST_LOG_SEV(my_logger::get(), info) << "log message 1";

  return 0;
}
