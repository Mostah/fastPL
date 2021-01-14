#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"
#include <iostream>

#include "../include/app.h"

int main(int argc, char *argv[]) {
  YAML::Node yaml_config = YAML::LoadFile("../app-config.yaml");
  App app = App();

  int exec_code = app.parseArgs(argc, argv);
  if (exec_code == 1) {
    return 1;
  } else if (exec_code == 0) {
    return 0;
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
