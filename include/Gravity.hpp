#ifndef GRAVITY_HPP
#define GRAVITY_HPP

#include <iostream>
#include "Body.hpp"

class Gravity
{
public:
  Gravity(double gravityCst);

  const std::vector<double> ComputeForcePair(const Body &body1, const Body &body2) const;

private:
  const double _G;
};

#endif
