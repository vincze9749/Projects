#include <iostream>
#include <fstream>
#include <map>

#define lastseat 20
using namespace std;

class bus {
    int resnum=1000;
    static long lastresnum;
    int seatnum=0;
    string name;
    int numofseats;
public:
    bus() {};
    bus(string name, int seatnum);
    long getresnum() { return resnum; };
    int getseatnum() { return seatnum; };

    static long getlastresnum();
    static void setlastresnum(long num);

    bool search(int seat);

    friend ofstream& operator<<(ofstream& o, bus& b);
    friend ifstream& operator>>(ifstream& i, bus& b);
    friend ostream& operator<<(ostream& o, bus& b);
};


long bus::lastresnum = 1000;

class menu {
    multimap <long, bus> seats;
public:
    menu();
    void info();
    void checkreservations();
    void res(string name, int seatnum);
    void multipleres(string name, int seatnum);
    void deleteres(long resnum);
    void showseats();
    bool search(int seatnum);

    ~menu();
};

bus::bus(string name, int seatnum) {
    lastresnum++;
    this->seatnum = seatnum;
    resnum = lastresnum;
    this->name = name;
}

long bus::getlastresnum() {
    return lastresnum;
}

void bus::setlastresnum(long num) {
    lastresnum = num;
}

bool bus::search(int seat) {
    if (seat == seatnum) { return 1; }
    else return 0;
}

ofstream& operator<<(ofstream& o, bus& b) {
    o << b.resnum << endl << b.name << endl << b.seatnum << endl;
    return o;
}
ifstream& operator>>(ifstream& i, bus& b) {
    i >> b.resnum >> b.name >> b.seatnum;
    return i;
}
ostream& operator<<(ostream& o, bus& b) {
    o << b.resnum << endl << b.name << endl << b.seatnum << endl;
    return o;
}

menu::menu() {
    bus b;
    ifstream inf;
    inf.open("bus.data");

    if (!inf) {
        cout << "File not found!" << endl;
        return;
    }
    while (inf >> b) {
        seats.insert(pair<long, bus>(b.getresnum(), b));
    }
    bus::setlastresnum(b.getresnum());
    inf.close();
}

void menu::info() {
    cout << "Bus information..." << endl;
}

void menu::checkreservations() {
    multimap <long, bus>::iterator itr;
    for (itr = seats.begin(); itr != seats.end(); itr++) {
        cout << itr->second << endl;
    }
}

void menu::res(string name, int seatnum) {
    bus b(name, seatnum);
    ofstream ouf;
    ouf.open("bus.data");
    seats.insert(pair<long, bus>(b.getresnum(), b));
    multimap <long, bus>::iterator itr;
    for (itr = seats.begin(); itr != seats.end(); itr++) {
        ouf << itr->second;
    }
    ouf.close();
}
void menu::multipleres(string name, int seatnum) {
    bus b(name, seatnum);
    ofstream ouf;
    ouf.open("bus.data");
    seats.insert(pair<long, bus>(b.getresnum(), b));
    multimap <long, bus>::iterator itr;
    for (itr = seats.begin(); itr != seats.end(); itr++) {
        ouf << itr->second;
    }
    bus::setlastresnum(b.getresnum()-1);
    ouf.close();
}

void menu::deleteres(long resnum) {
    seats.erase(resnum);
    cout << "Reservation is deleted" << endl;
}

void menu::showseats() {
    cout << "Reserved seats are marked with X " << endl;
    int found = 0;
    multimap <long, bus>::iterator itr;
    for (int i = 1; i <= lastseat; i++) {
        if ((i - 1) % 2 == 0 && (i - 1) % 4 != 0) {cout << "\t"; }
        if ((i - 1) % 4 == 0) { cout << "\n"; }
        if (search(i) == 1) cout << "X ";
        else cout << i << " ";
        found=0;
        if (i == lastseat) cout << endl;
    }
}

bool menu::search(int seatnum) {
    multimap <long, bus>::iterator itr;
    for (itr = seats.begin(); itr != seats.end(); itr++) {
        if (itr->second.search(seatnum) == 1) return 1;
    }
    return 0;
}

menu::~menu() {
    ofstream ouf;
    ouf.open("bus.data");
    multimap <long, bus>::iterator itr;
    for (itr = seats.begin(); itr != seats.end(); itr++) {
        ouf << itr->second;
    }
    ouf.close();
}

void reservationchoice(int numofseats, menu& m) {
    string name;
    int seatnum;
    multimap <long, bus>::iterator itr;
    if (numofseats == 1) {
        cout << "Enter name" << endl;
        cin >> name;
        cout << "Enter chosen seat number" << endl;
        do {
            cin >> seatnum;
            if (m.search(seatnum) == 1 || seatnum>lastseat || seatnum < 1) cout << "Chosen seat is taken or non-existent, please chose an other one" << endl;
        } while (m.search(seatnum) == 1 || seatnum > lastseat || seatnum < 1);
        m.res(name,seatnum);
    }
    else {
        for (int i = 1; i <= numofseats; i++) {
            cout << "Enter name" << endl;
            cin >> name;
            cout << "Enter chosen seat number" << endl;
            do {
                cin >> seatnum;
                if (m.search(seatnum) == 1 || seatnum > lastseat || seatnum < 1) cout << "Chosen seat is taken or non-existent, please chose an other one" << endl;
            } while (m.search(seatnum) == 1 || seatnum > lastseat || seatnum < 1);
            m.multipleres(name,seatnum);
        }
    }
}



int main()
{
    bus b;
    menu m;
    int choice;
    int numofseats;
    string name;
    long resnum;
    cout << "\t\t BUS RESERVATION" << endl;
    
    do {
        cout << "\t\t Chose one of the options below:" << endl << "\t1. Check bus information" << endl
            << "\t2. Check reservations" << endl << "\t3. Reserve seats" << endl << "\t4. Delete reservation" << endl<<"\t5. Show seats"<<endl << "\t6. Exit" << endl;
        cin >> choice;

        switch (choice) {
        case 1:m.info();
            break;
        case 2:m.checkreservations();
            break;
        case 3:
            m.showseats();
            cout << "Enter number of seats to reserve" << endl;
            cin>>numofseats;
            reservationchoice(numofseats, m);
            break;
        case 4:
            cout << "Enter your reservation number:" << endl;
            cin >> resnum;
            m.deleteres(resnum);
            break;
        case 5:
            m.showseats();
        case 6: break;
        default: cout << "Please enter a valid option" << endl;
        }

    } while (choice != 6);
}
