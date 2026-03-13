#include <gtest/gtest.h>
#include <ivshm/utils/logger.h>

#include <string_view>

namespace ivshm {

namespace {
constexpr std::string_view k_log_tag = "Ivshm.Test.Logger";
}

class IvshmTestLogger : public ::testing::Test {
 public:
  auto SetUp() -> void override  // NOLINT
  {}

  auto TearDown() -> void override  // NOLINT
  {}

 protected:
};

TEST_F(IvshmTestLogger, Base) {
  constexpr std::string_view k_log_path = "test.log";
  InitLog(Severity::TRACE, k_log_path.data(), true, false, 0, 0, true);
  IVSHM_LOG_TAG(TRACE, k_log_tag, "{}", "This is type test trace log.");
  IVSHM_LOG_TAG(DEBUG, k_log_tag, "{}", "This is type test debug log.");
  IVSHM_LOG_TAG(INFO, k_log_tag, "{}", "This is type test info log.");
  IVSHM_LOG_TAG(WARN, k_log_tag, "{}", "This is type test warn log.");
  IVSHM_LOG_TAG(ERROR, k_log_tag, "{}", "This is type test error log.");
  SetMinSeverity(Severity::INFO);
  auto min_severity = GetMinSeverity();
  EXPECT_EQ(min_severity, Severity::INFO);
  SetSampleDuration(std::chrono::milliseconds(100));
  SetSampleInterval(std::chrono::milliseconds(100));
  SetConsole(false);
  auto console = GetConsole();
  EXPECT_EQ(console, false);
  SetAsync(true);
  auto async = GetAsync();
  EXPECT_EQ(async, true);
  Flush();
  StopAsyncLog();

  auto function = [](Record &record) {};
  AddAppenders(function);
  AddAppender(function);
}

}  // namespace ivshm