#pragma once

#include "npc.h"

std::shared_ptr<NPC> factory(std::istream &is);
std::shared_ptr<NPC> factory(NpcType type, std::string& name, int x, int y);

void save_array(const set_t &array, const std::string &filename);

set_t load_from_file(const std::string &filename);

std::ostream &operator<<(std::ostream &os, const set_t &array);

set_t fight(const set_t &array, size_t distance);

void print_npcs(const set_t &array);

//void simulate_fight(set_t &array, size_t max_fight_distance);

void generate_npcs(set_t &array, size_t count, size_t sizeX, size_t sizeY);

void initialize_file();

void print_alives(const set_t &array);

template <int grid>
void grid_screen(set_t &array, std::array<char, grid * grid> &fields, size_t step_x, size_t step_y) {
    fields.fill(0);
    for (const std::shared_ptr<NPC> &npc : array)
    {
        const auto [x, y] = npc->position();
        int i = x / step_x;
        int j = y / step_y;

        if (npc->is_alive())
        {
            switch (npc->get_type())
            {
            case RogueType:
                fields[i + grid * j] = 'R';
                break;
            case KnightErrantType:
                fields[i + grid * j] = 'K';
                break;
            case ElfType:
                fields[i + grid * j] = 'E';
                break;

            default:
                break;
            }
        }else{
            fields[i + grid * j] = '+';
        }
    }
    
    {
        
        for (int j = 0; j < grid; ++j)
        {
            for (int i = 0; i < grid; ++i)
            {
                char c = fields[i + j * grid];
                if (c != 0)
                    std::cout << "[" << c << "]";
                else
                    std::cout << "[ ]";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}