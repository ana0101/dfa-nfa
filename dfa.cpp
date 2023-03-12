#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
using namespace std;

ifstream fin1("graf.in");
ifstream fin2("cuvant.in");

int main()
{
    int nrStari, nrStariFinale;
    fin1 >> nrStari;
    fin1 >> nrStariFinale;
    int stariFinale[nrStariFinale+1];
    for(int i=0; i<nrStariFinale; i++)
        fin1 >> stariFinale[i];

    int nrLitere;
    fin1 >> nrLitere;
    char litera;
    map <char, int> litere;
    for(int i=0; i<nrLitere; i++)
    {
        fin1 >> litera;
        litere[litera] = i;
    }

    int matrice[nrStari+2][nrLitere+2];
    for(int i=0; i<nrStari; i++)
        for(int j=0; j<nrLitere; j++)
            matrice[i][j] = -1;

    int s1, s2;
    while(fin1 >> s1 >> litera >> s2)
        matrice[s1][litere[litera]] = s2;

    /*for(int i=0; i<nrStari; i++)
    {
        for(int j=0; j<nrLitere; j++)
            cout << matrice[i][j] << " ";
        cout << "\n";
    }*/

    char cuvant[101];
    fin2 >> cuvant;
    
    bool acceptat = true;
    int n = strlen(cuvant), stare = 0, drum[n+1];
    drum[0] = 0;
    for(int i=1; i<=n; i++)
    {
        if(matrice[stare][litere[cuvant[i-1]]] != -1)
        {
            stare = matrice[stare][litere[cuvant[i-1]]];
            drum[i] = stare;
        }
        else
        {
            acceptat = false;
            break;
        }
    }

    if(acceptat == true)
    {
        acceptat = false;
        for(int i=0; i<nrStariFinale; i++)
            if(stariFinale[i] == stare)
            {
                acceptat = true;
                break;
            }
    }

    if(acceptat == true)
    {
        cout << "Cuvantul este acceptat" << "\n" << "Drum: ";
        for(int i=0; i<=n; i++)
            cout << drum[i] << " ";
    }
    else
        cout << "Cuvantul nu este acceptat";

    fin1.close();
    fin2.close();

    return 0;
}