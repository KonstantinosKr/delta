/*
 * The step-loop contract this app needs from a 3D engine: one step, one record
 * snapshot, two diagnostics. It exists so main() does not #ifdef around the two
 * engines -- no physics lives here.
 *
 * shortcut: app-local adapter, promote to src/core/backend.h when a third
 * backend or a second consumer (test/app) needs the same contract.
 */
#ifndef DELTA_APPS_DEM3D_BACKEND_H_
#define DELTA_APPS_DEM3D_BACKEND_H_

#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/data/Meta.h"
#include "core/data/ParticleRecord.h"
#include "dim3/backends/serial/Engine.h"
#include "dim3/world/structure/Object.h"
#ifdef DELTA_USE_AMREX
#include "dim3/backends/amrex/DemAmrexEngine.h"
#endif

namespace delta {
namespace app {

class Backend
{
public:
	virtual ~Backend() {}

	virtual void step() = 0;
	// Non-const: both engines' record accessors are non-const-qualified.
	virtual std::vector<delta::core::data::ParticleRecord>& records() = 0;
	virtual std::size_t contacts() = 0;
	virtual iREAL maxForce() = 0;
};

/* The serial Engine: owns its state, no global initialisation required. */
class SerialBackend : public Backend
{
public:
	SerialBackend(std::vector<delta::world::structure::Object> particles,
				  const std::array<iREAL, 6>& boundary,
				  const delta::core::data::Meta::Simulation& meta)
		: _engine(std::move(particles), boundary, meta) {}

	void step() { _engine.iterate(); }

	std::vector<delta::core::data::ParticleRecord>& records()
	{
		return _engine.getParticleRecords();
	}
	std::size_t contacts() { return _engine.getContactPairs().size(); }
	iREAL maxForce() { return _engine.getMaxForceMagnitude(); }

private:
	delta::core::Engine _engine;
};

#ifdef DELTA_USE_AMREX
/*
 * The AMReX backend. amrex::Initialize() must already have been called, and the
 * object must be destroyed before amrex::Finalize().
 *
 * Its container carries one position per particle, so the initial state comes
 * from the serial engine: that is what makes _centre and _centreOfMass agree
 * bit for bit, which the container requires.
 */
class AmrexBackend : public Backend
{
public:
	AmrexBackend(std::vector<delta::world::structure::Object> particles,
				 const std::array<iREAL, 6>& boundary,
				 const delta::core::data::Meta::Simulation& meta,
				 delta::core::parallel::DemParallelConfig config)
		: _meta(meta)
	{
		delta::core::Engine seed(particles, boundary, meta);
		_state = seed.getParticleRecords();
		_engine.reset(new delta::core::parallel::DemAmrexEngine(_state, config));
		_engine->syncTo(_state);
		if (!_engine->sizingIsSound())
		{
			std::cerr << "amrex backend: unsound neighbour sizing: "
					  << _engine->sizingDiagnostic() << std::endl;
		}
	}

	void step()
	{
		_engine->step(_meta.dt, _meta.gravityVector);
		_engine->syncTo(_state);
	}

	std::vector<delta::core::data::ParticleRecord>& records() { return _state; }
	std::size_t contacts() { return static_cast<std::size_t>(_engine->contactPairCount()); }
	iREAL maxForce() { return _engine->maxForceMagnitude(); }

private:
	delta::core::data::Meta::Simulation                      _meta;
	std::vector<delta::core::data::ParticleRecord>            _state;
	std::unique_ptr<delta::core::parallel::DemAmrexEngine>    _engine;
};
#endif

/*
 * Names: "serial" (always), "amrex" (only when configured with
 * -DDELTA_USE_AMREX=ON). Returns nullptr for an unknown or unbuilt name and
 * explains why on stderr.
 */
inline std::unique_ptr<Backend> makeBackend(const std::string& name,
											std::vector<delta::world::structure::Object> particles,
											const std::array<iREAL, 6>& boundary,
											const delta::core::data::Meta::Simulation& meta,
											const std::string& gridMode = "nogrid")
{
	if (name == "serial")
	{
		return std::unique_ptr<Backend>(new SerialBackend(std::move(particles), boundary, meta));
	}
	if (name == "amrex")
	{
#ifdef DELTA_USE_AMREX
		delta::core::parallel::DemParallelConfig config;
		if (gridMode == "regular")       {config.mode = delta::core::parallel::GridMode::Regular;}
		else if (gridMode == "adaptive") {config.mode = delta::core::parallel::GridMode::Adaptive;}
		else if (gridMode != "nogrid")
		{
			std::cerr << "unknown --mode '" << gridMode << "' (nogrid|regular|adaptive)" << std::endl;
			return std::unique_ptr<Backend>();
		}
		return std::unique_ptr<Backend>(
			new AmrexBackend(std::move(particles), boundary, meta, config));
#else
		(void)gridMode;
		std::cerr << "backend 'amrex' is not compiled in; configure with -DDELTA_USE_AMREX=ON"
				  << std::endl;
		return std::unique_ptr<Backend>();
#endif
	}
	std::cerr << "unknown --backend '" << name << "' (serial|amrex)" << std::endl;
	return std::unique_ptr<Backend>();
}

}  // namespace app
}  // namespace delta

#endif /* DELTA_APPS_DEM3D_BACKEND_H_ */
