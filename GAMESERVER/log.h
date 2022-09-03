#ifndef __GAMESERVER__
#define __GAMESERVER__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <fstream>

namespace gameserver{

//日志事件，存放字段属性
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr;  // smart pointer enable copy
    /**
     * @brief 构造函数
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] file 文件名
     * @param[in] line 文件行号
     * @param[in] elapse 程序启动依赖的耗时(毫秒)
     * @param[in] thread_id 线程id
     * @param[in] fiber_id 协程id
     * @param[in] time 日志事件(秒),时间戳
     * @param[in] thread_name 线程名称
     */
    LogEvent ();
private:
    const char* m_file = nullptr;
    int32_t m_line = 0;
    uint32_t m_elapse = 0;
    uint32_t m_thread = 0;
    uint32_t m_fiberID = 0;
    uint64_t m_time = 0;
    std::string m_content;
};

class LogLevel {
public:
    /**
     * @brief 日志级别枚举
     */
    enum Level {
        /// 未知级别
        UNKNOW = 0,
        /// DEBUG 级别
        DEBUG = 1,
        /// INFO 级别
        INFO = 2,
        /// WARN 级别
        WARN = 3,
        /// ERROR 级别
        ERROR = 4,
        /// FATAL 级别
        FATAL = 5
    };

    /**
     * @brief 将日志级别转成文本输出
     * @param[in] level 日志级别
     */
    static const char* ToString(LogLevel::Level level);
    
    /**
     * @brief 将文本转换成日志级别
     * @param[in] str 日志级别文本
     */
    static LogLevel::Level FromString(const std::string& str);
};

// 输出处理
class LogHandler{
public:
    typedef std::shared_ptr<LogHandler> ptr;
    virtual ~LogHandler () {}  // destructer

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

    void setFormatter(LogFormatter::ptr val) {m_formatter = val;}
    LogFormatter::ptr getFormatter();

    virtual std::string toYamlString() = 0;
protected:
    /// 日志级别
    LogLevel::Level m_level = LogLevel::DEBUG;
    /// 是否有自己的日志格式器
    bool m_hasFormatter = false;
    /// Mutex
    MutexType m_mutex;
    /// 日志格式器
    LogFormatter::ptr m_formatter;
};

//输出格式
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    /**
     * @brief 构造函数
     * @param[in] pattern 格式模板
     * @details 
     *  %m 消息
     *  %p 日志级别
     *  %r 累计毫秒数
     *  %c 日志名称
     *  %t 线程id
     *  %n 换行
     *  %d 时间
     *  %f 文件名
     *  %l 行号
     *  %T 制表符
     *  %F 协程id
     *  %N 线程名称
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
    std::string fomat(LogEvent::ptr event);
private:
};

//日志类
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;
    // 在cpp文件里完成
    Logger (const std::string name = "root");

    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addHandler(LogHandler::ptr handler);
    void delHandler(LogHandler::ptr handler);
    LogLevel::Level getLevel() const {return m_level;}
    void setLevel(LogLevel::Level val) {m_level = val;};
private:
    std::string m_name;
    LogLevel::Level m_level;
    std::list<LogHandler::ptr> m_handlers;  //Handler集合
    LogFormatter::ptr m_formatter;
    Logger::ptr m_root; //主日志器
};

//handlers
/**
 * @brief 输出到控制台的Handler
 */
class StdoutLogHandler : public LogHandler{
public:
    typedef std::shared_ptr<StdoutLogHandler> ptr;
    // 需要实现的函数
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    virtual std::string toYamlString() override;
private:

};

/**
 * @brief 输出到文件的Handler
 */
class FlieLogHandler : public LogHandler{
public:
    typedef std::shared_ptr<FlieLogHandler> ptr;
    FlieLogHandler(const std::string& filename);
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    virtual std::string toYamlString() override;

    /**
     * @brief 重新打开日志文件
     * @return 成功返回true
     */
    bool reopen();
private:
    /// 文件路径
    std::string m_filename;
    /// 文件流
    std::ofstream m_filestream;
    /// 上次重新打开时间
    uint64_t m_lastTime = 0;
};
}

#endif