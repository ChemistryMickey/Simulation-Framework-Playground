#include <gtest/gtest.h>
#include <vector>
#include "Eigen/Core"
#include "models/model.hpp"
#include "architecture/integrator.hpp"
#include "architecture/SimManager.hpp"
#include "models/ChargedParticle.hpp"

struct TestSimpleIntegration : ::testing::Test {
	Integrator<6> integrator{};
	SimulationManager simManager{0.05};

	// Environment
	Eigen::Vector3d extMagField{0, 0, 1e-3};

	// Sesnors
	ChargedParticle proton{integrator, extMagField};
	ChargedParticle electron{integrator, extMagField, 1.6e-19, -9.11e-31};

	// Controllers

	// Actuators

	// Logging

	void SetUp() override {
		// Connect circular dependencies
		proton.otherParticles.emplace_back(electron);
		electron.otherParticles.emplace_back(proton);

		// Register integration job
		std::vector<Job> jobs{
			{
				.func = [this]() {this->integrator.integrate(1.0 / 10.0);},
				.frequency = 10,
				.phase = SimPhase::Integration
			}
		};
		simManager.register_jobs(jobs);
	}
};

TEST_F(TestSimpleIntegration, SimpleRun) {
	// Setup particle states
	proton.state(1, 0) = 1;
	proton.state(0, 0) = 3;
	electron.state(1, 1) = 0.5;

	// Runzo
	simManager.run();
}
