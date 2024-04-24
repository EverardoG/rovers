#ifndef THYME_ENVIRONMENTS_ROVERS_GLOBAL
#define THYME_ENVIRONMENTS_ROVERS_GLOBAL

#include <rovers/core/detail/pack.hpp>

namespace rovers::rewards {

/*
 *
 * Default environment reward: checks if all constraints are satisfied
 *
 */
class Global {
   public:
    [[nodiscard]] double compute(const AgentPack& pack) const {
        // TODO pass in a view of POIContainer filtered by observed()
        // TODO Keep filtering over this view for speed-up
        double reward = 0.0;
        for (const auto& poi : pack.entities) {
            // std::cout << "Checking poi" << std::endl;
            // if (poi->observed()) {std::cout << "Poi is observed" << std::endl; continue;}
            // else{std::cout << "POI is not observed" << std::endl;}

            if (poi->constraint_satisfied({poi, pack.agents, pack.entities})) {
                poi->set_observed(true);  // TDDO emit signal to POI instead.
                reward += poi->value();
            }
            // else{
            //     // std::cout << "POI constraint is not satisfied" << std::endl;
            // }
        }
        return reward;
    }
};

}  // namespace rovers::rewards

#endif