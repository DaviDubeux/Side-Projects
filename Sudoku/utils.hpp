#include <iostream>
#include <list>
// #define NDEBUG

struct Tile {
    int element;
    bool collapsed;
    bool visiblePencilMarks[10];
    std::list<int> pencilMark;

    Tile(){
        element = 0;
        collapsed = false;
        for (int i = 1; i <= 9; i++){
            visiblePencilMarks[i] = true;
            pencilMark.push_back(i);
        }
    }
    ~Tile(){}
};

class Sudoku {
private:
    Tile grid[9][9];
    int solved;

    void update(int &i, int &j){
        int k = 9*i + j;
        k += 1 ; k %= 81; 
        i = k/9; j = k % 9;
    }

    void checkLoneSingle(int y, int x, std::string s){
        if (grid[y][x].collapsed) return;
        if (grid[y][x].pencilMark.size() != 1) return;

        set(y, x, grid[y][x].pencilMark.front(), s);
    }

    void hiddenSingles(int y, int x){
        if (grid[y][x].collapsed) return;

        bool hS; // hidden single
        int sY0 = 3*(y/3), sX0 = 3*(x/3);
        for (int n : grid[y][x].pencilMark){

            // row
            hS = true;
            for (int j = 0; j < 9 && hS; j++){
                if (j == x) continue;

                std::list<int>::iterator it;
                for (it = grid[y][j].pencilMark.begin(); it != grid[y][j].pencilMark.end() && hS; it++){
                    if (*it == n) hS = false;
                }
            }
            if (hS){ set(y, x, n, "Hidden singles"); return; }

            // collumn
            hS = true;
            for (int i = 0; i < 9 && hS; i++){
                if (i == y) continue;

                std::list<int>::iterator it;
                for (it = grid[i][x].pencilMark.begin(); it != grid[i][x].pencilMark.end() && hS; it++){
                    if (*it == n) hS = false;
                }
            }
            if (hS){ set(y, x, n, "Hidden singles"); return; }

            // square
            hS = true;
            for (int k = 0; k < 9 && hS; k++){
                int i = sY0 + k/3;
                int j = sX0 + k%3;

                std::list<int>::iterator it;
                for (it = grid[i][j].pencilMark.begin(); it != grid[i][j].pencilMark.end() && hS; it++){
                    if (*it == n) hS = false;
                }
            }
            if (hS){ set(y, x, n, "Hidden singles"); return; }
        }
    }

    void nakedPairs(int y, int x){
        if (grid[y][x].collapsed == true) return;
        if (grid[y][x].pencilMark.size() != 2) return;

        bool nP;
        int tempY, tempX;
        int sY0 = 3*(y/3), sX0 = 3*(x/3);

        int n1 = grid[y][x].pencilMark.front();
        int n2 = grid[y][x].pencilMark.back();

        // row
        nP = false;
        for (int j = 0; j < 9 && !nP; j++){
            if (j == x) continue;
            if (grid[y][j].pencilMark != grid[y][x].pencilMark) continue;
            nP = true;
            tempX = j;
        }
        for (int j = 0; j < 9 && nP; j++){
            if (j == x || j == tempX) continue;

            grid[y][j].pencilMark.remove(n1); grid[y][j].visiblePencilMarks[n1] = false;
            grid[y][j].pencilMark.remove(n2); grid[y][j].visiblePencilMarks[n2] = false;
        }
        for (int j = 0; j < 9 && nP; j++){
            checkLoneSingle(y, j, "Naked pairs");
        }

        // collumn
        nP = false;
        for (int i = 0; i < 9 && !nP; i++){
            if (i == y) continue;
            if (grid[i][x].pencilMark != grid[y][x].pencilMark) continue;
            nP = true;
            tempY = i;
        }
        for (int i = 0; i < 9 && nP; i++){
            if (i == y || i == tempY) continue;
            
            grid[i][x].pencilMark.remove(n1); grid[i][x].visiblePencilMarks[n1] = false;
            grid[i][x].pencilMark.remove(n2); grid[i][x].visiblePencilMarks[n2] = false;
        }
        for (int i = 0; i < 9 && nP; i++){
            checkLoneSingle(i, x, "Naked pairs");
        }

        // square
        nP = false;
        for (int k = 0; k < 9 && !nP; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;
            if (i == y && j == x) continue;
            if (grid[i][j].pencilMark != grid[y][x].pencilMark) continue;

            nP = true;
            tempY = i;
            tempX = j;
        }
        for (int k = 0; k < 9 && nP; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;                
            if (i == y && j == x) continue;
            if (i == tempY && j == tempX) continue;

            grid[i][j].pencilMark.remove(n1); grid[i][j].visiblePencilMarks[n1] = false;
            grid[i][j].pencilMark.remove(n2); grid[i][j].visiblePencilMarks[n2] = false;
        }
        for (int k = 0; k < 9 && nP; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;
            checkLoneSingle(i, j, "Naked pairs");
        }
    }

    void nakedTriplets(int y, int x){
        if (grid[y][x].collapsed) return;
        if (grid[y][x].pencilMark.size() != 3) return;

        bool nT;
        int sY0 = 3*(y/3), sX0 = 3*(x/3);
        int tempY1, tempY2, tempX1, tempX2;

        std::list<int>::iterator it = grid[y][x].pencilMark.begin();
        int n1 = *it; it++;
        int n2 = *it; it++;
        int n3 = *it;

        // row
        nT = false;
        for (int j1 = 0; j1 < 9 && !nT; j1++){
            if (j1 == x) continue;
            if (grid[y][j1].pencilMark != grid[y][x].pencilMark) continue;

            for (int j2 = 0; j2 < 9 && !nT; j2++){
                if (j2 == x || j2 == j1) continue;
                if (grid[y][j2].pencilMark != grid[y][x].pencilMark) continue;

                nT = true;
                tempX1 = j1;
                tempX2 = j2;
            }
        }
        for (int j = 0; j < 9 && nT; j++){
            if (j == x || j == tempX1 || j == tempX2) continue;

            grid[y][j].pencilMark.remove(n1); grid[y][j].visiblePencilMarks[n1] = false;
            grid[y][j].pencilMark.remove(n2); grid[y][j].visiblePencilMarks[n2] = false;
            grid[y][j].pencilMark.remove(n3); grid[y][j].visiblePencilMarks[n3] = false;
        }
        for (int j = 0; j < 9 && nT; j++){
            checkLoneSingle(y, j, "Naked triplets");
        }

        // collumn
        nT = false;
        for (int i1 = 0; i1 < 9 && !nT; i1++){
            if (i1 == y) continue;
            if (grid[i1][x].pencilMark != grid[y][x].pencilMark) continue;

            for (int i2 = 0; i2 < 9 && !nT; i2++){
                if (i2 == y || i2 == i1) continue;
                if (grid[i2][x].pencilMark != grid[y][x].pencilMark) continue;

                nT = true;
                tempY1 = i1;
                tempY2 = i2;
            }
        }
        for (int i = 0; i < 9 && nT; i++){
            if (i == y || i == tempY1 || i == tempY2) continue;

            grid[i][x].pencilMark.remove(n1); grid[i][x].visiblePencilMarks[n1] = false;
            grid[i][x].pencilMark.remove(n2); grid[i][x].visiblePencilMarks[n2] = false;
            grid[i][x].pencilMark.remove(n3); grid[i][x].visiblePencilMarks[n3] = false;
        }
        for (int i = 0; i < 9 && nT; i++){
            checkLoneSingle(i, x, "Naked triplets");
        }

        // square
        nT = false;
        for (int k1; k1 < 9 && !nT; k1++){
            int i1 = sY0 + k1/3;
            int j1 = sX0 + k1%3;
            if (i1 == y && j1 == x) continue;
            if (grid[i1][j1].pencilMark != grid[y][x].pencilMark) continue;
            
            for (int k2 = 0; k2 < 9 && !nT; k2++){
                int i2 = sY0 + k2/3;
                int j2 = sX0 + k2%3;
                if (i2 == i1 && j2 == j1) continue;
                if (i2 == y  && j2 == x ) continue;
                if (grid[i2][j2].pencilMark != grid[y][x].pencilMark) continue;

                nT = true;
                tempY1 = i1; tempY2 = i2;
                tempX1 = j1; tempX2 = j2;
            }
        }
        for (int k = 0; k < 9 && nT; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;
            if (i == y && j == x) continue;
            if (i == tempY1 && j == tempX1) continue;
            if (i == tempY2 && j == tempX2) continue;
            
            grid[i][j].pencilMark.remove(n1); grid[i][j].visiblePencilMarks[n1] = false;
            grid[i][j].pencilMark.remove(n2); grid[i][j].visiblePencilMarks[n2] = false;
            grid[i][j].pencilMark.remove(n3); grid[i][j].visiblePencilMarks[n3] = false;
        }
        for (int k = 0; k < 9 && nT; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;
            checkLoneSingle(i, j, "Naked triplets");
        }
    }

    void nakedQuads(int y, int x){
        if (grid[y][x].collapsed) return;
        if (grid[y][x].pencilMark.size() != 4) return;

        bool nQ;
        int sY0 = 3*(y/3), sX0 = 3*(x/3);
        int tempY1, tempY2, tempY3, tempX1, tempX2, tempX3;

        std::list<int>::iterator it = grid[y][x].pencilMark.begin();
        int n1 = *it; it++;
        int n2 = *it; it++;
        int n3 = *it; it++;
        int n4 = *it;

        // row
        nQ = false;
        for (int j1 = 0; j1 < 9 && !nQ; j1++){
            if (j1 == x) continue;
            if (grid[y][j1].pencilMark != grid[y][x].pencilMark) continue;

            for (int j2 = 0; j2 < 9 && !nQ; j2++){
                if (j2 == x || j2 == j1) continue;
                if (grid[y][j2].pencilMark != grid[y][x].pencilMark) continue;

                for (int j3 = 0; j3 < 9 && !nQ; j2++){
                    if (j3 == x || j3 == j2 || j3 == j1) continue;
                    if (grid[y][j3].pencilMark != grid[y][x].pencilMark) continue;

                    nQ = true;
                    tempX1 = j1;
                    tempX2 = j2;
                    tempX3 = j3;
                }
            }
        }
        for (int j = 0; j < 9 && nQ; j++){
            if (j == x || j == tempX1 || j == tempX2 || j == tempX3) continue;

            grid[y][j].pencilMark.remove(n1); grid[y][j].visiblePencilMarks[n1] = false;
            grid[y][j].pencilMark.remove(n2); grid[y][j].visiblePencilMarks[n2] = false;
            grid[y][j].pencilMark.remove(n3); grid[y][j].visiblePencilMarks[n3] = false;
            grid[y][j].pencilMark.remove(n4); grid[y][j].visiblePencilMarks[n4] = false;
        }
        for (int j = 0; j < 9 && nQ; j++){
            checkLoneSingle(y, j, "Naked triplets");
        }

        // collumn
        nQ = false;
        for (int i1 = 0; i1 < 9 && !nQ; i1++){
            if (i1 == y) continue;
            if (grid[i1][x].pencilMark != grid[y][x].pencilMark) continue;

            for (int i2 = 0; i2 < 9 && !nQ; i2++){
                if (i2 == y || i2 == i1) continue;
                if (grid[i2][x].pencilMark != grid[y][x].pencilMark) continue;

                for (int i3 = 0; i3 < 9 && !nQ; i3++){
                    if (i3 == y || i3 == i2 || i3 == i1) continue;
                    if (grid[i3][x].pencilMark != grid[y][x].pencilMark) continue;

                    nQ = true;
                    tempY1 = i1;
                    tempY2 = i2;
                    tempY3 = i3;
                }
            }
        }
        for (int i = 0; i < 9 && nQ; i++){
            if (i == y || i == tempY1 || i == tempY2 || i == tempY3) continue;

            grid[i][x].pencilMark.remove(n1); grid[i][x].visiblePencilMarks[n1] = false;
            grid[i][x].pencilMark.remove(n2); grid[i][x].visiblePencilMarks[n2] = false;
            grid[i][x].pencilMark.remove(n3); grid[i][x].visiblePencilMarks[n3] = false;
            grid[i][x].pencilMark.remove(n4); grid[i][x].visiblePencilMarks[n4] = false;
        }
        for (int i = 0; i < 9 && nQ; i++){
            checkLoneSingle(i, x, "Naked triplets");
        }

        // square
        nQ = false;
        for (int k1; k1 < 9 && !nQ; k1++){
            int i1 = sY0 + k1/3;
            int j1 = sX0 + k1%3;
            if (i1 == y && j1 == x) continue;
            if (grid[i1][j1].pencilMark != grid[y][x].pencilMark) continue;
            
            for (int k2 = 0; k2 < 9 && !nQ; k2++){
                int i2 = sY0 + k2/3;
                int j2 = sX0 + k2%3;
                if (i2 == y  && j2 == x ) continue;
                if (i2 == i1 && j2 == j1) continue;
                if (grid[i2][j2].pencilMark != grid[y][x].pencilMark) continue;

                for (int k3 = 0; k3 < 9 && !nQ; k3++){
                    int i3 = sY0 + k3/3;
                    int j3 = sX0 + k3%3;
                    if (i3 == y  && j3 == x ) continue;
                    if (i3 == i1 && j3 == j1) continue;
                    if (i3 == i2 && j3 == j2) continue;

                    nQ = true;
                    tempY1 = i1; tempY2 = i2;
                    tempX1 = j1; tempX2 = j2;
                }
            }
        }
        for (int k = 0; k < 9 && nQ; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;
            if (i == y && j == x) continue;
            if (i == tempY1 && j == tempX1) continue;
            if (i == tempY2 && j == tempX2) continue;
            if (i == tempY3 && j == tempX3) continue;
            
            grid[i][j].pencilMark.remove(n1); grid[i][j].visiblePencilMarks[n1] = false;
            grid[i][j].pencilMark.remove(n2); grid[i][j].visiblePencilMarks[n2] = false;
            grid[i][j].pencilMark.remove(n3); grid[i][j].visiblePencilMarks[n3] = false;
        }
        for (int k = 0; k < 9 && nQ; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;
            checkLoneSingle(i, j, "Naked triplets");
        }
    }

    void Omission(int y, int x){
        if (grid[y][x].collapsed) return;

        int sY0 = 3*(y/3), sX0 = 3*(x/3);

        for (int n : grid[y][x].pencilMark){
            
        }
    }

    void hiddenPairs(int y, int x);

    void hiddenQuads(int y, int x);

    void xWing(int y, int x);

    void swordFish(int y, int x);

    void xyWing(int y, int x);

    void uniqueRectangle(int y, int x);

public:
    Sudoku(){ solved = 0; }
    ~Sudoku(){}

    void print(){
        #ifndef NDEBUG
        int i1, i2, i3, j1, j2, j3;
        std::string horizontal = "+-----+-----+-----+ +-----+-----+-----+ +-----+-----+-----+\n";
        for (i1 = 0; i1 < 9; i1++){
            std::cout << horizontal;
            if (i1 == 3 || i1 == 6) std::cout << horizontal;
            for (i2 = 0; i2 < 3; i2++){
                for (j1 = 0; j1 < 9; j1++){
                    std::cout << "|";
                    if (j1 == 3 || j1 == 6) std::cout << " |";
                    for (j2 = 1; j2 <= 3; j2++){
                        if (grid[i1][j1].visiblePencilMarks[3*i2 + j2]) std::cout << (3*i2 + j2);
                        else std::cout << ' ';
                        if (j2 == 1 || j2 == 2) std::cout << ' ';
                    }
                }
                std::cout << "|\n";
            }
        }
        std::cout << horizontal;
        std::cout << "\n\n";
        #else
        for (int i = 0; i < 9; i++){
            std::cout << "+---+---+---+ +---+---+---+ +---+---+---+ \n";
            if (i == 3 || i == 6) std::cout << "+---+---+---+ +---+---+---+ +---+---+---+ \n";
            for (int j = 0; j < 9; j++){
                std::cout << "| ";
                if (j == 3 || j == 6) std::cout << "| ";
                if (grid[i][j].collapsed) std::cout << grid[i][j].element;
                else std::cout << ' ';
                std::cout << ' ';
            }
            std::cout << "|\n";
        }
        std::cout << "+---+---+---+ +---+---+---+ +---+---+---+ \n\n";
        #endif
    }

    void set(int y, int x, int v, std::string s){
        int sY0 = 3*(y/3), sX0 = 3*(x/3);

        grid[y][x].element = v;
        grid[y][x].collapsed = true;
        grid[y][x].pencilMark.clear();
        for (int it = 1; it <= 9; it++){
            if (it == v) continue;
            grid[y][x].visiblePencilMarks[it] = false;
        }
        solved++;

        // pencilmark update

        // line
        for (int j = 0; j < 9; j++){
            if (j == x) continue;
            
            grid[y][j].pencilMark.remove(v);
            grid[y][j].visiblePencilMarks[v] = false;
        }

        // collumn
        for (int i = 0; i < 9; i++){
            if (i == y) continue;
            
            grid[i][x].pencilMark.remove(v);
            grid[i][x].visiblePencilMarks[v] = false;
        }
        
        // square
        for (int k = 0; k < 9; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;

            if (i == y && j == x) continue;
        
            grid[i][j].pencilMark.remove(v);
            grid[i][j].visiblePencilMarks[v] = false;
        }
 
        #ifndef NDEBUG
        if (s != "No description") std::cout << s << '\n';

        print();
        #endif

        // recursive checks

        // line
        for (int j = 0; j < 9; j++){
            if (j == x) continue;
            
            checkLoneSingle(y, j, "Recursive");
        }

        // collumn
        for (int i = 0; i < 9; i++){
            if (i == y) continue;
            
            checkLoneSingle(i, x, "Recursive");
        }
        
        // square
        for (int k = 0; k < 9; k++){
            int i = sY0 + k/3;
            int j = sX0 + k%3;

            if (i == y && j == x) continue;
        
            checkLoneSingle(i, j, "Recursive");
        }
    }

    void solve(){
        int i = 0, j = 0;
        bool giveUp = false;
        int prevSolved = -1;
        while (solved != 81 && (i || j || solved != prevSolved)){
            if (i == 0 && j == 0){
                std::cout << '.';
                prevSolved = solved;
            }
            update(i, j);

            if (grid[i][j].collapsed) continue;

            // Lone singles
            checkLoneSingle(i, j, "Check lone single");

            // visual elimination is a case of hidden single
            // repeat(&visualElimination);

            // Hidden Singles
            hiddenSingles(i, j);

            // Naked pairs
            nakedPairs(i, j);

            // Naked triplets
            nakedTriplets(i, j);

            // Naked quads
            // nakedQuads(i, j);

            // Omission
            // Omission(i, j);

            // 

        }
        std::cout << '\n';
        if (solved != 81){ // couldn't do anything
            std::cout << "\nI give up :(\n";
        }
    }
};