# Assignment 3.0 Alica turtlesim

Go through [the tutorial on alica](https://github.com/rapyuta-robotics/alica/tree/devel/supplementary/alica_ros1/alica_ros_turtlesim).

<b>NOTE</b>: Alica is already part of this repository. Please use this repository as catkin workspace instead of `catkin_ws`.

Try out different master plans, attach screnshots/gifs of results.
Hint: lookup launch files to find out how to set certain master plan.

# Assignment 3.1: Alica in Flatland

In this assignment you are supposed to make alica turtlesim tutorial in [Flatland simulation](https://flatland-simulator.readthedocs.io/en/latest/overview.html).

Go into [alica_ros_flatland_turtle](https://github.com/Gamezar/ros-training/tree/main/src/alica_ros_flatland_turtle) package and remove CATKIN_IGNORE file.

This package contains draft of the setup - you need to make it complete.

## Guidance

To complete it let's understand how alica in assignment 3.0 works with turtlesim.

Please keep alica designer open and navigate through it when some instance of turtlesim state machine is mentioned.

### Playing with Flatland

Try out running flatland simulation using following command after building flatland:
1. Build `flatland_models`
2. Execute:
```bash
roslaunch flatland_models sim.launch
```

Spawn a model using `Spawn model` button. Use turtlebot model in flatland_models directory.

Try playing with `rostopic` and `rosservice`. See what you can do in simulation using them alone.

### Moving turtle to it's place in a formation

Let's try to look into alica plan from previous assignment.

Inside the `Simulation` plan we can see first state contains plan `TeleportToRandomPosition`.

As name suggests this plan will move turtle into random position.

After turtle was teleported `MakeFormation` plan is executed.

Let's look inside this plan. You can see 2 states: one is for going to the center, the other one is for going to the position on the circle. Both of them use the behaviour `GoToCalculatedResult`.

To see the implementation of this behaviour refer to [the code](https://github.com/rapyuta-robotics/alica/blob/2ea37c705ee3ba43575c5b7636c41a19fed74af5/supplementary/alica_turtlesim/libalica-turtlesim/src/GoToCalculatedResult.cpp#L24).

You can see that behaviour commands turtle with following method: `_turtle->moveTowardPosition`. Looking up [what is _turtle](https://github.com/rapyuta-robotics/alica/blob/2ea37c705ee3ba43575c5b7636c41a19fed74af5/supplementary/alica_turtlesim/libalica-turtlesim/include/GoToCalculatedResult.h#L23), we find out that the type is [`turtlesim::TurtleInterfaces`](https://github.com/rapyuta-robotics/alica/blob/2ea37c705ee3ba43575c5b7636c41a19fed74af5/supplementary/alica_turtlesim/include/alica_turtlesim/turtle_interfaces.hpp#L20), which is an interface that allows usage of different implementations of turtle using basic polymorphism.

So to be able to use existing functionality in Flatlad, we need to simply implement this interface. For that modify [the draft file](https://github.com/Gamezar/ros-training/blob/main/src/alica_ros_flatland_turtle/src/turtle_ros1_interfaces.cpp).

After implementing it you will be still getting compilation errors. Fix them.

Find out how to launch simulation and turtles using `roslaunch` (without using `flatland_models`'s launch file) and observe turtles spawning and at least attempting to form a circle.

# Assignment 3.2: Improving the setup

If you try running the setup with bigger number of turtles you will notice robots having trouble to form a circle because of surrounding walls.

Let's change the setup such that turtles will have no troubles with walls.

1. Let's teleport turtles to the big room on the right. For that:
- Look into the plan where robots are teleported.
- Try to access behaviours' blackboards (on ui it is small tables near icons of behaviours) and see what you can do
- After changes made. Export alica workspace and replace the current one with updated one.

2. Let's change the constraint so that robots make a formation in this room also:
- Look into the plan where you think formation is decided.
- Look into conditions of this plan
- Find the place in code where the formation is decided and make a required change

Try building `alica_turtlesim` after changes and running everything again. Turtles should align in a circle in the room.

# Assignment 3.3: Dealing with walls

Try running new setup with 5 robots.

Try sending commands to one robot to leave formation and join formation.

You may notice that robot behaves strange with a wall and may even fail to join formation again.

For this final assignment you will need to modify `moveTowardPosition` function using A* algorithm.

Few hints:
- If you are running `env.launch` file for simulation. `map_server` will be already setup for you. Try to look into `/map` topic and what information you can get from it.
- Planning robot path you will also need to take <b>robot footprint</b> into consideration. Find what is the footprint of the turtlebot's model used within the project and consider it in path planning. <b>Note</b>: it is ok to consider 2d footprint of robot's main body.


Test your soluition with joining and leaving formation. Try also to expiriment with setting up different teleport positions.
