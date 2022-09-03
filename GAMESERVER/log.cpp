#include "log.h"

namespace gameserver{

Logger::Logger (const std::string name)
    :m_name(name) {
}

void Logger::addHandler(LogHandler::ptr handler){
    m_handlers.push_back(handler);
}

void Logger::delHandler(LogHandler::ptr handler){
    for(auto it = m_handlers.begin(); it != m_handlers.end(); ++it) {
        if (*it == handler) {
            m_handlers.erase(it);
            break;
        }
    }
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level) {
        for(auto& i : m_handlers) {
            i->log(self, level, event);  // handler 的 log函数
        }       
    }
}

void Logger::debug(LogEvent::ptr event){
    log(LogLevel::DEBUG, event);
}

void Logger::info(LogEvent::ptr event){
    log(LogLevel::INFO, event);
}

void Logger::warn(LogEvent::ptr event){
    log(LogLevel::WARN, event);
}

void Logger::error(LogEvent::ptr event){
    log(LogLevel::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event){
    log(LogLevel::FATAL, event);
}

FlieLogHandler::FlieLogHandler(const std::string& filename)
    :m_filename(filename){
}

void FlieLogHandler::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    if(level >= m_level) {
        m_filestream << m_formatter.format(event)
    }
}

bool FlieLogHandler::reopen() {
    if(m_filestream){
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;  // !! 意思是非0转为1，0还是0
}

void StdoutLogHandler::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    if(level >= m_level) {
        MutexType::Lock lock(m_mutex);
        m_formatter->format(std::cout, logger, level, event);
    }
}

}