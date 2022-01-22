# GLFW Dear ImGui example

An example of using [Dear ImGui](https://github.com/ocornut/imgui) with [GLFW](https://www.glfw.org).

![GLFW and Dear ImGui](/img/screenshot.png "GLFW and Dear ImGui")

More information in the following [article](https://retifrav.github.io/blog/2019/08/04/glfw-dear-imgui/).

## Building

### Without Conan

Copy required Dear ImGui sources to `_dependencies/DearImGui/`.

You might also need to set paths to required libraries:

- glad: `-DGLAD_PREFIX="/path/to/glad/generated"`
- GLFW: `-DGLFW_PREFIX="/path/to/glfw"`

``` sh
$ mkdir build && cd $_
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../install" -DUSING_CONAN=0 ..
$ cmake --build . --target install
$ ../install/bin/glfw-imgui
```

### With Conan

``` sh
$ mkdir build && cd $_
$ conan install .. --remote=YOUR-CONAN-REMOTE
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../install" -DUSING_CONAN=1 ..
$ cmake --build . --target install
$ ../install/bin/glfw-imgui
```
