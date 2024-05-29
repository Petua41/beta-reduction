#pragma once

#include <memory>

#include "model/terms.h"

namespace test::sample_terms {

using namespace model::term;

// clang-format off
// Simple:
auto const kSimpleAbstraction = std::make_shared<Abstraction>(
        Variable{"x"},
		std::make_shared<Variable>("x"));

auto const kSimpleApplication = std::make_shared<Application>(
        std::make_shared<Variable>("x"),
		std::make_shared<Variable>("x"));

auto const kSimpleRedex = std::make_shared<Application>(
    std::make_shared<Abstraction>(
        Variable{"x"},
        std::make_shared<Variable>("x")),
    std::make_shared<Variable>("y"));

// Church numerals:
auto const kChurch2 = std::make_shared<Abstraction>(
	Variable{"f"},
	std::make_shared<Abstraction>(
		Variable{"x"},
		std::make_shared<Application>(
			std::make_shared<Variable>("f"),
			std::make_shared<Application>(
				std::make_shared<Variable>("f"),
				std::make_shared<Variable>("x")))));

// Misc named:
auto const kFixpointCombinator = std::make_shared<Abstraction>(
	Variable("f"),
	std::make_shared<Application>(
		std::make_shared<Abstraction>(
			Variable("x"),
			std::make_shared<Application>(
				std::make_shared<Variable>("f"),
				std::make_shared<Application>(
					std::make_shared<Variable>("x"),
					std::make_shared<Variable>("x")))),
		std::make_shared<Abstraction>(
			Variable("x"),
			std::make_shared<Application>(
				std::make_shared<Variable>("f"),
				std::make_shared<Application>(
					std::make_shared<Variable>("x"),
					std::make_shared<Variable>("x"))))));

// Misc complex:
auto const kPlus = std::make_shared<Abstraction>(
    Variable{"m"},
    std::make_shared<Abstraction>(
        Variable{"n"},
        std::make_shared<Abstraction>(
            Variable{"f"},
            std::make_shared<Abstraction>(
                Variable{"x"},
                std::make_shared<Application>(
                    std::make_shared<Application>(
                        std::make_shared<Variable>("m"),
                        std::make_shared<Variable>("f")),
                    std::make_shared<Application>(
                        std::make_shared<Application>(
                            std::make_shared<Variable>("n"),
                            std::make_shared<Variable>("f")),
                        std::make_shared<Variable>("x")))))));

auto const k2Plus2 = std::make_shared<Application>(
    std::make_shared<Application>(
        kPlus,
        kChurch2),
    kChurch2);

auto const kWeakNormalForm = std::make_shared<Abstraction>(
    Variable{"x"},
    std::make_shared<Application>(
        std::make_shared<Abstraction>(
            Variable{"y"},
            std::make_shared<Variable>("y")),
        std::make_shared<Variable>("x")));
// clang-format off

}  // namespace test::sample_terms
