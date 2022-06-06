#include <iostream>
#include <climits>
#include <vector>

using namespace std;

#define MAXF 100

void leeMatFloyd(int mat[MAXF][MAXF], int p[MAXF][MAXF], int edges, vector<string>  col1, vector<string> col2,
                vector<string> colNames, vector<int> colCentral, vector<int> valEdges){
    int a, b;
    // Inicializar
    for(int i = 0; i<MAXF; i++){
        mat[i][i] = 0;
        for(int j = i+1; j<MAXF; j++){
            mat[i][j] = mat[j][i] = INT_MAX; //INT_MAX representa el max entero
            p[i][j] = p[j][i] = -1;
        }
    }
    // Lectura
    for(int i=0; i<edges; i++){
        for(int j=0; j<colNames.size(); j++){
            if(col1[i]==colNames[j]){
                a = j;
            }
            if(col2[i]==colNames[j]){
                b = j;
            }
        }
        mat[a][b] = mat[b][a] = valEdges[i];
    }
}


// Floyd Algorithm
// Complejidad: O(n^3)
void floyd(int mat[MAXF][MAXF], int p[MAXF][MAXF], int c){
    for (int k = 0; k<c; k++){
        for (int i = 0; i<c; i++){
            for (int j = 0; j<c; j++){
                if(mat[i][k] != INT_MAX && mat[k][j] != INT_MAX && max(mat[i][k], mat[k][j]) < mat[i][j]){
                    mat[i][j] = mat[i][k] + mat[k][j];
                    p[i][j] = k;
                }
            }
        }
    }
    
}

void checaTrayectoria(int p[MAXF][MAXF], int a, int b, vector<string> colNames){
    if(p[a][b] != -1){
        checaTrayectoria(p, a, p[a][b], colNames);
        cout << colNames[p[a][b]] << " - ";
        checaTrayectoria(p, p[a][b], b, colNames);
    }
}

// Function to consult each query
void consultas(int mat[MAXF][MAXF], int p[MAXF][MAXF], int nodes, vector<int> colCentral, vector<string> colNames){
    for(int i=0; i<nodes; i++){
        if(colCentral[i]){
            for(int j=i+1; j<nodes; j++){
                if(colCentral[j]){
                    if(mat[i][j] == INT_MAX){
                        cout << "no path" << endl;
                    } 
                    else{
                        cout << colNames[i] << " - ";
                        checaTrayectoria(p, i, j, colNames);
                        cout << colNames[j];
                        cout << " ("<< mat[i][j] << ")" << endl;
                    }
                }
            }
        }
    }
}