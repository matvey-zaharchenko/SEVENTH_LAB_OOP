#pragma once

#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <set>
#include <cmath>
#include <vector>
#include <shared_mutex>

#define TOTAL_GAME_TIME 30

// type for npcs
struct NPC;
struct Rogue;
struct Knight_Errant;
struct Elf;
using set_t = std::set<std::shared_ptr<NPC>>;

enum NpcType
{
    Unknown = 0,
    RogueType = 1,
    KnightErrantType = 2,
    ElfType = 3
};

bool is_end_of_game(std::chrono::steady_clock::time_point start);
double time_since_start(std::chrono::steady_clock::time_point start);
int switcher(int num, std::shared_ptr<NPC> npc);
std::pair<int, int> randomCoord(std::shared_ptr<NPC> npc);
int random_num(int l, int r);

struct IFightObserver{
    virtual void on_fight(const std::shared_ptr<NPC> attacker,const std::shared_ptr<NPC> defender,bool win) = 0;
};

class AttackVisitor {
private:
    std::shared_ptr<NPC> attacker;
    
public:
    AttackVisitor(std::shared_ptr<NPC> attacker);
    bool visit(std::shared_ptr<Rogue> rogue);
    bool visit(std::shared_ptr<Knight_Errant> knight_errant);
    bool visit(std::shared_ptr<Elf> elf);
    virtual ~AttackVisitor() = default;
};

struct NPC : public std::enable_shared_from_this<NPC>
{
    mutable std::shared_mutex mtx;
    NpcType type;
    int x{0};
    int y{0};
    std::string name;
    bool alive{true};
    std::pair<int, int> killAndStepLen{0, 0};
    std::vector<std::shared_ptr<IFightObserver>> observers;

    NPC(NpcType t, std::string& _name, int _x, int _y);
    NPC(NpcType t, std::istream &is);

    void subscribe(std::shared_ptr<IFightObserver> observer );
    void fight_notify(const std::shared_ptr<NPC> defender,bool win);
    virtual bool is_close(const std::shared_ptr<NPC> &other, size_t distance) const;

    virtual bool fight(std::shared_ptr<Rogue> other) = 0;
    virtual bool fight(std::shared_ptr<Knight_Errant> other) = 0;
    virtual bool fight(std::shared_ptr<Elf> other) = 0;
    virtual void print() = 0;

    virtual void save(std::ostream &os);

    friend std::ostream &operator<<(std::ostream &os, NPC &npc);
    virtual bool accept(std::shared_ptr<NPC> attacker) = 0;
    void move(int shift_x, int shift_y, int max_x, int max_y);
    bool is_alive() const;
    void must_die();
    int roll_cube();
    int stepLen() const;
    int killLen() const;
    std::pair<int, int> position() const;
    NpcType get_type() const;
};