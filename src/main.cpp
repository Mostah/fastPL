#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"
#include <iostream>

struct Config {
  std::__1::shared_ptr<spdlog::logger> logger;
  std::string data_dir = "../data/";
  std::string env = "staging";
};

void initialize_logger(YAML::Node &yml_conf, Config &app_conf) {
  std::string log_path;
  if (yml_conf["log_file"]) {
    log_path = yml_conf["log_file"].as<std::string>();
  } else {
    log_path = "../logs/app_log.txt";
  }
  app_conf.logger =
      spdlog::basic_logger_mt("app_logger", "../logs/app_log.txt");

  if (yml_conf["env"][app_conf.env]["log_level"].as<std::string>() == "DEBUG") {
    spdlog::set_level(spdlog::level::debug);
  } else if (yml_conf["env"][app_conf.env]["log_level"].as<std::string>() ==
             std::string("INFO")) {
    spdlog::set_level(spdlog::level::info);
  } else if (yml_conf["env"][app_conf.env]["log_level"].as<std::string>() ==
             std::string("WARN")) {
    spdlog::set_level(spdlog::level::warn);
  } else if (yml_conf["env"][app_conf.env]["log_level"].as<std::string>() ==
             std::string("ERROR")) {
    spdlog::set_level(spdlog::level::err);
  } else if (yml_conf["env"][app_conf.env]["log_level"].as<std::string>() ==
             std::string("CRITICAL")) {
    spdlog::set_level(spdlog::level::critical);
  } else {
    spdlog::set_level(spdlog::level::info);
  }
}

static void show_usage(std::string name) {
  std::cerr
      << "Usage: " << name << " <option(s)> SOURCES"
      << "Options:\n"
      << "\t-h,--help\t\tShow this help message\n"
      << "\t-e,--env ENVIRONMENT\tSpecify the environment: staging or prod"
      << std::endl;
}

int parseArgs(int argc, char *argv[], Config &app_conf) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-h") || (arg == "--help")) {
      show_usage(argv[0]);
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
        app_conf.env = env;
      } else {
        std::cerr << "--env option requires one argument." << std::endl;
        return 1;
      }
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  YAML::Node yaml_config = YAML::LoadFile("../app-config.yaml");
  Config app_conf;

  int exec_code = parseArgs(argc, argv, app_conf);
  if (exec_code == 1) {
    return 1;
  } else if (exec_code == 0) {
    return 0;
  }

  initialize_logger(yaml_config, app_conf);

  app_conf.logger->info("Starting...");
  std::string conf_info =
      "Config loaded: { env: " + app_conf.env + ", log_level: " +
      yaml_config["env"][app_conf.env]["log_level"].as<std::string>() +
      ", log_path: " + yaml_config["log_file"].as<std::string>() +
      ", data_dir: " + app_conf.data_dir + " }";
  app_conf.logger->info(conf_info.c_str());

  app_conf.logger->info("App terminated");
  return 0;
}
