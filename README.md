# GLFW Dear ImGui example

An example of using [Dear ImGui](https://github.com/ocornut/imgui) with [GLFW](https://www.glfw.org).

![GLFW and Dear ImGui](/img/screenshot.png "GLFW and Dear ImGui")

More information in the following [article](https://retifrav.github.io/blog/2019/08/04/glfw-dear-imgui/).

## Building

### Without Conan

Dependencies are built from sources. Provide paths to required libraries source code:

- glad: `-DGLAD_PREFIX="/path/to/glad/generated"`
- GLFW: `-DGLFW_PREFIX="/path/to/glfw"`
- Dear ImGui: `-DDEAR_IMGUI_PREFIX="/path/to/dear-imgui"`

By default these paths are set to `_dependencies/LIBRARY-NAME` (*so you can copy those there*).

``` sh
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../install" -DUSING_CONAN=0 ..
$ cmake --build .
$ cmake --install . --component glfw-imgui
$ ../install/bin/glfw-imgui/glfw-imgui
```

### With Conan

Pre-built dependencies are fetched from Conan server, only the application itself is built from sources.

``` sh
$ mkdir build && cd $_
$ conan install .. --remote=YOUR-CONAN-REMOTE
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../install" -DUSING_CONAN=1 ..
$ cmake --build . --target install
$ ../install/bin/glfw-imgui/glfw-imgui
```
