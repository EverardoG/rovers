#ifndef THYME_ENVIRONMENTS_ROVERS_POI_COUNT_CONSTRAINT
#define THYME_ENVIRONMENTS_ROVERS_POI_COUNT_CONSTRAINT

#include <rovers/core/poi/poi.hpp>
#include <rovers/core/rover/rover.hpp>
#include <rovers/utilities/math/norms.hpp>

namespace rovers {

/*
 *
 * Constraint satifisted by a count of observations
 *
 */
class CountConstraint {
   public:
    explicit CountConstraint(size_t count = 1) : count_constraint(count) {}

    [[nodiscard]] bool is_satisfied(const EntityPack& entity_pack) const {
        size_t count = 0;
        for (const auto& rover : entity_pack.agents) {
            double dist = l2_norm(rover->position(), entity_pack.entity->position());
            if (dist <= rover->obs_radius() && dist <= entity_pack.entity->obs_radius()) {
                ++count;
                // Basically, check how many rovers 
                if (count >= count_constraint) return true;
                std::cout << count << std::endl;
            }
        }
        return false;
    }

    size_t count_constraint;
//    private:
    
};

}  // namespace rovers

#endif