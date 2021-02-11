#ifndef APP_H
#define APP_H

/**
 * @file app.h
 * @brief app container responsible of handling everything related to fastpl.
 *
 * The app file hold the Config structure as well as the App class.
 */

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"

/**
 * @struct Config app.h
 * @brief High level configuration of the app.
 *
 * Config structure is responsible for holding the high level configuration
 * defined by the user to run the application and to apply over the whole
 * pipeline.
 */
struct Config {
  std::shared_ptr<spdlog::logger>
      logger; /*!< Logger pointer when logging is requested. */
  std::string data_dir = "../data/"; /*!< Directory where the data is stored */
  std::string env =
      "staging"; /*!< Environment (defining the set of config) to use */
};

/**
 * @class App app.h
 * @brief App container of the fastpl project.
 *
 * App is responsible of setting up the right environment of execution given by
the config and launch the fastpl pipeline to solve the problem. It is also
responsible for error handling to avoid crashing the whole application and
terminating everything properly.
*/
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