//=================//
//                 //
//    TEXTURE.H    //
//                 //
//======================================//
// texture managing component to handle //
// loading and organizing gl textures.  //
//============================================================================80

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

// STRUCTS //-------------------------------------------------------------------

// texture parameters to load in an image
typedef struct texture {
  unsigned int gl_ptr;// gl reference to texture
  int w, h;        // width, height, channels
  char* pixel_buf;    // buffer for pixel bytes
  char* name;
} texture;

// PROTOTYPES //----------------------------------------------------------------
char*   create_image (char* path, int w, int h, int c);
texture new_texture  (char* path, char* name, int width, int height);
void    bind_texture (texture t, unsigned int shader);

#endif