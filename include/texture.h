//=================//
//                 //
//    texture.h    //
//                 //
//======================================//
// texture managing component to handle //
// loading and organizing gl textures.  //
//============================================================================80

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

// structs //-------------------------------------------------------------------

// texture parameters to load in an image
typedef struct texture {
  unsigned int gl_ptr;             // gl reference to texture
  int w, h, c;                     // width, height, channels
  char* pixel_buf;                 // buffer for pixel bytes
  union {char* path; char* name;}; // path to image file
} texture;

// prototypes //----------------------------------------------------------------
char*   create_image (char* path, int* w, int* h, int* c);
texture new_texture  (char* path, char* name);
void    bind_texture (texture t, unsigned int shader);

#endif
