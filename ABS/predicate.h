
#ifndef SYMBOLIC_PREDICATE_H_
#define SYMBOLIC_PREDICATE_H_

#include "action.h"

namespace VAL {

	class pred_decl;

}  // namespace VAL

namespace symbolic {

	class Predicate : public Action {
	public:
		Predicate(const Pddl& pddl, const VAL::pred_decl* symbol);

		const VAL::pred_decl* symbol() const { return symbol_; }

		/**
		 * Predicate head.
		 */
		const std::string& name() const { return name_; }

		/**
		 * List of predicate parameters.
		 */
		const std::vector<Object>& parameters() const { return parameters_; }

		/**
		 * Combination generator for predicate parameters.
		 */
		const ParameterGenerator& parameter_generator() const { return param_gen_; }

		/**
		 * Creates a string representation of the predicate 
		 * with the default parameters.
		 */
		std::string to_string() const;

		/**
		 * Creates a string representation of the predicate 
		 * with the given arguments.
		 */
		std::string to_string(const std::vector<Object>& arguments) const;

		friend std::ostream& operator<<(std::ostream& os, const Predicate& pred);

	private:
		const VAL::pred_decl* symbol_ = nullptr;
		std::string name_;
		std::vector<Object> parameters_;
		ParameterGenerator param_gen_;
	};

}  // namespace symbolic

#endif  // SYMBOLIC_PREDICATE_H_
