#pragma once
#include "npc.h"

struct Elf : public NPC
{
    Elf(std::string& name, int x, int y);
    Elf(std::istream &is);

    void print() override;

    bool accept(std::shared_ptr<NPC> attacker) override;
    bool fight(std::shared_ptr<Rogue> other) override;
    bool fight(std::shared_ptr<Knight_Errant> other) override;
    bool fight(std::shared_ptr<Elf> other) override;

    void save(std::ostream &os) override;

    friend std::ostream &operator<<(std::ostream &os, Elf &elf);
};