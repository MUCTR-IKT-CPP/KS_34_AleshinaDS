
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <concepts>
#include <utility>

/**
 * @brief Интерфейс игровой сущности.
 * Определяет общие методы для всех игровых персонажей.
 */
class IEntity {
public:
    /**
     * @brief Обновляет состояние игровой сущности.
     * @param dt Промежуток игрового времени.
     */
    virtual void update(double dt) = 0;
    virtual std::string name() const = 0;
    virtual ~IEntity() = default;
};

/**
 * @brief Простая политика искусственного интеллекта.
 * Выводит сообщение о том, что персонаж думает.
 */
class SimpleAI {
public:
    /**
     * @brief Выполняет простое действие искусственного интеллекта.
     * @param entity Ссылка на игрового персонажа.
     */
    void decide(IEntity& entity) {
        std::cout << entity.name()
            << " is thinking.\n";
    }
};

/**
 * @brief Простая политика физики.
 * Отвечает за вывод сообщения о перемещении персонажа.
 */
class SimplePhysics {
public:
    /**
     * @brief Выполняет перемещение персонажа.
     * @param entity Ссылка на игрового персонажа.
     * @param dt Величина перемещения в условных единицах.
     */
    void apply(IEntity& entity, double dt) {
        std::cout << entity.name()
            << " moved by "
            << dt << " units.\n";
    }
};

/**
 * @brief Концепт для проверки политики искусственного интеллекта.
 * Проверяет наличие метода decide(IEntity&),
 * возвращающего void.
 * @tparam T Проверяемый тип политики AI.
 */
template <typename T>
concept AIModel = requires(T ai, IEntity & entity) {
    { ai.decide(entity) } -> std::same_as<void>;
};

/**
 * @brief Концепт для проверки политики физики.
 * Проверяет наличие метода apply(IEntity&, double),
 * возвращающего void.
 * @tparam T Проверяемый тип политики физики.
 */
template <typename T>
concept PhysicsModel = requires(T physics, IEntity & entity, double dt) {
    { physics.apply(entity, dt) } -> std::same_as<void>;
};

/**
 * @brief Шаблонный класс игрового персонажа.
 * @tparam A Тип политики искусственного интеллекта.
 * @tparam P Тип политики физики.
 */
template <AIModel A, PhysicsModel P>
class NPC : public IEntity {
private:
    std::string name_;  
    A ai;              
    P physics;         
public:
    /**
     * @brief Создаёт игрового персонажа.
     * @param name Имя создаваемого персонажа.
     */
    NPC(const std::string& name) : name_(name) {}

    /**
     * @brief Возвращает имя персонажа.
     * @return Имя игрового персонажа.
     */
    std::string name() const override {
        return name_;
    }

    /**
     * @brief Обновляет состояние персонажа.
     * Вызывает методы искусственного интеллекта
     * и физической модели.
     * @param dt Промежуток игрового времени.
     */
    void update(double dt) override {
        std::cout << "NPC: " << name_ << '\n';

        ai.decide(*this);
        physics.apply(*this, dt);
    }
};

/**
 * @brief Агрессивная политика искусственного интеллекта.
 * Выводит сообщение об атаке противника.
 */
class AggressiveAI {
public:
    /**
     * @brief Выполняет агрессивное поведение персонажа.
     * @param entity Ссылка на атакующего персонажа.
     */
    void decide(IEntity& entity) {
        std::cout << entity.name()
            << ": Character attacks the enemy!\n";
    }
};

/**
 * @brief Пассивная политика искусственного интеллекта.
 * Выводит сообщение об избегании боя и восстановлении сил.
 */
class PassiveAI {
public:
    /**
     * @brief Выполняет пассивное поведение персонажа.
     * @param entity Ссылка на игрового персонажа.
     */
    void decide(IEntity& entity) {
        std::cout << entity.name()
            << ": Character avoids combat and restores strength.\n";
    }
};

/**
 * @brief Фабричная функция для создания игровых персонажей.
 * Создаёт NPC с выбранными политиками поведения
 * и возвращает уникальный указатель на IEntity.
 * @tparam A Тип политики искусственного интеллекта.
 * @tparam P Тип политики физики.
 * @param name Имя создаваемого персонажа.
 * @return Уникальный указатель на игровую сущность.
 */
template <AIModel A, PhysicsModel P>
std::unique_ptr<IEntity> createNPC(const std::string& name) {
    return std::make_unique<NPC<A, P>>(name);
}

/**
 * @brief Класс игрового мира.
 * Хранит игровые сущности в векторе уникальных
 * указателей и управляет их обновлением.
 */
class World {
private:
    std::vector<std::unique_ptr<IEntity>> entities;

public:
    /**
     * @brief Добавляет игровую сущность в мир.
     * Передаёт владение объектом контейнеру World.
     * @param entity Уникальный указатель на игровую сущность.
     */
    void add(std::unique_ptr<IEntity> entity) {
        entities.push_back(std::move(entity));
    }

    /**
     * @brief Обновляет все игровые сущности.
     * Последовательно вызывает метод update()
     * для каждого персонажа.
     * @param dt Промежуток игрового времени.
     */
    void tick(double dt) {
        for (auto& entity : entities) {
            entity->update(dt);
        }
    }
};

int main() {
    World world;
    world.add(createNPC<SimpleAI, SimplePhysics>("Guard"));
    world.add(createNPC<AggressiveAI, SimplePhysics>("Warrior"));
    world.add(createNPC<PassiveAI, SimplePhysics>("Healer"));
    world.tick(2.5);

    return 0;
}
