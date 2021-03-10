#ifndef APP_H
#define APP_H

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"

struct Config {
  std::shared_ptr<spdlog::logger> logger;
  std::string data_dir = "../data/";
  std::string env = "staging";
  int model_batch_size = 50;
  int max_iterations = 100;
};

class App {

public:
  /** App Base constructor
   */
  App();

  /** App Base constructor, stores the given config.
   *
   * @param config app config to start and run the application.
   */
  App(Config &config);

  /** getConf config getter
   *
   * @return config
   */
  Config getConf() const;

  /** Run start the application and run all subsequent processes
   *
   */
  void run();

  /** initializeLogger initialize the logger based on the yaml config and store
   * it into the app config
   *
   * @param yml_conf
   * @param app_conf
   */
  void initializeLogger(YAML::Node &yml_conf);

  /** showUsage print in the stderr the help information from the CLI command
   */
  static void showUsage(std::string name);

  /** parseArgs parse the args given in the CLI command and store them into the
   * app config
   *
   * @param argc from main
   * @param argv from main
   * @param app_config
   */
  int parseArgs(int argc, char *argv[]);

private:
  Config conf;
};

#endif