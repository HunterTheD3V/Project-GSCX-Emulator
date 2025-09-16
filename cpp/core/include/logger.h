#pragma once
#include <string>
#include <functional>

namespace gscx {

class Logger {
public:
    using Sink = std::function<void(const char*)>;

    static void set_info(Sink s) { info_ = std::move(s); }
    static void set_warn(Sink s) { warn_ = std::move(s); }
    static void set_error(Sink s) { error_ = std::move(s); }

    static void info(const std::string& m)  { if (info_)  info_(m.c_str()); }
    static void warn(const std::string& m)  { if (warn_)  warn_(m.c_str()); }
    static void error(const std::string& m) { if (error_) error_(m.c_str()); }

private:
    static inline Sink info_{};
    static inline Sink warn_{};
    static inline Sink error_{};
};

} // namespace gscx