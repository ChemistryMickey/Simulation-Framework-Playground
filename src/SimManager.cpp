#include "SimManager.hpp"
#include <numeric>
#include <iostream>
#include <cmath>
#include "magic_enum/magic_enum.hpp"

void SimulationManager::register_job(const Job& j){
	if(j.activation_time_ns != 0){
		deactivated_phases[j.phase].push_back(j);
		// Oooo... I wonder how to handle delta t for suddenly activated jobs.
		// Crumbs.
	} else{
		active_phases[j.phase].push_back(j);
	}
}

void SimulationManager::recalculate_sim_time_step(){
	t_freq = 1; // reset
	for(auto& [_, jobs] : active_phases){
		for(auto& job : jobs){
			// Update your dt (in frequency because of integer stuff)
			t_freq = std::lcm(t_freq, job.frequency);	
		}
	}

	dt_s = 1.0/t_freq;
}

void SimulationManager::register_jobs(const std::vector<Job>& js){
	for(auto& j : js){
		register_job(j);
	}
}

void SimulationManager::activate_deactivate_jobs(double curSimTime_s){
	for(uint8_t phase = 0; phase < static_cast<uint8_t>(SimPhase::END); ++phase){
		auto& activeJobs = active_phases[static_cast<SimPhase>(phase)];
		auto& deactivatedJobs = deactivated_phases[static_cast<SimPhase>(phase)];

		// Check to activate
		for(size_t i = 0; i < deactivatedJobs.size(); ++i){
			auto& job = deactivatedJobs.at(i);

			if(job.isDeactivated) continue;

			if((job.isActive == false) && (curSimTime_s * 1e9 > job.activation_time_ns)){
				job.isActive = true;
				// Why copy rather than move? Because the lifetime of lambdas is crazy
				activeJobs.emplace_back(job);
				recalculate_sim_time_step();
			}
		}

		// Clean up now defunct deactivated jobs
		for(ssize_t i = deactivatedJobs.size() - 1; i > -1; --i){
			if(deactivatedJobs[i].isActive){
				deactivatedJobs.erase(deactivatedJobs.begin() + i);
			}
		}

		// Check to deactivate
		for(size_t i = 0; i < activeJobs.size(); ++i){
			auto& job = activeJobs[i];
			if(job.deactivation_time_ns == 0) continue;

			if(curSimTime_s * 1e9 > job.deactivation_time_ns){
				job.isActive = false;
				deactivatedJobs.emplace_back(job);
				recalculate_sim_time_step();
			}
		}

		// Cleanup now defunct active jobs	
		for(ssize_t i = deactivatedJobs.size() - 1; i > -1; --i){
			if(!activeJobs[i].isActive){
				activeJobs.erase(activeJobs.begin() + i);
			}
		}
	}
}

void SimulationManager::run(){
	recalculate_sim_time_step();
	std::cout << "Computed initial sim time step to be " << dt_s << " [s]\n";

	double curSimTime_s = 0;
	while(curSimTime_s < stopTime_s)
	{
		activate_deactivate_jobs(curSimTime_s);

		// Run jobs
		for(uint8_t phase = 0; phase < static_cast<uint8_t>(SimPhase::END); ++phase){
			for(auto& job : active_phases[static_cast<SimPhase>(phase)]){
				job.run();
			}
		}

		// Tick time
		curSimTime_s += dt_s;
	}
}
