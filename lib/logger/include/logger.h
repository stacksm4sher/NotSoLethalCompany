#ifndef LOGGER_LIBRARY_H
#define LOGGER_LIBRARY_H

enum {
    ERROR_LEVEL = 0,
    INFO_LEVEL = 1,
    WARN_LEVEL = 2,
    TRACE_LEVEL = 3
};

static const int DEFAULT_LOG_LEVEL = INFO_LEVEL;

void setLogLevel(int logLevel);

void log_error(const char *message, ...);
void log_info(const char *message, ...);
void log_warn(const char *message, ...);
void log_trace(const char *message, ...);

#endif //LOGGER_LIBRARY_H
