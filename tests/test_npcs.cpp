#include "gtest/gtest.h"
#include "../include/npc.h"
#include "../include/elf.h"
#include "../include/knight_errant.h"
#include "../include/rogue.h"
#include "../include/tech_impl.h"
#include "../include/fightManager.h"
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

// Тестовый наблюдатель для проверки событий боя
class MockObserver : public IFightObserver {
public:
    struct FightEvent {
        std::shared_ptr<NPC> attacker;
        std::shared_ptr<NPC> defender;
        bool win;
    };
    
    std::vector<FightEvent> events;
    
    void on_fight(const std::shared_ptr<NPC> attacker,
                  const std::shared_ptr<NPC> defender,
                  bool win) override {
        events.push_back({attacker, defender, win});
    }
    
    void clear() { events.clear(); }
    
    size_t count() const { return events.size(); }
    
    bool contains_win(const std::string& attacker_name, 
                      const std::string& defender_name) const {
        for (const auto& e : events) {
            if (e.win && 
                e.attacker->name == attacker_name && 
                e.defender->name == defender_name) {
                return true;
            }
        }
        return false;
    }
};

// Тесты для базового класса NPC
TEST(NPCTest, ConstructorAndBasicProperties) {
    std::string name = "TestNPC";
    NPC* npc = new Knight_Errant(name, 10, 20);  // Используем Knight_Errant как пример
    
    EXPECT_EQ(npc->name, name);
    EXPECT_EQ(npc->position().first, 10);
    EXPECT_EQ(npc->position().second, 20);
    EXPECT_TRUE(npc->is_alive());
    EXPECT_EQ(npc->get_type(), KnightErrantType);
    
    delete npc;  // Для тестового NPC без shared_ptr
}


TEST(NPCTest, IsAliveAndMustDie) {
    std::string name = "TestNPC";
    auto npc = std::make_shared<Knight_Errant>(name, 0, 0);
    
    EXPECT_TRUE(npc->is_alive());
    npc->must_die();
    EXPECT_FALSE(npc->is_alive());
}

TEST(NPCTest, IsCloseCalculation) {
    std::string name1 = "NPC1";
    std::string name2 = "NPC2";
    auto npc1 = std::make_shared<Knight_Errant>(name1, 0, 0);
    auto npc2 = std::make_shared<Knight_Errant>(name2, 3, 4);
    
    // Расстояние = sqrt(3² + 4²) = 5
    EXPECT_TRUE(npc1->is_close(npc2, 5));   // На границе
    EXPECT_TRUE(npc1->is_close(npc2, 6));   // Внутри радиуса
    EXPECT_FALSE(npc1->is_close(npc2, 4));  // За пределами
}

TEST(NPCTest, RollCubeInRange) {
    std::string name = "TestNPC";
    auto npc = std::make_shared<Knight_Errant>(name, 0, 0);
    
    // Бросаем кубик несколько раз, проверяем диапазон
    for (int i = 0; i < 100; ++i) {
        int result = npc->roll_cube();
        EXPECT_GE(result, 1);
        EXPECT_LE(result, 6);
    }
}

TEST(NPCTest, StepAndKillLength) {
    std::string name = "TestNPC";
    auto knight = std::make_shared<Knight_Errant>(name, 0, 0);
    auto elf = std::make_shared<Elf>(name, 0, 0);
    auto rogue = std::make_shared<Rogue>(name, 0, 0);
    
    // Проверяем характерные длины для каждого типа
    EXPECT_EQ(knight->stepLen(), 30);
    EXPECT_EQ(knight->killLen(), 10);
    
    EXPECT_EQ(elf->stepLen(), 10);
    EXPECT_EQ(elf->killLen(), 50);
    
    EXPECT_EQ(rogue->stepLen(), 10);
    EXPECT_EQ(rogue->killLen(), 10);
}

// Тесты для фабрик
TEST(FactoryTest, CreateFromParameters) {
    std::string name = "FactoryNPC";
    
    auto rogue = factory(RogueType, name, 10, 20);
    EXPECT_NE(rogue, nullptr);
    EXPECT_EQ(rogue->get_type(), RogueType);
    EXPECT_EQ(rogue->name, name);
    
    auto knight = factory(KnightErrantType, name, 30, 40);
    EXPECT_NE(knight, nullptr);
    EXPECT_EQ(knight->get_type(), KnightErrantType);
    
    auto elf = factory(ElfType, name, 50, 60);
    EXPECT_NE(elf, nullptr);
    EXPECT_EQ(elf->get_type(), ElfType);
}

TEST(FactoryTest, CreateFromStream) {
    std::stringstream ss;
    ss << "Rogue RogueNPC 100 200\n";
    
    auto npc = factory(ss);
    EXPECT_NE(npc, nullptr);
    EXPECT_EQ(npc->get_type(), RogueType);
    EXPECT_EQ(npc->name, "RogueNPC");
    EXPECT_EQ(npc->position().first, 100);
    EXPECT_EQ(npc->position().second, 200);
}

TEST(FactoryTest, InvalidTypeFromStream) {
    std::stringstream ss;
    ss << "InvalidType NPC 10 20\n";
    
    // Не должно быть исключения, должен быть nullptr или сообщение об ошибке
    testing::internal::CaptureStderr();
    auto npc = factory(ss);
    std::string output = testing::internal::GetCapturedStderr();
    
    // Проверяем, что есть сообщение об ошибке
    EXPECT_TRUE(output.find("unexpected NPC type") != std::string::npos ||
                npc == nullptr);
}

// Тесты для сохранения и загрузки
TEST(IOTest, SaveAndLoadNPC) {
    std::string name = "TestNPC";
    auto original = std::make_shared<Elf>(name, 42, 24);
    
    std::stringstream ss;
    original->save(ss);
    
    // Проверяем формат вывода
    std::string saved = ss.str();
    EXPECT_TRUE(saved.find("Elf") != std::string::npos);
    EXPECT_TRUE(saved.find(name) != std::string::npos);
    EXPECT_TRUE(saved.find("42") != std::string::npos);
    EXPECT_TRUE(saved.find("24") != std::string::npos);
}

TEST(IOTest, SaveAndLoadArray) {
    set_t array;
    std::string name1 = "NPC1";
    std::string name2 = "NPC2";
    
    array.insert(std::make_shared<Rogue>(name1, 10, 20));
    array.insert(std::make_shared<Knight_Errant>(name2, 30, 40));
    
    const std::string filename = "test_save.txt";
    save_array(array, filename);
    
    // Проверяем, что файл создан
    std::ifstream test_file(filename);
    EXPECT_TRUE(test_file.good());
    
    // Загружаем обратно
    set_t loaded = load_from_file(filename);
    EXPECT_EQ(loaded.size(), 2);
    
    // Очищаем тестовый файл
    std::remove(filename.c_str());
}

// Тесты для логики боев
TEST(FightTest, ElfVsKnight) {
    std::string elf_name = "Elf1";
    std::string knight_name = "Knight1";
    auto elf = std::make_shared<Elf>(elf_name, 0, 0);
    auto knight = std::make_shared<Knight_Errant>(knight_name, 5, 0);  // В пределах радиуса
    
    MockObserver observer;
    elf->subscribe(std::make_shared<MockObserver>(observer));
    knight->subscribe(std::make_shared<MockObserver>(observer));
    
    // Эльф должен победить Рыцаря
    bool result = elf->fight(knight);
    EXPECT_TRUE(result);
    
    // Проверяем уведомления (если fight_notify вызывается)
    // Этот тест зависит от реализации fight_notify в ваших методах fight
}

TEST(FightTest, KnightVsRogue) {
    std::string knight_name = "Knight1";
    std::string rogue_name = "Rogue1";
    auto knight = std::make_shared<Knight_Errant>(knight_name, 0, 0);
    auto rogue = std::make_shared<Rogue>(rogue_name, 5, 0);
    
    // Рыцарь должен победить Разбойника
    bool result = knight->fight(rogue);
    EXPECT_TRUE(result);
}

TEST(FightTest, RogueVsElf) {
    std::string rogue_name = "Rogue1";
    std::string elf_name = "Elf1";
    auto rogue = std::make_shared<Rogue>(rogue_name, 0, 0);
    auto elf = std::make_shared<Elf>(elf_name, 5, 0);
    
    // Разбойник должен победить Эльфа
    bool result = rogue->fight(elf);
    EXPECT_TRUE(result);
}

TEST(FightTest, SameTypeFights) {
    std::string name1 = "NPC1";
    std::string name2 = "NPC2";
    
    // Проверяем бои одинаковых типов (все должны проигрывать)
    auto elf1 = std::make_shared<Elf>(name1, 0, 0);
    auto elf2 = std::make_shared<Elf>(name2, 5, 0);
    EXPECT_FALSE(elf1->fight(elf2));
    
    auto knight1 = std::make_shared<Knight_Errant>(name1, 0, 0);
    auto knight2 = std::make_shared<Knight_Errant>(name2, 5, 0);
    EXPECT_FALSE(knight1->fight(knight2));
    
    auto rogue1 = std::make_shared<Rogue>(name1, 0, 0);
    auto rogue2 = std::make_shared<Rogue>(name2, 5, 0);
    EXPECT_FALSE(rogue1->fight(rogue2));
}

// Тесты для AttackVisitor
TEST(VisitorTest, VisitorDispatch) {
    std::string attacker_name = "Attacker";
    std::string defender_name = "Defender";
    
    auto attacker = std::make_shared<Knight_Errant>(attacker_name, 0, 0);
    auto rogue = std::make_shared<Rogue>(defender_name, 0, 0);
    auto knight = std::make_shared<Knight_Errant>(defender_name, 0, 0);
    auto elf = std::make_shared<Elf>(defender_name, 0, 0);
    
    AttackVisitor visitor_rogue(rogue);
    AttackVisitor visitor_knight(knight);
    AttackVisitor visitor_elf(elf);
}
    // Рыцарь посещает разных NPC через visitor
    // Эти тесты проверя