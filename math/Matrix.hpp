//
//  Matrix.hpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 12/27/21.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <stdio.h>
#include <cassert>
#include <vector>

template <typename T>
class Matrix2D {
    
    
public:
    int numRows;
    int numColumns;
    std::vector<T> data;
    
    Matrix2D(int rows, int columns):
    numRows(rows), numColumns(columns), data(std::vector<T>(numRows*numColumns)) {}
    
    Matrix2D(int rows, int columns, T value) :
    numRows(rows), numColumns(columns), data(std::vector<T>(numRows*numColumns)) {
        for (unsigned int i = 0; i < data.size(); ++i){
            data[i] = value;
        }
    }
    
    Matrix2D(const Matrix2D& oldMatrix) {
        numRows = oldMatrix.numRows;
        numColumns = oldMatrix.numColumns;
        data = oldMatrix.data;
    }
    
    Matrix2D& operator=(const Matrix2D& rhs) {
        return Matrix2D(rhs);
    }
    
    Matrix2D operator+ (const Matrix2D& M2) {
        assert(M2.numRows == numRows);
        assert(M2.numColumns == numColumns);
        
        auto newMatrix = Matrix2D<T>(numRows, numColumns);
        for (unsigned int i = 0; i < data.size(); ++i){
            newMatrix.data[i] = M2.data[i] + data[i];
        }
        
        return newMatrix;
    }
    
    Matrix2D operator* (Matrix2D& M2) {
        assert(numColumns == M2.numRows);
        
        auto newMatrix = Matrix2D<T>(numRows, M2.numColumns);
        for (int row = 0; row < newMatrix.numRows; ++row){
            for (int col = 0; col < newMatrix.numColumns; ++col){
                newMatrix(row, col) = T(0);
                for (int i = 0; i < numColumns; ++i){
                    newMatrix(row, col) += (*this)(row, i) + M2(i, col);
                }
            }
        }
        
        return newMatrix;
    }
    
    T& operator() (int row, int col){
        return data[row + col*numRows];
    }
    
    T& operator[] (int index){
        return data[index];
    }
    
    Matrix2D transpose(){
        auto newMatrix = Matrix2D<T>(numColumns, numRows);
        for (int row = 0; row < newMatrix.numRows; ++row){
            for (int col = 0; col < newMatrix.numColumns; ++col){
                newMatrix(row, col) = (*this)(col, row);
            }
        }
        return newMatrix;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix2D<T>& M){
    for (int row = 0; row < M.numRows; ++row){
        for (int col = 0; col < M.numColumns; ++col){
            os << M.data[row + col*M.numRows] << ", ";
        }
        std::cout << std::endl;
    }
    return os;
}

#endif /* Matrix_hpp */
