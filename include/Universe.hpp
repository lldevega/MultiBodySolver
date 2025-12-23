#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <iostream>
#include "Body.hpp"
#include "Gravity.hpp"

class Universe
{
public:
  Universe(const Gravity &gravity);

  void AddBody(const Body body);

  void SolveMotion(double timeEnd, int timeSteps);

private:
  const Gravity &_gravity;
  std::vector<Body> _bodies;
};

#endif
