#include <gtest/gtest.h>
#include "SimManager.hpp"

struct TestSimManager : ::testing::Test{
	SimulationManager simManager{1.0};

	Job simpleJob{
		.func = [](){}, // Do nothing.
		.frequency = 12,
		.phase = SimPhase::Actuators
	};

	Job delayedJob{
		.func = [](){}, // Do nothing
		.frequency = 3,
		.phase = SimPhase::Environment,
		.activation_time_ns = 500'000'000
	};

	Job timeoutJob{
		.func = [](){}, // Do nothing
		.frequency = 3,
		.phase = SimPhase::Sensors,
		.deactivation_time_ns = 500'000'000
	};

	Job activateDeactivateJob{
		.func = [](){}, // Do nothing
		.frequency = 3,
		.phase = SimPhase::Sensors,
		.activation_time_ns = 500'000'000,
		.deactivation_time_ns = 1'500'000'000
	};
};

TEST_F(TestSimManager, EmptyQueue){	
	EXPECT_DOUBLE_EQ(simManager.stopTime_s, 1.0);

	EXPECT_EQ(simManager.active_phases.size(), 0);
	EXPECT_EQ(simManager.deactivated_phases.size(), 0);
}

TEST_F(TestSimManager, OneActiveJob){
	simManager.register_job(simpleJob);

	EXPECT_EQ(simManager.active_phases.size(), 1);
	simManager.run();
	EXPECT_EQ(simManager.active_phases[SimPhase::Actuators][0].procCounter, 12);
}

TEST_F(TestSimManager, ActivateJob){
	simManager.stopTime_s = 2;
	simManager.register_job(delayedJob);
	EXPECT_EQ(simManager.active_phases.size(), 0);
	simManager.run();
	EXPECT_EQ(simManager.active_phases[delayedJob.phase][0].procCounter, 4);
}

TEST_F(TestSimManager, DeactivateJob){
	simManager.stopTime_s = 2;
	simManager.register_job(timeoutJob);
	EXPECT_EQ(simManager.active_phases.size(), 1);
	simManager.run();
	EXPECT_EQ(simManager.deactivated_phases[timeoutJob.phase][0].procCounter, 2);
}

TEST_F(TestSimManager, ActivateDeactivateJob){
	simManager.stopTime_s = 2;
	simManager.register_job(activateDeactivateJob);
	EXPECT_EQ(simManager.active_phases.size(), 0);
	simManager.run();
	EXPECT_EQ(simManager.deactivated_phases[activateDeactivateJob.phase][0].procCounter, 2);
}
