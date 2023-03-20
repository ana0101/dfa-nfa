#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <deque>
using namespace std;

ifstream fin1("graf.in");
ifstream fin2("cuvant.in");

int drum[101], nrStariFinale, stariFinale[11];
char cuvant[101];
vector <int> matrice[11][5];
map <char, int> litere;
bool acceptat = false;


void parcurgere(int nrLit)
{
    char litera = cuvant[nrLit-1];
    int indiceLitera = litere[litera];
    int stareCurenta = drum[nrLit-1];
    if(matrice[stareCurenta][indiceLitera].size() != 0)
    {
        for(int j=0; j<matrice[stareCurenta][indiceLitera].size(); j++)
        {
            drum[nrLit] = matrice[stareCurenta][indiceLitera][j];
            if(nrLit == strlen(cuvant))
            {
                for(int i=0; i<nrStariFinale; i++)
                    if(drum[nrLit] == stariFinale[i])
                    {
                        if(acceptat == false)
                        {
                            acceptat = true;
                            cout << "Cuvantul este acceptat\nDrumuri:\n";
                        }
                        for(int k=0; k<=nrLit; k++)
                            cout << drum[k] << " ";
                        cout << "\n";
                        break;
                    }
            }
            else
                parcurgere(nrLit+1);
        }
    }
}


int main()
{
    int nrStari, stareInitiala;
    fin1 >> nrStari;
    fin1 >> stareInitiala;
    fin1 >> nrStariFinale;
    for(int i=0; i<nrStariFinale; i++)
        fin1 >> stariFinale[i];

    int nrLitere;
    fin1 >> nrLitere;
    char litera;
    for(int i=0; i<nrLitere; i++)
    {
        fin1 >> litera;
        litere[litera] = i;
    }

    int s1, s2;
    while(fin1 >> s1 >> litera >> s2)
        matrice[s1][litere[litera]].push_back(s2);

    /*for(int i=0; i<nrStari; i++)
    {
        for(int j=0; j<nrLitere; j++)
            for(int k=0; k<matrice[i][j].size(); k++)
                cout << matrice[i][j][k] << " ";
        cout << "\n";
    }*/

    fin2 >> cuvant;
    cout << cuvant << "\n";

    if(strcmp(cuvant, "") == 0)
    {
        acceptat = false;
        for(int i=0; i<nrStariFinale; i++)
            if(stareInitiala == stariFinale[i])
                acceptat = true;

        if(acceptat == true)
            cout << "Cuvantul este acceptat\n" << "Drum: " << stareInitiala;
        else    
            cout << "Cuvantul nu este acceptat";
    }
    else
    {
        drum[0] = stareInitiala;
        parcurgere(1);

        if(acceptat == false)
            cout << "Cuvantul nu este acceptat";
    }

    fin1.close();
    fin2.close();

    return 0;
}