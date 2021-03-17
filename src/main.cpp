/**
 * @file main.cpp
 * @author Emma Dixneuf, Thibault Monsel, Thomas Vindard
 * @version 0.1
 * @date 4 February 2021
 * @brief main entry of the project
 *
 * School project aiming at scaling up the pymcda repository by
 * implementing the Olivier Sobrie's metaheuristic in C++ with parallelization.
 */

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"
#include <iostream>

#include "../include/app.h"

int main(int argc, char *argv[]) {
  YAML::Node yaml_config;
  try {
    yaml_config = YAML::LoadFile("../app-config.yaml");
  } catch (...) {
    std::cerr << "ERROR: app-config.yaml file not found at ../. from where the "
                 "program got executed."
              << std::endl;
    return 1;
  }
  App app = App();

  int exec_code = app.parseArgs(argc, argv);
  if (exec_code != -1) {
    return exec_code;
  }

  app.initializeLogger(yaml_config);
  Config app_conf = app.getConf();

  std::string conf_info =
      "Config loaded: { env: " + app_conf.env + ", log_level: " +
      yaml_config["env"][app_conf.env]["log_level"].as<std::string>() +
      ", log_path: " + yaml_config["log_file"].as<std::string>() +
      ", data_dir: " + app_conf.data_dir + " }";
  app_conf.logger->info(conf_info.c_str());

  app.run();

  app_conf.logger->info("App terminated");
  return 0;
}
