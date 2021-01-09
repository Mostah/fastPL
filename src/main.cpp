#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <iostream>

void initialize_logger() {
  auto logger = spdlog::basic_logger_mt("app_logger", "../logs/app_log.txt");
  spdlog::set_level(spdlog::level::info);
}

int main() {
  initialize_logger();
  auto logger = spdlog::get("app_logger");
  logger->info("Starting...");

  logger->info("App terminated");
  return 0;
}
