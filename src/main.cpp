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
#include "../include/config.h"

int main(int argc, char *argv[]) {
  // yaml config that holds general config of the application
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

  // get all the data to set up the app
  int exec_code = app.parseArgs(argc, argv);
  if (exec_code != -1) {
    return exec_code;
  }
  app.extractConfig(yaml_config);

  Config app_conf = app.getConf();

  std::string conf_info =
      "Config loaded: { log_level: " +
      yaml_config["log_level"].as<std::string>() +
      ", log_path: " + app_conf.log_file + ", data_dir: " + app_conf.data_dir +
      ", model_batch_size: " + std::to_string(app_conf.model_batch_size) +
      ", max_iterations: " + std::to_string(app_conf.max_iterations) +
      ", dataset: " + app_conf.dataset + ", output: " + app_conf.output + " }";
  app_conf.logger->info(conf_info.c_str());

  // launch the app
  int app_status = app.run();

  // return status: success or failure
  return app_status;
}
