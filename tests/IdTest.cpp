#include <Base/Assert.h>
#include <UpstreamGL/UpstreamGL.h>
#include <iostream>
#include <unordered_set>

/**
 * @brief Generates random IDs and checks for collisions
 */
int main(int argc, char **argv) {
  std::size_t iterations = 0;
  int seeds = 0;

  if (argc < 3) {
    std::cout << "Not enough arguments\nUsage: id {ITERATIONS} {SEEDS}" << std::endl;
    exit(1);
  }

  iterations = std::stoull(argv[1]);
  seeds = std::stoi(argv[2]);

  auto ids = std::unordered_set<upgl::ID>(iterations * seeds);
  int collisions = 0;

  for (int j = 0; j < seeds; ++j) {
    upgl::ID::SetSeed(j);
    for (std::size_t i = 0; i < iterations; ++i) {
      auto it = ids.emplace(upgl::ID::Generate());
      if (!it.second)
        ++collisions;
    }
  }

  TEST_ASSERT_EQUAL(collisions, 0, "There were " + std::to_string(collisions) + " collisions!");

  exit(0);
}
