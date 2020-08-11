#include "probability_distribution.h"

#include <cassert>

using namespace std;

namespace prost::parser {
inline int DiscretePD::getNumberOfOutcomes() const {
    assert(isWellDefined());
    return values.size();
}

bool DiscretePD::isWellDefined() const {
    // Only use this function in assertions, it's quite inefficient!

    // Each value must have a probability
    if (isUndefined() || (values.size() != probabilities.size())) {
        return false;
    }

    // The sum of the probabilities must be 1, each individual probability must
    // not be <= 0.0 and the values must be ordered and unique
    double probSum = probabilities[0];
    double lastVal = values[0];
    for (unsigned int i = 1; i < probabilities.size(); ++i) {
        if (utils::MathUtils::doubleIsSmallerOrEqual(probabilities[i], 0.0)) {
            return false;
        }
        probSum += probabilities[i];
        if (!utils::MathUtils::doubleIsGreater(values[i], lastVal)) {
            return false;
        }
        lastVal = values[i];
    }
    return utils::MathUtils::doubleIsEqual(probSum, 1.0);
}

void DiscretePD::print(ostream& out) const {
    out << "[ ";
    for (unsigned int i = 0; i < values.size(); ++i) {
        out << values[i] << ":" << probabilities[i] << " ";
    }
    out << "]" << endl;
}
} // namespace prost::parser
