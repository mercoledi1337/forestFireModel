#include <random>
#include "gnuplot_iostream.h"
#include <time.h>

using namespace std;

const float p = 0.6;
const int maxX = 10;
const int maxY = 10;

void fire(vector<vector<int>>& forest, int y, int x, vector<vector<int>> newForest) {


    if (x == 0) {
        //left bottom corner
        if (y == 0) {
            forest[y][x + 1] = newForest[y][x + 1] == 1 ? forest[y][x + 1]+1 : forest[y][x + 1];
            forest[y + 1][x + 1] = newForest[y + 1][x + 1] == 1 ? forest[y + 1][x + 1]+1 : forest[y + 1][x + 1];
            forest[y + 1][x] = newForest[y + 1][x] == 1 ? forest[y + 1][x]+1 : forest[y + 1][x];
            return;
        }

        //left top corner
        if (y == maxY - 1) {
            forest[y][x + 1] = newForest[y][x + 1] == 1 ? forest[y][x + 1]+1 : forest[y][x + 1];
            forest[y - 1][x + 1] = newForest[y - 1][x + 1] == 1 ? forest[y - 1][x + 1]+1 : forest[y - 1][x + 1];
            forest[y - 1][x] = newForest[y - 1][x] == 1 ? forest[y - 1][x]+1 : forest[y - 1][x];
            return;
        }

        //left wall
        forest[y + 1][x] = newForest[y + 1][x] == 1 ? forest[y + 1][x]+1 : forest[y + 1][x];
        forest[y + 1][x + 1] = newForest[y + 1][x + 1] == 1 ? forest[y + 1][x + 1]+1 : forest[y + 1][x + 1];
        forest[y][x + 1] = newForest[y][x + 1] == 1 ? forest[y][x + 1]+1 : forest[y][x + 1];
        forest[y - 1][x + 1] = newForest[y - 1][x + 1] == 1 ? forest[y - 1][x + 1]+1 : forest[y - 1][x + 1];
        forest[y - 1][x] = newForest[y - 1][x] == 1 ? forest[y - 1][x]+1 : forest[y - 1][x];
        return;
    }

    if (x == maxX - 1) {
        //right bottom corner
        if (y == 0) {
            forest[y][x - 1] = newForest[y][x - 1] == 1 ? forest[y][x - 1]+1 : forest[y][x - 1];
            forest[y + 1][x - 1] = newForest[y + 1][x - 1] == 1 ? forest[y + 1][x - 1]+1 : forest[y + 1][x - 1];
            forest[y + 1][x] = newForest[y + 1][x] == 1 ? forest[y + 1][x]+1 : forest[y + 1][x];
            return;
        }

        //right top corner
        if (y == maxY - 1) {
            forest[y][x - 1] = newForest[y][x - 1] == 1 ? forest[y][x - 1]+1 : forest[y][x - 1];
            forest[y - 1][x - 1] = newForest[y - 1][x - 1] == 1 ? forest[y - 1][x - 1]+1 : forest[y - 1][x - 1];
            forest[y - 1][x] = newForest[y - 1][x] == 1 ? forest[y - 1][x]+1 : forest[y - 1][x];
            return;
        }

        //right wall
        forest[y + 1][x] = newForest[y + 1][x] == 1 ? forest[y + 1][x]+1 : forest[y + 1][x];
        forest[y + 1][x - 1] = newForest[y + 1][x - 1] == 1 ? forest[y + 1][x - 1]+1 : forest[y + 1][x - 1];
        forest[y][x - 1] = newForest[y][x - 1] == 1 ? forest[y][x - 1]+1 : forest[y][x - 1];
        forest[y - 1][x - 1] = newForest[y - 1][x - 1] == 1 ? forest[y - 1][x - 1]+1 : forest[y - 1][x - 1];
        forest[y - 1][x] = newForest[y - 1][x] == 1 ? forest[y - 1][x]+1 : forest[y - 1][x];
        return;
    }

    //bottom wall
    if (y == 0) {
        forest[y][x - 1] = newForest[y][x - 1] == 1 ? forest[y][x - 1]+1 : forest[y][x - 1];
        forest[y + 1][x - 1] = newForest[y + 1][x - 1] == 1 ? forest[y + 1][x - 1]+1 : forest[y + 1][x - 1];
        forest[y + 1][x] = newForest[y + 1][x] == 1 ? forest[y + 1][x]+1 : forest[y + 1][x];
        forest[y + 1][x + 1] = newForest[y + 1][x + 1] == 1 ? forest[y + 1][x + 1]+1 : forest[y + 1][x + 1];
        forest[y][x + 1] = newForest[y][x + 1] == 1 ? forest[y][x + 1]+1 : forest[y][x + 1];
        return;
    }

    //top wall
    if (y == maxY - 1) {
        forest[y][x - 1] = newForest[y][x - 1] == 1 ? forest[y][x - 1]+1 : forest[y][x - 1];
        forest[y - 1][x - 1] = newForest[y - 1][x - 1] == 1 ? forest[y - 1][x - 1]+1 : forest[y - 1][x - 1];
        forest[y - 1][x] = newForest[y - 1][x] == 1 ? forest[y - 1][x]+1 : forest[y - 1][x];
        forest[y - 1][x + 1] = newForest[y - 1][x + 1] == 1 ? forest[y - 1][x + 1]+1 : forest[y - 1][x + 1];
        forest[y][x + 1] = newForest[y][x + 1] == 1 ? forest[y][x + 1]+1 : forest[y][x + 1];
        return;
    }

    forest[y + 1][x + 1] = newForest[y + 1][x + 1] == 1 ? forest[y + 1][x + 1]+1 : forest[y + 1][x + 1];
    forest[y][x + 1] = newForest[y][x + 1] == 1 ? forest[y][x + 1]+1 : forest[y][x + 1];
    forest[y - 1][x + 1] = newForest[y - 1][x + 1] == 1 ? forest[y - 1][x + 1]+1 : forest[y - 1][x + 1];
    forest[y + 1][x] = newForest[y + 1][x] == 1 ? forest[y + 1][x]+1 : forest[y + 1][x];
    forest[y - 1][x] = newForest[y - 1][x] == 1 ? forest[y - 1][x]+1 : forest[y - 1][x];
    forest[y + 1][x - 1] = newForest[y + 1][x - 1] == 1 ? forest[y + 1][x - 1]+1 : forest[y + 1][x - 1];
    forest[y][x - 1] = newForest[y][x - 1] == 1 ? forest[y][x - 1]+1 : forest[y][x - 1];
    forest[y - 1][x - 1] = newForest[y - 1][x - 1] == 1 ? forest[y - 1][x - 1]+1 : forest[y - 1][x - 1];

}

int main() {
    vector<vector<int>> test;
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        vector<int> a;
        for (int j = 0; j < 10; j++) {
        a.push_back(j);
        }
        test.push_back(a);
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int tab[3] = {0,1,2};
            test[i][j] = tab[rand() % 2];
        }
    }

    int xfire = rand() % 10;
    int yfire = rand() % 10;

    test[yfire][xfire] = 2;

    // uruchamiamy gnuplot
    Gnuplot gp("D:/gnuplot/bin/gnuplot.exe");

    gp << "set term wxt\n";  // żeby pokazać okno
    gp << "set autoscale fix\n";
    gp << "set palette defined (0 'white', 1 'green', 2 'red', 3 'black')\n";
    gp << "set cbrange[0:3]\n";
    gp << "unset cbtics\n";
    gp << "unset key\n";

    gp << "plot '-' matrix with image\n";

    gp.send1d(test);  // 1. matrix (image)
    cin.get();


    for (int temp = 0; temp < 20; temp++) {
        auto newforest = test;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (test[i][j] == 2) {
                    fire(test, i, j, newforest);

                }
            }
        }


        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {

            }
        }
        gp << "plot '-' matrix with image\n";
        gp.send1d(test);

        cin.get();
    }

    cin.get();
    return 0;
}
