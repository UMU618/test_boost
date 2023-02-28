module;

#if defined(_DEBUG) && defined(_UTF8)
#include <Windows.h>
#endif

#include <filesystem>
#include <format>

#include <boost/algorithm/string.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/value_ref.hpp>

#include <dry/log.hpp>

#include "log.inc"

export module log;

namespace fs = std::filesystem;

using namespace dry::log;

#pragma region Declarations
export namespace umutech::log {
struct Config {
  bool is_file_backend_enabled = false;
  SeverityLevel level = SeverityLevel::kInfo;

  fs::path directory;
  std::wstring stem;
};

src::severity_logger<SeverityLevel> g_logger;

void initialize() noexcept;
void config(const Config& config) noexcept;
void flush() noexcept;

}  // namespace umutech::log
#pragma endregion

#pragma region Implementations
namespace {

constexpr std::wstring_view kLogDirectory{L"log"};

// Print ProcessID as std::dec
struct NativeIdVisitor {
  typedef attrs::current_process_id::value_type::native_type result_type;

  result_type operator()(
      const attrs::current_process_id::value_type& pid) const {
    return pid.native_id();
  }
};

struct PosixTimeVisitor {
  typedef std::string result_type;

  // boost::posix_time::ptime
  result_type operator()(const attrs::local_clock::value_type& ts) const {
    return to_iso_extended_string(ts);
  }

  // boost::posix_time::time_duration
  result_type operator()(const attrs::timer::value_type& time) const {
    // too long
    // return to_iso_string(time);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << time.hours() << ":"
       << std::setw(2) << time.minutes() << ":" << std::setw(2)
       << time.seconds();
    return ss.str();
  }
};

void formatter(const boost::log::record_view& rec,
               boost::log::formatting_ostream& os) noexcept {
  std::stringstream ss;
  ss << '<' << logging::extract<SeverityLevel>("Severity", rec) << '>';

  os << logging::extract<attrs::local_clock::value_type>("TimeStamp", rec)
            .apply_visitor_or_default(PosixTimeVisitor(), "")
     << " ["
     << logging::extract<attrs::timer::value_type>("Uptime", rec)
            .apply_visitor_or_default(PosixTimeVisitor(), "")
     << "] ["
     << logging::extract<attrs::current_process_id::value_type>("ProcessID",
                                                                rec)
            .apply_visitor_or_default(NativeIdVisitor(), 0)
     << "/"
     << logging::extract<attrs::current_thread_id::value_type>("ThreadID", rec)
     << "] " << std::setw(MaxSeverityLevelNameSize() + 2) << std::left
     << ss.str() << " " << rec[expr::message];
}

void configBasicLogger(SeverityLevel log_level) noexcept {
  logging::add_common_attributes();
  auto core = logging::core::get();
  core->add_global_attribute("Uptime", attrs::timer());
  core->set_filter(Severity >= log_level);

#if _DEBUG
#ifdef _UTF8
  SetConsoleOutputCP(CP_UTF8);
#endif
  typedef sinks::asynchronous_sink<sinks::text_ostream_backend> TextSink;
  auto console_sink = boost::make_shared<TextSink>();
  console_sink->set_formatter(&formatter);
  auto backend = console_sink->locked_backend();
  backend->add_stream(
      boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
  backend->auto_flush(true);
  backend->set_auto_newline_mode(sinks::auto_newline_mode::always_insert);
  core->add_sink(console_sink);
#endif
}

void configLoggers(const umutech::log::Config& config) noexcept {
  namespace fs = std::filesystem;
  auto core = logging::core::get();
  if (!config.directory.empty()) {
    if (config.is_file_backend_enabled) {
      typedef sinks::asynchronous_sink<sinks::text_file_backend> FileSink;

      fs::path log_file{config.directory / (config.stem + L".log")};
      auto file_sink = boost::make_shared<FileSink>(
          keywords::file_name = log_file,
          keywords::target_file_name = config.stem + L"_%Y%m%d_%H%M%S_%5N.log",
          keywords::time_based_rotation =
              sinks::file::rotation_at_time_point(0, 0, 0));
      file_sink->set_formatter(&formatter);
      auto backend = file_sink->locked_backend();
      backend->set_file_collector(sinks::file::make_collector(
          keywords::target = config.directory,
          keywords::max_size = 1024 * 1024 * 1024,  // 1GB
          keywords::min_free_space = 100 * 1024 * 1024,
          keywords::max_files = 512));
      backend->scan_for_files();
      backend->auto_flush(true);
      backend->set_open_mode(std::ios_base::app);
      backend->set_auto_newline_mode(sinks::auto_newline_mode::always_insert);
      core->add_sink(file_sink);
    } else {
      DBG_ERROR("Failed to create log!");
    }
  }

  core->set_filter(Severity >= config.level);
}

}  // namespace

namespace umutech::log {

void initialize() noexcept {
  configBasicLogger(SeverityLevel::kInfo);
}

void config(const Config& config) noexcept {
  configLoggers(config);
}

void flush() noexcept {
  auto core = logging::core::get();
  core->flush();
}

}  // namespace umutech::log
#pragma endregion
