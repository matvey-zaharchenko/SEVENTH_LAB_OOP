#include "../include/elf.h"
#include "../include/knight_errant.h"
#include "../include/rogue.h"
#include "../include/npc.h"
#include <memory>

Elf::Elf(std::string& name, int x, int y) : NPC(ElfType, name, x, y) {
    killAndStepLen.first = 50;
    killAndStepLen.second = 10;
}
Elf::Elf(std::istream &is) : NPC(ElfType, is) {
    killAndStepLen.first = 50;
    killAndStepLen.second = 10;
}

void Elf::print()
{
    std::cout << *this;
}

bool Elf::accept(std::shared_ptr<NPC> attacker)
{
    AttackVisitor visitor(attacker);
    return visitor.visit(std::static_pointer_cast<Elf>(shared_from_this()));
}

bool Elf::fight(std::shared_ptr<Rogue> other) 
{
    fight_notify(other, false);
    return false;
}

bool Elf::fight(std::shared_ptr<Knight_Errant> other) 
{
    fight_notify(other, true);
    return true;
}

bool Elf::fight(std::shared_ptr<Elf> other) 
{
    fight_notify(other, false);
    return false;
}

void Elf::save(std::ostream &os) 
{
    os << "Elf ";
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Elf &elf)
{
    os << "Elf: " << *static_cast<NPC *>(&elf) << std::endl;
    return os;
}