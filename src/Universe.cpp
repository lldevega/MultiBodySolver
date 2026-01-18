#include "../include/Universe.hpp"
#include <algorithm>
#include <utility>

Universe::Universe(const double gravityCst): _gravity(gravityCst)
{
  _bodies.reserve(100);
}

void Universe::AddBody(const Body body)
{
  _bodies.push_back(body);
}

const std::vector<Body> &Universe::GetBodies()
{
  return _bodies;
}

void Universe::Reset() { _bodies.clear(); }

void Universe::AdvanceInTime(double dt)
{
  const auto nBodies = _bodies.size();
  if (!(nBodies > 0))
    throw std::runtime_error("Uninitialized bodies");

  std::vector<std::pair<int, int>> pairs;
  pairs.reserve(nBodies * (nBodies - 1) / 2); 

  for (int i = 0; i < nBodies; ++i) 
  {
    // create pairs
    for (int j = i + 1; j < nBodies; ++j) 
      pairs.push_back({i, j});
  }

  std::vector<double> initialTime;
  initialTime.reserve(nBodies);
  std::transform(_bodies.begin(), _bodies.end(), std::back_inserter(initialTime),
                 [](const auto& body) { return body.GetTime();});
  const bool consistent_time = std::all_of(
    initialTime.begin() + 1, initialTime.end(), [&](double t) {return t == initialTime[0];});
  if (!consistent_time)
    throw std::runtime_error("Inconsistent initial time of bodies");

  for (auto &bodyPairIndices : pairs)
  {
    const int i = bodyPairIndices.first;
    const int j = bodyPairIndices.second;

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
  std::cout << "*** Time step=" << dt + initialTime[0] << " ***\n";
  for (auto &body : _bodies)
  {
    body.UpdatePosition(dt);
    body.Log();
    std::cout << "\t" << body.GetName() << ", position=(" 
              << body.GetPosition()[0] << ", " << body.GetPosition()[1] << ", " << body.GetPosition()[2]
              <<  ")\n";
  }
}

void Universe::SolveMotion(double timeEnd, int timeSteps)
{
  double time = 0;
  double dt = timeEnd / timeSteps;
  while (time < timeEnd)
  {
    AdvanceInTime(dt);
    time += dt;
  }
}

