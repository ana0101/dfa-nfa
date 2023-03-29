#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

ifstream fin("lambda_nfa.in");
ofstream fout("dfa.out");

int nr_stari_lnfa, nr_stari_dfa, nr_stari_finale_lnfa, nr_stari_finale_dfa, stare_initiala_lnfa;
vector <int> stari_finale_lnfa, stari_finale_dfa;
char cuvant[101];
vector <int> lnfa[10][5], lnfa2[10][5], dfa[10][5];
map <char, int> litere;
map <int, char> litere2;
vector <int> lambda_inchideri[10];
vector <vector <int>> stari_dfa;

bool egale(vector <int> v1, vector <int> v2)
{
    if (v1.size() != v2.size())
        return false;
    else
    {
        for (int i = 0; i < v1.size(); i++)
            if (v1[i] != v2[i])
                return false;
        return true;
    }
}

bool apartine(vector <int> v, int x)
{
    for (int i = 0; i < v.size(); i++)
        if (v[i] == x)
            return true;
    return false;
}

void calcul_lambda_inchideri(int stare)
{
    for (int i = 0; i < lnfa[stare][litere['l']].size(); i++)
    {
        int stareNoua = lnfa[stare][litere['l']][i];
        bool ok = apartine(lambda_inchideri[stare], stareNoua);
        if (!ok)
        {
            lambda_inchideri[stare].push_back(stareNoua);
            calcul_lambda_inchideri(stareNoua);
            for (int j = 0; j < lambda_inchideri[stareNoua].size(); j++)
                if (!apartine(lambda_inchideri[stare], lambda_inchideri[stareNoua][j]))
                    lambda_inchideri[stare].push_back(lambda_inchideri[stareNoua][j]);
        }
    }
}

int main()
{
    fin >> nr_stari_lnfa;
    fin >> stare_initiala_lnfa;
    fin >> nr_stari_finale_lnfa;
    int stare;
    // citesc starile finale
    for (int i = 0; i < nr_stari_finale_lnfa; i++)
    {
        fin >> stare;
        stari_finale_lnfa.push_back(stare);
    }

    int nr_litere;
    fin >> nr_litere;
    char litera;
    for (int i = 0; i < nr_litere; i++)
    {
        fin >> litera;
        litere[litera] = i;
        litere2[i] = litera;
    }
    litere['l'] = nr_litere; // lambda

    int s1, s2;
    while (fin >> s1 >> litera >> s2)
        lnfa[s1][litere[litera]].push_back(s2);

    // for (int i = 0; i < nr_stari_lnfa; i++)
    // {
    //     for (int j = 0; j < nr_litere; j++)
    //     {
    //         for (int k = 0; k < lnfa[i][j].size(); k++)
    //             cout << lnfa[i][j][k] << " ";
    //         cout << "   ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n\n";

    for (int stare = 0; stare < nr_stari_lnfa; stare++)
        lambda_inchideri[stare].push_back(stare);

    // calculam lambda inchiderile
    for (int stare = 0; stare < nr_stari_lnfa; stare++)
        calcul_lambda_inchideri(stare);

    // for(int i=0; i<nrStari; i++)
    // {
    //     for(int j=0; j<lambda_inchideri[i].size(); j++)
    //         cout << lambda_inchideri[i][j] << " ";
    //     cout << "\n";
    // }

    for (int stare = 0; stare < nr_stari_lnfa; stare++)
    {
        for (int i = 0; i < lambda_inchideri[stare].size(); i++)
        {
            int stare2 = lambda_inchideri[stare][i];
            for (int j = 0; j < nr_litere; j++)
            {
                for (int k = 0; k < lnfa[stare2][j].size(); k++)
                {
                    int stare3 = lnfa[stare2][j][k];
                    for (int l = 0; l < lambda_inchideri[stare3].size(); l++)
                    {
                        int stare4 = lambda_inchideri[stare3][l];
                        if (!apartine(lnfa2[stare][j], stare4))
                            lnfa2[stare][j].push_back(stare4);
                    }
                }
            }
        }
    }

    for (int i = 0; i < nr_stari_lnfa; i++)
        for (int j = 0; j < nr_litere; j++)
            sort(lnfa2[i][j].begin(), lnfa2[i][j].end());

    // for (int i = 0; i < nr_stari_lnfa; i++)
    // {
    //     for (int j = 0; j < nr_litere; j++)
    //     {
    //         for (int k = 0; k < lnfa2[i][j].size(); k++)
    //             cout << lnfa2[i][j][k] << " ";
    //         cout << "    ";
    //     }
    //     cout << "\n";
    // }

    sort(lambda_inchideri[stare_initiala_lnfa].begin(), lambda_inchideri[stare_initiala_lnfa].end());
    stari_dfa.push_back(lambda_inchideri[stare_initiala_lnfa]);
    int nr_stari_dfa = stari_dfa.size();

    for (int i = 0; i < nr_stari_dfa; i++)
    {
        for (int j = 0; j < nr_litere; j++)
        {
            vector <int> stari;
            for (int k = 0; k < stari_dfa[i].size(); k++)
            {
                int stare = stari_dfa[i][k];
                for (int l = 0; l < lnfa2[stare][j].size(); l++)
                {
                    int stare_noua = lnfa2[stare][j][l];
                    if (!apartine(stari, stare_noua))
                        stari.push_back(stare_noua);
                }
            }
            sort(stari.begin(), stari.end());
            // verificam daca aceasta stare exista deja in starile dfa
            bool apartine = 0;
            for (int k = 0; k < stari_dfa.size(); k++)
                if (egale(stari, stari_dfa[k]) == true)
                {
                    apartine = 1;
                    dfa[i][j].push_back(k);
                }
            // daca nu apartine o adaugam
            if (!apartine)
            {
                stari_dfa.push_back(stari);
                nr_stari_dfa++;
                dfa[i][j].push_back(nr_stari_dfa - 1);
            }
        }
    }

    // for (int i = 0; i < stari_dfa.size(); i++)
    // {
    //     for (int j = 0; j < stari_dfa[i].size(); j++)
    //         cout << stari_dfa[i][j];
    //     cout << "\n";
    // }

    // cout << "\n";
    // for (int i = 0; i < nr_stari_dfa; i++)
    // {
    //     for (int j = 0; j < nr_litere; j++)
    //         cout << dfa[i][j][0] << " ";
    //     cout << "\n";
    // }

    // afisam numarul de stari pentru dfa si starea initiala
    fout << stari_dfa.size() << "\n";
    fout << 0 << "\n";

    // aflam starile finale pentru dfa
    for (int i = 0; i < nr_stari_dfa; i++)
    {
        for (int j = 0; j < stari_dfa[i].size(); j++)
        {
            int stare = stari_dfa[i][j];
            if (apartine(stari_finale_lnfa, stare))
            {
                stari_finale_dfa.push_back(i);
                break;
            }
        }
    }

    // afisam starile finale pentru dfa 
    fout << stari_finale_dfa.size() << "\n";
    for (int i = 0; i < stari_finale_dfa.size(); i++)
        fout << stari_finale_dfa[i] << " ";
    fout << "\n";

    // afisam tranzitiile pentru dfa
    for (int i = 0; i < nr_stari_dfa; i++)
    {
        for (int j = 0; j < nr_litere; j++)
        {
            fout << i << " ";
            fout << litere2[j] << " ";
            fout << dfa[i][j][0] << "\n";
        }
    }

    fin.close();
    fout.close();

    return 0;
}