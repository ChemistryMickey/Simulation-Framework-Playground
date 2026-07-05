#include <Eigen/Dense>
#include "model.hpp"
#include "LateRef.hpp"
#include "integrator.hpp"
#include "ChargedParticle.hpp"

ChargedParticle::ChargedParticle(Integrator<DATATABLE_COLS>& integrator, Eigen::Vector3d& localMagField_, double charge_C_, double mass_kg_): 
                Model<3>{integrator}, 
                localMagField{localMagField_}, 
                charge_C{charge_C_}, 
                mass_kg{mass_kg_}
        {
        }

Eigen::Vector3d ChargedParticle::magfield_at_point(const Eigen::Vector3d& pos){
	double leadingFactor = mu0/(4*pi) * this->charge_C;
	Eigen::Vector3d dist = pos - Eigen::Vector3d(state.row(0));
	Eigen::Vector3d numerator = state.row(1).cross(dist.normalized());
	double denominator = dist.dot(dist);

	// std::cout << "Leading Factor: " << leadingFactor << ", dist: " << dist.transpose() << ", Numerator: " << numerator.transpose() << ", Denominator: " << denominator << std::endl;

	return leadingFactor * (numerator/denominator);
}

Eigen::Vector3d ChargedParticle::elecfield_at_point(const Eigen::Vector3d& pos){
	double leadingFactor = 1/(4*pi*ep0) * this->charge_C;
	Eigen::Vector3d dist = pos - Eigen::Vector3d(state.row(0));
	Eigen::Vector3d numerator = dist.normalized();
	double denominator = dist.dot(dist);
	
	return leadingFactor * (numerator/denominator);
}

void ChargedParticle::derivative(){
	// F = qE + q(v \times B)
	// a = (q/m) * (E + v \times B)

	// Magnetism
	Eigen::Vector3d totalMagField = localMagField; // copy
	for(auto& particle : otherParticles){
		// \frac{\mu_0}{4\pi} \frac{q(v \times r)}{r^3}
		totalMagField += particle->magfield_at_point(state.row(0));
	}
	Eigen::Vector3d magFactor = state.row(1).cross(totalMagField);

	// Electrostatics
	Eigen::Vector3d totalElecField = Eigen::Vector3d::Zero();
	for(auto& particle : otherParticles){
		totalElecField += particle->elecfield_at_point(state.row(0));   
	}
	Eigen::Vector3d elecFactor = totalElecField;

	// Total
	this->state.row(2) = charge_C/mass_kg * (elecFactor + magFactor);
}

