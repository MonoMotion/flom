![flom](docs/logo.svg)

[![Travis CI](https://img.shields.io/travis/MonoMotion/flom.svg?style=flat-square)](https://travis-ci.org/MonoMotion/flom)
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FDeepL2%2Fflom.svg?type=small)](https://app.fossa.io/projects/git%2Bgithub.com%2FDeepL2%2Fflom?ref=badge_small)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/37de359208f14aa5ab2e2a67b170335a)](https://www.codacy.com/app/coord-e/flom?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=MonoMotion/flom&amp;utm_campaign=Badge_Grade)
[![license](https://img.shields.io/github/license/MonoMotion/flom.svg?style=flat-square)](COPYING)
[![Read the Docs](https://img.shields.io/readthedocs/flom/develop.svg?style=flat-square)](https://flom.readthedocs.org/)
[![GitHub release](https://img.shields.io/github/release/MonoMotion/flom.svg?style=flat-square)](https://github.com/MonoMotion/flom/releases)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-brightgreen.svg?style=flat-square)](https://isocpp.org/std/status)


[Documentation](https://flom.readthedocs.org/)

**flom** is a library to handle keyframed motion of robots in C++.
The main issue with formely used representation-like plain csv or json (used in DeepMimic) with keyframes-is lack of interoperability.
flom resolves that problem by providing basic functionalities to handle keyframed motion (looping, interpolation, etc) in one library.


## Features

- Obtain a frame correspond to arbitrary time point
- Iterate over frames at arbitary fps
- Import / Export the motion data
   - can be converted to JSON
   - the file format specification is represented in [protobuf](https://github.com/protocolbuffers/protobuf)
- Edit keyframes (insert/delete)
- Effector support
   - Effectors can express the reference pose of links


## Use from other languages

Currently, these bindings are available:

- [MonoMotion/flom-py](https://github.com/MonoMotion/flom-py) (Python)


## Installation

Download and install suitable package from [releases](https://github.com/MonoMotion/flom/releases)

Latest build artifacts can be found at [![bintray](https://img.shields.io/bintray/v/monomotion/flom/latest.svg?style=flat-square)](https://dl.bintray.com/monomotion/flom/)

Or you can [build manually](#build-manually)


## Build manually

See the [documentation](https://flom.readthedocs.io/en/develop/build.html).


## License

[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FDeepL2%2Fflom.svg?type=large)](https://app.fossa.io/projects/git%2Bgithub.com%2FDeepL2%2Fflom?ref=badge_large)
