
#include "object.h"
#include "pddl.h"
#include "ptree.h"

#include <algorithm>  // std::min, std::replace
#include <cassert>    // assert
#include <sstream>    // std::stringstream


namespace {

	using ::symbolic::Pddl;

	const VAL::pddl_type* GetTypeSymbol(const VAL::pddl_type_list* types,
		const VAL::pddl_type* symbol = nullptr) {
		if (symbol != nullptr) return symbol;

		// Iterate over domain types
		for (const VAL::pddl_type* type : *types) {
			// Iterate over ancestors of current type
			for (; type != nullptr; type = type->type) {
				if (type->type == nullptr && type->getName() == "object") return type;
			}
		}
		return nullptr;
	}

	const VAL::const_symbol* GetSymbol(const Pddl& pddl,
		const std::string& name_object) {

		//assert(pddl.symbol()->the_domain->constants != nullptr);
		if (pddl.symbol()->the_domain->constants != nullptr) {
			for (const VAL::const_symbol* obj : *pddl.symbol()->the_domain->constants) {
				assert(obj != nullptr);
				if (obj->getName() == name_object) return obj;
			}
		}
				
		if (pddl.symbol()->the_problem != nullptr) {
			//assert(pddl.symbol()->the_problem->objects != nullptr);
			if (pddl.symbol()->the_problem->objects != nullptr) {
				for (const VAL::const_symbol* obj : *pddl.symbol()->the_problem->objects) {
					assert(obj != nullptr);
					if (obj->getName() == name_object) return obj;
				}
			}
		}

		throw std::runtime_error("Object::Object(): Could not find object symbol "
			+ name_object + ".");
		return nullptr;
	}

	std::vector<std::string> TokenizeArguments(const std::string& proposition) {
		const size_t idx_start = proposition.find_first_of('(') + 1;
		const size_t idx_end =
			std::min(proposition.size(), proposition.find_last_of(')')) - idx_start;
		std::string str_args = proposition.substr(idx_start, idx_end);
		std::replace(str_args.begin(), str_args.end(), ',', ' ');
		std::stringstream ss(str_args);

		std::string arg;
		std::vector<std::string> args;
		while (ss >> arg) {
			args.emplace_back(std::move(arg));
		}
		return args;
	}

}  // namespace

namespace symbolic {

	bool Object::Type::IsSubtype(const std::string& type) const {
		for (const VAL::pddl_type* curr = symbol_; curr != nullptr; curr = curr->type)
			if (curr->getName() == type)
				return true;

		return false;
	}

	std::vector<std::string> Object::Type::ListTypes() const {
		std::vector<std::string> types;
		for (const VAL::pddl_type* curr = symbol_; curr != nullptr; curr = curr->type)
			types.push_back(curr->getName());

		return types;
	}


	Object::Object(const Pddl& pddl, const VAL::pddl_typed_symbol* symbol)
		: symbol_(symbol),
		type_(GetTypeSymbol(pddl.symbol()->the_domain->types, symbol->type)),
		hash_(std::hash<std::string>{}(name())) {}

	Object::Object(const VAL::pddl_type_list* types,
		const VAL::pddl_typed_symbol* symbol)
		: symbol_(symbol),
		type_(GetTypeSymbol(types, symbol->type)),
		hash_(std::hash<std::string>{}(name())) {}

	Object::Object(const Pddl& pddl, const std::string& name_object)
		: symbol_(GetSymbol(pddl, name_object)),
		type_(GetTypeSymbol(pddl.symbol()->the_domain->types, symbol_->type)),
		hash_(std::hash<std::string>{}(name())) {}


	std::vector<Object> Object::ParseArguments(const Pddl& pddl,
		const std::string& atom) {
		const std::vector<std::string> name_args = TokenizeArguments(atom);
		std::vector<Object> args;
		args.reserve(name_args.size());
		for (const std::string& name_arg : name_args)
			args.emplace_back(pddl, name_arg);
		return args;
	}

	std::vector<Object> Object::ParseArguments(const Pddl& pddl,
		const std::vector<std::string>& str_args) {
		std::vector<Object> args;
		args.reserve(str_args.size());
		for (const std::string& str_arg : str_args)
			args.emplace_back(pddl, str_arg);
		return args;
	}

	std::ostream& operator<<(std::ostream& os, const std::vector<Object>& objects) {
		if (objects.empty()) return os;
		os << objects.front();
		for (size_t i = 1; i < objects.size(); i++)
			os << ", " << objects[i];
		return os;
	}

}  // namespace symbolic
