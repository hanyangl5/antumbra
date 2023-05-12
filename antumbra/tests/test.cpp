#include <01core/logging/log.h>
#include <01core/math/math.h>
#include <01core/memory/memory.h>

int main()
{
    LOG_ERROR("{}", "test");
    LOG_INFO("{}", ant::math::_1DIVPI);
    auto stack_resource = ant::GetStackMemoryResource(1024);
    ant::vector<int> vec(&stack_resource);
    for (int i = 0; i < 10; i++) { vec.push_back(i); }
    for (auto &v : vec) { v = 5; }

    return 1;
}