#define _CRT_RAND_S
#include <random>
#include "gnuplot_iostream.h"
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;
std::mt19937 rng(std::random_device{}());

const float p = 600;
const float s = 1;
const int maxX = 50;
const int maxY = 50;
const int k = 5;

void fillWater(vector<int>& water) {
    for (int i = 0; i < maxY; i++) {
        double tmp = 2.0 * M_PI/50.0;
        double siny = sin(tmp * i) + 1;
        water.push_back(25 * siny);
    }
}

void init(vector<vector<int>>& baseForest) {
    baseForest.resize(maxY, vector(maxX, 1));

    vector<int> water;
    fillWater(water);

    // filing with trees and empty space
    for (int i = 0; i < maxY; i++) {
        for (int j = 0; j < maxX; j++) {
            if (rng() % 10 > 1){
            baseForest[i][j] = 1;
        }else {
               baseForest[i][j] = 0;
           }
        }
    }

    const int riverWidth = 2;
    for (int j = 0; j < maxX; j++) {
        for (int w = -riverWidth; w <= riverWidth; w++) {
            int y = water[j] + w;
            if (y >= 0 && y < maxY) {
                baseForest[y][j] = 4;
            }
        }
    }
}

void fire(vector<vector<int>>& forest, int y, int x, vector<vector<int>>& newForest,int wind) {
    if (newForest[y][x] != 2)
        return;

    // added values and start and end for not reaching outofstack
    int windMatrix[10][2] = {{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}, {1,0}};

    for (int i = -1; i <= 1; i++) {
        int randFire = static_cast<int>(rng()) % 1000;
        for (int j = -1; j <= 1; j++) {
            int ny = y + i;
            int nx = x + j;
            if (ny >= maxY || nx >= maxX || ny < 0 || nx < 0)
                continue;
            int windY = windMatrix[wind + j][0];
            int windX = windMatrix[wind + j][1];
            if (newForest[y][x] == 2 && forest[ny][nx] == 1)
                if (ny == windY + y && nx == windX + x) {
                    if (randFire + 200 > 400) {
                        forest[ny][nx] = 2;
                    }
                } else if (randFire - 100 > 400)
                    forest[ny][nx] = 2;
        }
    }
    forest[y][x] = 3;
}

pair<int,int> windVector(int wind) {
    switch (wind) {
        case 1: return {1,0}; // SE
        case 2: return {1,1}; // S
        case 3: return {0,1}; // SW
        case 4: return {-1, 1}; // W
        case 5: return {-1,0}; // E
        case 6: return {-1,-1}; // NW
        case 7: return { 0, -1}; // N
        case 8: return {1,-1}; // NE
    }
    return {0,0};
}

void startFire(vector<vector<int>>& baseForest){
    int xfire = rng() % maxX;
    int yfire = rng() % maxY;

    if (baseForest[yfire][xfire] == 4)
        startFire(baseForest);
    else {
        baseForest[yfire][xfire] = 2;
    }

}

int main() {
    unsigned int number;
    int randWind = static_cast<int>(rng()) % 8 + 1;

    vector<vector<int>> baseForest;

    init(baseForest);

    startFire(baseForest);

    // uruchamiamy gnuplot
    Gnuplot gp("D:/gnuplot/bin/gnuplot.exe");

    gp << "set term wxt\n";
    gp << "set autoscale fix\n";
    gp << "set palette defined (0 'white', 1 'green', 2 'red', 3 'black', 4 'blue')\n";
    gp << "set cbrange[0:4]\n";
    gp << "set colorbox\n";
    gp << "set cblabel 'Stan komórki'\n";
    gp << "set cbtics ('Puste' 0, 'Drzewo' 1, 'Ogien' 2, 'Spalone' 3, 'Woda' 4)\n";
    gp << "unset key\n";

    gp << "plot '-' matrix with image\n";

    gp.send1d(baseForest);  // 1. matrix (image)
    cin.get();

    for (int temp = 0; temp < 200; temp++) {
        if (temp % 25 == 0) {
            std::uniform_int_distribution<int> windDist(1,8);
            randWind = windDist(rng); // zawsze 1..8
        }
        auto newforest = baseForest;
        for (int i = 0; i < maxY; i++) {
            for (int j = 0; j < maxX; j++) {
                if (newforest[i][j] == 2) {
                    fire(baseForest, i, j, newforest, randWind);
                }
                rand_s(&number);
                if (newforest[i][j] == 1 && (unsigned int) ((double)number /
                       ((double) UINT_MAX + 1 ) * 10000) < s) {
                    baseForest[i][j] = 2;
                }
                if (newforest[i][j] == 3 && temp % 25 == 0)
                    baseForest[i][j] = 1;
            }
        }

        // Pobierz wektor wiatru
        auto [wx, wy] = windVector(randWind);

        // Normalizacja i minimalna długość
        double norm = sqrt(wx*wx + wy*wy);
        double arrowLength = 0.08; // długość strzałki w screen units
        double dx = 0.0, dy = 0.0;

        if (norm != 0) {
            dx = (wx / norm) * arrowLength;
            dy = (wy / norm) * arrowLength;
        } else {
            dx = dy = 0.0; // brak wiatru
        }


        gp << "unset arrow 1\n";
        gp << "set arrow 1 from screen 0.85, screen 0.85 to screen "
           << 0.85 + dx << ", screen " << 0.85 + dy
           << " lw 3 lc rgb 'gray' front head filled size screen 0.02,15,60\n";

        gp << "plot '-' matrix with image\n";
        gp.send1d(baseForest);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    cin.get();
    return 0;
}