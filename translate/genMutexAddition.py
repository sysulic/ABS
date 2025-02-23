from translate import *
import instantiate
import pddl
import copy
import fact_groups
from pddl import tasks

def get_abs_mutexgroup(task):
    with timers.timing("Instantiating", block=True):
        (relaxed_reachable, atoms, _, _,
         reachable_action_params) = instantiate.explore(task)

    if not relaxed_reachable:
        return unsolvable_sas_task("No relaxed solution")

    with timers.timing("Computing fact groups", block=True):
        groups = fact_groups.compute_abs_groups(
            task, atoms, reachable_action_params, True
            )
        
    return groups

class AbsMutex:
    def __init__(self) -> None:
        self.baggable_types_set = set() # {'airplane', 'airport', 'city', 'location', 'package', 'truck'}
        self.map_actual_predicates = {} # {"package":["package-at",...],} # in str type
        self.map_mutex_groups = {} # {"":[[]]}
        pass

    def addType(self,type_name):
        if type_name not in self.baggable_types_set:
            self.baggable_types_set.add(type_name)
            self.map_actual_predicates[type_name] = []
            self.map_mutex_groups[type_name] = []

    def addActualPred(self,type_name,pred:str):
        if type_name not in self.map_actual_predicates:
            return False
        if pred not in self.map_actual_predicates[type_name]:
            self.map_actual_predicates[type_name].append(pred)
        return True
    
    def addMutexPreds(self,type_name,preds:list):
        if type_name not in self.map_mutex_groups:
            return False
        if preds not in self.map_mutex_groups[type_name]:
            self.map_mutex_groups[type_name].append(preds)
        return True

    def exclude(self,type_name:str):
        if type_name in self.baggable_types_set:
            self.baggable_types_set.remove(type_name)
            self.map_actual_predicates.pop(type_name)
            self.map_mutex_groups.pop(type_name)

    def mergeGroups(self):
        # Used for merge multi groups, for example, childsnack14 may has ["at_kitchen_sandwich","notexist"] and [notexist at_kitchen_sandwich ontray] at the same time, as all mutex will be dected.
        for type_name in self.baggable_types_set:
            compact_groups = []
            while True:
                _,current_max_group = max(zip(map(len,self.map_mutex_groups[type_name]),self.map_mutex_groups[type_name]))
                self.map_mutex_groups[type_name].remove(current_max_group)
                compact_groups.append(current_max_group)
                for group in self.getMutexGroups()[type_name]:
                    if set(group) < set(current_max_group):
                        # absorb
                        self.map_mutex_groups[type_name].remove(group)
                if not self.map_mutex_groups[type_name]:
                    break
            self.map_mutex_groups[type_name] = compact_groups

    def getBaggableTypes(self) -> set:
        return copy.deepcopy(self.baggable_types_set)
    
    def getActualPredicates(self) -> dict:
        return copy.deepcopy(self.map_actual_predicates)
    
    def getMutexGroups(self) -> dict:
        return copy.deepcopy(self.map_mutex_groups)

    def dump(self):
        print(self.baggable_types_set)
        print(self.map_actual_predicates)
        print(self.map_mutex_groups)
    
    def _getAddition(self):
        dump_str = ""
        dump_str += str(len(self.baggable_types_set))
        for type_name in self.baggable_types_set:
            dump_str += f"\n{type_name} {len(self.map_mutex_groups[type_name])}"
            for group in self.map_mutex_groups[type_name]:
                dump_str += f"\n\t{len(group)}"
                dump_str += " " + " ".join(group)
        return dump_str
        
    def dumpAddition(self,file_path=""):
        if not file_path:
            print(self._getAddition())
        else:
            with open(file_path,'w') as f:
                f.write(self._getAddition())
            print(f"\nSuccessfully written to {file_path}.\n")

def analyseActions(task,abs_mutex:AbsMutex,debug=False):
    map_subclasses = {type_.name:[type_.name] for type_ in task.types}
    for type_ in task.types:
        for sup_type in type_.supertype_names:
            map_subclasses[sup_type].append(type_.name)
    if debug:
        print(map_subclasses)
    # single-type
    for action in task.actions:
        if debug:
            print(action.name)
        type_set = set()
        for para in action.parameters:
            para_type = para.type
            if para_type in type_set:
                # Delete from baggable_types_set
                abs_mutex.exclude(para_type)
            else:
                type_set.add(para_type)
        
        predicates = []
        argss = []

        # Collect actual predicates from effects
        predicates.extend([Eff.literal.predicate for Eff in action.effects])
        argss.extend([Eff.literal.args for Eff in action.effects])
        for index in range(len(predicates)):
            for arg in argss[index]:
                for type_name in map_subclasses[action.type_map[arg]]:
                    abs_mutex.addActualPred(type_name,predicates[index])

class PossMutexDect:
    def __init__(self,obj_type:str,) -> None:
        """_summary_

        Args:
            obj_type (str)
            task (tasks.Task)
            poss_preds (list): The predicates that do not appear in any effect of any action remaining as possible mutex groups
        """
        self.obj_type = obj_type
        self.objects = []
        self.map_pred_objs = {}
        self.pmgs = [] # possible mutex group

    def initialTask(self,task:tasks.Task,actual_preds:list):
        for obj in task.objects:
            if obj.type == self.obj_type:
                self.objects.append(obj.name)

        for atom in task.init:
            try:
                atom_args = atom.args
            except AttributeError:
                continue
            for obj_name in atom_args:
                if obj_name in self.objects:
                    pred = atom.predicate
                    if pred == "=" or pred in actual_preds:
                        continue
                    if pred not in self.map_pred_objs.keys():
                        self.map_pred_objs[pred] = [obj_name]
                    else:
                        if obj_name in self.map_pred_objs[pred]:
                            # pred is not possible -> some pred of this type is impossible to appear in the mutex group
                            return False
                        
                        self.map_pred_objs[pred].append(obj_name)
        # print(self.map_pred_objs)
        return True


    def __buildPMGs(self,remaining_preds:list,remaining_objs:list,current_mutex_group:list):
        set_remaining_objs = {obj for obj in remaining_objs}
        not_compatible = True
        for pred in remaining_preds:
            set_pred_objs = {obj for obj in self.map_pred_objs[pred]}
            # compatible
            if set_pred_objs.issubset(set_remaining_objs):
                new_remaining_preds = copy.deepcopy(remaining_preds)
                new_current_mutex_group = copy.deepcopy(current_mutex_group)
                new_remaining_preds.remove(pred)
                new_current_mutex_group.append(pred)
                
                new_remaining_objs = list(set_remaining_objs - set_pred_objs)
                # found one part
                if len(new_remaining_objs) == 0:
                    self.pmgs.append(new_current_mutex_group)
                    if len(new_remaining_preds) == 0:
                        return True
                    new_current_mutex_group = []
                    new_remaining_objs = self.objects
                
                if self.__buildPMGs(
                        new_remaining_preds,
                        new_remaining_objs,
                        new_current_mutex_group,
                        ):
                    return True
                not_compatible = False
            else:
                continue

        if not_compatible:
            return False

    def buildPMGs(self,task:tasks.Task,actual_preds:list):
        if not self.initialTask(task,actual_preds):
            return False
        if len(self.map_pred_objs.keys()) == 0:
            return True
        return self.__buildPMGs(list(self.map_pred_objs.keys()),self.objects,[])

    def getPMGs(self):
        return self.pmgs

def buildAbsMutex(task,debug=False):
    with timers.timing("Normalizing task"):
        normalize.normalize(task)
    
    abs_mutex = AbsMutex()
    MAP_OBJ_TYPE = {}
    for obj in task.objects:
        abs_mutex.addType(obj.type)
        MAP_OBJ_TYPE[obj.name] = obj.type

    analyseActions(task,abs_mutex,debug)
    
    for group in get_abs_mutexgroup(task):
        if not len(group):
            continue
        # Assume that there is only one not free variable
        poss_fixed_variables = set(group[0].args)
        if debug:
            print(group)
        [poss_fixed_variables.intersection_update(atom.args) for atom in group[1:]]
        for fixed_variable in poss_fixed_variables:
            if fixed_variable in MAP_OBJ_TYPE.keys():
                abs_mutex.addMutexPreds(MAP_OBJ_TYPE[fixed_variable],list(set([atom.predicate for atom in group])))

    print("================= abs_mutex info =================")
    abs_mutex.dump()

    # exclude 1-m
    for type_name in abs_mutex.getBaggableTypes():
        preds_in_mutex = set()
        [preds_in_mutex.update(set(mutex_group)) for mutex_group in abs_mutex.getMutexGroups()[type_name]]
        if len(preds_in_mutex) < len(abs_mutex.getActualPredicates()[type_name]):
            abs_mutex.exclude(type_name)
        
    # include possible mutex group from initial
    for type_name in abs_mutex.getBaggableTypes():
        pmd = PossMutexDect(type_name)
        if pmd.buildPMGs(task,abs_mutex.getActualPredicates()[type_name]):
            for poss_mutex_group in pmd.getPMGs():
                if debug:
                    print(f"Find Possible Mutex Group: {poss_mutex_group}")
                abs_mutex.addMutexPreds(type_name,poss_mutex_group)
        else:
            # not baggable type
            abs_mutex.exclude(type_name)
    
    # merge groups
    abs_mutex.mergeGroups()

    return abs_mutex

def main():
    argparser = argparse.ArgumentParser()
    argparser.add_argument(
        "domain", help="path to domain pddl file")
    argparser.add_argument(
        "task", help="path to task pddl file")
    argparser.add_argument(
        "output", help="path to output the addition file")
    args = argparser.parse_args()

    timer = timers.Timer()
    with timers.timing("Parsing", True):
        task = pddl.open(task_filename=args.task, domain_filename=args.domain)

    with timers.timing("Normalizing task"):
        normalize.normalize(task)

    abs_mutex = buildAbsMutex(task,debug=False)
    print("============ dump mutex addition file ============")
    print(abs_mutex._getAddition())
    abs_mutex.dumpAddition(args.output)

if __name__ == "__main__":
    main()