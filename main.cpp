#include <random>
#include "gnuplot_iostream.h"
#include <cmath>
#include <thread>
#include <chrono>

constexpr double M_PI = 3.14159265358979323846; // pi for sinus water

std::mt19937 rng(std::random_device{}());

Gnuplot gp("D:/gnuplot/bin/gnuplot.exe");

constexpr int maxX = 100;
constexpr int maxY = 100;
constexpr int riverWidth = 2;
constexpr int fireChance = 600;
constexpr int amountOfIterations = 50;
constexpr int windBonus = 250;
constexpr int iterations = 200;

enum class Cell {
    Empty = 0,
    Tree = 1,
    Fire = 2,
    Burned = 3,
    Water = 4
};

std::pair<int,int> windVector(const int wind) {
    switch (wind) {
        case 0: return {1,-1}; // NE
        case 1: return {1,0}; // SE
        case 2: return {1,1}; // S
        case 3: return {0,1}; // SW
        case 4: return {-1, 1}; // W
        case 5: return {-1,0}; // E
        case 6: return {-1,-1}; // NW
        case 7: return { 0, -1}; // N
        case 8: return {1,-1}; // NE
        case 9: return {1,0}; // SE
        default: ;
    }
    return {0,0};
}

void sinusWater(std::vector<int>& water) {
    for (int i = 0; i < maxY; i++) {
        constexpr double tmp = 2.0 * M_PI/maxY;
        const double sinusY = sin(tmp * i) + 1;
        water.push_back(static_cast<int>(maxY / 2.0 * sinusY));
    }
}

void fillWater(std::vector<int>& water, std::vector<std::vector<Cell>>& baseForest) {
    sinusWater(water);
    for (int j = 0; j < maxY; j++) {
        for (int w = -riverWidth; w <= riverWidth; w++) {
            int y = water[j] + w;
            if (y >= 0 && y < maxY) {
                baseForest[y][j] = Cell::Water;
            }
        }
    }
}

std::vector<std::vector<int>> toGnuplotMatrix(const std::vector<std::vector<Cell>>& forest) {
    std::vector out(maxY, std::vector<int>(maxX));
    for (int y = 0; y < maxY; y++)
        for (int x = 0; x < maxX; x++)
            out[y][x] = static_cast<int>(forest[y][x]);

    return out;
}

bool inBounds(const int y, const int x) {
    return y >= 0 && y < maxY && x >= 0 && x < maxX;
}

void fillWithTrees(std::vector<std::vector<Cell>>& baseForest) {
    for (int i = 0; i < maxY; i++) {
        for (int j = 0; j < maxX; j++) {
            baseForest[i][j] = rng() % 10 > 1 ? Cell::Tree : Cell::Empty;
        }
    }
}

void init(std::vector<std::vector<Cell>>& baseForest, std::vector<std::vector<Cell>>& temporaryForest) {
    baseForest.resize(maxY, std::vector(maxX, Cell::Empty));
    temporaryForest.resize(maxY, std::vector(maxX, Cell::Empty));
    std::vector<int> water;

    fillWithTrees(baseForest);
    fillWater(water, baseForest);
}

void initFire(std::vector<std::vector<Cell>>& baseForest){
    int x, y;
    do {
        std::uniform_int_distribution distX(0, maxX - 1);
        std::uniform_int_distribution distY(0, maxY - 1);

        x = distX(rng);
        y = distY(rng);

    } while (baseForest[y][x] == Cell::Water);

    baseForest[y][x] = Cell::Fire;
}

void fire(std::vector<std::vector<Cell>>& forest, const int y, const int x,
    const std::vector<std::vector<Cell>>& temporaryForest,const int wind) {

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int ny = y + dy;
            int nx = x + dx;

            if (!inBounds(ny,nx)) continue;

            auto [windY,windX] = windVector(wind + dx); //dx work as offset for neighbours

            if (temporaryForest[y][x] == Cell::Fire && forest[ny][nx] == Cell::Tree){
                if (ny == windY + y && nx == windX + x
                    && rng() % 1000 - windBonus <= fireChance)
                    forest[ny][nx] = Cell::Fire; // NOLINT(*-branch-clone)
                if (rng() % 1000 + windBonus <= fireChance)
                    forest[ny][nx] = Cell::Fire;
            }
        }
    }
    forest[y][x] = Cell::Burned;
}

void showForest(const std::vector<std::vector<Cell>>& baseForest) {
    gp << "set term wxt\n";
    gp << "set autoscale fix\n";
    //seting pallete for state
    gp << "set palette defined (0 'white', 1 'green', 2 'red', 3 'black', 4 'blue')\n";
    gp << "set cbrange[0:4]\n";
    gp << "set colorbox\n";
    gp << "set cblabel 'Stan komórki'\n";
    gp << "set cbtics ('Empty' 0, 'Tree' 1, 'Fire' 2, 'Burned' 3, 'Water' 4)\n";
    gp << "unset key\n";
    gp << "plot '-' matrix with image\n";
    gp.send1d(toGnuplotMatrix(baseForest));
}

void changeWind(int& randWind, int temp) {
    if (temp % amountOfIterations == 0) {
        std::uniform_int_distribution windDist(1,8);
        randWind = windDist(rng);
    }
}

void spreadFire(std::vector<std::vector<Cell>>& baseForest,const std::vector<std::vector<Cell>>& temporaryForest
    ,const int i, const int j,const int randWind) {
    if (temporaryForest[i][j] == Cell::Fire) {
        fire(baseForest, i, j, temporaryForest, randWind);
    }
}

void regenerationTree(std::vector<std::vector<Cell>>& baseForest,const std::vector<std::vector<Cell>>& temporaryForest
    ,const int i, const int j, const int temp) {
    if (temporaryForest[i][j] == Cell::Burned && temp % amountOfIterations == 0)
        baseForest[i][j] = Cell::Tree;
}

std::pair<double,double> drawWindArrow(const int randWind) {
    auto [wx, wy] = windVector(randWind);

    double norm = sqrt(wx*wx + wy*wy);
    double arrowLength = 0.08;
    double dx = 0.0, dy = 0.0;

    if (norm != 0) {
        dx = (wx / norm) * arrowLength;
        dy = (wy / norm) * arrowLength;
    } else {
        dx = dy = 0.0; // brak wiatru
    }

    return std::make_pair(dy, dx);
}


void drawCurrentForest(const std::vector<std::vector<Cell>>& baseForest,const int randWind) {
    auto [dy,dx] = drawWindArrow(randWind);

    gp << "unset arrow 1\n";
    gp << "set arrow 1 from screen 0.85, screen 0.85 to screen "
       << 0.85 + dy << ", screen " << 0.85 + dx
       << " lw 3 lc rgb 'gray' front head filled size screen 0.02,15,60\n";
    gp << "plot '-' matrix with image\n";

    gp.send1d(toGnuplotMatrix(baseForest));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void simulation(std::vector<std::vector<Cell>>& baseForest,int randWind) {
    for (int temp = 0; temp < iterations; temp++) {
        changeWind(randWind, temp);

        auto temporaryForest = baseForest;

        for (int i = 0; i < maxY; i++) {
            for (int j = 0; j < maxX; j++) {
                spreadFire(baseForest, temporaryForest, i, j, randWind);
                regenerationTree(baseForest, temporaryForest, i, j, temp);
            }
        }

        drawCurrentForest(baseForest, randWind);

        //start random fire chance == 1;
        if (rng() % 1000 == 1)
            initFire(baseForest);
    }
}

int main() {
    std::uniform_int_distribution windDist(1,8);
    int randWind = windDist(rng);
    std::vector<std::vector<Cell>> baseForest;
    std::vector<std::vector<Cell>> temporaryForest;

    init(baseForest, temporaryForest);
    initFire(baseForest);
    showForest(baseForest);
    std::cin.get();
    simulation(baseForest, randWind);
    return 0;
}
