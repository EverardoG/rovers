#ifndef THYME_ENVIRONMENTS_ROVERS_DIFFERENCE
#define THYME_ENVIRONMENTS_ROVERS_DIFFERENCE

#include <rovers/core/rewards/global.hpp>
#include <rovers/utilities/ranges.hpp>

namespace rovers::rewards {

/*
 *
 * Difference between reward and the reward without the agent acting
 *
 */
class Difference {
   public:
    [[nodiscard]] double compute(const AgentPack& pack) const {
        // std::cout << "-------------" << std::endl;
        std::cout << "start compute()" << std::endl;
        std::cout << "memory address of AgentPack in compute()" << &pack << std::endl;

        double reward = Global().compute(pack);
        std::cout << "pack size: " << pack.agents.size() << std::endl;
        auto rovers_without_me = thyme::utilities::filter(
            pack.agents, [&](const auto& rover) { return &rover != &pack.agent; });

    //     struct AgentPack {
    //     AgentPack(const Agent& agent, const std::vector<Agent>& agents,
    //             const std::vector<Entity>& entities)
    //         : agent(agent), agents(agents), entities(entities) {}
    //     Agent agent;
    //     std::vector<Agent> agents;
    //     std::vector<Entity> entities;
    // };

        std::vector<Agent> agents_without_me;
        std::cout << "memory address of me: " << &pack.agent << std::endl;
        for (const Agent& agent_in_vec : pack.agents) {
            if (&agent_in_vec != &pack.agent) {
                std::cout << "memory of other agent: " << &agent_in_vec << std::endl;
                agents_without_me.push_back(agent_in_vec);
            }
        }

        // Make a new agentpack
        const AgentPack& pack_without_me = AgentPack(pack.agent, agents_without_me, pack.entities);

//         AgentPack from_filter(const AgentPack& pack, std::function<bool(const Agent&)> predicate) {
//     return {pack.agent, thyme::utilities::filter(pack.agents, predicate), pack.entities};
// }

        // const AgentPack& pack_without_me = from_filter(pack, [&](const auto& rover) { return &rover != &pack.agent; });

        std::cout << "pack without me size: " << pack_without_me.agents.size() << std::endl;
        double reward_without_me = Global().compute(pack_without_me);
        std::cout << "reward: " << reward << " | reward_without_me: " << reward_without_me << std::endl;
        std::cout << "end compute()" << std::endl;
        return reward - reward_without_me;
    }
};

}  // namespace rovers::rewards

#endif