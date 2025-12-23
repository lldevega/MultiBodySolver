#ifndef BODY_HPP
#define BODY_HPP

#include <iostream>
#include <vector>
#include <string>

class Body
{
public:
  // constructor
  Body(const std::string name, const double mass, const std::vector<double> position, const std::vector<double> velocity);

  // force
  void IncrementForce(std::vector<double> f);

  // position
  void UpdatePosition(const double dt);

  // mass
  const double GetMass() const;

  // forces
  const std::vector<double> &GetForces() const;

  // get position
  const std::vector<double> &GetPosition() const;

  // log
  void Log();

private:
  const std::string _name;
  const double _mass;
  std::vector<double> _position;
  std::vector<double> _velocity;
  double _time;
  std::vector<double> _force;

};

#endif
