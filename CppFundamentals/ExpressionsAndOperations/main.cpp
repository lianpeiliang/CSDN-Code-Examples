#include <iostream>
#include <utility>

// This class only logs function calls; it does not manage a resource.
class Trace {
public:
    Trace() {
        std::cout << "default constructor\n";
    }

    Trace(const Trace&) {
        std::cout << "copy constructor\n";
    }

    Trace(Trace&&) noexcept {
        std::cout << "move constructor\n";
    }

    Trace& operator=(Trace&&) noexcept {
        std::cout << "move assignment\n";
        return *this;
    }

    ~Trace() {
        std::cout << "destructor\n";
    }
};

int main() {
    std::cout << "[1] Create first\n";
    Trace first;

    std::cout << "[2] Evaluate std::move(first) alone\n";
    (void)std::move(first);

    std::cout << "[3] Copy-construct second\n";
    Trace second(first);

    std::cout << "[4] Move-construct third\n";
    Trace third(std::move(first));

    std::cout << "[5] Move-assign to second\n";
    second = std::move(third);

    std::cout << "[6] Leave scope\n";
}
