#pragma once
#include <cstddef>
#include <Eigen/Core>
#include "architecture/constants.hpp"
#include "architecture/integrator.hpp"

template <size_t M>
struct Model {
        Eigen::Map<Eigen::Matrix<double, 3, M>> state;

        Model(Integrator<DATATABLE_COLS>& integrator) :
                state(integrator.register_model(*this)) {}

        virtual void derivative() = 0;
};
