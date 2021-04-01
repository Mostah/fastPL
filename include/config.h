#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief Config datastructure of the app.
 */

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

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
  std::string log_file = "../logs/app_log.txt";
  int model_batch_size = 50; /*!< Batch size of model for the learning algo */
  int max_iterations =
      100; /*!< Max iteration before terminating the learning algo */
  std::string dataset = "";
  std::string output = "";
};

#endif