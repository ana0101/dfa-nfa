#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

ifstream in("dfa1.in");
ofstream out("dfa.out");

int tranzitii[20][5];
int stari_finale[20];
map <char, int> litere;
int nr_stari, stare_init, nr_stari_finale, nr_litere;
vector <vector<int>> partitii;
int part[20];
bool accesibil[20];

int tranzitii2[20][5];
int stari_finale2[20];
int nr_stari2, stare_init2, nr_stari_finale2, nr_stari22;


void citire() {
    in >> nr_stari >> stare_init >> nr_stari_finale;

    for (int i = 0; i < nr_stari_finale; i ++) {
        in >> stari_finale[i];
    }

    in >> nr_litere;
    char litera;
    for (int i = 0; i < nr_litere; i ++) {
        in >> litera;
        litere[litera] = i;
    }

    int stare1, stare2;
    for (int i = 0; i < nr_stari * nr_litere; i ++) {
        in >> stare1 >> litera >> stare2;
        tranzitii[stare1][litere[litera]] = stare2;
    }
}

void afisare() {
    cout << "Nr stari: " << nr_stari << "\n";
    cout << "Stare init: " << stare_init << "\n";
    cout << "Nr stari finale: " << nr_stari_finale << "\n";
    cout << "Stari finale: ";
    for (int i = 0; i < nr_stari_finale; i ++)
        cout << stari_finale[i] << " ";
    cout << "\n";
    cout << "Nr litere: " << nr_litere << "\n";
    cout << "Litere: " << litere['a'] << " " << litere['b'] << "\n";
    cout << "Tranzitii: \n";
    for (int i = 0; i < nr_stari; i ++) {
        for (int j = 0; j < nr_litere; j ++) {
            cout << i << " " << j << " " << tranzitii[i][j] << "\n";
        }
    }
}

void afisare2() {
    cout << "Nr stari: " << nr_stari22 << "\n";
    cout << "Stare init: " << stare_init2 << "\n";
    cout << "Nr stari finale: " << nr_stari_finale2 << "\n";
    cout << "Stari finale: ";
    for (int i = 0; i < nr_stari_finale2; i ++)
        cout << stari_finale2[i] << " ";
    cout << "\n";
    cout << "Tranzitii: \n";
    for (int i = 0; i < nr_stari2; i ++) {
        for (int j = 0; j < nr_litere; j ++) {
            if (tranzitii2[i][j] != -1)
                cout << i << " " << j << " " << tranzitii2[i][j] << "\n";
        }
    }
}

// verifica daca un element se afla intr-un vector
bool apartine(int x, int v[], int n) {
    for (int i = 0; i < n; i ++)
        if (x == v[i])
            return 1;
    return 0;
}

void elim_stari_inaccesibile(int stare1, bool accesibil[20]) {
    // calculam in accesibil starile in care se poate ajunge
    for (int i = 0; i < nr_litere; i ++) {
        int stare2 = tranzitii[stare1][i];
        if (!accesibil[stare2]) {
            accesibil[stare2] = true;
            // cout << "ok " << stare2 << "\n";
            elim_stari_inaccesibile(stare2, accesibil);
        }
    }
}

// verifica daca o stare e finala
bool finala(int stare) {
    for (int i = 0; i < nr_stari_finale; i ++) {
        if (stare == stari_finale[i])
            return true;
    }
    return false;
}

// primele doua partitii: stari nefinale, stari finale
void init_partitii() {
    for (int i = 0; i < nr_stari; i ++) {
        if (accesibil[i]) {
            if (!finala(i)) {
                if (!partitii.empty()) {
                    partitii[0].push_back(i);
                }
                else {
                    partitii.push_back({i});
                }
                part[i] = 0;
            }
            else {
                if (partitii.size() > 1) {
                    partitii[1].push_back(i);
                }
                else {
                    partitii.push_back({i});
                }
                part[i] = 1;
            }
        }
    }
}

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


void calcul_partitii() {
    bool ok = false;
    while (!ok) {
        ok = true;
        vector <vector <pair <vector<int>, vector<int>>>> comb; // 0: ((A0, A0, B0), (q0, q1, q2, q3)) pt fiecare partitie
        comb = {};

        // for (int i = 0; i < partitii.size(); i ++) {
        //     for (int j = 0; j < partitii[i].size(); j ++) {
        //         cout << partitii[i][j] << " ";
        //     }
        //     cout << "\n";
        // }
        // cout << "\n";

        for (int i = 0; i < partitii.size(); i ++) {
            comb.push_back({});

                for (int j = 0; j < partitii[i].size(); j ++) {
                    int stare1 = partitii[i][j];
                    vector <int> part2; // partitiile in care ajunge cu fiecare litera
                    for (int k = 0; k < nr_litere; k ++) {
                        int stare2 = tranzitii[stare1][k];
                        if (accesibil[stare2])
                            part2.push_back(part[stare2]);
                        else
                            part2.push_back(-1);
                    }

                    // verificam daca combinatia de stari e noua sau nu
                    bool exista = false;
                    if (!comb[i].empty())
                        for (int k = 0; k < comb[i].size() && !exista; k ++) {
                            if (egale(part2, comb[i][k].first)) {
                                // daca exista deja, adaugam starea la combinatie
                                exista = true;
                                comb[i][k].second.push_back(stare1);
                            }
                        }

                    // daca nu exista, facem una
                    if (!exista) {
                        comb[i].push_back({part2, {stare1}});
                    }
                }
        }

        // vedem cate combinatii exista la fiecare partitie si astfel facem partitii noi
        for (int i = 0; i < comb.size(); i ++) {
            // daca exista mai mult de o combinatie, facem partitii noi
            if (comb[i].size() > 1) {
                ok = false;
                for (int j = 1; j < comb[i].size(); j ++) {
                    // facem partitia noua
                    partitii.push_back(comb[i][j].second);
                    // stergem starile din partitia veche
                    for (int k = 0; k < comb[i][j].second.size(); k ++) {
                        int stare = comb[i][j].second[k];
                        // cout << stare << " " << part[stare] << "\n";
                        vector<int>::iterator new_end;
                        new_end = remove(partitii[part[stare]].begin(), partitii[part[stare]].end(), stare);
                        partitii[part[stare]].pop_back();
                        // for (int l = 0; l < partitii[0].size(); l ++)
                        //     cout << partitii[0][l] << " ";
                        // cout << "\n";
                        part[stare] = partitii.size() - 1;
                    }
                }
            }
        }

        // daca nu au fost adaugate partitii noi, facem graful nou
        if (ok) {
            nr_stari2 = partitii.size();
            for (int i = 0; i < partitii.size(); i ++) {
                for (int j = 0; j < partitii[i].size(); j ++) {
                    if (partitii[i][j] == stare_init) 
                        stare_init2 = i;
                    if (finala(partitii[i][j]) && !apartine(i, stari_finale2, nr_stari_finale2)) {
                        stari_finale2[nr_stari_finale2] = i;
                        nr_stari_finale2 ++;
                    }
                }     
            }
            // cout << "ok";

            for (int i = 0; i < nr_stari2; i ++) {
                for (int j = 0; j < nr_litere; j ++) {
                    tranzitii2[i][j] = comb[i][0].first[j];
                }
            }
        }
    }
}


void parcurgere(int stare, bool vizitat[]) {
    for (int i = 0; i < nr_litere; i ++) {
        int stare2 = tranzitii2[stare][i];
        if (!vizitat[stare2]) {
            vizitat[stare2] = 1;
            parcurgere(stare2, vizitat);
        }
    }
}


void elim_inutile() {
    bool util[20]; 
    // plecam din fiecare stare
    for (int i = 0; i < nr_stari2; i ++) {
        bool vizitat[20] = {0}; // 1 daca am ajuns, 0 daca nu
        parcurgere(i, vizitat);

        // vedem daca se ajunge in macar o stare finala
        bool ok = 0;
        for (int j = 0; j < nr_stari_finale2; j ++) {
            if (vizitat[stari_finale2[j]])
                ok = 1;
        }

        if(ok)
            util[i] = 1;
    }

    for (int i = 0; i < nr_stari2; i ++) {
        if (!util[i]) {
            // stergem starea din tranzitii
            for (int j = 0; j < nr_litere; j ++) {
                tranzitii2[i][j] = -1;
            }
            for (int j = 0; j < nr_stari2; j ++) {
                for (int k = 0; k < nr_litere; k ++) {
                    if (tranzitii2[j][k] == i)
                        tranzitii2[j][k] = -1;
                }
            }
            // scadem numarul de stari
            nr_stari22 --;
        }
    }
}


int main() {
    citire();
    afisare();
    accesibil[stare_init] = 1;
    elim_stari_inaccesibile(stare_init, accesibil);
    init_partitii();
    calcul_partitii();
    nr_stari22 = nr_stari2;
    for (int i = 0; i < partitii.size(); i ++) {
        for (int j = 0; j < partitii[i].size(); j ++) {
            cout << partitii[i][j] << " ";
        }
        cout << "\n";
    }

    elim_inutile();

    afisare2();

    // for (int i = 0; i < 20; i ++)
    //     cout << accesibil[i] << " ";

    return 0;
}