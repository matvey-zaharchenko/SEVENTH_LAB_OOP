#include "npc.h"
#include <random>

AttackVisitor::AttackVisitor(std::shared_ptr<NPC> attacker) : attacker(attacker) {}
    
bool AttackVisitor::visit(std::shared_ptr<Rogue> rogue) {
    return attacker->fight(rogue);
}
    
bool AttackVisitor::visit(std::shared_ptr<Knight_Errant> knight_errant) {
    return attacker->fight(knight_errant);
}
    
bool AttackVisitor::visit(std::shared_ptr<Elf> elf) {
    return attacker->fight(elf);
}

NPC::NPC(NpcType t, std::string& _name, int _x, int _y) : type(t), name(_name), x(_x), y(_y) {}
NPC::NPC(NpcType t, std::istream &is) : type(t)
{
    is >> name;
    is >> x;
    is >> y;
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer)
{
   observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win)
{
    for (auto &o : observers)
        o->on_fight(shared_from_this(), defender, win);
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const {
    return pow((x - other->x), 2) + pow((y - other->y), 2) <= distance * distance;
}

void NPC::save(std::ostream &os) {
    os << name << " " << x << " " << y << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc) {
    os << "name: " << npc.name << " { x:" << npc.x << ", y:" << npc.y << " }";
    return os;
}

void NPC::move(int shift_x, int shift_y, int max_x, int max_y)
{
    std::lock_guard<std::shared_mutex> lck(mtx);

    if ((x + shift_x >= 0) && (x + shift_x <= max_x))
        x += shift_x;
    if ((y + shift_y >= 0) && (y + shift_y <= max_y))
        y += shift_y;
}

bool NPC::is_alive() const { 
    std::shared_lock<std::shared_mutex> lck(mtx);
    return alive; 
}

void NPC::must_die() {
    std::lock_guard<std::shared_mutex> lck(mtx);
    alive = false;
}

int random_num(int l, int r){
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distrib(l, r); 
    
    int random_num = distrib(gen);
    return random_num;
}
int NPC::roll_cube() {
    return random_num(1, 6);
}

int NPC::stepLen() const{
    return killAndStepLen.first;
}
int NPC::killLen() const {
    return killAndStepLen.second;
}

std::pair<int, int> NPC::position() const {
    std::shared_lock<std::shared_mutex> lck(mtx);
    return {x, y};
}

bool is_end_of_game(std::chrono::steady_clock::time_point start){
    return time_since_start(start) >= TOTAL_GAME_TIME;
}

double time_since_start(std::chrono::steady_clock::time_point start){
    return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - start).count();
}

int switcher(int num, std::shared_ptr<NPC> npc){
    switch (num)
    {
    case 1:
        return npc->stepLen();
    case 2:
        return -npc->stepLen();
    case 3:
        return 0;
    }
}

std::pair<int, int> randomCoord(std::shared_ptr<NPC> npc){
    int rand_x = random_num(1, 3);
    int rand_y = random_num(1, 3);
    std::pair res = {switcher(rand_x, npc), switcher(rand_y, npc)};
    return res;
}

NpcType NPC::get_type() const { return type; }