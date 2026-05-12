#include "LoxCallable.h"
#include <chrono>

std::shared_ptr<LoxCallable> createClockFunction() {
    class ClockCallable : public LoxCallable {
    public:
        int arity() const override { return 0; }
        std::any call(Interpreter& interpreter,
                     std::vector<std::any>& arguments) override {
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            return static_cast<double>(millis) / 1000.0;
        }
        std::string toString() const override {
            return "<native fn>";
        }
    };
    
    return std::make_shared<ClockCallable>();
}