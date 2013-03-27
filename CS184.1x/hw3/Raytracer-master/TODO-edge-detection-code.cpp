6a7
> #include <math.h>
8a10
> 
15a18,20
> bool useAccelStructure = false;
> 
> 
24a30,31
> bool edgeDetection = false;
> 
37a45
>     cout << "-j instead of rendering an image, render a depiction of that image's edges\n";
47c55
<     while ((opt = getopt(argc, argv, "x:yphdsaerl")) != -1) {
---
>     while ((opt = getopt(argc, argv, "x:yphdsaerlj")) != -1) {
81a90,92
>             case 'j':
>                 edgeDetection = true;
>                 break;
161c172,188
<     //we might want to apply processing to the image - anti aliasing or edge detection
---
> 
>     //implements Sobel edge detection
>     if (edgeDetection) {
>         const float sobelThreshold = 0.5;
>         
>         CImg<float> grayScale(renderedImage);
> 
>         //first convert to greyscale
>         cimg_forXY(renderedImage, i,j ) {
> 
>             float r = *(renderedImage.data(i,j,0,0));
>             float g = *(renderedImage.data(i,j,0,1));
>             float b = *(renderedImage.data(i,j,0,2));
>             float intensity = 0.30*r + 0.59*g + 0.11*b;
>             const float grey[] = {intensity, intensity, intensity};
>             grayScale.draw_point(i,j,grey);
>         }
163a191,220
>         //then compute edges
>         //cf http://blog.saush.com/2011/04/20/edge-detection-with-the-sobel-operator-in-ruby/
>         CImg<float> edge(grayScale);
>         cimg_forXY(edge, i, j) {
> 
>             float sobel;  
>             //ignore edge pixels
>             if (i == 0 || i == width-1 || j == 0 || j == height-1) {
>                 sobel = 1.0;
>             }
>             else {
>                 float p1 = *(edge.data(i-1,j-1,0,0));
>                 float p2 = *(edge.data(i,j-1,0,0));
>                 float p3 = *(edge.data(i+1,j-1,0,0));
> 
>                 float p4 = *(edge.data(i-1,j,0,0));
>                 float p6 = *(edge.data(i+1,j,0,0));
> 
>                 float p7 = *(edge.data(i-1,j+1,0,0));
>                 float p8 = *(edge.data(i,j+1,0,0));
>                 float p9 = *(edge.data(i+1,j+1,0,0));
> 
>                 sobel = abs((p1 + 2*p2 + p3) - (p7 + 2*p8 + p9)) + abs( (p3 + 2*p6 + p9) - (p1 + 2*p4 + p7));
>             }
>             
>             const float color[] = {1.0,0.0,0.0};
>             if (sobel > sobelThreshold) {
>                 edge.draw_point(i,j,color);
>             }
>         }
164a222,230
>         CImgDisplay edgesDisplay(edge, "Here are some detected edges!");
>         while (!edgesDisplay.is_closed()) {
>             edgesDisplay.wait();
>             if (edgesDisplay.is_keyESC()) {
>                 break;
>             }
>         }
>     }
>     
