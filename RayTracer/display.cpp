/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
// OSX systems require different headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Transform.h"
#include "Geometry.h"

using namespace std ; 
#include "variables.h"
#include "readfile.h"

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  
void transformvec (const GLfloat input[4], GLfloat output[4]) 
{
  glm::vec4 inputvec(input[0], input[1], input[2], input[3]);
  glm::vec4 outputvec = modelview * inputvec;
  output[0] = outputvec[0];
  output[1] = outputvec[1];
  output[2] = outputvec[2];
  output[3] = outputvec[3];
}

void display() 
{
  glClearColor(0, 0, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Either use the built-in lookAt function or the lookAt implemented by the user.
  if (useGlu) {
    modelview = glm::lookAt(eye,center,up); 
  } else {
    modelview = Transform::lookAt(eye,center,up); 
  }
  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);

  // Lights are transformed by current modelview matrix. 
  // The shader can't do this globally. 
  // So we need to do so manually.  
  if (numused) {
    glUniform1i(enablelighting,true);

    // YOUR CODE FOR HW 2 HERE.  
    // You need to pass the light positions and colors to the shader. 
    // glUniform4fv() and similar functions will be useful. See FAQ for help with these functions.
    // The lightransf[] array in variables.h and transformvec() might also be useful here.
    // Remember that light positions must be transformed by modelview.  

      //transforming light positions by view matrix can be done for example via transformvec() function,
      //you need to extract pos vector values from appropriate array one by one, apply this function to each set of 4 and then add results to lightransf array in exactly the same order and positions.
      //transformvec (const GLfloat input[4], GLfloat output[4])
      for (int i = 0; i < numused; i++) {
          GLfloat input[4] = { lightposn[i * 4], lightposn[i * 4 + 1], lightposn[i * 4 +2], lightposn[i * 4 + 3]};
          GLfloat output[4];
          transformvec(input, output);
          
          lightransf[(i * 4)] = output[0];
          lightransf[(i * 4) + 1] = output[1];
          lightransf[(i * 4) + 2] = output[2];
          lightransf[(i * 4) + 3] = output[3];
          
      }
      
      //you need to provide all light colors and transformed (!) positions in a SINGLE glUniform call per array, this call and the process of passing data to shader is quite unintuitive, therefore I suggest reading glUniform documentation and also I've found a very helpful hint to avoid certain error
      
      //The glUniform* functions sends fixed data down the shader pipeline and can be accessed through every programmable stage. You wouldn't use uniforms to send vertex data, but constant data. Transform data is usually sent with uniform.
      //Pass them to the light.frag.glsl using glUniform4fv
      glUniform1i(numusedcol, numused);
      glUniform4fv(lightpos, numused, lightransf);
      glUniform4fv(lightcol, numused, lightcolor);
  } else {
    glUniform1i(enablelighting,false); 
  }

  // Transformations for objects, involving translation and scaling 
  mat4 sc(1.0) , tr(1.0), transf(1.0); 
  sc = Transform::scale(sx,sy,1.0); 
  tr = Transform::translate(tx,ty,0.0); 

  // YOUR CODE FOR HW 2 HERE.  
  // You need to use scale, translate and modelview to 
  // set up the net transformation matrix for the objects.  
  // Account for GLM issues, matrix order, etc.  
  // construct transform matrix (transf), by multiplying translate, scale and modelview (mv) matrices (not necessarily in this order!)
    transf = modelview*tr*sc;
    
  // The object draw functions will need to further modify the top of the stack,
  // so assign whatever transformation matrix you intend to work with to modelview
  // rather than use a uniform variable for that.
   modelview = transf;
   glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &transf[0][0]);

  for (int i = 0 ; i < numobjects ; i++) {
    object* obj = &(objects[i]); // Grabs an object struct.

    // YOUR CODE FOR HW 2 HERE. 
    // Set up the object transformations 
    // And pass in the appropriate material properties
    // Again glUniform() related functions will be useful

      
      //you can access object fields by using "obj->fieldname" notation,
      // remember to finally load matrix into modelview that furthermore correctly includes current object transform and your general transform! This makes perfect sense since you need to include both camera orientation and object orientation! If this still seems confusing, imagine that objects are drawn one after another, each with their own transformations and material properties applied beforehand (this is not entirely true, but serves as a good visualization of how this code flows).
      
      //make sure to supply shader with shininess and other material properties using float version of glUniform, not the int!
      glUniform4fv(ambientcol, 1, obj->ambient);
      glUniform4fv(diffusecol, 1, obj->diffuse);
      glUniform4fv(specularcol, 1, obj->specular);
      glUniform4fv(emissioncol, 1, obj->emission);
      
      const GLfloat shine[] = { obj->shininess};
      glUniform1fv(shininesscol, 1, shine);
      
      mat4 objTrans = transf * obj->transform;
      modelview = objTrans;
      glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &objTrans[0][0]);
      // Actually draw the object
      // We provide the actual drawing functions for you.
      // Remember that obj->type is notation for accessing struct fields
    if (obj->type == cube) {
      solidCube(obj->size); 
    }
    else if (obj->type == sphere) {
      const int tessel = 20; 
      solidSphere(obj->size, tessel, tessel); 
    }
    else if (obj->type == teapot) {
      solidTeapot(obj->size); 
    }
  }

  glutSwapBuffers();
}
