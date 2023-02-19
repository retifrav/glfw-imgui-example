# GLFW Dear ImGui example

An example of using [Dear ImGui](https://github.com/ocornut/imgui) with [GLFW](https://www.glfw.org).

![GLFW and Dear ImGui](/img/screenshot.png "GLFW and Dear ImGui")

More information about the application in the following [article](https://decovar.dev/blog/2019/08/04/glfw-dear-imgui/).

<!-- MarkdownTOC -->

- [Building](#building)
    - [Without package managers](#without-package-managers)
    - [With Conan](#with-conan)
    - [With vcpkg](#with-vcpkg)
- [Vulkan](#vulkan)

<!-- /MarkdownTOC -->

## Building

### Without package managers

Dependencies are built from sources. Provide paths to required libraries source code:

- glad: `-DGLAD_PREFIX="/path/to/glad/generated"`
- GLFW: `-DGLFW_PREFIX="/path/to/glfw"`
- Dear ImGui: `-DDEAR_IMGUI_PREFIX="/path/to/dear-imgui"`

By default these paths are set to `_dependencies/LIBRARY-NAME` (*so you can copy those there*).

``` sh
$ cd /path/to/glfw-imgui-example
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../install" \
    -DUSING_CONAN=0 \
    ..
$ cmake --build . --target install
$ ../install/bin/glfw-imgui/glfw-imgui
```

### With Conan

Pre-built dependencies are fetched from Conan server, only the application itself is built from sources.

Without using CMake preset:

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

Using CMake preset:

``` sh
$ cd /path/to/glfw-imgui-example
$ mkdir build/conan && cd $_
$ conan install ../.. -r YOUR-CONAN-REMOTE
$ cd ../..
$ cmake --preset conan
$ cmake --build --preset conan
```

More information about resolving dependencies with Conan [here](https://decovar.dev/blog/2022/02/06/cpp-dependencies-with-conan/).

### With vcpkg

Preparation:

- you need to have [vcpkg](https://vcpkg.io/) installed;
    + its executable is available in the `PATH`;
    + `VCPKG_ROOT` environment variable is set;
- to be able to use CMake presets `v3`, you need to have CMake at least `v3.21`.

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

## Vulkan

The application uses OpenGL as the graphics backend. I would like to add an optional Vulkan support, but that's a work in progress still.

It looks mostly done, vcpkg ports of dependencies have been adjusted accordingly, so you can try to build it like this:

``` sh
$ cd /path/to/glfw-imgui-example
$ cmake --preset vcpkg-windows-static -DVCPKG_MANIFEST_NO_DEFAULT_FEATURES=1 -DVCPKG_MANIFEST_FEATURES="with-vulkan" -DUSING_VULKAN=1
$ cmake --build --preset vcpkg-windows-static
```

But at the moment it crashes on the first call to `vkEnumeratePhysicalDevices()` on Windows 11.
