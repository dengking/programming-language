#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging

#include <memory>
#include <sstream>

static const char *DefaultFormat = "[%Y-%m-%d %H:%M:%S.%e][%l][%t][%s:%#] %v";
using LoggerType = std::shared_ptr<spdlog::logger>;
class LoggerBase
{
public:
	virtual ~LoggerBase()
	{
		spdlog::shutdown();
	}
	inline LoggerBase &SetEnable(bool Enable)
	{
		m_Enable = Enable;
		return *this;
	}
	inline LoggerBase &SetEnable(int Enable)
	{
		if (Enable <= 0)
		{
			m_Enable = false;
		}
		else
		{
			m_Enable = true;
		}
		return *this;
	}
	inline LoggerBase &SetLogLevel(int LogLevel)
	{
		if (m_Logger)
		{
			m_Logger->set_level(ToSpdlogLevel(LogLevel));
		}
		return *this;
	}
	inline LoggerBase &SetEncryptFlag(bool Flag)
	{
		m_EncryptFlag = Flag;
		return *this;
	}
	static inline spdlog::level::level_enum ToSpdlogLevel(int Level)
	{
		if (Level == 1)
		{
			return spdlog::level::level_enum::debug;
		}
		else if (Level == 2)
		{
			return spdlog::level::level_enum::info;
		}
		else if (Level == 3)
		{
			return spdlog::level::level_enum::warn;
		}
		else if (Level == 4)
		{
			return spdlog::level::level_enum::err;
		}
		else if (Level == 5)
		{
			return spdlog::level::level_enum::critical;
		}
		else
		{
			return spdlog::level::level_enum::info;
		}
	}
	inline LoggerType GetLogger()
	{
		return m_Logger;
	}
	inline bool IsEnable()
	{
		return m_Enable;
	}
	/**
	 * 是否加密
	 * @return
	 */
	inline bool IsEncrypt()
	{
		return m_EncryptFlag;
	}

protected:
	bool m_Enable{true};
	spdlog::level::level_enum m_LogLevel{spdlog::level::level_enum::info};
	bool m_EncryptFlag{false};
	LoggerType m_Logger;
};

class CommunicateLogger : public LoggerBase
{
public:
	static inline CommunicateLogger &Instance()
	{
		static CommunicateLogger Logger;
		return Logger;
	}
	inline CommunicateLogger &SetTime(int Hour = 23, int Minute = 59)
	{
		m_Hour = Hour;
		m_Minute = Minute;
		return *this;
	}

private:
	CommunicateLogger()
	{
		std::string FileName = "logs/Communicate.log";

		// m_Logger = spdlog::basic_logger_mt("daily_communicate_logger", "logs/Communicate.log");
		m_Logger = spdlog::daily_logger_mt("daily_communicate_logger", FileName, m_Hour, m_Minute);
		spdlog::set_pattern(DefaultFormat); // [文件名] [行号]
		m_Logger->set_level(spdlog::level::level_enum::info);
		spdlog::flush_on(spdlog::level::info);
	}

private:
	int m_Hour{23};
	int m_Minute{59};
};

/**
 * 系统日志，默认使用这个logger
 */
class SystemLogger : public LoggerBase
{
public:
	static inline SystemLogger &Instance()
	{
		static SystemLogger Logger;
		return Logger;
	}

private:
	SystemLogger()
	{
		m_Logger = spdlog::daily_logger_mt("daily_system_logger", "logs/System.log", m_Hour, m_Minute);
		spdlog::set_pattern(DefaultFormat); // [文件名] [函数名] [行号]
		m_Logger->set_level(spdlog::level::level_enum::info);
		spdlog::flush_on(spdlog::level::info);
	}

private:
	int m_Hour{23};
	int m_Minute{59};
};
//#define COMMUNICATE_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__)
#define FORMAT_STRING(Buffer, fmt, ...)                       \
	{                                                         \
		snprintf(Buffer, sizeof(Buffer), fmt, ##__VA_ARGS__); \
	}

#define FLUSH_LOG() {CommunicateLogger::Instance().GetLogger()->flush(); SystemLogger::Instance().GetLogger()->flush();}
#pragma region 通信日志 /////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOG_MSG_DEBUG(...)                                                           \
	if (CommunicateLogger::Instance().IsEnable())                                    \
	{                                                                                \
		SPDLOG_LOGGER_DEBUG(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_MSG_INFO(...)                                                           \
	if (CommunicateLogger::Instance().IsEnable())                                   \
	{                                                                               \
		SPDLOG_LOGGER_INFO(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_MSG_WARN(...)                                                           \
	if (CommunicateLogger::Instance().IsEnable())                                   \
	{                                                                               \
		SPDLOG_LOGGER_WARN(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_MSG_ERROR(...)                                                           \
	{                                                                                \
		SPDLOG_LOGGER_ERROR(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_MSG_CRITICAL(...)                                                           \
	if (CommunicateLogger::Instance().IsEnable())                                       \
	{                                                                                   \
		SPDLOG_LOGGER_CRITICAL(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_MSG_BODY_DEBUG(struct_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                                  \
	{                                                                                              \
		std::ostringstream Stream;                                                                 \
		Stream << struct_obj_ptr;                                                                  \
		SPDLOG_LOGGER_DEBUG(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_MSG_BODY_INFO(struct_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                                 \
	{                                                                                             \
		std::ostringstream Stream;                                                                \
		Stream << struct_obj_ptr;                                                                 \
		SPDLOG_LOGGER_INFO(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_MSG_BODY_WARN(struct_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                                 \
	{                                                                                             \
		std::ostringstream Stream;                                                                \
		Stream << struct_obj_ptr;                                                                 \
		SPDLOG_LOGGER_WARN(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_MSG_BODY_ERROR(struct_obj_ptr)                                                         \
	{                                                                                              \
		std::ostringstream Stream;                                                                 \
		Stream << struct_obj_ptr;                                                                  \
		SPDLOG_LOGGER_ERROR(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_MSG_BODY_CRITICAL(struct_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                                     \
	{                                                                                                 \
		std::ostringstream Stream;                                                                    \
		Stream << struct_obj_ptr;                                                                     \
		SPDLOG_LOGGER_CRITICAL(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_PACKER_DEBUG(packer_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                                \
	{                                                                                            \
		std::ostringstream Stream;                                                               \
		Stream << packer_obj_ptr;                                                                \
		SPDLOG_LOGGER_DEBUG(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_PACKER_INFO(packer_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                               \
	{                                                                                           \
		std::ostringstream Stream;                                                              \
		Stream << packer_obj_ptr;                                                               \
		SPDLOG_LOGGER_INFO(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_PACKER_WARN(packer_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                               \
	{                                                                                           \
		std::ostringstream Stream;                                                              \
		Stream << packer_obj_ptr;                                                               \
		SPDLOG_LOGGER_WARN(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_PACKER_ERROR(packer_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                                \
	{                                                                                            \
		std::ostringstream Stream;                                                               \
		Stream << packer_obj_ptr;                                                                \
		SPDLOG_LOGGER_ERROR(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}

#define LOG_PACKER_CRITICAL(packer_obj_ptr)                                                         \
	if (CommunicateLogger::Instance().IsEnable())                                                   \
	{                                                                                               \
		std::ostringstream Stream;                                                                  \
		Stream << packer_obj_ptr;                                                                   \
		SPDLOG_LOGGER_CRITICAL(CommunicateLogger::Instance().GetLogger(), "{}", Stream.str()); \
	}
#define COMMUNICATE_LOG_DEBUG(...) if(CommunicateLogger::Instance().IsEnable()) SPDLOG_LOGGER_DEBUG(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__)

#define COMMUNICATE_LOG_INFO(...) if(CommunicateLogger::Instance().IsEnable()) SPDLOG_LOGGER_INFO(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__)

#define COMMUNICATE_LOG_WARN(...) if(CommunicateLogger::Instance().IsEnable()) SPDLOG_LOGGER_WARN(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__)

#define COMMUNICATE_LOG_ERROR(...) if(CommunicateLogger::Instance().IsEnable()) SPDLOG_LOGGER_ERROR(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__)

#define COMMUNICATE_LOG_CRITICAL(...) if(CommunicateLogger::Instance().IsEnable()) SPDLOG_LOGGER_CRITICAL(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__)

#pragma endregion 通信日志 /////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region 系统日志 /////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOG_DEBUG(...)                                                          \
	if (SystemLogger::Instance().IsEnable())                                    \
	{                                                                           \
		SPDLOG_LOGGER_DEBUG(SystemLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_INFO(...)                                                          \
	if (SystemLogger::Instance().IsEnable())                                   \
	{                                                                          \
		SPDLOG_LOGGER_INFO(SystemLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_WARN(...)                                                          \
	if (SystemLogger::Instance().IsEnable())                                   \
	{                                                                          \
		SPDLOG_LOGGER_WARN(SystemLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_ERROR(...)                                                          \
	if (SystemLogger::Instance().IsEnable())                                    \
	{                                                                           \
		SPDLOG_LOGGER_ERROR(SystemLogger::Instance().GetLogger(), __VA_ARGS__); \
	}

#define LOG_CRITICAL(...)                                                          \
	if (SystemLogger::Instance().IsEnable())                                       \
	{                                                                              \
		SPDLOG_LOGGER_CRITICAL(SystemLogger::Instance().GetLogger(), __VA_ARGS__); \
	}
#pragma endregion 系统日志 /////////////////////////////////////////////////////////////////////////////////////////////////////
#endif					   /* PUBLIC_API_API_IMPL_COMMON_NEW_LOGGER_LOG_H_ */
