#include "preprocessor.h"

#include "prost_planner.h"
#include "logical_expressions.h"
#include "actions.h"
#include "conditional_probability_functions.h"

#include "utils/math_utils.h"

#include <iostream>

using namespace std;

PlanningTask* Preprocessor::preprocess(map<string,int>& stateVariableIndices) {
    MathUtils::initTwoToThePowerOfMap();
    MathUtils::initThreeToThePowerOfMap();

    simplifyFormulas();
    createProbabilisticPlanningTaskRepresentation(stateVariableIndices);
    return probPlanningTask->determinizeMostLikely(task);
}

void Preprocessor::simplifyFormulas() {
    map<StateFluent*,NumericConstant*> replacements;

    //simplify CPFs
    bool simplifyAgain = true;
    
    while(simplifyAgain) {
        simplifyAgain = false;
        vector<ConditionalProbabilityFunction*> cpfs;
        task->getCPFs(cpfs, true);
        for(unsigned int i = 0; i < cpfs.size(); ++i) {
            simplifyAgain = cpfs[i]->simplify(task, replacements);
            if(simplifyAgain) {
                //TODO: This is actually not entirely correct, we
                //should also check if the value is equal to the value
                //in the initial state!
                task->removeCPF(cpfs[i]);
                break;
            }
        }
    }

    //simplify SACs
    vector<StateActionConstraint*> sacs;
    task->getStateActionConstraints(sacs);
    vector<StateActionConstraint*> fullfilledSACs;

    for(unsigned int i = 0; i < sacs.size(); ++i) {
        if(sacs[i]->simplify(task, replacements)) {
            fullfilledSACs.push_back(sacs[i]);
        }
    }

    for(unsigned int i = 0; i < fullfilledSACs.size(); ++i) {
        task->removeStateActionConstraint(fullfilledSACs[i]);
    }

}

void Preprocessor::createProbabilisticPlanningTaskRepresentation(map<string,int>& stateVariableIndices) {
    vector<ActionFluent*> actionFluents;
    task->getActionFluents(actionFluents);

    vector<ConditionalProbabilityFunction*> cpfs;
    task->getCPFs(cpfs);

    vector<StateActionConstraint*> sacs;
    task->getStateActionConstraints(sacs);

    probPlanningTask->initialize(actionFluents, cpfs, sacs, task->numberOfConcurrentActions, task->horizon, task->discountFactor, stateVariableIndices);
}
