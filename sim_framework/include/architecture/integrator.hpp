#pragma once
#include "yaml-cpp/yaml.h"
#include <Eigen/Dense>
#include <functional>
#include <vector>

#include "magic_enum/magic_enum.hpp"
#include "utils/logging.hpp"

enum class IntegrationMethod { Euler, RK4, RK45, PositionVerlet };

// Needs forward declaration. Can't just include model.hpp
template <size_t M> struct Model;

struct IntegratorConfig {
  IntegrationMethod integrationMethod;
};
template <size_t N> struct Integrator {
  Eigen::Matrix<double, 3, N> dataTable{};
  size_t curHead = 0;
  std::vector<std::function<void(void)>> derivatives{};
  IntegratorConfig config;

  Integrator(const YAML::Node &node) {
    config.integrationMethod =
        magic_enum::enum_cast<IntegrationMethod>(
            node["simulation"]["integration"]["method"].as<std::string>())
            .value();

    log<LogLevel::Debug>(
        "Using {} integration method",
        magic_enum::enum_name<IntegrationMethod>(config.integrationMethod));
  }

  template <size_t M>
  Eigen::Map<Eigen::Matrix<double, 3, M>> register_model(Model<M> &model) {
    auto mapOut = Eigen::Map<Eigen::Matrix<double, 3, M>>(
        dataTable.data() + 3 * curHead, 3, M);

    derivatives.push_back([&model]() { model.derivative(); });

    curHead += M;

    return mapOut;
  }

  void integrate(double dt) {
    // Just euler here for simplicity and to have mercy on my chromebook
    // But this would be configurable

    switch (config.integrationMethod) {
    case IntegrationMethod::Euler:
      for (auto &d : this->derivatives) {
        d();
      }
      dataTable.row(0) += dataTable.row(1) * dt;
      dataTable.row(1) += dataTable.row(2) * dt;
      break;
    case IntegrationMethod::RK4:
      throw std::runtime_error("Integration method not implemented");
    case IntegrationMethod::RK45:
      throw std::runtime_error("Integration method not implemented");
    case IntegrationMethod::PositionVerlet:
      throw std::runtime_error("Integration method not implemented");
    default:
      throw std::runtime_error("Integration method not implemented");
    }
  }
};
