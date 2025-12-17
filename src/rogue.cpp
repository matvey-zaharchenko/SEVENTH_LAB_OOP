#include "../include/elf.h"
#include "../include/knight_errant.h"
#include "../include/rogue.h"
#include "../include/npc.h"
#include <memory>

Rogue::Rogue(std::string& name, int x, int y) : NPC(RogueType, name, x, y) {
    killAndStepLen.first = 10;
    killAndStepLen.second = 10;
}
Rogue::Rogue(std::istream &is) : NPC(RogueType, is) {
    killAndStepLen.first = 10;
    killAndStepLen.second = 10;
}

void Rogue::print()
{
    std::cout << *this;
}

bool Rogue::accept(std::shared_ptr<NPC> attacker)
{
    AttackVisitor visitor(attacker);
    return visitor.visit(std::static_pointer_cast<Rogue>(shared_from_this()));
}

bool Rogue::fight(std::shared_ptr<Rogue> other) 
{
    fight_notify(other, false);
    return false;
}

bool Rogue::fight(std::shared_ptr<Knight_Errant> other) 
{
    fight_notify(other, false);
    return false;
}

bool Rogue::fight(std::shared_ptr<Elf> other) 
{
    fight_notify(other, true);
    return true;
}

void Rogue::save(std::ostream &os) 
{
    os << "Rogue ";
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Rogue &rogue)
{
    os << "Rogue: " << *static_cast<NPC *>(&rogue) << std::endl;
    return os;
}