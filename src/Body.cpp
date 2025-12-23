#include "../include/Body.hpp"
#include <fstream>

Body::Body(const std::string name, const double mass, const std::vector<double> position, 
           const std::vector<double> velocity) :
  _name(name), _mass(mass), _position(position), _velocity(velocity),
  _time(0.0), _force({0.0, 0.0, 0.0})
{ 
  std::ofstream out(_name);
  out << "x \t y \t z\t t\n";
}

void Body::IncrementForce(std::vector<double> f)
{
  for (std::size_t i = 0; i < 3; ++i)
    _force[i] += f[i];
}

void Body::UpdatePosition(const double dt)
{
  for (std::size_t i = 0; i < 3; ++i)
  {
      double a = _force[i] / _mass;
      _position[i] +=  _velocity[i] * dt + 0.5 * a * dt * dt;
      _velocity[i] += a * dt;
      _force[i] = 0.0;
  }
  _time += dt;
}

void Body::Log()
{
  std::ofstream out(_name, std::ios::app);
  out << _position[0] << "\t" << _position[1] << "\t" << _position[2] << "\t" << _time << "\n";
}

const double Body::GetMass() const { return _mass; }

const std::vector<double> &Body::GetPosition() const { return _position; }

const std::vector<double> &Body::GetForces() const { return _force; }
