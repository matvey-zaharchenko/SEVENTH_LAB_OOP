#include "include/npc.h"
#include "include/tech_impl.h"
#include "include/fightManager.h"
#include <thread>
#include <array>

using namespace std::chrono_literals;
std::mutex cout_mtx;

int main()
{
    initialize_file();

    set_t npcArray;
    const int MAX_X{100};
    const int MAX_Y{100};

    std::cout << "Generating 50 NPCs..." << std::endl;
    generate_npcs(npcArray, 50, MAX_X, MAX_Y);

    auto start_time = std::chrono::steady_clock::now();
    std::thread fight_thread(std::ref(FightManager::get()), start_time);
    std::thread move_thread([&npcArray, MAX_X, MAX_Y, start_time]()
                            {
            while (true)
            {
                for (const std::shared_ptr<NPC> & npc : npcArray)
                    if(npc->is_alive()) {
                        auto [dx, dy] = randomCoord(npc);
                        npc->move(dx, dy, MAX_X - 1, MAX_Y - 1);
                    }

                for (const std::shared_ptr<NPC> & npc : npcArray)
                    for (const std::shared_ptr<NPC> & other : npcArray) 
                        if ((other != npc) && (npc->is_alive()) && (other->is_alive()) && (npc->is_close(other, npc->killLen())))
                          FightManager::get().add_event({npc, other});
                std::this_thread::sleep_for(10ms);
                if (is_end_of_game(start_time))
                    break;
             }        
        });

    const int grid{20}, step_x{MAX_X / grid}, step_y{MAX_Y / grid};
    std::array<char, grid * grid> fields{0};
    while (true)
    {
        {
            std::lock_guard<std::mutex> lck(cout_mtx);
            grid_screen<20>(npcArray, fields, step_x, step_y);
        }
        std::this_thread::sleep_for(1s);

        if (is_end_of_game(start_time))
            break;
    };
    move_thread.join();
    fight_thread.join();

    grid_screen<20>(npcArray, fields, step_x, step_y);
    print_alives(npcArray);
    return 0;
}