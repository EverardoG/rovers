#ifndef THYME_ENVIRONMENTS_ROVERS_REWARD_COMPUTER
#define THYME_ENVIRONMENTS_ROVERS_REWARD_COMPUTER

#include <rovers/core/rewards/global.hpp>
#include <rovers/core/rewards/difference.hpp>
#include <rovers/core/rover/rover.hpp>
#include <rovers/utilities/ranges.hpp>
#include <rovers/utilities/math/norms.hpp>

namespace rovers::rewards {

class RewardComputer {
    public:
    using Reward = std::vector<double>;

    RewardComputer(std::vector<Agent> rovers, std::vector<Entity> pois) {
        // std::cout << "RewardComputer::RewardComputer()" << std::endl;
        m_rovers = rovers;
        m_pois = pois;
    }

    std::vector<std::vector<int>> prep_all_or_nothing_influence() const {
        // Each element contains the indicies of rovers (as in, nominal type "rover") influenced
        // by the agent in this index.
        // (Only going to count nominal type "uav" agents as being able to influence)

        // std::cout << "RewardComputer::prep_all_or_nothing_influence()" << std::endl;

        int t_final = m_rovers[0]->path().size();

        // std::cout << "RewardComputer::prep_all_or_nothing_influence() t_final | " << t_final << std::endl;

        // Counters tell us how much each agent was influenced by other agents
        // First index (k) is the agent being influenced
        // Second index (i) is how much agent i influenced agent k 
        std::vector<std::vector<int>> counters(m_rovers.size(), std::vector<int>(m_rovers.size(), 0));
        for (int t=0; t < t_final; ++t) {
            // std::cout << "t " << t << std::endl;
            // agent i is the influencing agent
            for (int i=0; i < m_rovers.size(); ++i) {
                // std::cout << "i " << i << std::endl;
                // agent k is the agent being influenced
                for (int k=0; k < m_rovers.size(); ++k) {
                    // std::cout << "k " << k << std::endl;
                    if (i != k && m_rovers[i]->type() == "uav" && m_rovers[k]->type() == "rover" && is_influencing(m_rovers[i], m_rovers[k], t) ) {
                        // std::cout << "Increasing counter at counters["<<k<<"]["<<i<<"]" << std::endl;
                        counters[k][i]++;
                    }
                }
            }
        }

        // std::cout << "RewardComputer::prep_all_or_nothing_influence() Finished creating counters" << std::endl;
        // std::cout << "counters.size() " << counters.size() << std::endl;
        for (int k=0; k < counters.size(); ++k) {
            // std::cout << "counters[" << k << "]" << std::endl; 
            for (int i=0; i < counters.size(); ++i) {
                // std::cout << "counters[" << k << "][" << i << "] = " << counters[k][i] << std::endl;
            }
        }
        // std::cout << "counters " << counters[0] << std::endl;

        // Create the sets of agents to remove for each agent
        // Index is the agent being influenced (k)
        // This index gives a vector of indicies of agents that agent k influenced
        std::vector<std::vector<int>> influence_sets(m_rovers.size(), std::vector<int>({}));
        // Include yourself in your influence set
        // std::cout << "RewardComputer::prep_all_or_nothing_influence() Insert yourself into your influence set (start)" << std::endl;
        for (int i=0; i < influence_sets.size(); ++i) {
            influence_sets[i].push_back(i);
            // std::cout << "RewardComputer::prep_all_or_nothing_influence() Ran influence_sets[i].push_back(i) with i = " << i << std::endl;
        }
        // std::cout << "RewardComputer::prep_all_or_nothing_influence() Adding other agents to influence sets" << std::endl;
        for (int k=0; k < m_rovers.size(); ++k) {
            // std::cout << "RewardComputer::prep_all_or_nothing_influence() on agent k = " << k << std::endl;
            int highest_ind = -1;
            int num_influence = 0;

            // std::cout << "Beginning iteration through counters[" << k << "].size()" << std::endl;
            for (int i=0; i < counters[k].size(); ++i) {
                // std::cout << "RewardComputer::prep_all_or_nothing_influence() k = " << k << " , i = " << i << std::endl;
                if (counters[k][i] > num_influence) {
                    num_influence = counters[k][i];
                    highest_ind = i;
                }
            }

            // Who was agent k most influenced by?
            // Agent i gets credit for influencing agent k (unless agent i == -1, meaning there was no agent that influenced agent k)
            if (highest_ind != -1) {influence_sets[highest_ind].push_back(k);}
            // influence_sets[highest_ind].push_back(k);
            // std::cout << "RewardComputer::prep_all_or_nothing_influence() Ran influence_sets[highest_ind].push_back(k) on k = " << k << std::endl;
        }

        // std::cout << "RewardComputer::prep_all_or_nothing_influence() Finished building influence_sets" << std::endl;

        return influence_sets;
    }
    
    // TODO: This is based on position RIGHT NOW of each agent
    // need to make this based on position of agents at A PARTICULAR POINT IN TIME ALONG THEIR PATHS
    int is_influencing(Agent agent0, Agent agent1, int t) const {
        if (l2_norm(agent0->path()[t], agent1->path()[t]) <= 5.0) {
            return 1.0;
        }
        else {
            return 0.0;
        }
    }

    [[nodiscard]] Reward compute() const {
        // std::cout << "Reward::compute()" << std::endl;
        Reward rewards;
        // Compute G
        double G = m_Global.compute(AgentPack(0, m_rovers, m_pois));
        // std::cout << "Reward::compute() Computed G" << std::endl;
        // Prep for computing Indirect D
        std::vector<std::vector<int>> influence_sets = prep_all_or_nothing_influence();
        // std::cout << "Reward::compute() Computed influence_sets" << std::endl;

        // Now compute the rewards for each agent
        // std::cout << "Reward::compute() Computing rewards for each agent" << std::endl;
        for (int i = 0; i < m_rovers.size(); ++i) {
            // std::cout << "Reward::compute() Computing reward for agent " << i << std::endl;
            double reward = 0.0;
            // Get the reward type
            std::string reward_type = m_rovers[i]->reward_type();
            // Compute the reward for this agent based on the reward type
            if (reward_type == "Global") {
                // std::cout << "Reward::compute() Computing Global reward" << std::endl;
                reward = G;
            }
            else if (reward_type == "Difference") {
                // std::cout << "Reward::compute() Computing Difference reward" << std::endl;
                reward = G - m_Global.compute_without_me(AgentPack(0, m_rovers, m_pois), i);
            }
            else if (reward_type == "IndirectDifference") {
                // std::cout << "Reward::compute() Computing Indirect Difference" << std::endl;
                // Start simple. 
                // Assume that only rovers can count as being influenced
                // Use all or nothing influence assignment. Just remove the entire trajectories.
                // Refactor for more options later.
                if (m_rovers[i]->indirect_difference_parameters().m_assignment == "manual") {
                    reward = G - m_Global.compute_without_inds(AgentPack(0, m_rovers, m_pois), m_rovers[i]->indirect_difference_parameters().m_manual);
                }
                else if (m_rovers[i]->indirect_difference_parameters().m_assignment == "automatic") {
                    reward = G - m_Global.compute_without_inds(AgentPack(0, m_rovers, m_pois), influence_sets[i]);
                }
            }
            rewards.push_back(reward);
        }
        return rewards;
    }

    Global m_Global;
    std::vector<Agent> m_rovers;
    std::vector<Entity> m_pois;
};

}  // namespace rovers::rewards

#endif
