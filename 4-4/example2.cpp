#include <iostream>

class Marine {
    static int num_marines;
    int hp;                // 마린 체력
    int coord_x, coord_y;  // 마린 위치
    int damage;            // 공격력
    const int default_damage;
    bool is_dead;

   public:
    Marine();              // 기본 생성자
    Marine(int x, int y);  // x, y 좌표에 마린 생성
    Marine(int x, int y, int damage);

    int attack() const;                    // 데미지를 리턴한다.
    Marine& be_attacked(int damage_earn);  // 입는 데미지
    void move(int x, int y);               // 새로운 위치

    void show_status();  // 상태를 보여준다.
    static void show_total_marines();

    ~Marine();
};

int Marine::num_marines = 0;

Marine::Marine()
    : hp(50),
      coord_x(0),
      coord_y(0),
      damage(5),
      default_damage(5),
      is_dead(false) {
    num_marines++;
}

Marine::Marine(int x, int y)
    : coord_x(x),
      coord_y(y),
      hp(50),
      damage(5),
      default_damage(5),
      is_dead(false) {
    num_marines++;
}
Marine::Marine(int x, int y, int damage)
    : coord_x(x),
      coord_y(y),
      hp(50),
      damage(damage),
      default_damage(5),
      is_dead(false) {
    num_marines++;
}

void Marine::move(int x, int y) {
    coord_x = x;
    coord_y = y;
}
int Marine::attack() const { return damage; }
Marine& Marine::be_attacked(int damage_earn) {
    hp -= damage_earn;
    if (hp <= 0) is_dead = true;

    return *this;
}
void Marine::show_status() {
    std::cout << " *** Marine *** " << std::endl;
    std::cout << " Location : ( " << coord_x << " , " << coord_y << " ) "
              << std::endl;
    std::cout << " HP : " << hp << std::endl;
    std::cout << "Total marines: " << num_marines << std::endl;
}

Marine::~Marine() { num_marines--; }

void Marine::show_total_marines() {
    std::cout << "Total marines: " << num_marines << std::endl;
}

void create_temp_marine() {
    Marine marine(5, 7, 60);
    marine.show_status();
}

int main() {
    Marine marine1(2, 3);
    Marine marine2(3, 5);

    marine1.show_status();
    marine2.show_status();

    create_temp_marine();

    std::cout << std::endl << "마린 1 이 마린 2 를 2번 공격! " << std::endl;
    marine2.be_attacked(marine1.attack()).be_attacked(marine1.attack());

    marine1.show_status();
    marine2.show_status();

    Marine::show_total_marines();
}