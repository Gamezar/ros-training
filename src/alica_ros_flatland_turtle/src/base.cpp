#include <DynamicBehaviourCreator.h>
#include <DynamicConditionCreator.h>
#include <DynamicConstraintCreator.h>
#include <DynamicLoadingUtils.h>
#include <DynamicPlanCreator.h>
#include <DynamicTransitionConditionCreator.h>
#include <DynamicUtilityFunctionCreator.h>
#include <alica_ros_flatland/base.hpp>
#include <alica_ros_flatland/turtle_ros1_interfaces.hpp>
#include <clock/AlicaROSClock.h>
#include <clock/AlicaRosTimer.h>
#include <communication/AlicaRosCommunication.h>
#include <constraintsolver/CGSolver.h>
#include <engine/AlicaContext.h>
#include <engine/logging/Logging.h>
#include <logger/AlicaRosLogger.h>
#include <ros/ros.h>

namespace turtle_flatland
{

Base::Base(ros::NodeHandle& nh, ros::NodeHandle& privNh, const std::string& name, const int agent_id, const std::string& roleset,
        const std::string& master_plan, const std::vector<std::string>& paths, std::optional<std::string> placeholderMapping)
        : spinner(0)
{
    // Initialize Alica
    ac = std::make_unique<alica::AlicaContext>(AlicaContextParams(name, paths, roleset, master_plan, false, agent_id, placeholderMapping));

    ac->setCommunicator<alicaRosProxy::AlicaRosCommunication>();
    ac->setTimerFactory<alicaRosTimer::AlicaRosTimerFactory>();
    ac->setLogger<alicaRosLogger::AlicaRosLogger>();

    LockedBlackboardRW bb(ac->editGlobalBlackboard());
    bb.set<std::shared_ptr<turtlesim::TurtleInterfaces>>("turtle", /*instantiate a turtle*/);
    bb.set("spawned", false);
}

void Base::start()
{
    alica::AlicaCreators creators(std::make_unique<DynamicConditionCreator>(), std::make_unique<alica::DynamicUtilityFunctionCreator>(),
            std::make_unique<alica::DynamicConstraintCreator>(), std::make_unique<alica::DynamicBehaviourCreator>(),
            std::make_unique<alica::DynamicPlanCreator>(), std::make_unique<alica::DynamicTransitionConditionCreator>());

    spinner.start(); // start spinner before initializing engine, but after setting context
    ac->init(std::move(creators), false);
    ac->addSolver<alica::reasoner::CGSolver>(ac->getConfig());
}

Base::~Base()
{
    spinner.stop(); // stop spinner before terminating engine
    ac->terminate();
}

} // namespace turtlesim
