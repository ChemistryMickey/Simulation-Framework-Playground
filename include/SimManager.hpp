#pragma once
#include <functional>
#include <unordered_map>
#include <cinttypes>

enum class SimPhase : uint8_t{
        Environment,
        Sensor,
        SensorDelay,
        Controls,
        Actuators,
        Integration,
        Logging,
        END
};

struct Job{
        std::function<void(void)> func;
        uint64_t frequency;
        SimPhase phase;

	// NOTE: Jobs are copied/moved into the simulation manager

	// If these are both zero, the job is active from the start
	// There's currently no way to reactivate a job once disabled (e.g. transients)
	uint64_t activation_time_ns = 0;
	uint64_t deactivation_time_ns = 0;

	// How many times has this job run?
	uint64_t procCounter = 0;

	// Is the job currently active
	bool isActive = true;
	
	// Only flips if  it's been deactivated and shouldn't be reactivated
	bool isDeactivated = false;

	void run(){
		func();
		procCounter += 1;
	}
};

struct SimulationManager{
	uint64_t t_freq = 1;
	double dt_s;
	double stopTime_s = 0;
        std::unordered_map<SimPhase, std::vector<Job>> active_phases{};
        std::unordered_map<SimPhase, std::vector<Job>> deactivated_phases{};

	SimulationManager(double stopTime_s_ = 0.): stopTime_s{stopTime_s_} {}

        void register_job(const Job& j);
	void register_jobs(const std::vector<Job>& js);
        void run();
	void recalculate_sim_time_step();
	void activate_deactivate_jobs(double);
};

