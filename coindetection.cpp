#include <cstdlib>
#include <cmath>
#include<iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <list>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <stack>
#include <cstring>
using namespace std;
using namespace std::chrono;

int HEIGHT;
int WIDTH;
int THRESHOLD = 120;
int LT = 50;
int HT = 140;
int VT = 70;
//60, 150, 62 for image 1
//80, 170, 116 for image 2
string GRAYSCALE = "imageg.ppm";
string INPUT = "image.ppm";
string OUTPUT = "image1.ppm";
string NMS = "image2.ppm";
string FINAL = "imagef.ppm";
string COINS = "imageCC.ppm";
string LIGHT = "imagev.ppm";
string TEXT = "results.txt";
string FINALCOINS = "imageCoins.ppm";
string CIRCLES = "imageCircles.ppm";
int TCircle = 50;

int TL = 0;
int TR = 0;
int BL = 0;
int BR = 0;

int rpl = 1000;
int rph = 1000;
int rdl = 1000;
int rdh = 1000;
int rnl = 1000;
int rnh = 1000;
int rql = 1000;
int rqh = 1000;
int rhl = 1000;
int rhh = 1000;

double p = 0;
double q = 0;
double n = 0;

void PPM (int** edges){
    ofstream myfile;
    myfile.open ("imagem.ppm");
    myfile << "P3 " << WIDTH << " " << HEIGHT << " 1" << endl;

    for (int i = 0; i<HEIGHT; i++){
        for (int j = 0; j<WIDTH; j++){
            if (edges[i][j] == 0){
                myfile << "0 0 0 ";
            }
            else{
                myfile << "1 1 1 ";
            }
        }  
        myfile << endl;
    }

}

int** makeArray(int height, int width){
    int** array2D = new int*[height];
    for (int h = 0; h < height; h++)
    {
        array2D[h] = new int[width];
        for (int w = 0; w < width; w++)
        {
            array2D[h][w] = 1;
        }
    }
    return array2D;
}

void illuminate(int x, int y, vector<vector<int>> &vec){
    if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
        vec[x][y] = vec[x][y] + 1;
    }
}

int** illuminate(int x, int y, int** array2D, int mode = 0){
    if(mode == 0){
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
            array2D[x][y] = 1;
        }
    }
    if(mode == 1){
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
            array2D[x][y] = 2;
        }
    }
    if(mode == 2){
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
            array2D[x][y] = 3;
        }
    }
    if(mode == 3){
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
            array2D[x][y] = 4;
        }
    }
    if(mode == 4){
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
            array2D[x][y] = 5;
        }
    }
    if(mode == 5){
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
            array2D[x][y] = 6;
        }
    }
    return array2D;
}

//DA X axis Northeast - checked
void NorthEastX(int x1, int y1, int x2, int y2, vector<vector<int>> &vec){
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int j = y1;
    int epsilon = deltaY - deltaX;

    for (int i = x1; i <= x2-1; ++i){
        illuminate(i,j, vec);
        if (epsilon >= 0){
            j += 1;
            epsilon -= deltaX;
        }
        epsilon += deltaY;
    }
}

//DA X axis Northwest - checked
void NorthWestX(int x1, int y1, int x2, int y2, vector<vector<int>> &vec){
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int j = y1;
    int epsilon = deltaY + deltaX;

    for (int i = x1; i >= x2-1; --i){
        illuminate(i,j, vec);
        if (epsilon >= 0){
            j += 1;
            epsilon += deltaX;
        }
        epsilon += deltaY;
    }
}

//DA X axis East
void East(int x1, int y1, int x2, int y2, vector<vector<int>> &vec){
    for (int i = x1; i<=x2; ++i){
        illuminate(i,y1, vec);
    }
}

//DA Y axis NorthEast - checked
void NorthEastY(int x1, int y1, int x2, int y2, vector<vector<int>> &vec){
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int i = x1;
    int epsilon = deltaX - deltaY;

    for (int j = y1; j <= y2; ++j){
        illuminate(i,j, vec);
        if (epsilon >= 0){
            i += 1;
            epsilon -= deltaY;
        }
        epsilon += deltaX;
    }
}

//DA Y axis NorthWest
void NorthWestY(int x1, int y1, int x2, int y2, vector<vector<int>> &vec){
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int i = x1;
    int epsilon = deltaX - deltaY;

    for (int j = y1; j >= y2; --j){
        illuminate(i,j, vec);
        if (epsilon >= 0){
            i += 1;
            epsilon += deltaY;
        }
        epsilon += deltaX;
    }
}

//DA Y axis North
void North(int x1, int y1, int x2,  int y2, vector<vector<int>> &vec){
    for (int j = y1; j<=y2; ++j){
        illuminate(x1,j, vec);
    }
}

void LineType(int x1, int y1, int x2, int y2, vector<vector<int>> &vec){
    int dx = x2-x1; //37
    int dy = y2 - y1; //-169
    if (dx > 0 && dy > 0 && dx > dy){
        NorthEastX(x1, y1, x2, y2, vec);
    }
    else if (dx < 0 && dy < 0 && abs(dx) > abs(dy)){
        NorthEastX(x2, y2, x1, y1, vec);
    }
    else if (dx < 0 && dy > 0 && abs(dx) > abs(dy)){
        NorthWestX(x1, y1, x2, y2, vec);
    }
    else if (dx > 0 && dy < 0 && abs(dx) > abs(dy)){
        NorthWestX(x2, y2, x1, y1, vec);
    }
    else if (dx > 0 && dy == 0){
        East(x1, y1, x2, y2, vec);
    }
    else if (dx < 0 && dy == 0){
        East(x2, y2, x1, y1, vec);
    }
    else if (dx > 0 && dy > 0 && dx < dy){
        NorthEastY(x1, y1, x2, y2, vec);
    }
    else if (dx < 0 && dy < 0 && abs(dx) < abs(dy)){
        NorthEastY(x2, y2, x1, y1, vec);
    }
    else if (dx < 0 && dy > 0 && abs(dx) < abs(dy)){
        NorthWestY(x2, y2, x1, y1, vec);
    }
    else if (dx > 0 && dy < 0 && abs(dx) < abs(dy)){
        NorthWestY(x1, y1, x2, y2, vec);
    }
    else if (dx == 0 && dy > 0){
        North(x1, y1, x2, y2, vec);
    }
    else{
        North(x2, y2, x1, y1, vec);
    }
}

int** DrawCircle (int cx, int cy, int radius, int** array2D, int mode = 0){
    int x;
    int y;
    int xmax;
    int y2;
    int y2_new;
    int ty;
    xmax = (int) (radius * 0.70710678); 
    y = radius;
    y2 = y * y;
    ty = (2 * y) - 1; 
    y2_new = y2;
    for (x = 0; x <= xmax+1; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        array2D = illuminate(x+cx, y+cy, array2D, mode);
        array2D = illuminate(x+cx, -y+cy, array2D, mode);
        array2D = illuminate(-x+cx, y+cy, array2D, mode);
        array2D = illuminate(-x+cx, -y+cy, array2D, mode);
        array2D = illuminate(y+cx, x+cy, array2D, mode);
        array2D = illuminate(y+cx, -x+cy, array2D, mode);
        array2D = illuminate(-y+cx, x+cy, array2D, mode);
        array2D = illuminate(-y+cx, -x+cy, array2D, mode);
        y2_new -= (2 * x) - 3;
    }
    return array2D;
}

int** makeIntensity(){
    ifstream infile;
    infile.open(INPUT);
    string magic_num;
    int width, height, max_color;
    infile >> magic_num >> width >> height >> max_color;

    HEIGHT = height;
    WIDTH = width;

    int** intensityMatrix = makeArray(height, width);

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            int red, green, blue;
            infile >> red >> green >> blue;
            //cout << red << " " << green << " " << blue << endl;
            int intensity = (red + green + blue) / 3;
            intensityMatrix[i][j] = intensity;
        }
    }
    return intensityMatrix;
}

void makeGrayScale(int** array2D){
    ofstream myfile;
    myfile.open (GRAYSCALE);
    myfile << "P3 " << WIDTH << " " << HEIGHT << " 255" << endl;

    for (int i = 0; i<HEIGHT; i++){
        for (int j = 0; j<WIDTH; j++){
            string n = std::to_string(array2D[i][j]);
            string rgb = n + " " + n + " " + n + " ";
            myfile << rgb;
        }  
        myfile << endl;
    }   
}

double** createDx(int x[3][3], double** intensity){
    double** dx = new double*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        dx[h] = new double[WIDTH];
    }
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH-1){
                dx[i][j] = 0;
            }
            else{
                double temp = intensity[i-1][j-1] * x[0][0] + intensity[i-1][j] * x[0][1] + intensity[i-1][j+1] * x[0][2]
                + intensity[i][j-1] * x[1][0] + intensity[i][j] * x[1][1] + intensity[i][j+1] * x[1][2]
                + intensity[i+1][j-1] * x[2][0] + intensity[i+1][j] * x[2][1] + intensity[i+1][j+1] * x[2][2];

                dx[i][j] = temp;
            }
        }
    }
    return dx;
}

double** createDy(int y[3][3], double** intensity){
    double** dy = new double*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        dy[h] = new double[WIDTH];
    }
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH-1){
                dy[i][j] = 0;
            }
            else{
                double temp = intensity[i-1][j-1] * y[0][0] + intensity[i-1][j] * y[0][1] + intensity[i-1][j+1] * y[0][2]
                + intensity[i][j-1] * y[1][0] + intensity[i][j] * y[1][1] + intensity[i][j+1] * y[1][2]
                + intensity[i+1][j-1] * y[2][0] + intensity[i+1][j] * y[2][1] + intensity[i+1][j+1] * y[2][2];

                dy[i][j] = temp;
            }
        }
    }
    return dy;
}

double** createMagnitude(double** dx, double** dy){
    double** mag = new double*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        mag[h] = new double[WIDTH];
    }

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            double temp = sqrt(dx[i][j] * dx[i][j] + dy[i][j] * dy[i][j]);
            mag[i][j] = temp;
        }
    }
    return mag;
}

int** findEdges(double** magnitude){
    int** edge = new int*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        edge[h] = new int[WIDTH];
    }

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(magnitude[i][j] >= THRESHOLD){
                edge[i][j] = 1;
            }
            else{
                edge[i][j] = 0;
            }
        }
    }

    return edge;
}

int** twoThresholds(double** magnitude){
    int** edge = new int*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        edge[h] = new int[WIDTH];
    }

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            //top left
            if (j >= 0 && j <= (2 * WIDTH)/3 && i>=0 && i<= HEIGHT/2.5){
                if(magnitude[i][j] > HT+TL){
                    edge[i][j] = 2;
                }
                else if (magnitude[i][j] > LT+TL){
                    edge[i][j] = 1;
                }
                else{
                    edge[i][j] = 0;
                }
            }
            //bottom left
            else if (i > HEIGHT/2 && i <= HEIGHT && j >= 0 && j<= WIDTH/2){
                if(magnitude[i][j] > HT+BL){
                    edge[i][j] = 2;
                }
                else if (magnitude[i][j] > LT+BL){
                    edge[i][j] = 1;
                }
                else{
                    edge[i][j] = 0;
                }
            }
            //bottom right
            else if(i >= (2 * HEIGHT)/3 && i <= HEIGHT && j>=WIDTH/2 && j<= WIDTH){
                if(magnitude[i][j] > HT+BR){
                    edge[i][j] = 2;
                }
                else if (magnitude[i][j] > LT+BR){
                    edge[i][j] = 1;
                }
                else{
                    edge[i][j] = 0;
                }
            }
            else{
                if(magnitude[i][j] > HT + TR){
                    edge[i][j] = 2;
                }
                else if (magnitude[i][j] > LT + TR){
                    edge[i][j] = 1;
                }
                else{
                    edge[i][j] = 0;
                }
            }
        }
    }

    return edge;
}

void hysthereses(int** &edges, int x, int y){
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH){
        return;
    }
    else if (edges[x][y] == 2 || edges[x][y] == 1){
        edges[x][y] = 3;
        hysthereses(edges, x+1, y);
        hysthereses(edges, x-1, y);
        hysthereses(edges, x, y+1);
        hysthereses(edges, x, y-1);
        hysthereses(edges, x+1, y+1);
        hysthereses(edges, x-1, y-1);
        hysthereses(edges, x+1, y-1);
        hysthereses(edges, x-1, y+1);
    }
    else{
        return;
    }
 
}

int** find2(int** edges){
    //coordinates with value 2
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(edges[i][j] == 2){
                hysthereses(edges, i, j);
            }
        }
    }
    return edges;
}

void PPM2 (int** edges){
    ofstream myfile;
    myfile.open (OUTPUT);
    myfile << "P3 " << WIDTH << " " << HEIGHT << " 1" << endl;

    for (int i = 0; i<HEIGHT; i++){
        for (int j = 0; j<WIDTH; j++){
            if (edges[i][j] == 3){
                myfile << "1 1 1 ";
            }
            else{
                myfile << "0 0 0 ";
            }
        }  
        myfile << endl;
    }

}

int** findAngles(double** dx, double** dy){
    int** angle = new int*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        angle[h] = new int[WIDTH];
    }

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            angle[i][j] = (atan2(dx[i][j], -1*dy[i][j]))*57.2958;
        }
    }

    return angle;
}

int** angleToDirection(int** angle){
    int** directionAngle = new int*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        directionAngle[h] = new int[WIDTH];
    }
    vector<int> pixelAngles = {-180, -135, -90, -45, 0, 45, 90, 135, 180};
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            int closest = -180;
            for (auto & num : pixelAngles){
                if(abs(num - angle[i][j]) < abs(closest - angle[i][j])){
                    closest = num;
                }
            }
            directionAngle[i][j] = closest;
        }
    }
    return directionAngle;
}

void gradient(int** &angle, double** magnitude){
    for(int i = 1; i < HEIGHT; i++){
        for(int j = 1; j < WIDTH; j++){
            //compare to side
            if(angle[i][j] == 180 || angle[i][j] == 0 || angle[i][j] == -180){
                if(magnitude[i][j] >= magnitude[i][j-1] || magnitude[i][j] >= magnitude[i][j+1]){
                    angle[i][j] = 1;
                }
                else{
                    angle[i][j] = 0;
                }
            }
            //compare up down
            else if(angle[i][j] == 90 ||  angle[i][j] == -90){
                if(magnitude[i][j] >= magnitude[i-1][j] || magnitude[i][j] >= magnitude[i+1][j]){
                    angle[i][j] = 1;
                }
                else{
                    angle[i][j] = 0;
                }
            }
            //compare y=x
            else if(angle[i][j] == 45 || angle[i][j] == -135){
                if(magnitude[i][j] >= magnitude[i+1][j+1] || magnitude[i][j] >= magnitude[i-1][j-1]){
                    angle[i][j] = 1;
                }
                else{
                    angle[i][j] = 0;
                }
            }
            //compare y=-x
            else if(angle[i][j] == -45 || angle[i][j] == 135){
                if(magnitude[i][j] >= magnitude[i+1][j+1] || magnitude[i][j] >= magnitude[i-1][j-1]){
                    angle[i][j] = 1;
                }
                else{
                    angle[i][j] = 0;
                }
            }

        }
    }
}

void PPM3 (int** angle){
    ofstream myfile;
    myfile.open (NMS);
    myfile << "P3 " << WIDTH << " " << HEIGHT << " 1" << endl;

    for (int i = 0; i<HEIGHT; i++){
        for (int j = 0; j<WIDTH; j++){
            if (angle[i][j] == 1){
                myfile << "1 1 1 ";
            }
            else{
                myfile << "0 0 0 ";
            }
        }  
        myfile << endl;
    }

}

int** DTandNMS(int** edges, int** angles){
    int** final = new int*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        final[h] = new int[WIDTH];
    }

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(edges[i][j] == 3 && angles[i][j] == 1){
                final[i][j] = 1;
            }
            else{
                final[i][j] = 0;
            }
        }
    }
    return final;
}

void PPM4 (int** final){
    ofstream myfile;
    myfile.open (FINAL);
    myfile << "P3 " << WIDTH << " " << HEIGHT << " 1" << endl;

    for (int i = 0; i<HEIGHT; i++){
        for (int j = 0; j<WIDTH; j++){
            if (final[i][j] == 1){
                myfile << "1 1 1 ";
            }
            else{
                myfile << "0 0 0 ";
            }
        }  
        myfile << endl;
    }
}

class ExtendedLine{
    private:
        double angle;
        int x;
        int y;
        double slope;
        double intercept;
        int ix;
        int iy;
        int fx;
        int fy;
    public:
        ExtendedLine(double a, int xcoor, int ycoor, double**dx, double**dy){
            angle = a;
            x = xcoor;
            y = ycoor;
            createLine(dx, dy);
        }

        void createLine(double** dx, double** dy){
            
            //slope = tan(angle);
            if(dy[x][y] == 0){
                slope = dx[x][y] / .000000000000000001;
            }
            else{
                slope = dx[x][y] / (dy[x][y]);
            }
            intercept = y - (slope) * x;
            if(slope == 0){
                slope = .00000000000000000000000001;
            }
            if(slope == INFINITY){
                slope = 100000000000000000;
            }
            //---10% box optimization---
            iy = y - .03 * WIDTH;
            fy = y + .03 * WIDTH;
            ix = (iy - intercept) / slope;
            fx = (fy - intercept) / slope;
        }

        void fillVotes(vector<vector<int>> &vec){
            LineType(ix, iy, fx, fy, vec);
        }
};

void PPMgrayscale (vector<vector<int>> &vec){
    int max = 0;
    for (int i = 0; i <HEIGHT; i++){
        int temp = *max_element(vec[i].begin(), vec[i].end());
        if(temp > max){
            max = temp;
        }
    }
    ofstream myfile;
    myfile.open (LIGHT);
    myfile << "P3 " << WIDTH << " " << HEIGHT << " " << max << endl;

    for (int i = 0; i<HEIGHT; i++){
        for (int j = 0; j<WIDTH; j++){
            string v = to_string(vec[i][j]) + " ";
                myfile << v+v+v;
        }  
        myfile << endl;
    }
}

void Gaussian(double GKernel[][5])
{
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
 
    double sum = 0.0;
 
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            r = sqrt(x * x + y * y);
            GKernel[x + 2][y + 2] = (exp(-(r * r) / s)) / (M_PI * s);
            sum += GKernel[x + 2][y + 2];
        }
    }
 
    for (int i = 0; i < 5; ++i){
        for (int j = 0; j < 5; ++j){
            GKernel[i][j] /= sum;
        }
            
    }
}

double ** applyFilter(double GKernel[5][5], int** votes){
    double** finalVotes = new double*[HEIGHT];;
    for (int h = 0; h < HEIGHT; h++)
    {
        finalVotes[h] = new double[WIDTH];
        for (int w = 0; w < WIDTH; w++)
        {
            finalVotes[h][w] = 0;
        }
    }
    for(int i = 1; i < HEIGHT; i++){
        for(int j = 1; j < WIDTH; j++){
            if(i == 1 || j == 1 || i == HEIGHT - 2 || j == WIDTH-2 || i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH-1){
                finalVotes[i][j] = 0;
            }
            else{
                int temp = votes[i-2][j-2] * GKernel[0][0] + votes[i-2][j-1] * GKernel[0][1] + votes[i-2][j] * GKernel[0][2] + votes[i-2][j+1] * GKernel[0][3] + votes[i-2][j+2] * GKernel[0][4] +
                votes[i-1][j-2] * GKernel[1][0] + votes[i-1][j-1] * GKernel[1][1] + votes[i-1][j] * GKernel[1][2] + votes[i-1][j+1] * GKernel[1][3] + votes[i-1][j+2] * GKernel[1][4] + 
                votes[i][j-2] * GKernel[2][0] + votes[i][j-1] * GKernel[2][1] + votes[i][j] * GKernel[2][2] + votes[i][j+1] * GKernel[2][3] + votes[i][j+2] * GKernel[2][4] +
                votes[i+1][j-2] * GKernel[3][0] + votes[i+1][j-1] * GKernel[3][1] + votes[i+1][j] * GKernel[3][2] + votes[i+1][j+1] * GKernel[3][3] + votes[i+1][j+2] * GKernel[3][4] + 
                votes[i+2][j-2] * GKernel[4][0] + votes[i+2][j-1] * GKernel[4][1] + votes[i+2][j] * GKernel[4][2] + votes[i+2][j+1] * GKernel[4][3] + votes[i+2][j+2] * GKernel[4][4];

                finalVotes[i][j] = temp;
            }
        }
    }
    return finalVotes;
}

int** voteEdges(int** angles, int** finalEdges, double**dx, double**dy){
    vector<vector<int>> vec;
    for (int i = 0; i < HEIGHT; i++) {
        vector<int> v1;
        for (int j = 0; j < WIDTH; j++) {
            v1.push_back(0);
        }
        vec.push_back(v1);
    }

    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH; j++){
            if(finalEdges[i][j] == 1){
                ExtendedLine e (angles[i][j], i, j, dx,dy);
                e.fillVotes(vec);
            }
        }
    }
    
    PPMgrayscale(vec);

    int** array2D = new int*[HEIGHT];;
    for (int h = 0; h < HEIGHT; h++)
    {
        array2D[h] = new int[WIDTH];
        for (int w = 0; w < WIDTH; w++)
        {
            array2D[h][w] = 0;
        }
    }
    
    for(int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            //top left
            if (i >= 0 && i <= HEIGHT/2 && j>=0 && j<= WIDTH/2){
                if(vec[i][j] >= VT){
                    array2D = DrawCircle(i,j,1,array2D);
                    array2D = DrawCircle(i,j,2,array2D);
                    array2D = DrawCircle(i,j,3,array2D);
                    array2D = DrawCircle(i,j,4,array2D);
                }
            }
            //bottom left
            else if (i > HEIGHT/2 && i <= HEIGHT && j >= 0 && j<= WIDTH/2){
                if(vec[i][j] >= VT){
                    array2D = DrawCircle(i,j,1,array2D);
                    array2D = DrawCircle(i,j,2,array2D);
                    array2D = DrawCircle(i,j,3,array2D);
                    array2D = DrawCircle(i,j,4,array2D);
                }
            }
            //top right
            else if (i >= 0 && i <= HEIGHT/2 && j>WIDTH/2 && j<= WIDTH){
                if(vec[i][j] >= VT){
                    array2D = DrawCircle(i,j,1,array2D);
                    array2D = DrawCircle(i,j,2,array2D);
                    array2D = DrawCircle(i,j,3,array2D);
                    array2D = DrawCircle(i,j,4,array2D);
                }
            }
            //bottom right
            else{
                if(vec[i][j] >= VT){
                    array2D = DrawCircle(i,j,1,array2D);
                    array2D = DrawCircle(i,j,2,array2D);
                    array2D = DrawCircle(i,j,3,array2D);
                    array2D = DrawCircle(i,j,4,array2D);
                }
            }
        }
    }

    return array2D;

}

void PPMoverlay(int** votes){
    ifstream infile;
    infile.open(INPUT);
    string magic_num;
    int width, height, max_color;
    infile >> magic_num >> width >> height >> max_color;

    ofstream myfile;
    myfile.open (COINS);
    myfile << "P3 " << WIDTH << " " << HEIGHT << " " << max_color<< endl;
    int red, green, blue;
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(votes[i][j] == 1){
                myfile << "255 0 0 ";
                infile >> red >> green >> blue;
            }
            else{
                
                infile >> red >> green >> blue;
                string write = to_string(red) + " " + to_string(green) + " " + to_string(blue) + " ";
                myfile << write;
            }
        }
    }
}

void PPMoverlay2(int** votes){
    ifstream infile;
    infile.open(INPUT);
    string magic_num;
    int width, height, max_color;
    infile >> magic_num >> width >> height >> max_color;

    ofstream myfile;
    myfile.open (FINALCOINS);
    myfile << "P3 " << WIDTH << " " << HEIGHT << " " << max_color<< endl;
    int red, green, blue;
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(votes[i][j] == 2){
                myfile << "255 0 0 ";
                infile >> red >> green >> blue;
            }
            else if(votes[i][j] == 3){
                 myfile << "255 0 255 ";
                 infile >> red >> green >> blue;
            }
            else if(votes[i][j] == 4){
                 myfile << "255 255 0 ";
                 infile >> red >> green >> blue;
            }
            else if(votes[i][j] == 5){
                 myfile << "0 0 255 ";
                 infile >> red >> green >> blue;
            }
            else if(votes[i][j] == 6){
                 myfile << "0 255 0 ";
                 infile >> red >> green >> blue;
            }
            else{
                
                infile >> red >> green >> blue;
                string write = to_string(red) + " " + to_string(green) + " " + to_string(blue) + " ";
                myfile << write;
            }
        }
    }
}

void PPMoverlayRed(int** votes){
    ifstream infile;
    infile.open(INPUT);
    string magic_num;
    int width, height, max_color;
    infile >> magic_num >> width >> height >> max_color;

    ofstream myfile;
    myfile.open (CIRCLES);
    myfile << "P3 " << WIDTH << " " << HEIGHT << " " << max_color<< endl;
    int red, green, blue;
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(votes[i][j] != 0){
                myfile << "255 0 0 ";
                infile >> red >> green >> blue;
            }
            else{
                
                infile >> red >> green >> blue;
                string write = to_string(red) + " " + to_string(green) + " " + to_string(blue) + " ";
                myfile << write;
            }
        }
    }
}

// void part3(int argc, char **argv){
//     for (int i = 0; i < argc; ++i){
//         if(strcmp(argv[i], "-f") == 0){
//             INPUT = string(argv[i+1]);
//         }
//         else if(strcmp(argv[i], "-lt") == 0){
//             LT = stoi(string(argv[i+1]));
//         }
//         else if (strcmp(argv[i], "-ht") == 0){
//             HT = stoi(string(argv[i+1]));
//         }
//         else if (strcmp(argv[i], "-fg") == 0){
//             GRAYSCALE = string(argv[i+1]);
//         }
//         else if (strcmp(argv[i], "-f1") == 0){
//             OUTPUT = string(argv[i+1]);
//         }
//         else if (strcmp(argv[i], "-f2") == 0){
//             NMS = string(argv[i+1]);
//         }
//         else if (strcmp(argv[i], "-ff") == 0){
//             FINAL = string(argv[i+1]);
//         }
//         else if (strcmp(argv[i], "-TC") == 0){
//             VT = stoi(argv[i+1]);
//         }
//         else if (strcmp(argv[i], "-fv") == 0){
//             LIGHT = string(argv[i+1]);
//         }
//         else if (strcmp(argv[i], "-fcc") == 0){
//             COINS = string(argv[i+1]);
//         }
//     }

//     int** intensity = makeIntensity();
//     makeGrayScale(intensity);
//     int x[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
//     int y[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};
//     int** dx = createDx(x, intensity);
//     int** dy = createDy(y, intensity);
//     int** angles = findAngles(dx, dy);
//     int** magnitude = createMagnitude(dx,dy);
    
//     int** directionAngle = angleToDirection(angles);
//     gradient(directionAngle, magnitude);
//     PPM3(directionAngle);
//     int** edges = twoThresholds(magnitude);
//     edges = find2(edges);
//     PPM2(edges);
//     int** finalEdges = DTandNMS(edges, directionAngle);
//     PPM4(finalEdges);
//     int** votes = voteEdges(angles, finalEdges, dx, dy);
//     PPMoverlay(votes);
// }

void illuminate(int x, int y, int** finalEdges, double &c){
    if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH){
        if(finalEdges[x][y] == 1){
            c = c + 1;
        }
    }
}

int countCircle (int cx, int cy, int radius, int** finalEdges){
    int x;
    int y;
    int xmax;
    int y2;
    int y2_new;
    int ty;
    xmax = (int) (radius * 0.70710678); 
    y = radius;
    y2 = y * y;
    ty = (2 * y) - 1; 
    y2_new = y2;
    double c = 0;
    double total = 0;
    for (x = 0; x <= xmax+1; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        illuminate(x+cx, y+cy, finalEdges, c);
        illuminate(x+cx, -y+cy, finalEdges, c);
        illuminate(-x+cx, y+cy, finalEdges, c);
        illuminate(-x+cx, -y+cy, finalEdges, c);
        illuminate(y+cx, x+cy, finalEdges, c);
        illuminate(y+cx, -x+cy, finalEdges, c);
        illuminate(-y+cx, x+cy, finalEdges, c);
        illuminate(-y+cx, -x+cy, finalEdges, c);
        y2_new -= (2 * x) - 3;
        total = total + 8;
    }
    double percent = (c / total)*100;
    return int(percent);
}

double calcDistance(int ix, int iy, int fx, int fy){
    double d = sqrt((fx - ix) * (fx - ix) + (fy - iy) * (fy - iy));
    return d;
}


int** getCoins(vector<vector<int>> coinCenters, int** finalEdges){
    int** array2D = new int*[HEIGHT];
    for (int h = 0; h < HEIGHT; h++)
    {
        array2D[h] = new int[WIDTH];
        for (int w = 0; w < WIDTH; w++)
        {
            array2D[h][w] = 0;
        }
    }
    int minRadius = .02*WIDTH;
    int maxRadius = .06*WIDTH;
    int highest = 0;
    int radius = 0;
    vector<int> center{0,0};
    vector<vector<int>> finalp;
    vector<vector<int>> finalq;
    vector<vector<int>> finaln;
    vector<vector<int>> finald;
    vector<vector<int>> finalh;
    for (int i = 0; unsigned(i) < coinCenters.size(); i++){
        int* count = new int[maxRadius - minRadius + 1];
        for(int r = minRadius; r < maxRadius+1; r++){
            count[r-minRadius] = countCircle(coinCenters[i][0], coinCenters[i][1], r, finalEdges);
            if (count[r-minRadius] > highest){
                highest = count[r-minRadius];
                radius = r;
                center[0] = coinCenters[i][0];
                center[1] = coinCenters[i][1];
            }
        }

        if(i > 0){
            if (calcDistance(coinCenters[i-1][0], coinCenters[i-1][1], coinCenters[i][0], coinCenters[i][1]) > 20){
                if(highest > TCircle){
                    //penny
                    if (radius > rpl && radius <= rph){
                        finalp.push_back({center[0], center[1], radius, highest});
                    }
                    //quarter
                    else if (radius >= rql  && radius < rqh){
                       finalq.push_back({center[0], center[1], radius, highest});
                    }
                    //nickel
                    else if (radius >= rnl && radius <= rnh){
                       finaln.push_back({center[0], center[1], radius, highest});
                    }
                    //dime
                    else if (radius >= rdl && radius <= rdh){
                       finald.push_back({center[0], center[1], radius, highest});
                    }
                    //half dollar
                    else if (radius >= rhl && radius <= rhh){
                       finalh.push_back({center[0], center[1], radius, highest});
                    }
                }
                highest = 0;
                radius = 0;
                center[0] = 0;
                center[1] = 0;
            }
        }

    }
    
    for (int i = 0; unsigned(i) < finalp.size(); i++) {
        for (int j = i + 1; unsigned(j) < finalp.size(); j++) {
            double distance = 0.0;
            distance = sqrt(pow(finalp[i][0] - finalp[j][0], 2) + pow(finalp[i][1] - finalp[j][1], 2));
            double radius = abs(finalp[j][2] - finalp[i][2]);

            if (radius < p*WIDTH && distance < p*WIDTH) {
                finalp.erase(finalp.begin() + j);
                j--; // adjust j to account for the removed element
            }   
        }
    }

    for (int i = 0; unsigned(i) < finalq.size(); i++) {
        for (int j = i + 1; unsigned(j) < finalq.size(); j++) {
            double distance = 0.0;
            distance = sqrt(pow(finalq[i][0] - finalq[j][0], 2) + pow(finalq[i][1] - finalq[j][1], 2));
            double radius = abs(finalq[j][2] - finalq[i][2]);

            if (radius < q*WIDTH && distance < q*WIDTH) {
                    finalq.erase(finalq.begin() + j);
                    j--;
            }   
        }
    }

    for (int i = 0; unsigned(i) < finaln.size(); i++) {
        for (int j = i + 1; unsigned(j) < finaln.size(); j++) {
            double distance = 0.0;
            distance = sqrt(pow(finaln[i][0] - finaln[j][0], 2) + pow(finaln[i][1] - finaln[j][1], 2));
            double radius = abs(finaln[j][2] - finaln[i][2]);

            if (radius < n*WIDTH && distance < n*WIDTH) {
                finaln.erase(finaln.begin() + j);
                j--; // adjust j to account for the removed element
            }   
        }
    }

    for (int i = 0; unsigned(i) < finald.size(); i++) {
        for (int j = i + 1; unsigned(j) < finald.size(); j++) {
            double distance = 0.0;
            distance = sqrt(pow(finald[i][0] - finald[j][0], 2) + pow(finald[i][1] - finald[j][1], 2));
            double radius = abs(finald[j][2] - finald[i][2]);

            if (radius < .035*WIDTH && distance < .035*WIDTH) {
                finald.erase(finald.begin() + j);
                j--; // adjust j to account for the removed element
            }   
        }
    }

    for (int i = 0; unsigned(i) < finalh.size(); i++) {
        for (int j = i + 1; unsigned(j) < finalh.size(); j++) {
            double distance = 0.0;
            distance = sqrt(pow(finalh[i][0] - finalh[j][0], 2) + pow(finalh[i][1] - finalh[j][1], 2));
            double radius = abs(finalh[j][2] - finalh[i][2]);

            if (radius < .04*WIDTH && distance < .1*WIDTH) {
                finalh.erase(finalh.begin() + j);
                j--; // adjust j to account for the removed element
            }   
        }
    }

    for(int i = 0; unsigned(i) < finald.size(); i++){
        for(int j = 0; unsigned(j) < finalp.size(); j++){
            double distance = sqrt(pow(finald[i][0] - finalp[j][0], 2) + pow(finald[i][1] - finalp[j][1], 2));
            if (distance < .04*WIDTH) {
                finald.erase(finald.begin() + i);
            } 
        }
    }

    for(int i = 0; unsigned(i) < finalp.size(); i++){
        for(int j = 0; unsigned(j) < finaln.size(); j++){
            double distance = sqrt(pow(finalp[i][0] - finaln[j][0], 2) + pow(finalp[i][1] - finaln[j][1], 2));
            if (distance < .03*WIDTH) {
                finalp.erase(finalp.begin() + i);
            } 
        }
    }

    for(int i = 0; unsigned(i) < finaln.size(); i++){
        for(int j = 0; unsigned(j) < finalq.size(); j++){
            double distance = sqrt(pow(finaln[i][0] - finalq[j][0], 2) + pow(finaln[i][1] - finalq[j][1], 2));
            if (distance < .03*WIDTH) {
                finaln.erase(finaln.begin() + i);
            } 
        }
    }

    for(int i = 0; unsigned(i) < finaln.size(); i++){
        for(int j = 0; unsigned(j) < finalh.size(); j++){
            double distance = sqrt(pow(finaln[i][0] - finalh[j][0], 2) + pow(finaln[i][1] - finalh[j][1], 2));
            if (distance < .04*WIDTH) {
                finaln.erase(finaln.begin() + i);
            } 
        }
    }


    int pennies = 0;
    int quarters = 0;
    int nickels = 0;
    int dimes = 0;
    int halfDollar = 0;
    for(int i = 0; unsigned(i)<finalp.size(); i++){
        array2D = DrawCircle(finalp[i][0], finalp[i][1], finalp[i][2] - 1, array2D, 1);
        array2D = DrawCircle(finalp[i][0], finalp[i][1], finalp[i][2], array2D, 1);
        array2D = DrawCircle(finalp[i][0], finalp[i][1], finalp[i][2] + 1, array2D, 1);
        array2D = DrawCircle(finalp[i][0], finalp[i][1], 2, array2D, 1);
        array2D = DrawCircle(finalp[i][0], finalp[i][1], 3, array2D, 1);
        array2D = DrawCircle(finalp[i][0], finalp[i][1], 4, array2D, 1);
        pennies = pennies + 1;
    }

    for(int i = 0; unsigned(i)<finalq.size(); i++){
        array2D = DrawCircle(finalq[i][0], finalq[i][1], finalq[i][2] - 1, array2D, 2);
        array2D = DrawCircle(finalq[i][0], finalq[i][1], finalq[i][2], array2D, 2);
        array2D = DrawCircle(finalq[i][0], finalq[i][1], finalq[i][2] + 1, array2D, 2);
        array2D = DrawCircle(finalq[i][0], finalq[i][1], 2, array2D, 1);
        array2D = DrawCircle(finalq[i][0], finalq[i][1], 3, array2D, 1);
        array2D = DrawCircle(finalq[i][0], finalq[i][1], 4, array2D, 1);
        quarters = quarters + 1;
    }

    for(int i = 0; unsigned(i)<finaln.size(); i++){
        array2D = DrawCircle(finaln[i][0], finaln[i][1], finaln[i][2] - 1, array2D, 3);
        array2D = DrawCircle(finaln[i][0], finaln[i][1], finaln[i][2], array2D, 3);
        array2D = DrawCircle(finaln[i][0], finaln[i][1], finaln[i][2] + 1, array2D, 3);
        array2D = DrawCircle(finaln[i][0], finaln[i][1], 2, array2D, 1);
        array2D = DrawCircle(finaln[i][0], finaln[i][1], 3, array2D, 1);
        array2D = DrawCircle(finaln[i][0], finaln[i][1], 4, array2D, 1);
        nickels = nickels + 1;
    }

    for(int i = 0; unsigned(i)<finald.size(); i++){
        array2D = DrawCircle(finald[i][0], finald[i][1], finald[i][2] - 1, array2D, 4);
        array2D = DrawCircle(finald[i][0], finald[i][1], finald[i][2], array2D, 4);
        array2D = DrawCircle(finald[i][0], finald[i][1], finald[i][2] + 1, array2D, 4);
        array2D = DrawCircle(finald[i][0], finald[i][1], 2, array2D, 1);
        array2D = DrawCircle(finald[i][0], finald[i][1], 3, array2D, 1);
        array2D = DrawCircle(finald[i][0], finald[i][1], 4, array2D, 1);
        dimes = dimes + 1;
    }

    for(int i = 0; unsigned(i)<finalh.size(); i++){
        array2D = DrawCircle(finalh[i][0], finalh[i][1], finalh[i][2] - 1, array2D, 5);
        array2D = DrawCircle(finalh[i][0], finalh[i][1], finalh[i][2], array2D, 5);
        array2D = DrawCircle(finalh[i][0], finalh[i][1], finalh[i][2] + 1, array2D, 5);
        array2D = DrawCircle(finalh[i][0], finalh[i][1], 2, array2D, 1);
        array2D = DrawCircle(finalh[i][0], finalh[i][1], 3, array2D, 1);
        array2D = DrawCircle(finalh[i][0], finalh[i][1], 4, array2D, 1);
        halfDollar = halfDollar + 1;
    }

    cout << "Pennies: " << pennies << endl;
    cout << "Quarters: " << quarters << endl;
    cout << "Nickels: " << nickels << endl;
    cout << "Dimes: " << dimes << endl;
    cout << "Half Dollar: " << halfDollar << endl;

    double total = .01 * pennies + .25 * quarters + .05 * nickels + .10 * dimes + .5 * halfDollar;
    cout << "Total Sum: " << total << endl;

    ofstream myfile;
    myfile.open (TEXT);
    myfile << "Pennies: " << pennies << endl;
    myfile << "Quarters: " << quarters << endl;
    myfile << "Nickels: " << nickels << endl;
    myfile << "Dimes: " << dimes << endl;
    myfile << "Half Dollar: " << halfDollar << endl;
    myfile << "Total Sum: " << total << endl;

    return array2D;
}

vector<vector<int>> getCC(int** votes){
    vector<vector<int>> vec;
    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(votes[i][j] == 1){
                vector<int> vect;
                vect.push_back(i);
                vect.push_back(j);
                vec.push_back(vect);
            }
        }
    }
    return vec;
}

void part3(int argc, char **argv){
    for (int i = 0; i < argc; ++i){
        if(strcmp(argv[i], "-f") == 0){
            INPUT = string(argv[i+1]);
        }
        else if(strcmp(argv[i], "-lt") == 0){
            LT = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-ht") == 0){
            HT = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-ff") == 0){
            FINAL = string(argv[i+1]);
        }
        else if (strcmp(argv[i], "-TC") == 0){
            VT = stoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-fv") == 0){
            LIGHT = string(argv[i+1]);
        }
        else if (strcmp(argv[i], "-fcc") == 0){
            COINS = string(argv[i+1]);
        }
        else if(strcmp(argv[i], "-TCircle") == 0){
            TCircle = stoi(string(argv[i+1]));
        }
        else if(strcmp(argv[i], "-fCo") == 0){
            FINALCOINS = string(argv[i+1]);
        }
        else if(strcmp(argv[i], "-fCi") == 0){
            CIRCLES = string(argv[i+1]);
        }
        else if(strcmp(argv[i], "-fR") == 0){
            TEXT = string(argv[i+1]);
        }
        else if (strcmp(argv[i], "-tl") == 0){
            TL = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-tr") == 0){
            TR = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-bl") == 0){
            BL = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-br") == 0){
            BR = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rpl") == 0){
            rpl = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rph") == 0){
            rph = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rnl") == 0){
            rnl = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rnh") == 0){
            rnh = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rql") == 0){
            rql = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rqh") == 0){
            rqh = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rdl") == 0){
            rdl = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rdh") == 0){
            rdh = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rhl") == 0){
            rhl = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-rhh") == 0){
            rhh = stoi(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-p") == 0){
            p = stod(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-q") == 0){
            q = stod(string(argv[i+1]));
        }
        else if (strcmp(argv[i], "-n") == 0){
            n = stod(string(argv[i+1]));
        }


    }

    int** intensity = makeIntensity();
    makeGrayScale(intensity);
    double GKernel[5][5];
    Gaussian(GKernel);
    double** finalIntensity = applyFilter(GKernel, intensity);
    int x[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
    int y[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};
    double** dx = createDx(x, finalIntensity);
    double** dy = createDy(y, finalIntensity);
    int** angles = findAngles(dx, dy);
    double** magnitude = createMagnitude(dx,dy);
    int** directionAngle = angleToDirection(angles);
    gradient(directionAngle, magnitude);
    int** edges = twoThresholds(magnitude);
    edges = find2(edges);
    int** finalEdges = DTandNMS(edges, directionAngle);
    PPM4(finalEdges);
    int** votes = voteEdges(angles, finalEdges, dx, dy);
    PPMoverlay(votes);
    vector<vector<int>> coinCenters = getCC(votes);
    int** array2D = getCoins(coinCenters, finalEdges);
    PPMoverlayRed(array2D);
    PPMoverlay2(array2D);
}

int main(int argc, char **argv){
    part3(argc, argv);
}

