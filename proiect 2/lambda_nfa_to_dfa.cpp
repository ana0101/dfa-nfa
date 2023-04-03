#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

ifstream fin("lambda_nfa4.in");
ofstream fout("dfa.out");

int nr_stari_lnfa, nr_stari_dfa, nr_stari_finale_lnfa, nr_stari_finale_dfa, stare_initiala_lnfa;
vector <int> stari_finale_lnfa, stari_finale_dfa;
char cuvant[101];
vector <int> lnfa[10][5], nfa[10][5], dfa[10][5];
map <char, int> litere;
map <int, char> litere2;
vector <int> lambda_inchideri[10];
vector <vector <int>> stari_dfa;

// verifica daca doi vectori sunt egali
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

// verifica daca o valoare se afla intr-un vector
bool apartine(vector <int> v, int x)
{
    for (int i = 0; i < v.size(); i++)
        if (v[i] == x)
            return true;
    return false;
}

void calcul_lambda_inchideri(int stare)
{
    // parcurge toate starile in care se ajunge cu lambda din starea curenta
    for (int i = 0; i < lnfa[stare][litere['l']].size(); i++)
    {
        int stareNoua = lnfa[stare][litere['l']][i];
        bool ok = apartine(lambda_inchideri[stare], stareNoua);
        // daca starea nu se afla deja in lambda inchiderea starii
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

    // punem starea insasi in lambda inchiderea starii
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

    // parcurge toate starile
    for (int stare = 0; stare < nr_stari_lnfa; stare++)
    {
        // parcurge starile din lambda inchiderea starii
        for (int i = 0; i < lambda_inchideri[stare].size(); i++)
        {
            int stare2 = lambda_inchideri[stare][i];
            for (int j = 0; j < nr_litere; j++)
            {
                // parcurge toate starile in care se ajunge cu o anumita litera din starea din lambda inchidere
                for (int k = 0; k < lnfa[stare2][j].size(); k++)
                {
                    int stare3 = lnfa[stare2][j][k];
                    // parcurge toate starile din lambda inchiderea starii de mai sus
                    for (int l = 0; l < lambda_inchideri[stare3].size(); l++)
                    {
                        int stare4 = lambda_inchideri[stare3][l];
                        if (!apartine(nfa[stare][j], stare4))
                            nfa[stare][j].push_back(stare4);
                    }
                }
            }
        }
    }

    for (int i = 0; i < nr_stari_lnfa; i++)
        for (int j = 0; j < nr_litere; j++)
            sort(nfa[i][j].begin(), nfa[i][j].end());

    // for (int i = 0; i < nr_stari_lnfa; i++)
    // {
    //     for (int j = 0; j < nr_litere; j++)
    //     {
    //         for (int k = 0; k < nfa[i][j].size(); k++)
    //             cout << nfa[i][j][k] << " ";
    //         cout << "    ";
    //     }
    //     cout << "\n";
    // }

    sort(lambda_inchideri[stare_initiala_lnfa].begin(), lambda_inchideri[stare_initiala_lnfa].end());
    stari_dfa.push_back(lambda_inchideri[stare_initiala_lnfa]);
    int nr_stari_dfa = stari_dfa.size();

    // vedem cate stari sunt in dfa
    for (int i = 0; i < nr_stari_dfa; i++)
    {
        for (int j = 0; j < nr_litere; j++)
        {
            vector <int> stari;
            for (int k = 0; k < stari_dfa[i].size(); k++)
            {
                int stare = stari_dfa[i][k];
                for (int l = 0; l < nfa[stare][j].size(); l++)
                {
                    int stare_noua = nfa[stare][j][l];
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
            if (!apartine && stari.size() > 0)
            {
                stari_dfa.push_back(stari);
                nr_stari_dfa++;
                dfa[i][j].push_back(nr_stari_dfa - 1);
            }
        }
    }

    // cout << nr_stari_dfa << "\n";

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
    fout << "nr stari: " << stari_dfa.size() << "\n";
    fout << "stare initiala: " << 0 << "\n";

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
    fout << "nr stari finale: "<< stari_finale_dfa.size() << "\n";
    fout << "stari finale: ";
    for (int i = 0; i < stari_finale_dfa.size(); i++)
        fout << stari_finale_dfa[i] << " ";
    fout << "\n";

    // afisam tranzitiile pentru dfa
    fout << "tranzitii: \n";
    for (int i = 0; i < nr_stari_dfa; i++)
    {
        for (int j = 0; j < nr_litere; j++)
        {
            if (dfa[i][j].size() > 0)
            {
                fout << i << " ";
                fout << litere2[j] << " ";
                fout << dfa[i][j][0] << "\n";
            }
        }
    }

    fin.close();
    fout.close();

    return 0;
}