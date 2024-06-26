# ROS training

This repository contains practice materials for basics of ROS.

Repository has a structure of catkin workspace, use it to build your packages.

Access assignments in `tasks` folder.

**Note**: please use [catkin_tools](https://catkin-tools.readthedocs.io/en/latest/installing.html) instead of catkin_cmake.

## Prerequisites
1. Ubuntu focal 20.04
2. ROS Noetic ([installation instructions](http://wiki.ros.org/ROS/Installation/TwoLineInstall/))
3. Install submodules of the repository:
```bash
git submodule sync                                             
git submodule update --init --recursive
git-lfs pull
git submodule foreach git-lfs pull
```
4. Run `rosdep install --from-paths src --ignore-src` in your workspace

## Submitting solution to an assignment
For every assignment:
1. Create a new branch
2. Commit changes to new branch
3. Create a pull request to your private repository
4. Request review from [@Gamezar](https://github.com/Gamezar)
