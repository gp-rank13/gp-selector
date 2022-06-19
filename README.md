# GP Extension example for JUCE (with CMake)

This is an example extension using the [Gig Performer SDK](https://github.com/gigperformer/gp-sdk).
You can use it as a base for your own extensions developed in with JUCE in C++ and built using [CMake](https://cmake.org).

## How to build...

### ... on macOS

-   Make sure you have [CMake](https://cmake.org) installed.
    You might have to add the path to the cmake executable by adding this line to your bash profile:

    ```bash
    export PATH=/Applications/CMake.app/Contents/bin:$PATH
    ```

    Alternatively, you can install CMake via [Homebrew](https://brew.sh):

    ```bash
    brew install cmake
    ```

-   Build (and install) the project via CMake.
    For your convenience we have provided a `build.sh` which contains all necessary commands:

    ```bash
    ./build.sh
    ```

### ... on Windows

-   Make sure you have [CMake](https://cmake.org), [Git](https://git-scm.com) and a C(++) compiler installed.
    You can get that for example by installing [Visual Studio](https://visualstudio.microsoft.com) with the _Desktop development with C++_ workload and the _Git for Windows_ component.
-   Build (and install) the project via CMake.
    For your convenience we have provided a `build.cmd` which contains all necessary commands:

    ```bash
    build.cmd
    ```

    **Make sure to run the script from the Visual Studio developer command prompt!**
