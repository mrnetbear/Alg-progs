#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cctype>
#include <vector>
#include <algorithm>


void fillMatrix(size_t m, std::vector<size_t> &n, std::vector< std::vector<int> > &matrix){
    for (size_t i = 0; i < m; ++i){
        std::vector<int> buf;
        for (size_t j = 0; j < n[i]; ++j){
            buf.push_back(0);
            std::cin >> std::dec >> buf[j];
        }
        matrix.push_back(buf);
    }
}
int main(){

    //recieving ammount of rows and columns
    size_t m;
    std::vector<size_t> n;
    std::cin >> m; //reading ammount of rows
    if (std::cin.fail() || std::cin.eof() || (int)m < 0) //error checking
        return -1;
    for (size_t i = 0; i < m; ++i){ //reading ammount of columns
        size_t value;
        std::cin >> value;
        if (std::cin.fail() || std::cin.eof() || (int)value < 0)
        return -1;
        n.push_back(value);
    }

    std::vector< std::vector<int> > matrix;

    fillMatrix(m, n, matrix);

    

    return 0;
}