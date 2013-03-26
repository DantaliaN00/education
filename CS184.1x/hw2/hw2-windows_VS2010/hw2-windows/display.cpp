/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
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
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"

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
    GLfloat modelview[16]; // in column major order
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview); 

    for (int i = 0 ; i < 4 ; i++) {
        output[i] = 0; 
        for (int j = 0 ; j < 4 ; j++) {
            output[i] += modelview[4*j+i] * input[j]; 
        }
    }
}

mat4 multiplyStack(stack<mat4> st) {
	mat4 res(1);

	while (!st.empty()) {
		res = res * st.top();
		st.pop();
	}
	return res;
}

void display() 
{
    glClearColor(0, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // I'm including the basic matrix setup for model view to 
    // give some sense of how this works.  

    glMatrixMode(GL_MODELVIEW);
    mat4 mv; 

    // Either use the built-in lookAt function or the lookAt implemented by the user.
    if (useGlu) {
        mv = glm::lookAt(eye,center,up); 
    } else {
        mv = Transform::lookAt(eye,center,up); 
        mv = glm::transpose(mv); // accounting for row major
    }

    glLoadMatrixf(&mv[0][0]);

    // glColor3f(0.7, 0.7, 0);
    // glutSolidTeapot(2);


    // Lights are transformed by current modelview matrix. 
    // The shader can't do this globally. 
    // So we need to do so manually.  
    if (numused) {
        glUniform1i(enablelighting,true);

	glUniform1i(numusedcol, numused);

	for (int i=0; i<numused; i++) {
		GLfloat temp[4];
		GLfloat out[4];

		temp[0] = lightposn[4*i];
		temp[1] = lightposn[4*i+1]; 
		temp[2] = lightposn[4*i+2];
		temp[3] = lightposn[4*i+3];

		transformvec(temp,out);

		lightransf[4*i] = out[0];
		lightransf[4*i+1] = out[1];
		lightransf[4*i+2] = out[2];
		lightransf[4*i+3] = out[3];
	}
	glUniform4fv(lightpos,numLights,lightransf);
	glUniform4fv(lightcol,numLights,lightcolor);


        // YOUR CODE FOR HW 2 HERE.  
        // You need to pass the light positions and colors to the shader. 
        // glUniform4fv() and similar functions will be useful. See FAQ for help with these functions.
        // The lightransf[] array in variables.h and transformvec() might also be useful here.
        // Remember that light positions must be transformed by modelview.  


    } else {
        glUniform1i(enablelighting,false); 
    }

    // Transformations for objects, involving translation and scaling 
    mat4 sc(1.0) , tr(1.0), transf(1.0); 
    sc = Transform::scale(sx,sy,1.0); 
    tr = Transform::translate(tx,ty,0.0);

    transf = tr * sc * mv;

    // YOUR CODE FOR HW 2 HERE.  
    // You need to use scale, translate and modelview to 
    // set up the net transformation matrix for the objects.  
    // Account for GLM issues, matrix order (!!), etc.  

    // TODO: comment that out
    glLoadMatrixf(&transf[0][0]); 

    for (int i = 0 ; i < numobjects ; i++) {
        object* obj = &(objects[i]); // Grabs an object struct.

        // YOUR CODE FOR HW 2 HERE. 
        // Set up the object transformations 
        // And pass in the appropriate material properties
        // Again glUniform() related functions will be useful

	glUniform4fv(ambientcol, 1, obj->ambient);
	glUniform4fv(diffusecol, 1, obj->diffuse);
	glUniform4fv(specularcol, 1, obj->specular);
	glUniform4fv(emissioncol, 1, obj->emission);

	glUniform1fv(shininesscol, 1, &obj->shininess);

	transf = mv * glm::transpose(tr) * glm::transpose(sc) * glm::transpose(obj->transform);
	// glPushMatrix();
	// mat4 ot = obj->transform;

	glLoadMatrixf(&transf[0][0]); 
	// glMultMatrixf(&ot[0][0]);

        // Actually draw the object
        // We provide the actual glut drawing functions for you.  
        // Remember that obj->type is notation for accessing struct fields
        if (obj->type == cube) {
            glutSolidCube(obj->size); 
        }
        else if (obj->type == sphere) {
            const int tessel = 20; 
            glutSolidSphere(obj->size, tessel, tessel); 
        }
        else if (obj->type == teapot) {
            glutSolidTeapot(obj->size); 
        }

	// glPopMatrix();

    }

    glutSwapBuffers();
}