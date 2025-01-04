# ureka-framework with C++

> The Ureka framework is a user-centric security framework that prioritizes the protection of user devices and data through comprehensive authentication, authorization, and auditing functions.


## Environment

+ Hardware: Mac (Apple M2, arm64)
+ Platform: Docker (ubuntu/alpine) on MacOS (Darwin)
+ C++: C++17
+ Package Management: Makefile
+ Formatter: None
+ Testing: Google Test


## Get Started: Docker Environment

Requirement: Docker
+ There are different Dockerfiles for building the environment in `Ubuntu 24.04` and `Alpine 3.20`.
+ < Troubleshooting > 
  + It seems there are some non-deterministic bugs when running on `ubuntu-docker-macOS`, so I recommend using the `alpine-docker-macOS` version.

Project Structure
```
ureka-cpp-in-docker
├── README.md
├── Dockerfile.xxx: The Dockerfile for building the environment.
├── /installed: The to-be-installed libraries & configurations in the docker container.
└── /workdir: The shared directory ($ docker run -v) between the host machine and the docker container.
    ├── /the-project-repo (e.g., ureka-cpp)
    └── shared.txt
```

Build & Run the docker image (Ubuntu 24.04)
```
$ cd ureka-cpp-in-docker
$ docker build -t ureka-cpp-ubuntu:24.04 -f Dockerfile.ureka-cpp-in-ubuntu .
$ docker run -it -h ureka-cpp-ubuntu -v ./workdir:/root/workdir --name ureka-cpp-ubuntu ureka-cpp-ubuntu:24.04
```

Build & Run the docker image (Alpine 3.20)
```
$ cd ureka-cpp-in-docker
$ docker build -t ureka-cpp-alpine:3.20 -f Dockerfile.ureka-cpp-in-alpine .
$ docker run -it -h ureka-cpp-alpine -v ./workdir:/root/workdir --name ureka-cpp-alpine ureka-cpp-alpine:3.20
```

Remember your container name/id, so that you can start it again.
```
$ docker start -i <container-name or container-id>
```


## Get Started: Build (Compile) & Run the Test

The test is in the `framework/test` directory.
```
$ cd framework/test
```

Clean the previous execution files
```
$ make clean_all
```

Build the execution files
```
$ make all
```

Run the tests
```
$ make run
```

Or run the tests mulitple times
```
$ ./run_test.sh
```


## C/C++ Dependency

The dependency installation scripts have already embedded in the Dockerfile. 
+ We can install these dependencies from the Installer (e.g., `apt in Ubuntu`, `apk in Alpine`).
+ Or, we can self-build these dependencies from the Source (e.g., `CMake`, `make`, etc.).


### 1. [Google Test](https://github.com/google/googletest)

### 2. [MBedTLS](https://github.com/Mbed-TLS/mbedtls)

### 3. [nlohmann/json](https://github.com/nlohmann/json)

