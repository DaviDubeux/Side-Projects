#include <iostream>

using namespace std;

bool valid(int grid[][9], int y, int x, int v){
    int i, j, k, sY0, sX0;

    for (j = 0; j < 9; j++){ // row
        if (grid[y][j] == v) return false; 
    }

    for (i = 0; i < 9; i++){ // collumn
        if (grid[i][x] == v) return false;
    }

    sY0 = 3*(y/3); sX0 = 3*(x/3);
    for (k = 0; k < 9; k++){ // square
        i = sY0 + k/3; j = sX0 + k%3;
        if (grid[i][j] == v) return false;
    }

    return true;
}

// returns true or false to see if should erase and try again
bool solve(int grid[][9], int i = 0, int j = 0){
    if (i == 9) return true;
    if (j == 9) return solve(grid, i+1, 0);
    if (grid[i][j]) return solve(grid, i, j+1);
    // (i, j) is not solved and inside the grid
    
    for (int v = 1; v <= 9; v++){
        if (!valid(grid, i, j, v)) continue;
        grid[i][j] = v;
        if (solve(grid, i, j+1)) return true;
        grid[i][j] = 0;
    }
    return false;
}

void print(int grid[][9]){
    for (int i = 0; i < 9; i++){
        cout << "+---+---+---+ +---+---+---+ +---+---+---+\n";
        if (i == 3 || i == 6) cout << "+---+---+---+ +---+---+---+ +---+---+---+\n";
        for (int j = 0; j < 9; j++){
            cout << "| ";
            if (j == 3 || j == 6) cout << "| ";
            cout << grid[i][j];
            cout << ' ';
        }
        cout << "|\n";
    }
    cout << "+---+---+---+ +---+---+---+ +---+---+---+\n\n";
}

int main(){
    int n;
    int sudoku[9][9];

    cin >> n;
    while(n--){
        for (int k = 0; k < 81; k++){
            int i = k/9;
            int j = k%9;
            cin >> sudoku[i][j];
        }

        solve(sudoku);

        print(sudoku);
    }

    return 0;
}