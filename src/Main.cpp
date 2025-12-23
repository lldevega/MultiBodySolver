#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


#include "../include/Universe.hpp"
#include "../include/Body.hpp"


struct BodyData 
{
  //static std::string headerName("Bodies");
  std::string name;
  double values[7]; // holds mass, initial position and velocity
};


struct SimulationData 
{
  //static std::string headerName("Simulation");
  double simulationTime;
  int nbTimeSteps;
};

/* enum struct ReadMode
{
  uninitialized;
  bodies;
  simulation;
}; */

std::vector<BodyData> ParseInput(const std::string &fileName) 
{
    std::ifstream file(fileName);
    std::string line;
    std::vector<BodyData> entries;

    while (std::getline(file, line)) 
    {
      if (line.empty())
        continue; // Skip empty lines

      std::stringstream ss(line);
      BodyData data;

      if (!(ss >> data.name))
        continue;

      bool success = true;
      for (int i = 0; i < 7; ++i)
      {
        if (!(ss >> data.values[i]))
        {
          success = false;
          break;
        }
      }

      if (success) 
        entries.push_back(data);
      else 
        std::cerr << "Skipping malformed line: " << line << std::endl;
    }

    return entries;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
    throw std::runtime_error("Please provide a file describing the names, initial positions and velocities of the bodies");

  // create universe
  auto universe = Universe(6.67e-11);

  // fill it with celestial bodies
  const auto bodyDataArray = ParseInput(argv[1]);
  for (const auto &bodyData : bodyDataArray)
  {
    std::vector<double> initialPosition{bodyData.values[1], bodyData.values[2], bodyData.values[3]};
    std::vector<double> initialVelocity{bodyData.values[4], bodyData.values[5], bodyData.values[6]};
    auto body = Body(bodyData.name, bodyData.values[0] /*mass*/, initialPosition, initialVelocity);
    universe.AddBody(body);
  }

  // solve motion
  universe.SolveMotion(365 * 86400, 365);

  return 0;
}
