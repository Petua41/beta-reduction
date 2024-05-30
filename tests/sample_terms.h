#pragma once

#include <memory>

#include "model/abstraction.h"
#include "model/application.h"
#include "model/term.h"
#include "model/variable.h"

namespace test::sample_terms {

// clang-format off
// Simple:
auto const kSimpleAbstraction = std::make_shared<model::term::Abstraction>(
        model::term::Variable{"x"},
		std::make_shared<model::term::Variable>("x"));

auto const kSimpleApplication = std::make_shared<model::term::Application>(
        std::make_shared<model::term::Variable>("x"),
		std::make_shared<model::term::Variable>("x"));

auto const kSimpleRedex = std::make_shared<model::term::Application>(
    std::make_shared<model::term::Abstraction>(
        model::term::Variable{"x"},
        std::make_shared<model::term::Variable>("x")),
    std::make_shared<model::term::Variable>("y"));

// Church numerals:
auto const kChurch2 = std::make_shared<model::term::Abstraction>(
	model::term::Variable{"f"},
	std::make_shared<model::term::Abstraction>(
		model::term::Variable{"x"},
		std::make_shared<model::term::Application>(
			std::make_shared<model::term::Variable>("f"),
			std::make_shared<model::term::Application>(
				std::make_shared<model::term::Variable>("f"),
				std::make_shared<model::term::Variable>("x")))));

// Misc named:
auto const kFixpointCombinator = std::make_shared<model::term::Abstraction>(
	model::term::Variable("f"),
	std::make_shared<model::term::Application>(
		std::make_shared<model::term::Abstraction>(
			model::term::Variable("x"),
			std::make_shared<model::term::Application>(
				std::make_shared<model::term::Variable>("f"),
				std::make_shared<model::term::Application>(
					std::make_shared<model::term::Variable>("x"),
					std::make_shared<model::term::Variable>("x")))),
		std::make_shared<model::term::Abstraction>(
			model::term::Variable("x"),
			std::make_shared<model::term::Application>(
				std::make_shared<model::term::Variable>("f"),
				std::make_shared<model::term::Application>(
					std::make_shared<model::term::Variable>("x"),
					std::make_shared<model::term::Variable>("x"))))));

// Misc complex:
auto const kPlus = std::make_shared<model::term::Abstraction>(
    model::term::Variable{"m"},
    std::make_shared<model::term::Abstraction>(
        model::term::Variable{"n"},
        std::make_shared<model::term::Abstraction>(
            model::term::Variable{"f"},
            std::make_shared<model::term::Abstraction>(
                model::term::Variable{"x"},
                std::make_shared<model::term::Application>(
                    std::make_shared<model::term::Application>(
                        std::make_shared<model::term::Variable>("m"),
                        std::make_shared<model::term::Variable>("f")),
                    std::make_shared<model::term::Application>(
                        std::make_shared<model::term::Application>(
                            std::make_shared<model::term::Variable>("n"),
                            std::make_shared<model::term::Variable>("f")),
                        std::make_shared<model::term::Variable>("x")))))));

auto const k2Plus2 = std::make_shared<model::term::Application>(
    std::make_shared<model::term::Application>(
        kPlus,
        kChurch2),
    kChurch2);

auto const kWeakNormalForm = std::make_shared<model::term::Abstraction>(
    model::term::Variable{"x"},
    std::make_shared<model::term::Application>(
        std::make_shared<model::term::Abstraction>(
            model::term::Variable{"y"},
            std::make_shared<model::term::Variable>("y")),
        std::make_shared<model::term::Variable>("x")));
// clang-format off

}  // namespace test::sample_terms
