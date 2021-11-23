# **win**
window interface for sdl2 and opengl in c.

## dependencies
SDL2 and OpenGL.

## usage
```c
git clone https://github.com/sjdobesh/win.git
cd win
./install
```
## data structures
### shader
```c
struct shader {
  GLuint gl_ptr; // open gl ptr for the object
  char* name;    // file path or user given name
  int bound;     // bool
}l
```
### shader collection
```c
struct shader_collection {
  shader* shader; // list of shaders
  int count;      // number of shaders stored
}l
```
### win
A win struct is the master container for all window data.
```c
struct win {
  SDL_Window*       window;
  SDL_GLContext     context;
  shader_collection vert_shaders;
  shader_collection frag_shaders;
  shader_collection shader_programs;
  int w, h;
};
```
## window interface
```c
win init_win(); // initialize a window structure
int load_shader(win*, "file/path/frag", FRAG);
int bind_shader("name" | "filepath");
int render(win);
```
