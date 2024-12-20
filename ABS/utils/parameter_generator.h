
#ifndef SYMBOLIC_UTILS_PARAMETER_GENERATOR_H_
#define SYMBOLIC_UTILS_PARAMETER_GENERATOR_H_

#include "../object.h"
#include "combination_generator.h"

#include <unordered_map>  // std::unordered_map
#include <vector>         // std::vector


namespace symbolic {

	class ParameterGenerator
		: public CombinationGenerator<const std::vector<Object>> {
	public:
		using Base = CombinationGenerator<const std::vector<Object>>;
		using ObjectTypeMap = std::unordered_map<std::string, std::vector<Object>>;

		ParameterGenerator() = default;
		~ParameterGenerator() override = default;

		ParameterGenerator(const Pddl& pddl,
			const std::vector<Object>& params);

		ParameterGenerator(const ParameterGenerator& other);
		ParameterGenerator(ParameterGenerator&& other) noexcept;
		ParameterGenerator& operator=(const ParameterGenerator& rhs);
		ParameterGenerator& operator=(ParameterGenerator&& rhs) noexcept;

		const Pddl& pddl() const { return *pddl_; }

	private:
		const Pddl* pddl_ = nullptr;

		// Store parameter types for portability
		std::vector<std::vector<Object>> param_types_;
	};

}  // namespace symbolic

#endif  // SYMBOLIC_UTILS_PARAMETER_GENERATOR_H_
