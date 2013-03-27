#include <iostream>
#include "raytrace.h"
#include "parse.h"
//#include <glm/glm.hpp>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include "CImg.h"
#include "accel.h"
#include "Box.h"


#define NO_AA 0
#define AA_2X 1
#define EDGE_AA 3

using namespace std;
using namespace cimg_library;

SceneProperties scene;


bool debugParsing = false;
bool debugSingleRay = false;
bool useAccelStructure = false;

int singleRayX;
int singleRayY;

bool noSpecular = false;
bool noDiffuse = false;
bool noEmission = false;
bool noAmbient = false;
bool reflectRays = true;
bool antiAlias = false;
bool edgeDetection = false;
bool redEdgeDetection = false;
bool edgeAntiAlias = false;

float edgeAAThreshold;
float sobelThreshold;

void printHelp() {
    cout << "Usage\n";
    cout << "-x debug flag; must be used with an argument in the form XX,YY where XX and YY are the coordinates of a pixel to debug\n";
    cout << "-y debug parsing flag\n";
    cout << "-p enable parallelized ray tracing\n";
    cout << "-h print this help message\n";
    cout << "-s do not display specular components to lighting\n";
    cout << "-d do not display diffuse components to lighting\n";
    cout << "-e do not display emission components to lighting\n";
    cout << "-a do not display ambient components to lighting\n";
    cout << "-r disable reflectance\n";
    cout << "-l perform 2x anti aliasing\n";
    cout << "-j in addition to rendering an image, detect edges using the Sobel operator\n";
    cout << "-k n in addition to rendering an image, outline edges in red (using the Sobel operator)\n";
    cout << "-f use an acceleration structure when inserting objects into the scene (f for FAST!! :D)\n";
    cout << "-m n perform anti aliasing only on pixels near an edge (with Sobel threshold n) \n";
    exit(0);
}


CImg<float> detectEdges(CImg<float> origImage,float width, float height, float threshold) {
    CImg<float> grayScale(origImage);

    //first convert to greyscale
    cimg_forXY(origImage, i,j ) {
        float r = *(origImage.data(i,j,0,0));
        float g = *(origImage.data(i,j,0,1));
        float b = *(origImage.data(i,j,0,2));
        float intensity = 0.30*r + 0.59*g + 0.11*b;
        const float grey[] = {intensity, intensity, intensity};
        grayScale.draw_point(i,j,grey);
     }

     //then compute edges
     //cf http://blog.saush.com/2011/04/20/edge-detection-with-the-sobel-operator-in-ruby/
     CImg<float> edgeImage(grayScale);
     cimg_forXY(edgeImage, i, j) {

     float sobel;  
     //ignore edge pixels
     if (i == 0 || i == width-1 || j == 0 || j == height-1) 
         sobel = 1.0;

     else {
         float p1 = *(grayScale.data(i-1,j-1,0,1));
         float p2 = *(grayScale.data(i,j-1,0,1));
         float p3 = *(grayScale.data(i+1,j-1,0,1));

         float p4 = *(grayScale.data(i-1,j,0,1));
         float p6 = *(grayScale.data(i+1,j,0,1));

         float p7 = *(grayScale.data(i-1,j+1,0,1));
         float p8 = *(grayScale.data(i,j+1,0,1));
         float p9 = *(grayScale.data(i+1,j+1,0,1));

         //x filter
         float sobelX = p1*-1 + p3 + p4*-2 + p6*2 + p7*-1 + p9;

         //y filter
         float sobelY = p1 + p2*2 + p3 + p7*-1 + p8*-2 + p9*-1;

         sobel = sqrt(sobelX * sobelX + sobelY * sobelY);
     }

     float sobelColor[] = {0.0,0.0,0.0};

     //threshold being negative implies we dont' do red coloring
     //of edges
     if (threshold > 0.0) {
          if (sobel > threshold)  {
              sobelColor[0] = 1.0; sobelColor[1] = 0.0; sobelColor[2] = 0.0;
          }
          else {
              sobelColor[0] = 0.0; sobelColor[1] = 0.0; sobelColor[2] = 0.0;
          }
      }
      else {
          sobelColor[0] = sobel;
          sobelColor[1] = sobel;
          sobelColor[2] = sobel;
      }

      edgeImage.draw_point(i,j,sobelColor);
    }
     return edgeImage;
}


CImg<float> renderScene(CImgDisplay * renderDisp, SceneProperties scene, int AA, float AAthreshold, bool parallel) {

    const int updateDisplay = (scene.width*scene.height) / 50;

    CImg<float> toBeRendered(scene.width,scene.height,1,3,0);
    int counter = 0;

    if (AA == NO_AA) { //no AA

        cimg_forXY(toBeRendered, i, j) {
           glm::vec3 oc = traceRay((float) i,(float) j);
           float colores[] = {oc.r,oc.g,oc.b};
           toBeRendered.draw_point(i,j,colores);
           if (counter % updateDisplay == 0)
            toBeRendered.display(*renderDisp);
           counter++;
        }
        cout << "Rendered " << counter << " pixels\n";
    }

    else if (AA == AA_2X) { //basic 2x AA
        cout << "Going to anti-alias rendered image\n";
        cimg_forXY(toBeRendered, i, j) {
            glm::vec3 p1 = traceRay(i-0.25,j);
            glm::vec3 p2 = traceRay(i+0.25,j);
            glm::vec3 p3 = traceRay(i,j-0.25);
            glm::vec3 p4 = traceRay(i,j+0.25);

            glm::vec3 oc = (p1 + p2 + p3 + p4) * 0.25f;
            float colores[] = {oc.r,oc.g,oc.b};
            toBeRendered.draw_point(i,j,colores);

            if (counter % updateDisplay*4 == 0)
                toBeRendered.display(*renderDisp);

            counter += 4;
        }
        cout << "Rendered " << counter << " pixels\n";
    }

    else if (AA == EDGE_AA) { //edge only AA
        cout << "Performing 2x anti aliasing only on edge pixels\n";
        //first render the scene normally without AA
        toBeRendered = renderScene(renderDisp, scene, 0, 0, parallel);

        //then detect edges on that scene
        CImg<float>edge = detectEdges(toBeRendered, scene.width, scene.height, AAthreshold);

        int counter = 0;
        cimg_forXY(edge, i,j) {
            if ( *(edge.data(i,j,0,0)) ==  1.0) {
             glm::vec3 p1 = traceRay(i-0.25,j);
             glm::vec3 p2 = traceRay(i+0.25,j);
             glm::vec3 p3 = traceRay(i,j-0.25);
             glm::vec3 p4 = traceRay(i,j+0.25);

             glm::vec3 oc = (p1 + p2 + p3 + p4) * 0.25f;
             float colores[] = {oc.r,oc.g,oc.b};
             toBeRendered.draw_point(i,j,colores);

            if (counter % updateDisplay == 0)
                toBeRendered.display(*renderDisp);

             counter += 4;
            }
        }
        cout << "Rerendered " << counter << " pixels in edge-only anti aliasing\n";
    }

    return toBeRendered;
}



int main(int argc, char ** argv) {

    bool parallelize = false;

    //parse command line options
    int opt;
    while ((opt = getopt(argc, argv, "x:yphdsaerljk:fm:")) != -1) {
        switch (opt) {
            case 'y':
                debugParsing = true;
                break;
            case 'x':
                debugSingleRay=true;
                singleRayX = atoi(optarg);
                singleRayY = atoi(strchr(optarg, ',')+1);
                cout << singleRayX << " " << singleRayY << "\n";
                break;
            case 'p':
                parallelize = true;
                break;
            case 'h':
                printHelp();
                break;
            case 's':
                noSpecular = true;
                break;
            case 'd':
                noDiffuse = true;
                break;
            case 'e':
                noEmission = true;
                break;
            case 'a':
                noAmbient = true;
                break;
            case 'r':
                reflectRays = false;;
                break;
            case 'l':
                antiAlias = true;
                break;
            case 'k':
                redEdgeDetection = true;
                sobelThreshold = atof(optarg);
            case 'j':
                edgeDetection = true;
                break;

            case 'f':
                useAccelStructure = true;
                break;
            case 'm':
                edgeAAThreshold = atof(optarg);
                edgeAntiAlias = true;
                break;
        }
    }

    //if accelerating, init acceleration structure
    if (useAccelStructure) {
        cout << "Initializing acceleration structure\n";

        vector<SceneObject*> objPointer;
        for (vector<SceneObject>::iterator obj = scene.objects.begin(); obj != scene.objects.end(); obj++) {
            objPointer.push_back(&(*(obj)));
        }

        scene.accelStructure = new Box(&objPointer);
    }

    cout << "Starting raytracer\n";

    if (argc < 2) {
        cout << "No scene file specified, terminating.\n";
        return 1;
    }
    else {
        cout << "Opening scene file: " << argv[optind] << "\n";
        scene = parse(argv[optind]);
    }


    if (noSpecular) 
        cout << "Not displaying specular component of lighting\n";
    if (noDiffuse) 
        cout << "Not displaying diffuse component of lighting\n";
    if (noEmission)
        cout << "Not displaying emission component of lighting\n";
    if (noAmbient)
        cout << "Not displaying ambient component of lighting\n";

    if (reflectRays) 
        cout << "Reflecting is enabled\n";
    else 
        cout << "Reflecting is disabled\n";

    cout << "Beginning scene generation, going to write to " << scene.output << "\n";

    CImg<float> renderedImage(scene.width,scene.height,1,3,0);
    CImgDisplay mainDisplay(renderedImage, "Hey look at us we raytraced an image! Awesome! :D");
    mainDisplay.show();

    if (parallelize) 
        cout << "Sorry parallelization not implemented yet\n";

    //render with anti aliasing
    if (antiAlias) 
        renderedImage = renderScene(&mainDisplay, scene, AA_2X, 0, false);

    //render with edge anti aliasing
    else if (edgeAntiAlias)
        renderedImage = renderScene(&mainDisplay, scene,EDGE_AA, edgeAAThreshold, false);

    //render without anti aliasing
    else
        renderedImage = renderScene(&mainDisplay, scene, NO_AA, 0, false);
    
    if (edgeDetection) {

        CImg<float> edge;
        if (redEdgeDetection) 
            edge = detectEdges(renderedImage, scene.width, scene.height, sobelThreshold);

        else
            edge = detectEdges(renderedImage, scene.width, scene.height, -1.0f);

        edge.normalize(0,255);
        edge.save("edgeDetection.jpg");
        CImgDisplay edgesDisplay(edge, "Here are some detected edges!");
        while (!edgesDisplay.is_closed()) {
            edgesDisplay.wait();
            if (edgesDisplay.is_keyESC()) {
                break;
            }
        }
    }


    if (scene.output.size() > 0) {
        renderedImage.normalize(0,255);
        renderedImage.save(scene.output.c_str());
        cout << "Finished writing file " << scene.output << ", terminating\n";
    }
    else {
        renderedImage.save("default.tga", -1);
        cout << "No output file specified, writing to default.tga \n";
    }

    //mainDisplay main loop
    while (!mainDisplay.is_closed()) {
        mainDisplay.wait();
        if (mainDisplay.button() & 1) { //if left mouse button clicked
            cout << "Mouse: " << mainDisplay.mouse_x() << "," << mainDisplay.mouse_y() << "\n";

            float t;
            SceneObject object  = getObjectAtPixel(mainDisplay.mouse_x(), mainDisplay.mouse_y(), &t);
            if (t > 0.0 && t < maxT) {
                cout << "What you clicked on IS an object!\n";
            }
            else {
                cout << "What you clicked on isn't an object!\n";
            }

        }
        if (mainDisplay.is_keyESC()) {
            break;
        }
    }

    return 0;

}
