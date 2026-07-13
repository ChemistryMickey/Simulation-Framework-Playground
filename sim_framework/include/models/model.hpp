#pragma once
#include "architecture/constants.hpp"
#include "architecture/integrator.hpp"
#include <Eigen/Core>
#include <cstddef>

template <size_t M> struct Model {
  Eigen::Map<Eigen::Matrix<double, 3, M>> state;

  Model(Integrator<DATATABLE_COLS> &integrator)
      : state(integrator.register_model(*this)) {}

  virtual void derivative() = 0;
};
