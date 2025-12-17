#pragma once
#include "npc.h"

struct Knight_Errant : public NPC
{
    Knight_Errant(std::string& name, int x, int y);
    Knight_Errant(std::istream &is);

    void print() override;

    bool accept(std::shared_ptr<NPC> attacker) override;
    bool fight(std::shared_ptr<Rogue> other) override;
    bool fight(std::shared_ptr<Knight_Errant> other) override;
    bool fight(std::shared_ptr<Elf> other) override;

    void save(std::ostream &os) override;

    friend std::ostream &operator<<(std::ostream &os, Knight_Errant& knight_errant);
};