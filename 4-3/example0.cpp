#include <string.h>
#include <iostream>

class PhotonCannon {
    int hp, shield;
    int x, y;
    int damage;
    char* name;

   public:
    PhotonCannon(int _x, int _y);
    PhotonCannon(int _x, int _y, const char* _name);
    PhotonCannon(const PhotonCannon& photon_cannon);
    void print_status();
    ~PhotonCannon();
};

PhotonCannon::PhotonCannon(int _x, int _y) {
    std::cout << "Init call: parameters x y" << std::endl;
    hp = shield = 100;
    x = _x;
    y = _y;
    damage = 20;
    name = NULL;
}

PhotonCannon::PhotonCannon(int _x, int _y, const char* _name) {
    std::cout << "Init call: parameters x y name" << std::endl;
    hp = shield = 100;
    x = _x;
    y = _y;
    damage = 20;
    if (_name) {
        name = new char[strlen(_name) + 1];
        strcpy(name, _name);
    } else {
        name = NULL;
    }
}

PhotonCannon::PhotonCannon(const PhotonCannon& photon_cannon) {
    std::cout << "Init call: copy" << std::endl;
    hp = photon_cannon.hp;
    shield = photon_cannon.shield;
    x = photon_cannon.x;
    y = photon_cannon.y;
    damage = photon_cannon.damage;
    if (photon_cannon.name) {
        name = new char[strlen(photon_cannon.name) + 1];
        strcpy(name, photon_cannon.name);
    } else {
        name = NULL;
    }
}

void PhotonCannon::print_status() {
    std::cout << "Photon Cannon Status" << std::endl;
    std::cout << "Loc (" << x << ", " << y << ")" << std::endl;
    std::cout << "Hp: " << hp << ", Sh: " << shield << std::endl;
    std::cout << "Damage Capable: " << damage << std::endl;
    std::cout << "Name: ";
    if (name) std::cout << name;
    std::cout << std::endl;
}

PhotonCannon::~PhotonCannon() {
    if (name) delete[] name;
}

int main() {
    PhotonCannon a = PhotonCannon(5, 10);
    PhotonCannon b = PhotonCannon(a);
    PhotonCannon c = a;

    PhotonCannon d(10, 5, "Fuck");
    PhotonCannon e(d);
    PhotonCannon f = d;

    a.print_status();
    b.print_status();
    c.print_status();

    d.print_status();
    e.print_status();
    f.print_status();

    return 0;
}