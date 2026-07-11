- [x] Get Eigen
- [x] Get google test
- [x] Sim controller dynamic time update w/ job on off
- [ ] Flatbuffers schema definition
- [ ] Benchmarking capabilities
- Steal from Financial Monte Carlo
	- [ ] Convert to Bazel once you're back on something more powerful than a chomebook
		- Also, figure out how not to invalidate the cache between config builds. That's annoying
	- [ ] Convert to GCC > 12 once you're back on something more powerful than a chromebook
		- Requires fixing the logger
	- [ ] Timeseries logging via RTTR
	- [ ] YAML serialization? A fully reflectable thing there with boost pfr?
- [ ] SimController should check if it would step over a job that should be activated. If so, it should update the timestep to the diff
- [ ] Dynamics should update every loop. That should be a special job that has no frequency.
- [ ] Build things as a library rather than an application
- [ ] More better integration tests w/ things like closed form solutions and error calcs

Questions:
- [ ] Is it OK for things to only support integer frequencies?
- [ ] How do you want to handle circular dependencies?

