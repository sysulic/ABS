
#ifndef ADDITION_H_
#define ADDITION_H_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "predicate.h"
#include "pddl.h"

namespace addition {
	using namespace symbolic;
	using namespace std;

	const Predicate find( const vector<Predicate>& predicates, 
	    const string& name) {
		for (auto predicate : predicates) {
			if (predicate.name() == name)
				return predicate;
		}
		
		throw std::runtime_error(
			"addition::find(): Could not find the predicate \" " 
			+ name + "\"\n");
	}

	using TypePredicatesGroupsMap =
		unordered_map<string, vector<vector<Predicate>>>;

	TypePredicatesGroupsMap read(istream& is, const Pddl& pddl) {
		const vector<Predicate>& predicates = pddl.predicates();
		TypePredicatesGroupsMap type_groups_map;

		string type_name, predicate_name;
		int num_types, num_groups, num_predicts;
		is >> num_types; 
		for (int i = 0; i < num_types; i++) {		
			is >> type_name >> num_groups; 
			type_groups_map[type_name] = vector<vector<Predicate>>(
				num_groups, vector<Predicate>()); 
			
			for (int j = 0; j < num_groups; j++ ) { 
				is >> num_predicts; 
				for (int k = 0; k < num_predicts; k++) {
					is >> predicate_name; 
					type_groups_map[type_name][j].push_back( 
						find(predicates, predicate_name)); 
				}
			}
		}
		return type_groups_map;
	}	
}


#endif