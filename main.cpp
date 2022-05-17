#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <chrono>
#include <random>
#include <ctime>
using namespace std;


int random_number(){
    return ((double)rand() / RAND_MAX) * (999999 - 100000) + 100000;
}

struct Kadastr{
    int id;
    string SNT_adress;
};
struct KadastrTable{
    Kadastr *kadastr;
    int size;
    int append(Kadastr note);
    void print();

    Kadastr LinearSearchBarrier(int id);
    Kadastr BruteForce(int id);
    ~KadastrTable();
};

Kadastr KadastrTable::LinearSearchBarrier(int id){
    if(size == 0) {
        return Kadastr{0, "NO NAME"};
    }
    Kadastr last = kadastr[size-1];
    kadastr[size-1].id = id;
    int i;
    for(i=0; kadastr[i].id!= id;++i){}
    kadastr[size-1] = last;
    if(i != size-1 || id == last.id){
        return kadastr[i];
    }
    return Kadastr{0, "NO NAME"};
}
Kadastr KadastrTable::BruteForce(int id){
    for(int i = 0; i < size; i++){
        if(kadastr[i].id == id){
            return kadastr[i];
        }
    }
    return Kadastr{-1, "NULL"};
}

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

//Kadastr brute_force( const KadastrTable& kadastr_table, int id) {
//    for (int i = 0; i < kadastr_table.size; i++) {
//        if (kadastr_table.kadastr[i].id == id) {
//            return kadastr_table.kadastr[i];
//        }
//    }
//    return Kadastr{0, "NO NAME"};
//}
//
//Kadastr linear_search_barrier(const KadastrTable& kadastr_table, int id) {
//    if(kadastr_table.size == 0) {
//        return Kadastr{0, "NO NAME"};
//    }
//    Kadastr last = kadastr_table.kadastr[kadastr_table.size-1];
//    kadastr_table.kadastr[kadastr_table.size-1].id = id;
//    int i;
//    for(i=0; kadastr_table.kadastr[i].id!= id;++i){}
//    kadastr_table.kadastr[kadastr_table.size-1] = last;
//    if(i != kadastr_table.size-1 || id == last.id){
//        return kadastr_table.kadastr[i];
//    }
//    return Kadastr{0, "NO NAME"};
//}
//==================HASH TABLE==========================
struct HashTable{
    const int start_size = 1000000;
    HashTable();
    Kadastr *kadastr;
    int load=0;
    int size;
    double calculate_load_factor();
    int add(Kadastr note);
    Kadastr get(int id);
    ~HashTable();
};
HashTable::HashTable() {
    kadastr = new Kadastr[start_size];
    size = start_size;
}
int HashFunction(int id) {
    return id % 1000000;
}
double HashTable::calculate_load_factor() {
    return (double)load/size;
}
int HashTable::add(Kadastr note) {

    if(calculate_load_factor() > 0.7) {
        Kadastr *tmp = new Kadastr[size*2];
        for (int i = 0; i < size; i++) {
            tmp[i] = kadastr[i];
        }
        delete[] kadastr;
        kadastr = tmp;
        size *= 2;
    }

    int index = HashFunction(note.id);
    if(kadastr[index].id == 0) {
        kadastr[index] = note;
        load++;
        return 1;
    }
    else {
        int i = index;
        while(kadastr[i].id != 0) {
            i++;
            if(i == size) {
                i = 0;
            }
            if(i == index) {
                return 0;
            }
        }
        kadastr[i] = note;
        load++;
        return 1;
    }
}

HashTable::~HashTable() {
    delete[] kadastr;
}

Kadastr HashTable::get(int id) {
    int index = HashFunction(id);
    if(kadastr[index].id == id) {
        return kadastr[index];
    }
    else {
        int i = index;
        while(kadastr[i].id != id) {
            i++;
            if(i == size) {
                i = 0;
            }
            if(i == index) {
                return Kadastr{0, "NO NAME"};
            }
        }
        return kadastr[i];
    }
}

int main() {
    srand(time(NULL));
    for(int i = 100, d=0; i<=10000;i = d%2==0 ? i*=10 : i+=0){
        d++;
        KadastrTable kadastr_table{};
        HashTable hash_table{};
        for(int j = 0; j<i; j++){
            int id = random_number();
            kadastr_table.append(Kadastr{id, "SNT_" + to_string(id)});
            hash_table.add(Kadastr{id, "SNT_" + to_string(id)});
        }
        int random_search =kadastr_table.kadastr[rand()%i].id;
        cout<<"Random note: "<<random_search<<endl;
        int search_id = random_search;
        //cin>>search_id;


        auto start = chrono::steady_clock::now();
        kadastr_table.BruteForce(search_id);
        auto end = chrono::steady_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        cout<<"Brute force(" << i << "): "<< diff <<" ns"<<endl;

        start = chrono::steady_clock::now();
        kadastr_table.LinearSearchBarrier(search_id);
        end = chrono::steady_clock::now();
        diff = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        cout<<"Linear search barrier(" << i << "): "<<diff<<" ns"<<endl;


        Kadastr temp;
        start = chrono::steady_clock::now();
        temp = hash_table.get(search_id);
        end = chrono::steady_clock::now();
        diff = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        cout<<"Hash table(" << i << "): "<< diff <<" ns"<<endl;
    }
    return 0;
}
