#include <gtest/gtest.h>
#include "../include/npc.h"
#include "../include/elf.h"
#include "../include/knight_errant.h"
#include "../include/rogue.h"
#include "../include/tech_impl.h"
#include <sstream>
#include <fstream>
#include <memory>
#include <filesystem>

#include <gtest/gtest.h>
#include "tech_impl.h"
#include <sstream>
#include <fstream>
#include <memory>
#include <filesystem>


std::string create_name(const std::string& base) {
    static int counter = 0;
    return base + std::to_string(counter++);
}


std::shared_ptr<NPC> create_npc_with_suffix(NpcType type, const std::string& base, 
                                           const std::string& suffix, int x, int y) {
    std::string name = base + suffix;
    return factory(type, name, x, y);
}


// TEST(BasicTests, CreateAllNPCs) {
//     std::string name = create_name("TestNPC");
    
//     auto rogue = factory(RogueType, name, 10, 20);
//     auto elf = factory(ElfType, name, 30, 40);
//     auto knight = factory(KnightErrantType, name, 50, 60);
    
//     ASSERT_NE(rogue, nullptr);
//     ASSERT_NE(elf, nullptr);
//     ASSERT_NE(knight, nullptr);
    
//     EXPECT_NO_THROW(rogue->print());
//     EXPECT_NO_THROW(elf->print());
//     EXPECT_NO_THROW(knight->print());
// }


// TEST(BasicTests, FactoryCreatesNPCs) {
//     std::string name = create_name("FactoryNPC");
    
//     auto rogue = factory(RogueType, name, 100, 200);
//     auto elf = factory(ElfType, name, 300, 400);
//     auto knight = factory(KnightErrantType, name, 500, 600);
    
//     ASSERT_NE(rogue, nullptr);
//     ASSERT_NE(elf, nullptr);
//     ASSERT_NE(knight, nullptr);
    
//     std::stringstream ss;
//     EXPECT_NO_THROW(rogue->save(ss));
//     EXPECT_NO_THROW(elf->save(ss));
//     EXPECT_NO_THROW(knight->save(ss));
// }


// TEST(BasicTests, SaveToStream) {
//     std::string name = create_name("SaveTest");
//     auto npc = factory(RogueType, name, 123, 456);
//     ASSERT_NE(npc, nullptr);
    
//     std::stringstream ss;
//     npc->save(ss);
    
//     std::string type, loaded_name;
//     int x, y;
//     ss >> type >> loaded_name >> x >> y;
    
//     EXPECT_EQ(type, "Rogue");
//     EXPECT_EQ(loaded_name, name);
//     EXPECT_EQ(x, 123);
//     EXPECT_EQ(y, 456);
// }



// TEST(FightTests, DistanceCheck) {
//     std::string name1 = create_name("Dist1");
//     std::string name2 = create_name("Dist2");
    
//     auto npc1 = factory(RogueType, name1, 0, 0);
//     auto npc2 = factory(ElfType, name2, 3, 4);
    
//     ASSERT_NE(npc1, nullptr);
//     ASSERT_NE(npc2, nullptr);
    
//     EXPECT_TRUE(npc1->is_close(npc2, 10));
//     EXPECT_TRUE(npc1->is_close(npc2, 5));
//     EXPECT_FALSE(npc1->is_close(npc2, 4));
//     EXPECT_TRUE(npc2->is_close(npc1, 10));
// }

// TEST(FightTests, SimpleFight) {
//     set_t array;
    
//     std::string attacker_name = create_name("Attacker");
//     std::string defender_name = create_name("Defender");
    
//     auto attacker = factory(RogueType, attacker_name, 0, 0);
//     auto defender = factory(ElfType, defender_name, 1, 1);
    
//     array.insert(attacker);
//     array.insert(defender);
    
//     auto dead_list = fight(array, 10);
    
//     EXPECT_NO_THROW(fight(array, 10));
    
//     // Для отладки
//     std::cout << "Test SimpleFight: " << dead_list.size() << " killed" << std::endl;
// }


// TEST(FightTests, NoSelfFight) {
//     set_t array;
//     std::string name = create_name("Self");
    
//     auto npc = factory(RogueType, name, 0, 0);
//     array.insert(npc);
    
//     auto dead_list = fight(array, 1000);
//     EXPECT_EQ(dead_list.size(), 0);
// }


// TEST(FileTests, SaveAndLoadFile) {
//     set_t original;
//     std::string base_name = create_name("FileTest");
    
//     // Создаем переменные для имен
//     std::string name1 = base_name + "_1";
//     std::string name2 = base_name + "_2";
    
//     original.insert(factory(RogueType, name1, 10, 20));
//     original.insert(factory(ElfType, name2, 30, 40));
    
//     std::string filename = "test_save_load.txt";
//     save_array(original, filename);
    
//     std::ifstream check(filename);
//     EXPECT_TRUE(check.good());
    
//     check.seekg(0, std::ios::end);
//     size_t size = check.tellg();
//     check.close();
//     EXPECT_GT(size, 0);
    
//     set_t loaded = load_from_file(filename);
//     EXPECT_EQ(loaded.size(), original.size());
    
//     // Удаляем временный файл
//     std::remove(filename.c_str());
// }


// TEST(GenerationTests, GenerateNPCs) {
//     set_t array;
    
//     generate_npcs(array, 5, 100);
    
//     EXPECT_EQ(array.size(), 5);
    
//     for (const auto& npc : array) {
//         ASSERT_NE(npc, nullptr);
//         EXPECT_NO_THROW(npc->print());
//     }
// }


// TEST(VisitorTests, AcceptMethodWorks) {
//     std::string base_name = create_name("VisitorTest");
    
//     // Создаем отдельные переменные для имен
//     std::string attacker_name = base_name + "_att";
//     std::string rogue_name = base_name + "_rogue";
//     std::string elf_name = base_name + "_elf";
//     std::string knight_name = base_name + "_knight";
    
//     auto attacker = factory(RogueType, attacker_name, 0, 0);
//     auto defender_rogue = factory(RogueType, rogue_name, 1, 1);
//     auto defender_elf = factory(ElfType, elf_name, 1, 1);
//     auto defender_knight = factory(KnightErrantType, knight_name, 1, 1);
    
//     ASSERT_NE(attacker, nullptr);
//     ASSERT_NE(defender_rogue, nullptr);
//     ASSERT_NE(defender_elf, nullptr);
//     ASSERT_NE(defender_knight, nullptr);
    
//     EXPECT_NO_THROW(defender_rogue->accept(attacker));
//     EXPECT_NO_THROW(defender_elf->accept(attacker));
//     EXPECT_NO_THROW(defender_knight->accept(attacker));
// }

// TEST(FightTests, FightLogic) {
//     set_t array;
//     std::string base_name = create_name("LogicTest");
    
//     // Создаем отдельные переменные для имен
//     std::string rogue_name = base_name + "_rogue";
//     std::string elf_name = base_name + "_elf";
//     std::string knight_name = base_name + "_knight";
    
//     auto rogue = factory(RogueType, rogue_name, 0, 0);
//     auto elf = factory(ElfType, elf_name, 1, 1);
//     auto knight = factory(KnightErrantType, knight_name, 2, 2);
    
//     array.insert(rogue);
//     array.insert(elf);
//     array.insert(knight);
    
//     auto dead_list = fight(array, 10);
    
//     std::cout << "Test FightLogic: " << dead_list.size() << " killed" << std::endl;
// }


// TEST(FightTests, MultipleAttackers) {
//     set_t array;
//     std::string base_name = create_name("Multi");
    
//     // Цель в центре
//     std::string target_name = base_name + "_target";
//     auto target = factory(ElfType, target_name, 10, 10);
    
//     // Несколько атакующих вокруг
//     std::string att1_name = base_name + "_att1";
//     std::string att2_name = base_name + "_att2";
//     std::string att3_name = base_name + "_att3";
    
//     auto attacker1 = factory(RogueType, att1_name, 9, 10);
//     auto attacker2 = factory(RogueType, att2_name, 11, 10);
//     auto attacker3 = factory(RogueType, att3_name, 20, 20); // Далеко
    
//     array.insert(target);
//     array.insert(attacker1);
//     array.insert(attacker2);
//     array.insert(attacker3);
    
//     auto dead_list = fight(array, 5);
    
//     std::cout << "Test MultipleAttackers: " << dead_list.size() << " killed" << std::endl;
// }


// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
    
//     std::srand(42);
    
//     // Инициализируем файл логов
//     initialize_file();
    
//     return RUN_ALL_TESTS();
// }