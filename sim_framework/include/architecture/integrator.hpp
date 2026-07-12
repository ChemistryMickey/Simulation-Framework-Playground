#pragma once
#include <Eigen/Dense>
#include <vector>
#include <functional>
#include <iostream>

#include "architecture/constants.hpp"
#include "utils/logging.hpp"
#include "magic_enum/magic_enum.hpp"

enum class IntegrationMethod {
	Euler,
	RK4,
	RK45,
	PositionVerlet
};

// TODO: Transfer this to a YAML and have the integrator take in a YAML node.
static constexpr IntegrationMethod integrationMethod = IntegrationMethod::Euler;

// Needs forward declaration. Can't just include model.hpp
template <size_t M>
struct Model;

template <size_t N>
struct Integrator {
	Eigen::Matrix<double, 3, N> dataTable{};
	size_t curHead = 0;
	std::vector<std::function<void(void)>> derivatives{};

	Integrator() {
		log<LogLevel::Debug>("Using {} integration method", magic_enum::enum_name<IntegrationMethod>(integrationMethod));
	}

	template <size_t M>
	Eigen::Map<Eigen::Matrix<double, 3, M>> register_model(Model<M>& model) {
		auto mapOut = Eigen::Map<Eigen::Matrix<double, 3, M>>(dataTable.data() + 3 * curHead, 3, M);

		derivatives.push_back([&model]() {model.derivative();});

		curHead += M;

		return mapOut;
	}

	void integrate(double dt) {
		// Just euler here for simplicity and to have mercy on my chromebook
		// But this would be configurable

		switch (integrationMethod) {
		case IntegrationMethod::Euler:
			for (auto& d : this->derivatives) {
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
