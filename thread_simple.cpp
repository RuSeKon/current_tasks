#include <thread>
#include <iostream>
#include <mutex>

std::mutex m;

void hello()
{
    m.lock();
    std::cout << "Hello ";
    m.unlock();
}

void world()
{
    m.lock();
    std::cout << "Paralell world!\n";
    m.unlock();
}

main()
{
    std::thread t1(hello);
    std::thread t2(world);

    t1.join();
    t2.join();
}
