#include "../include/elf.h"
#include "../include/knight_errant.h"
#include "../include/rogue.h"
#include "../include/npc.h"
#include <memory>

Knight_Errant::Knight_Errant(std::string& name, int x, int y) : NPC(KnightErrantType, name, x, y) {
    killAndStepLen.first = 10;
    killAndStepLen.second = 30;
}
Knight_Errant::Knight_Errant(std::istream &is) : NPC(KnightErrantType, is) {
    killAndStepLen.first = 10;
    killAndStepLen.second = 30;
}

void Knight_Errant::print()
{
    std::cout << *this;
}

bool Knight_Errant::accept(std::shared_ptr<NPC> attacker)
{
    AttackVisitor visitor(attacker);
    return visitor.visit(std::static_pointer_cast<Knight_Errant>(shared_from_this()));
}

bool Knight_Errant::fight(std::shared_ptr<Rogue> other) 
{
    fight_notify(other, true);
    return true;
}

bool Knight_Errant::fight(std::shared_ptr<Knight_Errant> other) 
{
    fight_notify(other, false);
    return false;
}

bool Knight_Errant::fight(std::shared_ptr<Elf> other) 
{
    fight_notify(other, false);
    return false;
}

void Knight_Errant::save(std::ostream &os) 
{
    os << "Knight_Errant ";
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Knight_Errant &knight_errant)
{
    os << "Knight_Errant: " << *static_cast<NPC *>(&knight_errant) << std::endl;
    return os;
}