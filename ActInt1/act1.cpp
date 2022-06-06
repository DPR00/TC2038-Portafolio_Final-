#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

ifstream transmissionFile, mcodeFile;
ofstream outputFile;
#define MAX 1001

int mat[MAX][MAX];

// Struct to return the longest palindrome and its position
struct s_palindrome{
    int posicion;
    string pal;
};

// Function to read the transmission data
vector<string> readFile(vector<string> &nameFile) {

  vector<string> vec_data;
  string data;

  for(int i=0; i<nameFile.size(); i++){
    transmissionFile.open(nameFile[i]);
    if (!transmissionFile) cerr << "Error while opennig the file" << endl;
    transmissionFile >> data;
    vec_data.push_back(data);
    transmissionFile.close();
  }
  
  return vec_data;
}


/* ============== INCIDENTS NUMBER ===================
* Descrition: Function that write in the "outputFile"
* the number of incidents between the transmitted data
* and the malicious code as well as the positions where
* they were found.
* - Complex: O(n)
* - In charge: Ale uwu
*/
vector<int> z_function(string str)
{
    int n = str.length();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++)
    {
        if (i <= r)
        {
            z[i] = min(r - i + 1, z[i - 1]);
        }
        while (i < z[i] < n && str[i + z[i]] == str[z[i]])
        {
            z[i]++;
        }
        if (i + z[i] - 1 > r)
        {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

void incidentsNumber(string malcode, vector<string> data)
{
    string t, p;
    p = malcode;

    outputFile << "Código: " << p << endl;

    for (int i=0; i<data.size(); i++){
        vector<int> incidents;
        t = data[i];
        string str = p + "$" + t;
        vector<int> zf = z_function(str);

        for (int i=0; i < zf.size(); i++){
            if (zf[i] == p.length())
            	incidents.push_back(i-p.length());
        }
        outputFile << "Transmission" << i + 1 << ".txt "
                   << "==> " << "#Incidencias: " << incidents.size() << " veces" << endl;
		if(incidents.size() != 0){
			outputFile << "			          Posiciones: ";
			for(int i=0; i<incidents.size()-1;i++){
				outputFile << incidents[i] << ", ";
			}
			outputFile << incidents[incidents.size()-1] << endl;
		}
    }
    outputFile << "--------------" << endl;

}

/* ============== LONGEST PALINDROME ===================
* Descrition: Functions that adds, at the end of the
* "checkinFile", the longest palindrome of the
* transmitted data as well as the position where
* it is located. To find the position of the palindrome
* we use the Manacher Algorithm (manacher function);
* and to write in the file, the longestPalindrome() function.
*
* - In charge: Diego Palma
*/

/*
* Function to find the longest palindrome and its position
* using the Manacher algorithm.
* Complex: O(n)
*/
s_palindrome manacher(string texto){

	s_palindrome lon_palindrome;

  	string T = "";
	int n = texto.length();
	for (int i=0; i<n; i++){
		T += "|";
		T += texto[i];
	}
	T += "|";
	n = T.length();
	vector<int> L(n);
	L[0] = 0;
	L[1] = 1;
	int maxLong=0, maxCentro=0;
	bool exp;
	for (int c=1, li=0, ri=2; ri<n; ri++){
		li = c-(ri-c);
		exp = false;
		if (c-maxLong <= ri && ri >= c+maxLong){
			if (L[li] < (c+L[c]-ri)){
				L[ri] = L[li];
			}
			else if(L[li] == (c + L[c]) - ri && (c + L[c]) == 2*n){
				L[ri] = L[li];
			}
			else if(L[li] == (c + L[c]) - ri && (c + L[c]) < 2*n){
				L[ri] = L[li];
				exp = true;
			}
			else if(L[li] > (c + L[c]) - ri && (c + L[c]) < 2*n){
				L[ri] = (c+L[c]) - ri;
				exp = true;
			}
		}
		else{
			L[ri] = 0;
			exp = true;
		}
		if (exp){
			while((ri + L[ri] < n) && (ri - L[ri] > 0) && (T[ri - L[ri] - 1] == T[ri + L[ri] + 1])){
				L[ri]++;
			}
		}
		c = ri;
		if (L[ri] > maxLong){
	    	maxLong = L[ri];
	    	maxCentro = ri;
	    }
	}
	int inicio = (maxCentro - maxLong) / 2;
	string salida = "";
	for (int i=inicio; i<(inicio+maxLong); i++){
		salida+=texto[i];
	}
    lon_palindrome.posicion = inicio;
    lon_palindrome.pal = salida;
	return lon_palindrome;
}

// Function to write in the checkingFile the longest
// palindrome
void longestPalindrome(vector<string> data){

	outputFile << endl;
    outputFile << "==========================================" << endl;
	outputFile << "--------- PALINDROMO MAS GRANDE ---------" << endl;
	outputFile << endl;
	outputFile << "----" << endl;

    s_palindrome lon_palindrome;
    for(int i=0; i<data.size(); i++){
        lon_palindrome = manacher(data[i]);
        outputFile << "Transmission" << i+1 <<".txt ==> Posición: " << lon_palindrome.posicion<< endl;
        outputFile << "			          Palindromo: " << lon_palindrome.pal << endl;
        outputFile << "----" << endl;  
    }
  
}

/* ================ LONGEST SUBSTRING ====================
* Descrition: Function that displays the longest substrings
* of each of the three combinations in the "outputFile"
* (t1-t2, t2-t3, t1-t3).
* - Complex: O(n*m)
* - In charge: Ivan ᕕ( ᐛ )ᕗ
*/

string lcs(string s1, string s2){

    int n = s1.length(), m = s2.length();
    int mat_lcs[n][m];
    int max = 0;
	int lcsStringx, lcsStringy;
	string lcsString="";

    for(int i=0; i<n; i++){
        if(s1[i] == s2[0]){
            mat_lcs[i][0] = 1;
            max = 1;
        }
        else{
            mat_lcs[i][0] = 0;
        }
    }

    for(int j=0; j<m; j++){
        if(s1[0] == s2[j]){
            mat_lcs[0][j] = 1;
            max = 1;
        }
        else{
            mat_lcs[0][j] = 0;
        }
    }    

    for(int i=1; i<n; i++){
        for(int j=1; j<m; j++){
            if(s1[i] == s2[j]){
                mat_lcs[i][j] = mat_lcs[i-1][j-1] +1;
                if (mat_lcs[i][j]>max){
                    max = mat_lcs[i][j];
					lcsStringx = i;
					lcsStringy = j;
                }
            }
            else{
                mat_lcs[i][j] = 0;
            } 
        }   
    }   

	for(int i=0; i<max; i++){
		lcsString += s2[lcsStringy-max+1];
		lcsStringy++;
	}

    return lcsString;
}

void likeness(vector<string> data){

  outputFile << endl;
  outputFile << "==========================================" << endl;
  outputFile << "--------- SUBSTRING MAS GRANDE ---------" << endl;
  outputFile << endl;
  
  outputFile << "Transmission 1 y 2: ==> Substring mas largo: " << lcs(data[0], data[1])<< endl;
  outputFile << "Transmission 1 y 3: ==> Substring mas largo: " << lcs(data[0], data[2]) << endl;
  outputFile << "Transmission 2 y 3: ==> Substring mas largo: " << lcs(data[1], data[2]) << endl;

}

// =============
// Main Function
// ============
int main() {

  vector<string> nameFile{"transmission1.txt", "transmission2.txt", "transmission3.txt"};
  vector<string> data = readFile(nameFile);
  string malCode;

  mcodeFile.open("mcode.txt");
  if (!mcodeFile) {
    cerr << "Error while opennig the file" << endl;
    return 1;
  }

  outputFile.open("test_checking.txt");
  outputFile << "--------- NUMERO DE INCIDENTES ---------" << endl;
  outputFile << endl;
  outputFile << "--------------" << endl;
  while(mcodeFile >> malCode){
    incidentsNumber(malCode, data);
  }
  longestPalindrome(data);
  likeness(data);
  outputFile.close();
  
  return 0;
}