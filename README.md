# GLFW Dear ImGui example

An example of using [Dear ImGui](https://github.com/ocornut/imgui) with [GLFW](https://www.glfw.org).

![GLFW and Dear ImGui](/img/screenshot.png "GLFW and Dear ImGui")

More information about the application in the following [article](https://decovar.dev/blog/2019/08/04/glfw-dear-imgui/).

<!-- MarkdownTOC -->

- [Building](#building)
    - [With a package manager](#with-a-package-manager)
        - [vcpkg](#vcpkg)
        - [Conan](#conan)
    - [Without package managers](#without-package-managers)

<!-- /MarkdownTOC -->

## Building

### With a package manager

#### vcpkg

Preparation:

- you need to have [vcpkg](https://vcpkg.io/) installed;
    + its executable is available in the `PATH`;
    + `VCPKG_ROOT` environment variable is set;
- to be able to use CMake presets (*`v3`*), you need to have CMake version not older than `3.21`.

Without using CMake preset:

``` sh
$ cd /path/to/glfw-imgui-example
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DUSING_PACKAGE_MANAGER_VCPKG=1 \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    ..
$ cmake --build . --target install
```

Using CMake preset:

``` sh
$ cd /path/to/glfw-imgui-example
$ cmake --preset vcpkg-default-triplet
$ cmake --build --preset vcpkg-default-triplet
```

More information about resolving dependencies with vcpkg [here](https://decovar.dev/blog/2022/10/30/cpp-dependencies-with-vcpkg/).

#### Conan

**Be aware** that resolving dependencies with Conan has not been tested for a long time (*since I started using vcpkg*), so the instructions below might not be up to date or/and the project might no longer build with Conan-resolved dependencies.

Dependencies are expected to be fetched already pre-built from a custom Conan server/storage, so only the application itself will be built from sources. That is not a very common arrangement, as normally dependencies would also build from sources with Conan recipes, but back then I was a bit ~~more stupid~~ less experienced with this kind of things.

Anyway, here's without using CMake presets:

``` sh
$ cd /path/to/glfw-imgui-example
$ mkdir build && cd $_
$ conan install .. --remote=YOUR-CONAN-REMOTE
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DUSING_PACKAGE_MANAGER_CONAN=1 \
    ..
$ cmake --build . --target install
```

And here's with using CMake presets:

``` sh
$ cd /path/to/glfw-imgui-example
$ mkdir build/conan && cd $_
$ conan install ../.. -r YOUR-CONAN-REMOTE
$ cd ../..
$ cmake --preset conan
$ cmake --build --preset conan
```

More information about resolving dependencies with Conan [here](https://decovar.dev/blog/2022/02/06/cpp-dependencies-with-conan/).

### Without package managers

Dependencies are built from sources. Provide paths to required libraries source code:

- glad: `-DGLAD_PREFIX="/path/to/glad/generated"`
- GLFW: `-DGLFW_PREFIX="/path/to/glfw"`
- Dear ImGui: `-DDEAR_IMGUI_PREFIX="/path/to/dearimgui"`

By default these paths are set to `_dependencies/LIBRARY-NAME` (*so you can copy those there*).

``` sh
$ cd /path/to/glfw-imgui-example
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    ..
$ cmake --build . --target install
$ ../install/bin/glfw-imgui/glfw-imgui
```
