#include "SimManager.hpp"
#include <numeric>
#include <iostream>
#include <cmath>
#include <string>
#include "magic_enum/magic_enum.hpp"
#include "logging.hpp"

void SimulationManager::register_job(Job& j){
	if(j.activation_time_ns != 0){
		log<LogLevel::Trace>("Registering deactivated job at {}", (void*)&j);

		j.isActive = false;
		deactivated_phases[j.phase].emplace_back(j);
	} else{
		active_phases[j.phase].emplace_back(j);
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

void SimulationManager::register_jobs(std::vector<Job>& js){
	for(auto& j : js){
		register_job(j);
	}
}

void SimulationManager::activate_deactivate_jobs(double curSimTime_s){
	log<LogLevel::Trace>("Beginning activate/deactivate jobs");

	for(uint8_t phase = 0; phase < static_cast<uint8_t>(SimPhase::END); ++phase){
		auto& activeJobs = active_phases[static_cast<SimPhase>(phase)];
		auto& deactivatedJobs = deactivated_phases[static_cast<SimPhase>(phase)];

		log<LogLevel::Trace>("{} ({}): Checking to activate jobs", magic_enum::enum_name(static_cast<SimPhase>(phase)), curSimTime_s);
		log<LogLevel::Debug>("Num active jobs: {}, Num deactivated jobs: {}", activeJobs.size(), deactivatedJobs.size());

		// Check to activate
		for(size_t i = 0; i < deactivatedJobs.size(); ++i){
			auto& job = deactivatedJobs.at(i);

			if(job.isDeactivated) continue;

			if((job.isActive == false) && (curSimTime_s * 1e9 > job.activation_time_ns)){
				log<LogLevel::Debug>("Activating job at {}", (void*) &job);

				job.isActive = true;
				// Why copy rather than move? Ideally this doesn't happen too often
				activeJobs.emplace_back(std::move(job));
			}
		}

		// Clean up now defunct deactivated jobs
		for(ssize_t i = deactivatedJobs.size() - 1; i > -1; --i){
			if(deactivatedJobs[i].isActive){
				deactivatedJobs.erase(deactivatedJobs.begin() + i);
				recalculate_sim_time_step();
			}
		}
		log<LogLevel::Trace>("Got through activation");

		// Check to deactivate
		for(size_t i = 0; i < activeJobs.size(); ++i){
			auto& job = activeJobs.at(i);
			if(job.deactivation_time_ns == 0) continue;

			if(curSimTime_s * 1e9 > job.deactivation_time_ns){
				log<LogLevel::Debug>("Deactivating job at {}", (void*) &job);

				job.isActive = false;
				deactivatedJobs.emplace_back(std::move(job));
			}
		}

		// Cleanup now defunct active jobs	
		for(ssize_t i = activeJobs.size() - 1; i > -1; --i){
			if(!activeJobs[i].isActive){
				activeJobs.erase(activeJobs.begin() + i);
				recalculate_sim_time_step();
			}
		}
		log<LogLevel::Trace>("Got through deactivation");
	}
}

void SimulationManager::run(){
	double curSimTime_s = 0;
	while(curSimTime_s < stopTime_s)
	{
		activate_deactivate_jobs(curSimTime_s);
		recalculate_sim_time_step();
		log<LogLevel::Trace>("Calculated sim time step to be {} [s]", dt_s);

		// Run jobs
		for(uint8_t phase = 0; phase < static_cast<uint8_t>(SimPhase::END); ++phase){
			for(auto& job : active_phases[static_cast<SimPhase>(phase)]){
				job.run();
			}
		}

		// Tick time
		curSimTime_s += dt_s;
		log<LogLevel::Trace>("Incremented time to {}", curSimTime_s);
	}
}
