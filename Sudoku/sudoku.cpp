#include "utils.hpp"

int main(){

    int n;
    int v;

    std::cin >> n;
    while (n--){
        Sudoku sudoku;

        for (int k = 0; k < 81; k++){
            int i = k/9;
            int j = k%9;
            
            std::cin >> v;
            if (v) sudoku.set(i, j, v, "No description");
        }

        std::cout << "Board ready\n";

        sudoku.print();

        sudoku.solve();

        sudoku.print();

        std::cin >> n;
    }

    return 0;
}

// g++ sudoku.cpp -o sudoku.exe && ./sudoku.exe < input.in > output.out
