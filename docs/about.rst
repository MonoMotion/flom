##################
About this project
##################

**flom** is a library to handle keyframed motion of robots in C++.
The main issue with formely used representation-like plain csv or json (used in DeepMimic) with keyframes-is lack of interoperability.
flom resolves that problem by providing basic functionalities to handle keyframed motion (looping, interpolation, etc) in one library.

Features
********

- Obtain a frame correspond to arbitrary time point
- Iterate over frames at arbitary fps
- Import / Export the motion data

   - can be converted to JSON
   - the file format specification is represented in `protobuf <https://github.com/protocolbuffers/protobuf>`_

- Edit keyframes (insert/delete)
- Effector support

   - Effectors can express the reference pose of links


Use from other languages
************************

Currently, these bindings are available:

- `MonoMotion/flom-py <https://github.com/MonoMotion/flom-py>`_ (Python)


Relationship to MonoMotion
**************************

In the development of `MonoMotion <https://github.com/MonoMotion>`_, we needed common representation of robot motion.
flom is created for that purpose, but flom can be used for anything.


