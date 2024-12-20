
#ifndef ABSTRACTION_H
#define ABSTRACTION_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include "predicate.h"
#include "pddl.h"
#include "np.h"

namespace my_hash {
	template <typename T>
	struct hash_set {
		size_t operator()(const std::set<T>& s) const {
			size_t hash_value = 0;
			for (const auto& element : s) {
				hash_value ^= std::hash<T>()(element) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
			}
			return hash_value;
		}
	};
}


namespace abstraction {
	using namespace symbolic;
	using namespace std;

	using ObjectTypeMap = std::unordered_map<std::string, std::vector<Object>>;
	using TypePredicatesGroupsMap =
		std::unordered_map<std::string, std::vector<std::vector<symbolic::Predicate>>>;

	using PredPropsSetMap =
		std::unordered_map<std::string, std::set<symbolic::Proposition>>;

	using BagPropsGroupsMap = // std::unordered_map<std::string, std::set<std::set<Proposition>>>;
		std::unordered_map<std::string, std::set<std::set<Proposition>>>;

	class Feature;
	class BaggableObjects;
	class Action_Call;

	using TypeStGroupsMap =
		std::unordered_map<std::string, std::vector<BaggableObjects>>;

	/**
	 * step 0: type_predicts_group
	 * step 1: bags of GE objects
	 * step 2: all attrs for bags 
	 * step 3: numerical and boolean features
	 * step 4: init
	 * step 5: goal
	 * step 6: actions
	 */

	class Feature {
	public:
	Feature(bool numerical_,  
		BaggableObjects& baggableObjects, int bag_idx_, int attr_idx_)
		:numerical(numerical_),
		bags_ptr(&baggableObjects),
		bag_idx(bag_idx_),
		attr_idx(attr_idx_){}

	Feature(bool numerical_, symbolic::Proposition prop_)
		:numerical(numerical_), prop(prop_){}

		bool numerical;
		BaggableObjects* bags_ptr;
		int bag_idx;
		int attr_idx;
		symbolic::Proposition prop;
	};

	class Attribute: public symbolic::Object {
	public:
		Attribute(symbolic::Object obj, const std::vector<symbolic::Proposition>& attr)
			:Object(obj), attr(attr){}
		std::vector<symbolic::Proposition> attr;
	};

	class BaggableObjects {
	public:
		BaggableObjects(std::string type_name) :type_name(type_name) {};
		
		std::string type_name;
		std::vector<std::vector<symbolic::Object>> bags;
		std::vector<std::vector<std::set<symbolic::Proposition>>> bag_attrs;
	};

	void Apply_Objects(
		const symbolic::Pddl& pddl,
		Action_Call& action_call);

	class Action_Call: public symbolic::Action{
	public:
		Action_Call(const symbolic::Action action,
			const vector<symbolic::Object> args,
			const symbolic::Pddl& pddl)
			:Action(action), action_args(args) {
			action_call_ = this->to_string(action_args);
			Apply_Objects(pddl, *this);	
		}
	
		std::string action_call_;
		vector<symbolic::Object> action_args;

		//std::vector<symbolic::Proposition> precondition_probs;

		std::vector<symbolic::Proposition> pos_precondition_probs;
		std::vector<symbolic::Proposition> neg_precondition_probs;
		std::vector<symbolic::Proposition> add_effect_probs;
		std::vector<symbolic::Proposition> del_effect_probs;
	};
	
	std::ostream& operator<<(std::ostream& os,
		const Feature& f);
	
	std::ostream& operator<<(std::ostream& os, 
		const Action_Call& action_call);
	
	std::ostream& operator<<(std::ostream& os,
		const BaggableObjects& baggableObjects);
	
	std::vector<std::vector<const Feature*>> GenerateNumericalFeaturePres(
		const symbolic::State& state,
		const std::vector<Feature>& allNumericalFeatures,
		const std::vector<Feature>& allBooleanFeatures,
		const std::unordered_map<const Feature*, const NP::Feature*>& feature_map,
		const std::vector<BaggableObjects>& allBaggableObjects);

	std::vector<const Feature*> GenerateNumericalFeatureState(
		const symbolic::State& state,
		const std::vector<Feature>& allNumericalFeatures,
		const std::vector<Feature>& allBooleanFeatures,
		const std::unordered_map<const Feature*, 
		const NP::Feature*>& feature_map,
		const std::vector<BaggableObjects>& allBaggableObjects);

	NP::Problem* AbstractProblem(
		const symbolic::Pddl& pddl,
		const TypePredicatesGroupsMap& type_preds, 
		std::unordered_map<std::string, std::set<symbolic::Proposition>> pred_facts,
		std::ostream& os,
		const std::set <std::vector< std::string >>& compact_actions);

	std::vector<std::vector<symbolic::Object>> generateAllParams(const symbolic::Predicate& pred);

	std::vector<NP::Action*> AbstractAction(
		const Action_Call& action_call,
		const std::vector<Feature>& allNumericalFeatures,
		const std::vector<Feature>& allBooleanFeatures,
		const std::unordered_map<const Feature*, 
		const NP::Feature*>& feature_map,
		const std::vector<BaggableObjects>& allBaggableObjects,
		const std::unordered_map<std::string, std::set<int>> bag_nidx_map,
		const TypePredicatesGroupsMap& type_preds,
		const PredPropsSetMap& pred_facts,
		std::set<std::string> all_numerical_preds_name
	);
	
	std::vector<std::pair<const NP::Feature*, bool>> AbstractState(
		const symbolic::State state,
		bool is_partial_state,
		const std::vector<Feature>& allNumericalFeatures,
		const std::vector<Feature>& allBooleanFeatures,
		const std::unordered_map<const Feature*, 
		const NP::Feature*>& feature_map,
		const std::vector<BaggableObjects>& allBaggableObjects,
		const TypePredicatesGroupsMap& type_preds,
		std::unordered_map<std::string, std::set<int>> bag_nidx_map);
	
	std::vector<symbolic::Object> AbstractObject(
		const symbolic::Pddl& pddl,
		const std::vector<BaggableObjects>& allBaggableObjects);

	//std::vector<Action_Call> InstantiationAction(
	//	std::vector<symbolic::Object>objects,
	//	const symbolic::Pddl& pddl,
	//	const PredPropsSetMap& pred_facts
	//);
	std::vector<Action_Call> InstantiationAction(
		const std::vector<BaggableObjects> allBaggableObjects,
		const symbolic::Pddl& pddl,
		const PredPropsSetMap& pred_facts
	);

	void getPossArgs(const std::vector<std::vector<symbolic::Proposition>>& props_groups,
		const std::vector<std::vector<symbolic::Object>>& argobjs_groups,
		std::vector<std::unordered_map<std::string, symbolic::Object>>& res,
		int layer,
		std::unordered_map<std::string, symbolic::Object> tmp);

	bool ObjectRelated(
		const symbolic::Proposition& prop, 
		const symbolic::Object& obj);

	int BagRelated(
		const symbolic::Proposition& prop,
		const BaggableObjects& baggableObjects);

	bool TypeRelated(
		const symbolic::Predicate& pred, 
		const std::string& type_name);

	bool PropostionSimilar(
		const symbolic::Proposition& lhs,
		const symbolic::Proposition& rhs, 
		const std::string& type_name);

	bool AttributeSimilar(
		const Attribute& lhs, 
		const Attribute& rhs,
		const std::string& type_name);

	symbolic::State FormulaToState(const Pddl& pddl, const VAL::goal* symbol);

	BaggableObjects GoalEquivalent(
		const string type_name,
		const vector<vector<Predicate>>& pred_groups, 
		const Pddl& pddl);

	std::unordered_map<std::string, std::string> buildBagStMap(const std::vector<BaggableObjects>& allBaggableObjects);

	std::ostream& outPut(std::ostream& os,
		const BaggableObjects& baggableObjects,
		std::unordered_map<std::string, std::string> bag_st_map);

	std::ostream& outPut(std::ostream& os,
		const Feature& f,
		std::unordered_map<std::string, std::string> bag_st_map, const Pddl& pddl);

	BaggableObjects Partition(
		std::vector<symbolic::Object> objects,
		std::vector<Attribute> objects_attr);

	bool propTypeRelated(symbolic::Proposition prop, std::string type_name);

	bool propTypeBagRelated(symbolic::Proposition prop, std::string type_name, const std::vector<std::vector<symbolic::Predicate>> & );
	
	bool singleInconsistent(std::set<Proposition> props,
		const TypePredicatesGroupsMap& type_preds);

	bool singleInconsistent(std::set<Proposition> props1,
		std::set<Proposition> props2,
		const TypePredicatesGroupsMap& type_preds);

	BagPropsGroupsMap GenerateEavs(
			std::vector<BaggableObjects>& allBaggableObjects,
			const TypePredicatesGroupsMap& type_predicts,
			const PredPropsSetMap & pred_facts,
			std::vector<Feature>& allNumericalFeatures,
			std::vector<Feature>& allBooleanFeatures,
			const symbolic::Pddl& pddl);

	bool consistentWithFacts(const symbolic::Proposition& prop, const PredPropsSetMap & pred_facts);

	void GenerateFeatures(
		std::vector<BaggableObjects>& allBaggableObjects,
		const TypePredicatesGroupsMap& type_predicts,
		const PredPropsSetMap& pred_facts,
		std::vector<Feature>& allNumericalFeatures,
		std::vector<Feature>& allBooleanFeatures,
		std::unordered_map<std::set<symbolic::Proposition>, Feature*, my_hash::hash_set<Proposition>>& sets_Fptr_map,
		const symbolic::Pddl& pddl);
	
	void ReplacedBy(symbolic::Object& obj,
		const BaggableObjects& baggableObjects);

	void ReplacedBy(symbolic::Proposition& prob,
		const std::vector<BaggableObjects>& allBaggableObjects);

	void ReplacedBy(symbolic::State& state,
		const std::vector<BaggableObjects>& allBaggableObjects);

	void ReplacedBy(symbolic::Action& action,
		const std::vector<BaggableObjects>& allBaggableObjects);

	void Product(const std::vector<std::vector<symbolic::Predicate>>& groups,
		std::vector<std::vector<symbolic::Predicate>>& res,
		int layer, 	std::vector<symbolic::Predicate> tmp);

	void Product(const std::vector<std::vector<symbolic::Proposition>>& groups,
		std::vector<std::set<symbolic::Proposition>>& res,
		int layer, std::vector<symbolic::Proposition> tmp);

	template<class T>
	void Product(const std::vector<std::vector<T>>& groups,
		std::vector<std::vector<T>>& res,
		int layer, std::vector<T> tmp);

	template<class T>
	void Product(const std::vector<std::vector<T>>& groups,
		std::vector<std::set<T>>& res,
		int layer, std::vector<T> tmp);

	void Product(const std::vector<std::vector<const Feature*>> &groups,
		std::vector<std::vector<const Feature*>>& res,
		int layer, std::vector <const Feature*> tmp);

} // namespace abstraction


#endif // !ABSTRACTION_H
