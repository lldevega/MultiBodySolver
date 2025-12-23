#include "../include/Universe.hpp"
#include <utility>

Universe::Universe(const Gravity &gravity): _gravity(gravity)
{
  _bodies.reserve(100);
}

void Universe::AddBody(const Body body)
{
  _bodies.push_back(body);
}

void Universe::SolveMotion(double timeEnd, int timeSteps)
{
  const auto nBodies = _bodies.size();

  std::vector<std::pair<int, int>> pairs;
  pairs.reserve(nBodies * (nBodies - 1) / 2); 

  for (int i = 0; i < nBodies; ++i) 
  {
    for (int j = i + 1; j < nBodies; ++j) 
      pairs.push_back({i, j});
  }

  double time = 0;
  double dt = timeEnd / timeSteps;
  while (time < timeEnd)
  {
    // first loop: updates forces for all bodies at current time
    for (auto &bodyPairIndices : pairs)
    {
      const int i = bodyPairIndices.first;
      const int j = bodyPairIndices.second;
      std::cout << "pair (" << i << ", " << j << ")\n";

      // compute pair of forces
      auto fvec = _gravity.ComputeForcePair(_bodies[i], _bodies[j]);
      auto fvecNeg = fvec;
      for (auto& val : fvecNeg)
        val *= -1.0;

      // increment force
      _bodies[i].IncrementForce(fvec);
      _bodies[j].IncrementForce(fvecNeg);
    }

    // second loop: updates position for all bodies
    std::cout << "***** Time " << time << " *****\n";
    std::cout << "  Positions: \n";
    for (auto &body : _bodies)
    {
      // solve new position
      body.UpdatePosition(dt);
      const auto &newPosition = body.GetPosition();
      std::cout << "    x=" << newPosition[0] << ", y=" << newPosition[1]
                << ", z=" << newPosition[2] << "\n";
      body.Log();
    }
    
    time += dt;
  }
}

