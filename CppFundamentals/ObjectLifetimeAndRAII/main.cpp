#include <iostream>
#include <string>
#include <utility>

class Worker {
public:
    explicit Worker(std::string name) : name_(std::move(name)) {
        std::cout << name_ << " constructed\n";
    }

    ~Worker() {
        std::cout << name_ << " destroyed\n";
    }

private:
    std::string name_;
};

int main() {
    std::cout << "Entering scope\n";
    {
        Worker worker("worker");
        std::cout << "Working\n";
    }
    std::cout << "Left scope\n";
}
