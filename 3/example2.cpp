#include <iostream>

struct Animal {
    char name[30];
    int age;
    int health;
    int food;
    int clean;
};

void add_animal(Animal* animal) {
    std::cout << "Name of animal: ";
    std::cin >> animal->name;
    std::cout << "Age of animal: ";
    std::cin >> animal->age;
    animal->health = 100;
    animal->food = 100;
    animal->clean = 100;
}

void play_with_animal(Animal* animal) {
    animal->health += 10;
    animal->food -= 20;
    animal->clean -= 30;
}

void forward_one_day(Animal* animal) {
    animal->health -= 10;
    animal->food -= 30;
    animal->clean -= 20;
}

void print_stat(Animal* animal) {
    std::cout << animal->name << "'s stat:" << std::endl;
    std::cout << "Health: " << animal->health << std::endl;
    std::cout << "Food: " << animal->food << std::endl;
    std::cout << "Clean: " << animal->clean << std::endl;
}

int main() {
    Animal* list[30];
    int num_animal = 0;

    while (1) {
        std::cout << "1. Add animal" << std::endl;
        std::cout << "2. Play with animal" << std::endl;
        std::cout << "3. Print stat" << std::endl;

        int user_choice;
        std::cin >> user_choice;

        switch (user_choice) {
            int play_target;
            case 1:
                list[num_animal] = new Animal;
                add_animal(list[num_animal]);
                num_animal++;
                break;
            case 2:
                std::cout << "With which you will play?: ";
                std::cin >> play_target;
                if (play_target < num_animal)
                    play_with_animal(list[num_animal]);
                break;
            case 3:
                std::cout << "Print which animal?: ";
                std::cin >> play_target;
                if (play_target < num_animal) print_stat(list[play_target]);
                break;
        }
        for (int i = 0; i < num_animal; i++) forward_one_day(list[i]);
    }
    for (int i = 0; i < num_animal; i++) delete list[i];
    return 0;
}