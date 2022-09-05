#include <iostream>
#include "Log/log.h"

int main(int argc, char** argv) {
    gameserver::Logger::ptr logger(new gameserver::Logger);
    logger->addHandler(gameserver::LogHandler::ptr(new gameserver::StdoutLogHandler));

    // gameserver::FileLogHandler::ptr file_Handler(new gameserver::FileLogHandler("./log.txt"));
    // gameserver::LogFormatter::ptr fmt(new gameserver::LogFormatter("%d%T%p%T%m%n"));
    // file_Handler->setFormatter(fmt);
    // file_Handler->setLevel(gameserver::LogLevel::ERROR);

    // logger->addHandler(file_Handler);

    gameserver::LogEvent::ptr event(new gameserver::LogEvent(logger, __FILE__, __LINE__, 0, 1, 2, time(0), "name"));
    //event->getSS() << "hello gameserver log";
    logger->log(gameserver::LogLevel::DEBUG, event);
    std::cout << "hello gameserver log" << std::endl;

    return 0;
}