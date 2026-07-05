#pragma once
#include <Eigen/Dense>
#include "constants.hpp"
#include "integrator.hpp"

template <size_t M>
struct Model{
        Eigen::Map<Eigen::Matrix<double, 3, M>> state;

        Model(Integrator<DATATABLE_COLS>& integrator):
                state(integrator.register_model(*this)) 
        {
        }
 
        virtual void derivative() = 0;
};
