#include <cmath>
#include <iostream>

#include "../../../src/Common/Exceptions.hpp"
#include "../../../src/Runtime/Context.hpp"
#include "../../../src/Runtime/NativeLibrary.hpp"
#include "../../../src/Runtime/ObjectInstance.hpp"
#include "../../../src/Runtime/Value.hpp"

using namespace o2l;

/**
 * Example native library implementing advanced math functions
 * This demonstrates how to create a native library for O²L
 */
class MathLibrary : public NativeLibrary {
   private:
    std::string name_;
    std::string version_;

   public:
    MathLibrary() : name_("Math"), version_("1.0.0") {}

    ~MathLibrary() override = default;

    std::string getName() const override {
        return name_;
    }

    std::string getVersion() const override {
        return version_;
    }

    void initialize(Context& context) override {
        std::cout << "Initializing Math native library v" << version_ << std::endl;
    }

    void registerMethods(ObjectInstance* obj) override {
        // Register native methods
        obj->addMethod(
            "sqrt",
            [](const std::vector<Value>& args, Context& context) -> Value {
                if (args.size() != 1) {
                    throw EvaluationError("sqrt() expects exactly 1 argument");
                }

                if (std::holds_alternative<Double>(args[0])) {
                    double value = std::get<Double>(args[0]);
                    if (value < 0) {
                        throw EvaluationError(
                            "sqrt() cannot compute square root of negative number");
                    }
                    return Double(std::sqrt(value));
                } else if (std::holds_alternative<Int>(args[0])) {
                    int value = std::get<Int>(args[0]);
                    if (value < 0) {
                        throw EvaluationError(
                            "sqrt() cannot compute square root of negative number");
                    }
                    return Double(std::sqrt(static_cast<double>(value)));
                } else {
                    throw EvaluationError("sqrt() expects a numeric argument");
                }
            },
            true);  // Mark as external

        obj->addMethod(
            "pow",
            [](const std::vector<Value>& args, Context& context) -> Value {
                if (args.size() != 2) {
                    throw EvaluationError("pow() expects exactly 2 arguments");
                }

                double base = 0.0, exponent = 0.0;

                // Convert first argument (base)
                if (std::holds_alternative<Double>(args[0])) {
                    base = std::get<Double>(args[0]);
                } else if (std::holds_alternative<Int>(args[0])) {
                    base = static_cast<double>(std::get<Int>(args[0]));
                } else {
                    throw EvaluationError("pow() expects numeric arguments");
                }

                // Convert second argument (exponent)
                if (std::holds_alternative<Double>(args[1])) {
                    exponent = std::get<Double>(args[1]);
                } else if (std::holds_alternative<Int>(args[1])) {
                    exponent = static_cast<double>(std::get<Int>(args[1]));
                } else {
                    throw EvaluationError("pow() expects numeric arguments");
                }

                double result = std::pow(base, exponent);
                return Double(result);
            },
            true);

        obj->addMethod(
            "sin",
            [](const std::vector<Value>& args, Context& context) -> Value {
                if (args.size() != 1) {
                    throw EvaluationError("sin() expects exactly 1 argument");
                }

                double value = 0.0;
                if (std::holds_alternative<Double>(args[0])) {
                    value = std::get<Double>(args[0]);
                } else if (std::holds_alternative<Int>(args[0])) {
                    value = static_cast<double>(std::get<Int>(args[0]));
                } else {
                    throw EvaluationError("sin() expects a numeric argument");
                }

                return Double(std::sin(value));
            },
            true);

        obj->addMethod(
            "cos",
            [](const std::vector<Value>& args, Context& context) -> Value {
                if (args.size() != 1) {
                    throw EvaluationError("cos() expects exactly 1 argument");
                }

                double value = 0.0;
                if (std::holds_alternative<Double>(args[0])) {
                    value = std::get<Double>(args[0]);
                } else if (std::holds_alternative<Int>(args[0])) {
                    value = static_cast<double>(std::get<Int>(args[0]));
                } else {
                    throw EvaluationError("cos() expects a numeric argument");
                }

                return Double(std::cos(value));
            },
            true);

        obj->addMethod(
            "pi",
            [](const std::vector<Value>& args, Context& context) -> Value {
                if (args.size() != 0) {
                    throw EvaluationError("pi() expects no arguments");
                }
                return Double(M_PI);
            },
            true);

        obj->addMethod(
            "info",
            [this](const std::vector<Value>& args, Context& context) -> Value {
                return Text("Native Math Library v" + version_ +
                            " - Advanced mathematical functions");
            },
            true);
    }

    void cleanup() override {
        std::cout << "Cleaning up Math native library" << std::endl;
    }

    std::map<std::string, std::string> getMetadata() const override {
        return {{"author", "O²L Team"},
                {"description", "Advanced mathematical functions"},
                {"category", "mathematics"},
                {"license", "MIT"}};
    }
};

// Required C-style entry points
extern "C" {
NativeLibrary* create_library() {
    return new MathLibrary();
}

void destroy_library(NativeLibrary* lib) {
    delete lib;
}

const char* get_abi_version() {
    return "1.0.0";
}
}