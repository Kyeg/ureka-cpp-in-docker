# ureka-framework with C++

> The Ureka framework is a user-centric security framework that prioritizes the protection of user devices and data through comprehensive authentication, authorization, and auditing functions.


## Environment

+ Hardware: Mac (Apple M2, arm64) or Windows (amd64)
+ Platform: Docker on MacOS or on Windows Subsystem for Linux (WSL)
+ Editor: VSCode (with WSL Extension for Windows)
+ Programming Language: C++17
+ Dependency Management: Pure Makefile
+ Formatter: None
+ Testing: Google Test


## Get Started: Container (Docker) Environment

Requirement: MacOS or WSL in Windows
+ Open the "Editor", the "Revision Control" here.
+ Standardizing the development environment on Linux is recommended to eliminate cross-platform compatibility issues and ensure a consistent workflow.

Requirement: Container (e.g., Docker)
+ Run/Test the "Programming Language & Dependencies" here.
+ Standardizing the development environment in Container is recommended to eliminate dependency issues.
+ There are different Dockerfiles for building the container in `Ubuntu 24.04` and `Alpine 3.20`.
  + It seems there are some non-deterministic bugs when running on `ubuntu-docker-macOS-arm64`, so for macOS-arm64, using the `alpine-docker-macOS-arm64` version is recommended.
  + On the other hand, `ubuntu-docker-Windows-amd64` and `alpine-docker-Windows-amd64` should be both reliable. 

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

Build & Run the docker image (Alpine 3.20, reliable for MacOS or Windows)
```
$ cd ureka-cpp-in-docker
$ docker build -t ureka-cpp-alpine:3.20 -f Dockerfile.ureka-cpp-in-alpine .
$ docker run -it -h ureka-cpp-alpine -v ./workdir:/root/workdir --name ureka-cpp-alpine ureka-cpp-alpine:3.20
```


Or Build & Run the docker image (Ubuntu 24.04, reliable for Windows)
```
$ cd ureka-cpp-in-docker
$ docker build -t ureka-cpp-ubuntu:24.04 -f Dockerfile.ureka-cpp-in-ubuntu .
$ docker run -it -h ureka-cpp-ubuntu -v ./workdir:/root/workdir --name ureka-cpp-ubuntu ureka-cpp-ubuntu:24.04
```

Remember your container name/id, so that you can start it again.
```
$ docker start -i <container-name or container-id>
```


## Get Started: Build (Compile) & Run the Test

The tests are in the `framework/test` directory.
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

The dependency installation scripts have already been embedded in the `Dockerfile`. 
+ We can install these dependencies from the Installer (e.g., `apt in Ubuntu`, `apk in Alpine`).
+ Or, we can self-build these dependencies from the Source (e.g., `CMake`, `make`, etc.).


### 0. (Optional) [Zsh & Oh-My-Zsh](https://github.com/ohmyzsh/ohmyzsh)
+ Zsh & Oh-My-Zsh provide a Unix shell with more advanced features.
+ However, they are optional, and can be removed from Dockerfile.

### 1. [Google Test](https://github.com/google/googletest)

### 2. [MBedTLS](https://github.com/Mbed-TLS/mbedtls)

### 3. [nlohmann/json](https://github.com/nlohmann/json)

