#include <iostream>
#include <vector>
#include <cinttypes>
#include <array>
#include <functional>

#include "math.hpp"

static constexpr size_t DATATABLE_SIZE = 3;

enum class SimPhases{
	Environment,
	Sensor,
	SensorDelay,
	Controls,
	Actuators,
	Integration,
	Logging
};

template <size_t M>
struct Model;

template <size_t N>
struct Integrator{
	double stateTable[N]{};
	double dotStateTable[N]{};
	double ddotStateTable[N]{};
	size_t curHead = 0;
	std::vector<std::function<void(void)>> derivatives{};

	template <size_t M>
	void register_model(Model<M>& model){
		// std::cout << "CurHead State Table addresses: " << &stateTable[curHead] << ", " << &dotStateTable[curHead] << ", " << &ddotStateTable[curHead] << std::endl;
		model.statePtr = &stateTable[curHead];
		model.dotStatePtr = &dotStateTable[curHead];
		model.ddotStatePtr = &ddotStateTable[curHead];

		derivatives.push_back([&model](){model.derivative();});

		curHead += M;
	}

	void integrate(double dt){
		// Just euler here for simplicity and to have mercy on my chromebook
		for(auto& d : this->derivatives){
			d();
		}
		for(size_t i = 0; i < DATATABLE_SIZE; ++i){
			this->stateTable[i] += this->dotStateTable[i] * dt;
			this->dotStateTable[i] += this->ddotStateTable[i] * dt;
		}
	}

	void print_state_table()
	{
		static size_t callCount = 0;
		std::cout << callCount << ": ";
		for(size_t i = 0; i < DATATABLE_SIZE; ++i)
		{
			std::cout << "\t" << stateTable[i] << ", " << dotStateTable[i] << ", " << ddotStateTable[i];
			std::cout << std::endl;
		}
		std::cout << std::endl;

		callCount += 1;
	}
};

template <size_t M>
struct Model{
	double* statePtr = nullptr;
	double* dotStatePtr = nullptr;
	double* ddotStatePtr = nullptr;
	Model(Integrator<DATATABLE_SIZE>& integrator){
		integrator.register_model(*this);
	}

	virtual void derivative() = 0;
};

struct SimulationManager{};

// Models
struct Proton : Model<3>
{
	std::array<double, 3>* localMagField = nullptr;
	double charge_C = 1.6e-18; // This number isn't quite right. Electron mass is 9.11e-31, proton mass is 1.67e-27 but I forget the charge.
	double mass_kg = 1.67e-27;

	Proton(Integrator<DATATABLE_SIZE>& integrator, std::array<double, 3>* localMagField_): Model<3>{integrator}, localMagField{localMagField_} {
		// std::cout << "Acquired pointers: " << this->statePtr << ", " << this->dotStatePtr << ", " << this->ddotStatePtr << std::endl;
	}

	void derivative() override {
		// F = q(v \times B)
		// a = q(v \times B) / m

		auto crossProduct = math::cross(this->dotStatePtr, this->localMagField->data());
		for(size_t i = 0; i < 3; ++i){
			this->ddotStatePtr[i] = this->charge_C * (this->dotStatePtr[i] * crossProduct[i]) / this->mass_kg;
		}
	}

	//std::array<double, 3> magfield_perturbation(const std::array<double, 3>& pos)
	//{
	//}
};

int main(){
	// Simulation plumbing
	Integrator<DATATABLE_SIZE> integrator{};
	double dt = 1e-6; // [s] This should be derived from job frequency

	// Setup models
	std::array<double, 3> localMagField{0, 0, 1e-3};

	Proton p1{integrator, &localMagField};
	p1.dotStatePtr[0] = 1;
	//p1.dotStatePtr[1] = 1;

	// Proton p2{integrator, &localMagField};

	// Run simulation
	for(size_t frame = 0; frame < 10; ++frame)
	{
		integrator.print_state_table();
		integrator.integrate(dt);
	}
}
