#include "UpstreamGL/System/BlockList.h"
#include <UpstreamGL/UpstreamGL.h>

#include <Base/Assert.h>
#include <algorithm>
#include <iterator>
#include <vector>

int main(int argc, char **argv) {
  auto list = upgl::BlockList<int, 512>{};
  list.push(1);

  auto v = std::find(list.begin(), list.end(), 1);
  TEST_ASSERT_EQUAL(v, list.begin(), "Incorect iterator");
  TEST_ASSERT_EQUAL(*v, 1, "Incorect value");
}