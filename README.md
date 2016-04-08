PathViz
======

This is a library for rendering a randomly generated scene for any given path.

Only a `cameras.xml` file and CSV file of rig poses are required. A scene of
randomly placed, randomly textured cubes is constructed around the given path.
Images can then be rendered for each camera in the rig at each pose. Such
images can be used in testing visual tracking and visual odometry pipelines.

### Dependencies
- GLEW
- GLFW
- OpenCV
- Calibu (https://github.com/arpg/Calibu)
- Sophus (https://github.com/arpg/Sophus)
