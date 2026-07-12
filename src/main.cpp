#include <iostream>
#include <vector>
#include <cinttypes>
#include "Eigen/Dense"
#include <unordered_map>

#include "models/model.hpp"
#include "architecture/integrator.hpp"
#include "architecture/SimManager.hpp"
#include "architecture/constants.hpp"
#include "models/ChargedParticle.hpp"

int main() {
	// Simulation plumbing
	Integrator<DATATABLE_COLS> integrator{};
	SimulationManager simManager{0.05};

	// Setup models
	// Environment
	Eigen::Vector3d localMagField{0, 0, 1e-3};

	// Sensor/Actuators (hm... maybe this is where the fine point lies. Something is both a sensor and an actuator)
	ChargedParticle proton{integrator, localMagField};
	proton.state(1, 0) = 1;
	proton.state(0, 0) = 3;

	ChargedParticle electron{integrator, localMagField, 1.6e-19, 9.11e-31};
	electron.state(1, 1) = 0.5;

	// Connect models that can't otherwise be connected at construction time due to circular dependencies
	electron.otherParticles.emplace_back(proton);
	proton.otherParticles.emplace_back(electron);

	// Register things with the Simulation Manager
	std::vector<Job> jobs{
		{
			.func = [&integrator]() {std::cout << integrator.dataTable << std::endl;},
			.frequency = 17,
			.phase = SimPhase::Integration
		},
		{
			.func = [&integrator, &simManager]() {integrator.integrate(1.0 / 10.0);},
			.frequency = 10,
			.phase = SimPhase::Integration
		}
	};
	simManager.register_jobs(jobs);

	// Punch it
	simManager.run();
}

