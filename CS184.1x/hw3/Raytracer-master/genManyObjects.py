import sys

def genFile(width, height, numRows, numCols):
    """Generates a scenefile with that many spheres of size 
    0.3, spaced evenly in the x-y plane"""
    f = open("accelTest.scenefile", "w")
    f.write("size " + str(width) + " " + str(height) + "\n")
    f.write("maxdepth 6\n")

    f.write("camera 30 0 4  0 0 0  0 0 1 45\n") 
    f.write("ambient 0.2 0.2 0.2\n")
    f.write("directional 0 0 5 0.0 0.2 0.9\n")

    numRows = int(numRows)
    numCols  = int(numCols)
    for i in range(int(-numRows/2), int(numRows/2)):
        for j in range(int(-numCols/2), int(numCols/2)):
            f.write("sphere 0 " + str(i) + " " + str(j) + " 0.2\n")

    f.close()

argv = sys.argv
try:
    genFile(argv[1], argv[2], argv[3], argv[4])
except IndexError:
    print("Didn't specify arguments correctly. You want genManyObjects.py width height numSphereRows numSphereCols")



