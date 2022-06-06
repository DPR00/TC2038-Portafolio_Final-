#include "Problema1.h"
#include "Problema2.h"
#include "Problema3.h"
#include "Problema4.h"
#include <iostream>
#include <algorithm>

using namespace std;


// Funcion que modifica el orden de las columnas de tal manera que
// las colonias centrales esten al final.
void modificarOrden(vector<int> &colCentral, vector<string> &colNames,
                        vector<int> &posX, vector<int> &posY, int nodes){

    vector<int> tempColCentral = colCentral;
    for(int i=0; i<nodes; i++){
        if(tempColCentral[i]==1){
            string temp=colNames[i];
            int temp2=colCentral[i];
            int temp3=posX[i];
            int temp4=posY[i];
            colNames.erase(colNames.begin()+ i);
            colNames.push_back(temp);
            colCentral.erase(colCentral.begin()+ i);
            colCentral.push_back(temp2);
            posX.erase(posX.begin()+ i);
            posX.push_back(temp3);
            posY.erase(posY.begin()+ i);
            posY.push_back(temp4);
        }
    }
}

int main(){

    // Variables a utilizar
    int nodes, edges, numNewCols;
    cin >> nodes >> edges >> numNewCols;

    vector<string> colNames(nodes), col1(edges), col2(edges);  // Nombre de los nodos - edges
    vector<int> colCentral(nodes); // Contiene si es nodo central o no
    vector<int> valEdges(edges); // Costo entre nodos (arcos)
    vector<int> posX(nodes), posY(nodes); // Posiciones en el plano
    vector<string> colNews(numNewCols); // Nuevas columnas a agregar
    vector<int> posXNew(nodes), posYNew(nodes); // Posiciones de las nuevas columnas

    // Recibir y almacenar datos de entrada
    for(int i=0; i<nodes; i++){
        cin >> colNames[i] >> posX[i] >> posY[i] >> colCentral[i];
    }
    for(int i=0; i<edges; i++){
        cin >> col1[i] >> col2[i] >> valEdges[i];
    }
    for(int i=0; i <numNewCols; i++){
        cin >> colNews[i] >> posXNew[i] >> posYNew[i];
    }

    // Modificar el orden de las colonias
    modificarOrden(colCentral, colNames, posX, posY, nodes);

    // ---------------------------------------------------
    // 1er problema: Arbol de minima expansion con Kruskal
    // ---------------------------------------------------
    Graph g(nodes, edges);
    g.load(colNames, col1, col2, valEdges);
    g.kruskalMST();

    cout << "-------------------" << endl;
    cout << " 1 – Cableado óptimo de nueva conexión. "<< endl << endl;
    g.printResult(colNames);

    // ---------------------------------------------------
    // 2do problema: Travel Salesman problem con B&B
    // ---------------------------------------------------
    int matAdj[MAX][MAX]; // Asumir es base-1 A=1; B=2; C=3 ...
    iniciarMat(matAdj);
    leeArcos(matAdj, edges, col1, col2, colNames, colCentral, valEdges);

    TSP valTsp = tsp(matAdj, nodes, colNames, colCentral);
    string rutaOpt = valTsp.rutaOptima;
    int valOpt = valTsp.costoOptimo;

    cout << "-------------------" << endl;
    cout << " 2 – La ruta óptima. "<< endl;
    cout << endl;
    cout << rutaOpt << endl << endl;
    if(valOpt>= INT_MAX || valOpt<=-INT_MAX){
        cout << "La Ruta Óptima tiene un costo total de: INF" << endl;
    }
    else{
        cout << "La Ruta Óptima tiene un costo total de: " << valOpt << endl;
    }
    cout << endl;

    // ---------------------------------------------------
    // 3er problema: Floyd-Warshall
    // ---------------------------------------------------

    int mat[MAXF][MAXF], p[MAXF][MAXF]; // Asumir es base-1 A=1; B=2; C=3 ...
    leeMatFloyd(mat, p, edges, col1, col2, colNames, colCentral, valEdges);
    floyd(mat, p, nodes);

    cout << "-------------------" << endl;
    cout << " 3 – Caminos más cortos entre centrales. "<< endl;
    cout << endl;
    consultas(mat, p, nodes, colCentral, colNames);
    cout << endl;

    // ---------------------------------------------------
    // 4to problema: Distancia minima entre 2 puntos  Complejidad: O(q*n)
    // ---------------------------------------------------

	cout << "-------------------" << endl << "4 – Conexión de nuevas colonias." << endl;

	Point newPoint, nextPoint;
	int minDist, newDist ;
	string name;
	
		for(int i=0; i < numNewCols; i++){
				minDist = INT_MAX;
				newDist = 0;
				newPoint = Point(posXNew[i], posYNew[i]);
			for(int j=0; j<nodes; j++){
				nextPoint = Point(posX[j], posY[j]);
				newDist = dist(newPoint, nextPoint);
				
				if(newDist < minDist){
					minDist = newDist;
					name = colNames[j];
				}	
    	}
			cout << "\n" <<colNews[i] << " debe conectarse con " << name;
		}

	cout << endl << "\n -------------------" << endl;
    
    return 0;

}