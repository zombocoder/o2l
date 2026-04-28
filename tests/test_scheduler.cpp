#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../src/AST/Node.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Scheduler.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

// A node that yields N times and records its execution order
class TaskNode : public ASTNode {
   public:
    std::string name;
    int yields_remaining;
    std::vector<std::string>* execution_log;

    TaskNode(std::string name, int yields, std::vector<std::string>* log)
        : name(name), yields_remaining(yields), execution_log(log) {}

    Value evaluate(Context& context) override {
        if (Scheduler::instance().hasResumeValue()) {
            Scheduler::instance().consumeResumeValue();
        }

        while (yields_remaining > 0) {
            execution_log->push_back(name + "_yield_" + std::to_string(yields_remaining));
            yields_remaining--;
            Scheduler::instance().yield();
            // After yield, we need to handle resumption for the next iteration if this were a
            // BlockNode, but since we are in a manual while loop, we will just continue. Actually,
            // if yield throws, we exit. On re-entry, we hit the hasResumeValue check at the top.
        }
        execution_log->push_back(name + "_done");
        return Int(0);
    }

    std::string toString() const override {
        return name;
    }
};

// A node for testing sleep
class SleepNode : public ASTNode {
   public:
    uint64_t ms;
    bool* finished;
    SleepNode(uint64_t ms, bool* finished) : ms(ms), finished(finished) {}

    Value evaluate(Context& context) override {
        if (Scheduler::instance().hasResumeValue()) {
            Scheduler::instance().consumeResumeValue();
            *finished = true;
            return Int(1);
        }
        Scheduler::instance().suspendForSleep(ms);
        *finished = true;
        return Int(1);
    }

    std::string toString() const override {
        return "sleep";
    }
};

#include <gtest/gtest.h>

TEST(ConcurrencyTest, scheduler) {
    o2l::Scheduler::instance().reset();
    std::cout << "Testing Scheduler..." << std::endl;

    auto& sched = Scheduler::instance();
    std::vector<std::string> log;

    // Test 1: Round-robin execution
    std::cout << "  Test 1: Round-robin..." << std::endl;
    auto task1 = std::make_unique<TaskNode>("A", 2, &log);
    auto task2 = std::make_unique<TaskNode>("B", 2, &log);
    auto task3 = std::make_unique<TaskNode>("C", 2, &log);

    sched.spawn(task1.get(), Context());
    sched.spawn(task2.get(), Context());
    sched.spawn(task3.get(), Context());

    sched.run();

    // Expected order (Round Robin):
    // A_yield_2, B_yield_2, C_yield_2,
    // A_yield_1, B_yield_1, C_yield_1,
    // A_done, B_done, C_done

    assert(log.size() == 9);
    assert(log[0] == "A_yield_2");
    assert(log[1] == "B_yield_2");
    assert(log[2] == "C_yield_2");
    assert(log[3] == "A_yield_1");
    assert(log[4] == "B_yield_1");
    assert(log[5] == "C_yield_1");
    assert(log[6] == "A_done");
    assert(log[7] == "B_done");
    assert(log[8] == "C_done");
    std::cout << "    [âœ“] Round-robin passed." << std::endl;

    // Test 2: Sleep timers
    std::cout << "  Test 2: Sleep timers..." << std::endl;
    bool sleep_finished = false;
    auto sleep_task = std::make_unique<SleepNode>(50, &sleep_finished);

    auto start = std::chrono::steady_clock::now();
    sched.spawn(sleep_task.get(), Context());
    sched.run();
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    assert(sleep_finished);
    assert(duration >= 45);  // Tolerance
    std::cout << "    [âœ“] Sleep timer passed (duration: " << duration << "ms)." << std::endl;

    std::cout << "Scheduler tests passed!" << std::endl;
    return;
}


