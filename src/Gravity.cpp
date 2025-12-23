#include <iostream>
#include <vector>
#include <cmath>

#include "../include/Gravity.hpp"

Gravity::Gravity(double gravityCst) : _G(gravityCst) {}

const std::vector<double> Gravity::ComputeForcePair(const Body &body1, const Body &body2) const
{
  std::vector<double> r(3, 0);

  const auto &position1 = body1.GetPosition();
  const auto &position2 = body2.GetPosition();

  for (std::size_t i = 0; i < 3; ++i)
    r[i] = position1[i] - position2[i];

  const double r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];

  if (r2 == 0)
    throw std::runtime_error("Cannot compute the force that a body exerces on itself");

  const auto m1 = body1.GetMass();
  const auto m2 = body2.GetMass();
  const double f = -_G * m1 * m2  / r2;

  std::vector<double> fvec(3, 0);
  for (std::size_t i = 0; i < 3; ++i)
    fvec[i] = f * r[i] / std::sqrt(r2);

  return fvec;
}

