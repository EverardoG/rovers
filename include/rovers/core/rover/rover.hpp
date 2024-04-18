#ifndef THYME_ENVIRONMENTS_ROVERS_ROVER
#define THYME_ENVIRONMENTS_ROVERS_ROVER

#include <Eigen/Dense>
#include <iostream>
#include <rovers/core/detail/agent_types.hpp>
#include <rovers/core/detail/entity_types.hpp>
#include <rovers/core/detail/pack.hpp>
#include <rovers/core/rewards/global.hpp>
#include <rovers/utilities/math/cartesian.hpp>
#include <vector>

namespace rovers {

/*
 *
 * rover interface
 *
 */
class IRover {
    using Point = thyme::math::Point;
    using ActionType = Eigen::MatrixXd;
    using StateType = Eigen::MatrixXd;

   public:
    IRover(double obs_radius = 1.0) : m_obs_radius(obs_radius) {}
    virtual ~IRover() = default;

    void reset() { m_path.clear(); }

    const Point& position() const { return m_position; }
    void set_position(double x, double y) {
        m_position.x = x;
        m_position.y = y;
        m_path.push_back(Point(x, y));
    }
    void update_position (double dx, double dy){
        set_position(m_position.x + dx, m_position.y + dy);
    }

    const double& obs_radius() const { return m_obs_radius; }

    const std::vector<Point>& path() const { return m_path; }

    void update() {
        // housekeeping.
        // delegate to tick for implementers.
        tick();
    }

    [[nodiscard]] virtual StateType scan(const AgentPack&) const = 0;
    [[nodiscard]] virtual double reward(const AgentPack&) const = 0;

    // [TODO] temp cppyy super().__init__() fix
    virtual void act(const ActionType&) {}
    Point m_position;

   protected:
    virtual void tick() {}

   private:
    double m_obs_radius;
    std::vector<Point> m_path;
};

/*
 *
 * Default boilerplate rover
 *
 */
template <typename SensorType, typename ActionSpace, typename RewardType = rewards::Global>
class Rover final : public IRover {
    using SType = thyme::utilities::SharedWrap<SensorType>;
    using RType = thyme::utilities::SharedWrap<RewardType>;
    using ActionType = Eigen::MatrixXd;
   public:
    // I can set observation radius of this rover.
    // What does observation radius mean here?
    // obs_radius as in how far can the rover sense POIs and other rovers?
    // OR obs_radius as in how far can a POI be and the rover can still "observe" it for G?
    // sensor types can be defined - interesting idea for different agents to have
    // different sensors. For now, let's just do standard density sensors
    // did not know reward could be defined based on type
    // We can do things like give some agents G, some agents D, and some D-Indirect, and some
    // an entirely different reward
    Rover(double obs_radius = 1.0, SType sensor = SensorType(), RType reward = RewardType())
        : IRover(obs_radius), m_sensor(sensor), m_reward(reward) {}

    [[nodiscard]] virtual Eigen::MatrixXd scan(const AgentPack& pack) const override {
        return m_sensor->scan(pack);
    }
    [[nodiscard]] virtual double reward(const AgentPack& pack) const override {
        return m_reward->compute(pack);
    }
    void act(const ActionType& action) override {
        // default, move in x and y
        assert(action.rows() >= 2);
        auto act = static_cast<Eigen::Vector2d>(action);
        update_position(act[0], act[1]);
    }

   private:
    SType m_sensor;
    RType m_reward;
};

/*
 *
 * Example of bringing in a new Rover from the python bindings
 *
 */
class Drone final : public IRover {
   public:
    Drone(double obs_radius = 1.0) : IRover(obs_radius) {}

    [[nodiscard]] virtual Eigen::MatrixXd scan(const AgentPack&) const override { return {}; }
    [[nodiscard]] virtual double reward(const AgentPack&) const override { return 0; }
    void act(const Eigen::MatrixXd&) override { }
};

}  // namespace rovers

#endif
