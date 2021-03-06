#include <cbt/cbt.h>
#include <memory>
#include <iostream>

void print_1(cbt::yield_to& yield, int numPrints)
{
    for (int i = 0; i < numPrints; i++)
    {
        std::cout << "1: " << i << std::endl;
        yield(cbt::ACT_RUNNING);
    }
    yield(cbt::ACT_SUCCESS);
}

void print_2(cbt::yield_to& yield, int numPrints)
{
    for (int i = 0; i < numPrints; i++)
    {
        std::cout << "2: " << i << std::endl;
        yield(cbt::ACT_RUNNING);
    }
    yield(cbt::ACT_SUCCESS);
}

int main(int argc, char** argv)
{
    cbt::Leaf::shared_ptr leaf1 = cbt::Leaf::Create(std::make_shared<cbt::coroutine>(boost::bind(print_1, _1, 10)));
    cbt::Leaf::shared_ptr leaf2 = cbt::Leaf::Create(std::make_shared<cbt::coroutine>(boost::bind(print_2, _1, 10)));

    cbt::Sequence seq({leaf1, leaf2});
    cbt::Select select({leaf1, leaf2});
    cbt::Parallel parallel({leaf1, leaf2});

    cbt::ActStatus status = cbt::ACT_RUNNING;

    std::cout << "SEQUENCE: " << std::endl;
    while (status == cbt::ACT_RUNNING)
    {
        status = seq.Next();
    }

    std::cout << "SELECT: " << std::endl;
    status = cbt::ACT_RUNNING;
    while (status == cbt::ACT_RUNNING)
    {
        status = select.Next();
    }

    std::cout << "PARALLEL: " << std::endl;
    status = cbt::ACT_RUNNING;
    while (status == cbt::ACT_RUNNING)
    {
        status = parallel.Next();
    }

    return 0;
}
