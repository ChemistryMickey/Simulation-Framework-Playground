#pragma once
#include "architecture/integrator.hpp"
#include "models/model.hpp"
#include "utils/LateRef.hpp"
#include <Eigen/Dense>

struct ChargedParticle : Model<3> {
  Eigen::Vector3d &localMagField;
  double charge_C;
  double mass_kg;

  // Philosophically, should an E&M thing know about everything that produces a
  // field? Can we, instead have something which is able to agglomerate all the
  // fields produced by all particles?
  std::vector<LateRef<ChargedParticle>> otherParticles{};

  // Default is proton charge and mass
  ChargedParticle(Integrator<DATATABLE_COLS> &integrator,
                  Eigen::Vector3d &localMagField_, double charge_C_ = 1.6e-19,
                  double mass_kg_ = 1.67e-27);

  Eigen::Vector3d magfield_at_point(const Eigen::Vector3d &pos);
  Eigen::Vector3d elecfield_at_point(const Eigen::Vector3d &pos);
  void derivative() override;
};
