# The mrw::log namespace

The mrw::log namespace provieds classes vor convenient Qt logging. You can register multiple LoggerBase instances into the LoggerService singleton. The logging methods of each LoggingBase instance is called in the order of their registration.

```mermaid
classDiagram

class LoggerService
<<Singleton>> LoggerService

class LoggerBase
<<Abstract>> LoggerBase
LoggerBase : + debug(const char * message)
LoggerBase : + info(const char * message)
LoggerBase : + warn(const char * message)
LoggerBase : + critical(const char * message)
LoggerBase : + fatal(const char * message)

class UnifiedLogger
<<Abstract>> UnifiedLogger
UnifiedLogger : # write(const char * message)

LoggerBase <|-- UnifiedLogger
LoggerBase <|-- StdLogger
UnifiedLogger <|-- FileLogger
FileLogger <|-- TimestampedFileLogger


LoggerService ..> LoggerBase : uses
LoggerBase ..> LoggerService : register

```
