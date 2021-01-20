#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"

#include "../include/app.h"

#include <iostream>

void App::initializeLogger(YAML::Node &yml_conf) {
  std::string log_path;
  if (yml_conf["log_file"]) {
    log_path = yml_conf["log_file"].as<std::string>();
  } else {
    log_path = "../logs/app_log.txt";
  }
  conf.logger = spdlog::basic_logger_mt("app_logger", log_path);

  if (yml_conf["env"][conf.env]["log_level"].as<std::string>() == "DEBUG") {
    spdlog::set_level(spdlog::level::debug);
  } else if (yml_conf["env"][conf.env]["log_level"].as<std::string>() ==
             std::string("INFO")) {
    spdlog::set_level(spdlog::level::info);
  } else if (yml_conf["env"][conf.env]["log_level"].as<std::string>() ==
             std::string("WARN")) {
    spdlog::set_level(spdlog::level::warn);
  } else if (yml_conf["env"][conf.env]["log_level"].as<std::string>() ==
             std::string("ERROR")) {
    spdlog::set_level(spdlog::level::err);
  } else if (yml_conf["env"][conf.env]["log_level"].as<std::string>() ==
             std::string("CRITICAL")) {
    spdlog::set_level(spdlog::level::critical);
  } else {
    spdlog::set_level(spdlog::level::info);
  }
}

void App::showUsage(std::string name) {
  std::cerr
      << "Usage: " << name << " <option(s)> SOURCES"
      << "Options:\n"
      << "\t-h,--help\t\tShow this help message\n"
      << "\t-e,--env ENVIRONMENT\tSpecify the environment: staging or prod"
      << std::endl;
}

int App::parseArgs(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-h") || (arg == "--help")) {
      showUsage(argv[0]);
      return 0;
    } else if ((arg == "-e") || (arg == "--env")) {
      if (i + 1 < argc) {
        i++;
        std::string env = argv[i];
        if (env != "staging" && env != "prod") {
          std::cerr << "--env option takes its value in { prod, staging }."
                    << std::endl;
          return 1;
        }
        conf.env = env;
      } else {
        std::cerr << "--env option requires one argument." << std::endl;
        return 1;
      }
    }
  }
  return -1;
}

App::App(Config &config) { conf = config; }
App::App() {}

Config App::getConf() const { return conf; }

void App::run() { conf.logger->info("Starting..."); }