#include <iostream>
#include <vector>
#include <cinttypes>
#include <Eigen/Dense>

#include "model.hpp"
#include "integrator.hpp"
#include "constants.hpp"
#include "ChargedParticle.hpp"

enum class SimPhases{
	Environment,
	Sensor,
	SensorDelay,
	Controls,
	Actuators,
	Integration,
	Logging
};


struct SimulationManager{};

int main(){
	// Simulation plumbing
	Integrator<DATATABLE_COLS> integrator{};
	double dt = 1e-6; // [s] This should be derived from job frequency

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

	// Run simulation
	for(size_t frame = 0; frame < 10; ++frame)
	{
		std::cout << "Frame " << frame << ": \n" << integrator.dataTable << std::endl;
		integrator.integrate(dt);
	}
}
