#include "../include/npc.h"
#include "../include/elf.h"
#include "../include/knight_errant.h"
#include "../include/rogue.h"
#include "../include/tech_impl.h"
#include <fstream>
#include <vector>
#include <string>
#include <memory>

class FileObserver: public IFightObserver{
private:
    FileObserver(){};
public:
    static std::shared_ptr<IFightObserver> get(){
        static FileObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }
    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override{
        if (win)
        {
            std::ofstream ofs("../log.txt", std::ios::app);
            ofs << "Murder --------" << std::endl;
            ofs << "Attacker: ";
            attacker->save(ofs);
            ofs << "Killed: ";
            defender->save(ofs);
            ofs << std::endl;
        }
    }
};

void initialize_file(){
    std::ofstream file("../log.txt", std::ios::trunc);
}

int get_type_from_string(const std::string &str) {
    if (str == "Rogue") {
        return RogueType;
    }
    if (str == "Knight_Errant") {
        return KnightErrantType;
    }
    if (str == "Elf") {
        return ElfType;
    }
    return 0;
}

// Фабрики -----------------------------------
std::shared_ptr<NPC> factory(std::istream &is)
{
    std::shared_ptr<NPC> result;
    std::string type_str;
    is >> type_str;
    int type = get_type_from_string(type_str);
    if (type)
    {
        switch (type)
        {
        case RogueType:
            result = std::make_shared<Rogue>(is);
            break;
        case KnightErrantType:
            result = std::make_shared<Knight_Errant>(is);
            break;
        case ElfType:
            result = std::make_shared<Elf>(is);
            break;
        }
    }
    else
        std::cerr << "unexpected NPC type:" << type << std::endl;

    if (result) {
        result->subscribe(FileObserver::get());
    }

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, std::string& name, int x, int y)
{
    std::shared_ptr<NPC> result;
    switch (type)
    {
    case RogueType:
        result = std::make_shared<Rogue>(name, x, y);
        break;
    case KnightErrantType:
        result = std::make_shared<Knight_Errant>(name, x, y);
        break;
    case ElfType:
        result = std::make_shared<Elf>(name, x, y);
        break;
    default:
        break;
    }
    if (result) {
        result->subscribe(FileObserver::get());
    }

    std::cout << "Created ";
    result->print();

    return result;
}

void save_array(const set_t &array, const std::string &filename)
{
    std::ofstream fs(filename);
    fs << array.size() << std::endl;
    for (auto &n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load_from_file(const std::string &filename)
{
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open())
    {
        int count;
        is >> count;
        for (int i = 0; i < count; ++i)
            result.insert(factory(is));
        is.close();
    }
    else
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    return result;
}

std::ostream &operator<<(std::ostream &os, const set_t &array)
{
    for (auto &n : array)
        n->print();
    return os;
}

set_t fight(const set_t &array, size_t distance)
{
    set_t dead_list;

    for (const auto &attacker : array)
        for (const auto &defender : array)
            if ((attacker != defender) && (attacker->is_close(defender, distance)))
            {
                if (defender->accept(attacker)) {
                    dead_list.insert(defender);
                }
            }

    return dead_list;
}

void print_npcs(const set_t &array)
{
    std::cout << "NPCs:" << std::endl;
    for (const auto &n : array)
        n->print();
}

std::string generate_name() {
    static int counter = 0;
    return "NPC-" + std::to_string(counter++);
}

void generate_npcs(set_t &array, size_t count, size_t sizeX, size_t sizeY) {
    for (size_t i = 0; i < count; ++i) {
        std::string name = generate_name();
        array.insert(factory(NpcType(random_num(1, 3)), name,
            std::rand() % sizeX,
            std::rand() % sizeY));
    }
}

void print_alives(const set_t &array){
    int count = 0;
    std::cout << "Survived NPCs:" << std::endl;
    for(const auto& npc : array){
        if(npc->is_alive()){
            ++count;
            npc->print();
        }
    }
    std::cout << "Total survived: " << count << std::endl;
}
