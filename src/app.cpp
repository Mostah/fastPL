#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"

#include "../include/app.h"
#include "../include/learning/HeuristicPipeline.h"
#include "../include/types/AlternativesPerformance.h"
#include "../include/types/DataGenerator.h"

#include <filesystem>
#include <iostream>

void App::initializeLogger(YAML::Node &yml_conf) {
  std::string log_path;
  if (yml_conf["log_file"]) {
    conf.log_file = yml_conf["log_file"].as<std::string>();
  }
  conf.logger = spdlog::basic_logger_mt("app_logger", conf.log_file);

  if (yml_conf["log_level"].as<std::string>() == "DEBUG") {
    spdlog::set_level(spdlog::level::debug);
  } else if (yml_conf["log_level"].as<std::string>() == std::string("INFO")) {
    spdlog::set_level(spdlog::level::info);
  } else if (yml_conf["log_level"].as<std::string>() == std::string("WARN")) {
    spdlog::set_level(spdlog::level::warn);
  } else if (yml_conf["log_level"].as<std::string>() == std::string("ERROR")) {
    spdlog::set_level(spdlog::level::err);
  } else if (yml_conf["log_level"].as<std::string>() ==
             std::string("CRITICAL")) {
    spdlog::set_level(spdlog::level::critical);
  } else {
    spdlog::set_level(spdlog::level::info);
  }
}

void App::extractConfig(YAML::Node &yml_conf) {
  if (yml_conf["data_dir"]) {
    conf.data_dir = yml_conf["data_dir"].as<std::string>();
  }
  if (yml_conf["model_batch_size"]) {
    conf.model_batch_size = yml_conf["model_batch_size"].as<int>();
  }
  if (yml_conf["max_iterations"]) {
    conf.max_iterations = yml_conf["max_iterations"].as<int>();
  }
  this->initializeLogger(yml_conf);
}

void App::showUsage(std::string name) {
  std::cerr << "Usage: " << name << " <option(s)> SOURCES"
            << "Options:\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "\t-d,--dataset DATASET\tDataset path\n"
            << "\t-o,--output OUTPUT\tModel output path" << std::endl;
}

int App::parseArgs(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if ((arg == "-h") || (arg == "--help")) {
      showUsage(argv[0]);
      return 0;
    } else if ((arg == "-d") || (arg == "--dataset")) {
      if (i + 1 < argc) {
        i++;
        std::string dataset = argv[i];
        conf.dataset = dataset;
      } else {
        std::cerr << "--dataset option requires one argument." << std::endl;
        return 1;
      }
    } else if ((arg == "-o") || (arg == "--output")) {
      if (i + 1 < argc) {
        i++;
        std::string output = argv[i];
        conf.output = output;
      } else {
        std::cerr << "--output option requires one argument." << std::endl;
        return 1;
      }
    }
  }
  if (conf.dataset == "") {
    std::cerr << "\n--dataset option is required.\n" << std::endl;
    showUsage(argv[0]);
    return 1;
  }
  if (conf.output == "") {
    std::cerr << "\n--output option is required.\n" << std::endl;
    showUsage(argv[0]);
    return 1;
  }
  return -1;
}

App::App(Config &config) { conf = config; }
App::App() {}

Config App::getConf() const { return conf; }

int App::run() {
  conf.logger->info("Starting...");
  DataGenerator dg = DataGenerator(conf);
  std::string data_path = conf.data_dir + conf.dataset;
  std::string model_path = conf.data_dir + conf.output;

  // verify dataset exists at given path
  std::filesystem::path data_f{data_path};
  if (!std::filesystem::exists(data_f)) {
    std::cerr << "No file found in dataset path: " << data_path << std::endl;
    conf.logger->error("No file found in data set path.");
    return 1;
  }
  // verify output directory path exists
  std::string directory_models;
  for (int i = model_path.size() - 1; i >= 0; i--) {
    if (model_path[i] == *"/") {
      directory_models = model_path.substr(0, i);
      break;
    }
  }
  std::filesystem::path model_f{directory_models};
  if (!std::filesystem::exists(model_f)) {
    std::cerr << "No directory found for output at: " << directory_models
              << std::endl;
    conf.logger->error("No directory found for output.");
    return 1;
  }

  AlternativesPerformance dataset = dg.loadDataset(data_path);
  conf.logger->info("Dataset loaded");

  HeuristicPipeline hp = HeuristicPipeline(conf, dataset);
  MRSortModel opti = hp.start();
  conf.logger->info("Saving models...");
  dg.saveModel(model_path, opti.lambda, opti.criteria, opti.profiles, true,
               opti.getId());
  conf.logger->info("App terminated");
  return 0;
}