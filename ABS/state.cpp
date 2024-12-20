
#include "state.h"
#include "pddl.h"
#include "utils/unique_vector.h"

#include <algorithm>  // std::lower_bound, std::sort
#include <cassert>    // assert


namespace {

	using ::symbolic::Predicate;

	std::vector<size_t> PredicateCumSum(const std::vector<Predicate>& predicates) {
		std::vector<size_t> idx_pred;
		idx_pred.reserve(predicates.size() + 1);
		idx_pred.push_back(0);
		for (const Predicate& pred : predicates) {
			const size_t idx = idx_pred.back() + pred.parameter_generator().size();
			idx_pred.push_back(idx);
		}
		return idx_pred;
	}

	std::unordered_map<std::string, size_t> PredicateIndices(
		const std::vector<Predicate>& predicates) {
		std::unordered_map<std::string, size_t> idx_predicates;
		for (size_t i = 0; i < predicates.size(); i++) {
			const Predicate& pred = predicates[i];
			idx_predicates[pred.name()] = i;
		}
		return idx_predicates;
	}

}  // namespace

namespace symbolic {

	State::State(const Pddl& pddl,
		const std::unordered_set<std::string>& str_state) {
		reserve(str_state.size());
		for (const std::string& str_prop : str_state) {
			emplace(pddl, str_prop);
		}
	}

	std::unordered_set<std::string> State::Stringify() const {
		std::unordered_set<std::string> str_state;
		str_state.reserve(size());
		for (const Proposition& prop : *this) {
			str_state.insert(prop.to_string());
		}
		return str_state;
	}

	std::ostream& operator<<(std::ostream& os, const State& state) {
		// Sort propositions.
		std::vector<std::string> props_sorted;
		props_sorted.reserve(state.size());
		for (const Proposition& prop : state) {
			props_sorted.push_back(prop.to_string());
		}
		std::sort(props_sorted.begin(), props_sorted.end());

		std::string delimiter;
		os << "{ ";
		for (const std::string& prop : props_sorted) {
			os << delimiter << prop;
			if (delimiter.empty()) delimiter = ", ";
		}
		os << " }";
		return os;
	}

	bool PartialState::contains(const PropositionBase& prop) const {
		if (pos_.contains(prop)) return true;
		if (neg_.contains(prop)) return false;
		throw UnknownEvaluation(prop);
	}
	bool PartialState::does_not_contain(const PropositionBase& prop) const {
		if (pos_.contains(prop)) return false;
		if (neg_.contains(prop)) return true;
		throw UnknownEvaluation(prop);
	}

	int PartialState::insert(const PropositionBase& prop) {
		const int was_negated = static_cast<int>(neg_.erase(prop));
		const int was_added = static_cast<int>(pos_.insert(prop));
		return was_negated + was_added;
	}

	int PartialState::insert(Proposition&& prop) {
		const int was_negated = static_cast<int>(neg_.erase(prop));
		const int was_added = static_cast<int>(pos_.insert(std::move(prop)));
		return was_negated + was_added;
	}

	int PartialState::erase(const PropositionBase& prop) {
		const int was_negated = static_cast<int>(pos_.erase(prop));
		const int was_erased = static_cast<int>(neg_.insert(prop));
		return was_negated + was_erased;
	}

	int PartialState::erase(Proposition&& prop) {
		const int was_negated = static_cast<int>(pos_.erase(prop));
		const int was_erased = static_cast<int>(neg_.insert(std::move(prop)));
		return was_negated + was_erased;
	}

	bool PartialState::IsConsistent() const {
		for (const Proposition& prop : pos_) {
			if (neg_.contains(prop)) return false;
		}
		return true;
	}

	std::pair<std::unordered_set<std::string>, std::unordered_set<std::string>>
		PartialState::Stringify() const {
		return { pos_.Stringify(), neg_.Stringify() };
	}

	std::ostream& operator<<(std::ostream& os, const PartialState& state) {
		os << "(and" << std::endl;
		for (const Proposition& prop : state.pos_) {
			os << "\t" << prop << std::endl;
		}
		for (const Proposition& prop : state.neg_) {
			os << "\tnot " << prop << std::endl;
		}
		os << ")" << std::endl;
		return os;
	}

	StateIndex::StateIndex(const std::vector<Predicate>& predicates, bool use_cache)
		: pddl_(&predicates.front().pddl()),
		predicates_(predicates),
		idx_predicate_group_(PredicateCumSum(predicates_)),
		idx_predicates_(PredicateIndices(predicates_)),
		use_cache_(use_cache) {}

	Proposition StateIndex::GetProposition(size_t idx_proposition) const {
		// Check cache
		if (use_cache_ &&
			cache_propositions_.find(idx_proposition) != cache_propositions_.end()) {
			return cache_propositions_.at(idx_proposition);
		}

		// Find index of predicate through bisection
		// std::lower_bound returns first element >= value. Get first element > value
		// and then subtract 1.
		const auto it =
			std::lower_bound(idx_predicate_group_.begin(), idx_predicate_group_.end(),
				idx_proposition + 1);
		const size_t idx_pred = it - idx_predicate_group_.begin() - 1;
		for (size_t i = 0; i < predicates_.size(); i++) {
		}

		// Find index of argument combination from remainder
		const size_t idx_args = idx_proposition - idx_predicate_group_[idx_pred];

		// Get proposition
		const Predicate& pred = predicates_[idx_pred];
		std::vector<Object> args = pred.parameter_generator()[idx_args];

		// Cache results
		if (use_cache_) {
			cache_propositions_[idx_proposition] = Proposition(pred.name(), args);
		}

		return Proposition(pred.name(), std::move(args));
	}

	size_t StateIndex::GetPropositionIndex(const Proposition& prop) const {
		// Check cache
		if (use_cache_) {
			const auto it = cache_idx_propositions_.find(prop.to_string());
			if (it != cache_idx_propositions_.end()) {
				return it->second;
			}
		}

		const size_t idx_pred = idx_predicates_.at(prop.name());

		// Get predicate
		const Predicate& pred = predicates_[idx_pred];
		const ParameterGenerator& param_gen = pred.parameter_generator();

		// Get arguments
		const size_t idx_args = param_gen.find(prop.arguments());

		const size_t idx_proposition = idx_predicate_group_[idx_pred] + idx_args;

		// Cache results
		if (use_cache_) {
			cache_idx_propositions_[prop.to_string()] = idx_proposition;
		}
		return idx_proposition;
	}

	// NOLINTNEXTLINE(performance-unnecessary-value-param)
	State StateIndex::GetState(Eigen::Ref<const IndexedState> indexed_state) const {
		assert(indexed_state.size() == size());
		State state;

		// Iterate over nonzero elements of indexed state
		for (size_t i = 0; i < indexed_state.size(); i++) {
			if (!indexed_state(i)) continue;
			state.insert(GetProposition(i));
		}

		return state;
	}

	StateIndex::IndexedState StateIndex::GetIndexedState(const State& state) const {
		IndexedState indexed_state = IndexedState::Zero(size());

		// Iterate over propositions in state
		for (const Proposition& prop : state) {
			const size_t idx_prop = GetPropositionIndex(prop);
			indexed_state[idx_prop] = true;
		}

		return indexed_state;
	}

}  // namespace symbolic

namespace {

	constexpr size_t kHashOffset = 0x9e3779b9;
	constexpr size_t kHashL = 6;
	constexpr size_t kHashR = 2;

}  // namespace

namespace std {

	size_t hash<::symbolic::State>::operator()(
		const ::symbolic::State& state) const noexcept {
		size_t seed = 0;
		for (const ::symbolic::Proposition& prop : state) {
			seed ^= hash<::symbolic::PropositionBase>{}(prop)+kHashOffset +
				(seed << kHashL) + (seed >> kHashR);
		}
		return seed;
	}

}  // namespace std
