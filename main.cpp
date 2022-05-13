#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <chrono>
using namespace std;

struct Kadastr{
    int id;
    string SNT_adress;
};
struct KadastrTable{
    Kadastr *kadastr;
    int size;
    int append(Kadastr note);
    void print();
    ~KadastrTable();
};

int KadastrTable::append(Kadastr note) {
    Kadastr *tmp = new Kadastr[size+ 1];
    //memcpy(tmp, kadastr, size * sizeof(Kadastr));
    for (int i = 0; i < size; i++) {
        tmp[i] = kadastr[i];
    }
    size++;
    tmp[size - 1] = note;
    delete[] kadastr;
    kadastr = tmp;
    return 1;
}
void KadastrTable::print() {
    for (int i = 0; i < size; i++) {
        cout << kadastr[i].id << " " << kadastr[i].SNT_adress << endl;
    }
}
KadastrTable::~KadastrTable() {
    delete[] kadastr;
}

Kadastr brute_force( const KadastrTable& kadastr_table, int id) {
    for (int i = 0; i < kadastr_table.size; i++) {
        if (kadastr_table.kadastr[i].id == id) {
            return kadastr_table.kadastr[i];
        }
    }
    return Kadastr{0, "NO NAME"};
}

Kadastr linear_search_barrier(const KadastrTable& kadastr_table, int id) {
    if(kadastr_table.size == 0) {
        return Kadastr{0, "NO NAME"};
    }
    Kadastr last = kadastr_table.kadastr[kadastr_table.size-1];
    kadastr_table.kadastr[kadastr_table.size-1].id = id;
    int i;
    for(i=0; kadastr_table.kadastr[i].id!= id;i++){}
    kadastr_table.kadastr[kadastr_table.size-1] = last;
    if(i != kadastr_table.size-1 || id == last.id){
        return kadastr_table.kadastr[i];
    }
    return Kadastr{0, "NO NAME"};
}

int main() {
    srand(time(NULL));
    for(int i = 100; i<=10000;i*=10){
        KadastrTable kadastr_table{};
        for(int j = 0; j<i; j++){
            kadastr_table.append(Kadastr{j, "SNT_" + to_string(j)});
        }
        cout<<"Random note: "<<kadastr_table.kadastr[rand()%i].id<<endl;
        int search_id;
        cin>>search_id;

        auto start = chrono::steady_clock::now();
        brute_force(kadastr_table, search_id);
        auto end = chrono::steady_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end-start).count();
        cout<<"Brute force(" << i << "): "<< diff <<" mks"<<endl;

        auto start1 = chrono::steady_clock::now();
        cout<<linear_search_barrier(kadastr_table, search_id).SNT_adress<<endl;
        auto end1 = chrono::steady_clock::now();
        auto diff1 = end1 - start1;
        cout<<"Linear search barrier(" << i << "): "<<chrono::duration<double, micro>(diff1).count()<<" mks"<<endl;

    }
    return 0;
}
