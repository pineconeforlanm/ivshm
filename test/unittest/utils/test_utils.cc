#include <gtest/gtest.h>
#include <ivshm/utils/logger.h>
#include <ivshm/utils/utils.h>

namespace ivshm {
namespace {
constexpr std::string_view k_log_tag = "Ivshm.Test.Utils";
}
class IvshmUtilsTest : public ::testing::Test {
 public:
  auto SetUp() -> void override  // NOLINT
  {}

  auto TearDown() -> void override  // NOLINT
  {}
};

TEST_F(IvshmUtilsTest, Base) {
  [[maybe_unused]] auto is_host = IsHost();
  // EXPECT_TRUE(is_host);

  auto content = ReadFileContent("/etc/os-release");
  EXPECT_TRUE(content.has_value());
  EXPECT_FALSE(content.value().empty());

  content = ReadFileContent("/etc/os-release_not_exist");
  EXPECT_FALSE(content.has_value());
}

class IvshmTestError : public ::testing::Test {
 public:
  auto SetUp() -> void override  // NOLINT
  {}

  auto TearDown() -> void override  // NOLINT
  {}

 protected:
};

TEST_F(IvshmTestError, Base) {
  constexpr auto location = std::source_location::current();
  constexpr auto error = Error(Kind::Unknown, location);
  [[maybe_unused]] auto error_location = error.Location();
  EXPECT_EQ(error.GetErrorKind(), Error::Kind::Unknown);
  EXPECT_EQ(error.Line(), location.line());
  EXPECT_EQ(error.Column(), location.column());
  EXPECT_EQ(error.FunctionName(),
            "virtual void ivshm::IvshmTestError_Base_Test::TestBody()");
  EXPECT_EQ(error.FullFileName(), __FILE__);
  EXPECT_EQ(error.FileName(), "test_utils.cc");
}

TEST_F(IvshmTestError, Message) {
  constexpr auto location = std::source_location::current();
  constexpr auto error = Error(Kind::Unknown, location);
  const auto has_backtrace = error.Message(true);
  EXPECT_FALSE(has_backtrace.empty());
  const auto error_message =
      std::format("Kind: [0:Unknown] at test_utils.cc:{}:{}", location.line(),
                  location.column());
  EXPECT_EQ(error.Message(), error_message);
  [[maybe_unused]] auto ok_message = error.Message(false);
  [[maybe_unused]] auto ok_has_backtrace = error.Message(true);
  try {
    constexpr auto k_bad_kind = static_cast<Error::Kind>(0xf0);
    const auto bad_error = Error(k_bad_kind);
    [[maybe_unused]] auto bad_message = bad_error.Message();
    [[maybe_unused]] auto bad_has_message = bad_error.Message(true);
  } catch (const std::exception& exp) {
    IVSHM_LOG_TAG(ERROR, k_log_tag, "Exception: {}", exp.what());
  }
}

}  // namespace ivshm