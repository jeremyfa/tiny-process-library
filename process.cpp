#include "process.hpp"

namespace TinyProcessLib {

Process::Process(const std::vector<string_type> &arguments, const string_type &path,
                 std::function<void(const char *bytes, size_t n)> read_stdout,
                 std::function<void(const char *bytes, size_t n)> read_stderr,
                 bool open_stdin, const Config &config) noexcept
    : closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin), config(config) {
  open(arguments, path);
  async_read();
}

Process::Process(const string_type &command, const string_type &path,
                 std::function<void(const char *bytes, size_t n)> read_stdout,
                 std::function<void(const char *bytes, size_t n)> read_stderr,
                 bool open_stdin, const Config &config) noexcept
    : closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin), config(config) {
  open(command, path);
  async_read();
}

Process::Process(const std::vector<string_type> &arguments, const string_type &path,
                 const environment_type &environment,
                 std::function<void(const char *bytes, size_t n)> read_stdout,
                 std::function<void(const char *bytes, size_t n)> read_stderr,
                 bool open_stdin, const Config &config) noexcept
    : closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin), config(config) {
  open(arguments, path, &environment);
  async_read();
}

Process::Process(const string_type &command, const string_type &path,
                 const environment_type &environment,
                 std::function<void(const char *bytes, size_t n)> read_stdout,
                 std::function<void(const char *bytes, size_t n)> read_stderr,
                 bool open_stdin, const Config &config) noexcept
    : closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin), config(config) {
  open(command, path, &environment);
  async_read();
}

Process::~Process() noexcept {
  close_fds();
}

Process::id_type Process::get_id() const noexcept {
  return data.id;
}

bool Process::write(const std::string &str) {
  return write(str.c_str(), str.size());
}

int Process::tick_until_exit_status(const std::function<void()> &tick, int tick_interval_ms) {
    while (true) {
        tick();

        int exit_status;
        if (try_get_exit_status(exit_status)) {
            tick();
            return exit_status;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(tick_interval_ms));
    }
}

} // namespace TinyProcessLib
