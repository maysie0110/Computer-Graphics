// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
//#include <cstdio>
#include <iostream>
// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  mat3 ret;
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1.
    //axis needs to be normalize before use
    vec3 normalizedAxis = glm::normalize(axis);
    // Convert degrees to radians
    float rad = degrees * pi / 180 ;
    
    //identity matrix - 1st matrix
    mat3 identity = mat3(1.0);
    
    //second matrix in axis-angle formula
    mat3 second = mat3(normalizedAxis.x * normalizedAxis.x, normalizedAxis.x * normalizedAxis.y, normalizedAxis.x * normalizedAxis.z, normalizedAxis.x * normalizedAxis.y, normalizedAxis.y * normalizedAxis.y, normalizedAxis.y * normalizedAxis.z, normalizedAxis.x * normalizedAxis.z, normalizedAxis.y * normalizedAxis.z, normalizedAxis.z * normalizedAxis.z);
    
    //thrid matix in axis-angle formula
    mat3 third = mat3(0.0f, normalizedAxis.z, -normalizedAxis.y, -normalizedAxis.z, 0.0f, normalizedAxis.x, normalizedAxis.y, -normalizedAxis.x, 0.0f);
    
    //rotation matrix
    ret = mat3(cos(rad) * identity + (1- cos(rad)) * second + sin(rad) * third);
  return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.
  // The input is the degrees of rotation, the current eye 3-vector and current up 3-vector.
  // Goal: Update the eye and up vectors appropriately for the user moving left
        
        eye = eye * rotate(-degrees, up);
        up = up * rotate(-degrees, up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.
  vec3 newUp = glm::normalize(glm::cross(up, eye));
  eye = eye * rotate(-degrees, newUp);
  up = up * rotate(-degrees, newUp);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    vec3 forward = glm::normalize(eye - center);
    vec3 left = glm::normalize(glm::cross(up,forward));
    vec3 u = glm::cross(forward, left);
    
    mat4 rotate = mat4(left.x, u.x, forward.x, 0, left.y, u.y, forward.y, 0, left.z, u.z, forward.z, 0, 0, 0, 0, 1);
    
    mat4 translate = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -eye.x, -eye.y, -eye.z, 1);
    
    mat4 ret = rotate * translate;
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.

  // lookAt (camera position, target position, up vector)
//    ret = glm::lookAt(vec3(eye.x, eye.y, eye.z), vec3(center.x, center.y, center.z), vec3(up.x, up.y, up.z));
//    printf("lookAt by user");
    
  return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    //convert fovy/2 to radian
    float theta = glm::radians(fovy)/2;
    float d = 1/ glm::tan(theta);
    float A = -((zFar+zNear)/(zFar-zNear));
    float B = -((2*zFar*zNear)/(zFar-zNear));
  
    mat4 ret = mat4( d/aspect, 0, 0, 0, 0, d, 0, 0, 0, 0, A, -1, 0, 0, B, 0);
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.

  return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
//  mat4 ret;
    mat4 ret = mat4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
  // YOUR CODE FOR HW2 HERE
  // Implement scaling
  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 ret = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
  // YOUR CODE FOR HW2 HERE
  // Implement translation 
  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
