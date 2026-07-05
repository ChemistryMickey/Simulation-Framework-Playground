#pragma once
#include <Eigen/Dense>
#include <vector>
#include <functional>
#include <iostream>

#include "constants.hpp"

// Needs forward declaration. Can't just include model.hpp
template <size_t M>
struct Model;

template <size_t N>
struct Integrator{
        Eigen::Matrix<double, 3, N> dataTable{};
        size_t curHead = 0;
        std::vector<std::function<void(void)>> derivatives{};

        template <size_t M>
        Eigen::Map<Eigen::Matrix<double, 3, M>> register_model(Model<M>& model){
                auto mapOut = Eigen::Map<Eigen::Matrix<double, 3, M>>(dataTable.data() + 3 * curHead, 3, M);

                derivatives.push_back([&model](){model.derivative();});

                curHead += M;

                return mapOut;
        }

        void integrate(double dt){
                // Just euler here for simplicity and to have mercy on my chromebook
                for(auto& d : this->derivatives){
                        d();
                }
                dataTable.row(0) += dataTable.row(1) * dt;
                dataTable.row(1) += dataTable.row(2) * dt;
        }
};
