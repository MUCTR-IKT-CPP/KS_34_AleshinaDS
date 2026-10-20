// laba5v6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class SportEquipment {
private:
    const double MAX_CONDITION = 100.0;
    const double BREAK_WEAR_LIMIT = 80.0;
    const int BREAK_CHANCE = 100;
    const int MIN_REPAIR_PERCENT = 50;
    const int MAX_REPAIR_PERCENT = 80;
    const double REPAIR_COST_RATE = 0.2;
    const int MIN_DURABILITY = 1;
    const int MAX_DURABILITY = 100;
    const double MIN_PRICE = 0.0;
    const int MIN_REPAIRABLE_WEAR = 0;
    const double MAX_DURABILITY_FOR_WEAR = 100.0;
    const double MIN_RANDOM_FACTOR = 0.5;
    const int RANDOM_FACTOR_STEPS = 151;
    const double RANDOM_SCALE = 100.0;
    const int MIN_INTENSITY = 1;
    const int MAX_INTENSITY = 5;
    const int MIN_MINUTES = 1;
    const int MAX_MINUTES = 10;

    std::string _type;
    int _durability;
    double _price;
    double _condition;
public:
    /**
     * @brief Создаёт спортивный инвентарь и проверяет входные значения.
     * @param type Тип спортивного инвентаря.
     * @param durability Износостойкость от 1 до 100.
     * @param price Стоимость предмета.
     */
    SportEquipment(
        const std::string& type,
        int durability,
        double price
    ) {
        _type = type;
        if (durability < MIN_DURABILITY) {
            durability = MIN_DURABILITY;
        }
        if (durability > MAX_DURABILITY) {
            durability = MAX_DURABILITY;
        }
        if (price < MIN_PRICE) {
            price = MIN_PRICE;
        }
        _durability = durability;
        _price = price;
        _condition = MAX_CONDITION;
    }
    
    virtual ~SportEquipment() = default;

  
    bool isBroken() const {
        return _condition <= 0;
    }

    std::string getType() const {
        return _type;
    }
  
    int getDurability() const {
        return _durability;
    }
    double getPrice() const {
        return _price;
    }
    double getCondition() const {
        return _condition;
    }

    double getWear() const {
        return MAX_CONDITION - _condition;
    }
    /**
     * @brief Проверяет вероятность внезапной поломки при износе более 80%.
     */
    void checkSuddenBreak() {
        if (getWear() > BREAK_WEAR_LIMIT && rand() % BREAK_CHANCE == 0) {
            _condition = 0;
        }
    }
    /**
     * @brief Задаёт интерфейс использования инвентаря для производных классов.
     * @param intensity Интенсивность упражнения от 1 до 5.
     * @param minutes Длительность упражнения от 1 до 10 минут.
     */
    virtual void use(int intensity, int minutes) = 0;


    /**
     * @brief Задаёт интерфейс вывода текущего износа.
     */
    virtual void checkWear() const = 0;

    /**
     * @brief Уменьшает состояние предмета на величину износа, не допуская отрицательных значений.
     * @param wear Величина добавляемого износа в процентах.
     */
    void applyWear(double wear) {
        if (wear <= 0) {
            return;
        }

        _condition -= wear;

        if (_condition < 0) {
            _condition = 0;
        }
    }

    /**
     * @brief Восстанавливает случайную долю от 50% до 80% накопленного износа.
     * @return Стоимость выполненного ремонта; 0, если износа нет.
     */
    double repair() {
        double wear = getWear();

        if (wear <= MIN_REPAIRABLE_WEAR) {
            return 0;
        }

        double repairPercent = MIN_REPAIR_PERCENT + rand() % (MAX_REPAIR_PERCENT - MIN_REPAIR_PERCENT + 1);

        double restoredWear = wear * repairPercent / MAX_CONDITION;

        _condition += restoredWear;

        double repairCost = _price * REPAIR_COST_RATE * wear / MAX_CONDITION;

        return repairCost;
    }


protected:
    /**
     * @brief Рассчитывает износ при упражнении и проверяет возможную поломку.
     * @param intensity Интенсивность упражнения от 1 до 5.
     * @param minutes Длительность упражнения от 1 до 10 минут.
     * @param rate Базовая скорость износа конкретного вида инвентаря.
     */
    void useWithRate(int intensity, int minutes, double rate) {
        if (isBroken()) {
            std::cout << "Equipment is broken!\n";
            return;
        }
        if (intensity < MIN_INTENSITY || intensity > MAX_INTENSITY ||
            minutes < MIN_MINUTES || minutes > MAX_MINUTES) {
            std::cout << "Invalid exercise parameters.\n";
            return;
        }

        double baseWear = rate * (MAX_DURABILITY_FOR_WEAR / getDurability());
        double randomFactor = MIN_RANDOM_FACTOR + (rand() % RANDOM_FACTOR_STEPS) / RANDOM_SCALE;
        double wear = baseWear * intensity * minutes * randomFactor;

        applyWear(wear);
        if (!isBroken()) {
            checkSuddenBreak();
        }
    }
};

class Ball : public  SportEquipment {
private:
    const double BASE_WEAR_RATE = 0.1;

    double _diameter;

public:
    /**
     * @brief Создаёт мяч с указанными характеристиками.
     * @param diameter Диаметр мяча в сантиметрах.
     * @param durability Износостойкость мяча от 1 до 100.
     * @param price Стоимость мяча.
     */
    Ball(double diameter, int durability, double price)
        : SportEquipment("Ball", durability, price) {
        if (diameter <= 0) {
            std::cout << "Invalid diameter. Using 1.0.\n";
            diameter = 1.0;
        }
        _diameter = diameter;
    }

    /**
     * @brief Возвращает диаметр мяча.
     * @return Диаметр в сантиметрах.
     */
    double getDiameter() const {
        return _diameter;
    }

    /**
     * @brief Применяет к мячу общий расчёт износа с коэффициентом мяча.
     * @param intensity Интенсивность упражнения от 1 до 5.
     * @param minutes Длительность упражнения от 1 до 10 минут.
     */
    void use(int intensity, int minutes) override {
        useWithRate(intensity, minutes, BASE_WEAR_RATE);
    }

    /**
     * @brief Выводит текущий износ мяча.
     */
    void checkWear() const override {
        std::cout << "Ball wear: "
            << getWear() << "%\n";
    }
};
class JumpRope : public SportEquipment {
private:
    const double BASE_WEAR_RATE = 0.2;

    double _length;

public:
    /**
     * @brief Создаёт скакалку с указанными характеристиками.
     * @param length Длина скакалки в метрах.
     * @param durability Износостойкость скакалки от 1 до 100.
     * @param price Стоимость скакалки.
     */
    JumpRope(double length, int durability, double price)
        : SportEquipment("JumpRope", durability, price) {
        if (length <= 0) {
            std::cout << "Invalid length. Using 1.0.\n";
            length = 1.0;
        }
        _length = length;
    }

    /**
     * @brief Возвращает длину скакалки.
     * @return Длина в метрах.
     */
    double getLength() const {
        return _length;
    }

    /**
     * @brief Применяет к скакалке общий расчёт износа с коэффициентом скакалки.
     * @param intensity Интенсивность упражнения от 1 до 5.
     * @param minutes Длительность упражнения от 1 до 10 минут.
     */
    void use(int intensity, int minutes) override {
        useWithRate(intensity, minutes, BASE_WEAR_RATE);
    }

    /**
     * @brief Выводит текущий износ скакалки.
     */
    void checkWear() const override {
        std::cout << "JumpRope wear: "
            << getWear() << "%\n";
    }
};

class Dumbbells : public SportEquipment {
private:
    const double BASE_WEAR_RATE = 0.05;

    double _weight;

public:
    /**
     * @brief Создаёт гантели с указанными характеристиками.
     * @param weight Вес гантелей в килограммах.
     * @param durability Износостойкость гантелей от 1 до 100.
     * @param price Стоимость гантелей.
     */
    Dumbbells(double weight, int durability, double price)
        : SportEquipment("Dumbbells", durability, price) {
        if (weight <= 0) {
            std::cout << "Invalid weight. Using 1.0.\n";
            weight = 1.0;
        }
        _weight = weight;
    }

    /**
     * @brief Возвращает вес гантелей.
     * @return Вес в килограммах.
     */
    double getWeight() const {
        return _weight;
    }

    /**
     * @brief Применяет к гантелям общий расчёт износа с коэффициентом гантелей.
     * @param intensity Интенсивность упражнения от 1 до 5.
     * @param minutes Длительность упражнения от 1 до 10 минут.
     */
    void use(int intensity, int minutes) override {
        useWithRate(intensity, minutes, BASE_WEAR_RATE);
    }

    /**
     * @brief Выводит текущий износ гантелей.
     */
    void checkWear() const override {
        std::cout << "Dumbbells wear: "
            << getWear() << "%\n";
    }
};

// Перегрузка << — статический полиморфизм: подходящая функция выбирается по типу объекта.
/**
 * @brief Выводит общие характеристики спортивного инвентаря.
 * @param out Поток, в который выводится информация.
 * @param equipment Базовый объект спортивного инвентаря.
 * @return Ссылка на поток вывода для продолжения цепочки <<.
 */
std::ostream& operator<<(std::ostream& out, const SportEquipment& equipment) {
    out << equipment.getType()
        << " | Durability: " << equipment.getDurability()
        << " | Price: " << equipment.getPrice()
        << " | Condition: " << equipment.getCondition() << "%"
        << " | Wear: " << equipment.getWear() << "%";
    return out;
}

/**
 * @brief Выводит общие характеристики и диаметр мяча.
 * @param out Поток, в который выводится информация.
 * @param ball Объект мяча.
 * @return Ссылка на поток вывода для продолжения цепочки <<.
 */
std::ostream& operator<<(std::ostream& out, const Ball& ball) {
    out << static_cast<const SportEquipment&>(ball)
        << " | Diameter: " << ball.getDiameter() << " cm";
    return out;
}

/**
 * @brief Выводит общие характеристики и длину скакалки.
 * @param out Поток, в который выводится информация.
 * @param rope Объект скакалки.
 * @return Ссылка на поток вывода для продолжения цепочки <<.
 */
std::ostream& operator<<(std::ostream& out, const JumpRope& rope) {
    out << static_cast<const SportEquipment&>(rope)
        << " | Length: " << rope.getLength() << " m";
    return out;
}

/**
 * @brief Выводит общие характеристики и вес гантелей.
 * @param out Поток, в который выводится информация.
 * @param dumbbells Объект гантелей.
 * @return Ссылка на поток вывода для продолжения цепочки <<.
 */
std::ostream& operator<<(std::ostream& out, const Dumbbells& dumbbells) {
    out << static_cast<const SportEquipment&>(dumbbells)
        << " | Weight: " << dumbbells.getWeight() << " kg";
    return out;
}

struct UsageRecord {
    int equipmentIndex;
    int intensity;
    int minutes;
    double wear;
};

class Gym {
private:
    static const int MAX_EQUIPMENT = 10;
    static const int MAX_TRAININGS = 100;
    static const int MIN_EXERCISES = 5;
    static const int MAX_EXERCISES = 15;
    static const int MIN_INTENSITY = 1;
    static const int MAX_INTENSITY = 5;
    static const int MIN_MINUTES = 1;
    static const int MAX_MINUTES = 10;
    static const int MAX_HISTORY = 10000;
    const double REPAIR_COST_RATE = 0.2;
    const double REPLACEMENT_WEAR_LIMIT = 70.0;
    const int MAX_REPAIRS_BEFORE_REPLACE = 3;

    UsageRecord _history[MAX_HISTORY];
    int _historyCount = 0;
    SportEquipment* _equipment[MAX_EQUIPMENT];
    int _count;
    int _repairCount[MAX_EQUIPMENT] = {};
    int _breakCount[MAX_EQUIPMENT] = {};
    double _totalRepairSpent = 0;
    int _trainingsCompleted = 0;

public:
    /**
     * @brief Создаёт пустой спортзал.
     */
    Gym() {
        _count = 0;
    }

    /**
     * @brief Добавляет предмет в спортзал и принимает владение им при успехе.
     * @param equipment Указатель на добавляемый предмет.
     * @return true при успешном добавлении; false, если указатель пустой или нет места.
     */
    bool addEquipment(SportEquipment* equipment) {
        if (equipment == nullptr || _count >= MAX_EQUIPMENT) {
            return false;
        }

        _equipment[_count] = equipment;
        _count++;
        return true;
    }
    /**
     * @brief Добавляет в историю запись об одном упражнении, если есть свободное место.
     * @param index Индекс использованного предмета.
     * @param intensity Интенсивность упражнения.
     * @param minutes Длительность упражнения в минутах.
     * @param wear Износ, полученный за упражнение, в процентах.
     */
    void addHistory(int index, int intensity,
        int minutes, double wear) {
        if (_historyCount >= MAX_HISTORY) {
            return;
        }

        _history[_historyCount] = { index, intensity, minutes, wear };

        _historyCount++;
    }

    /**
     * @brief Запрещает копирование спортзала, чтобы не возникло двойного удаления инвентаря.
     */
    Gym(const Gym&) = delete;
    /**
     * @brief Запрещает копирующее присваивание спортзала.
     */
    Gym& operator=(const Gym&) = delete;

    /**
     * @brief Выводит все сохранённые записи об упражнениях.
     */
    void printHistory() const {
        std::cout << "\n--- Usage History ---\n";

        if (_historyCount == 0) {
            std::cout << "History is empty.\n";
            return;
        }

        for (int i = 0; i < _historyCount; i++) {
            int index = _history[i].equipmentIndex;

            std::cout << "\nExercise " << i + 1 << '\n';

            std::cout << "Equipment: "
                << _equipment[index]->getType() << '\n';

            std::cout << "Intensity: "
                << _history[i].intensity << '\n';

            std::cout << "Duration: "
                << _history[i].minutes << " min\n";

            std::cout << "Wear added: "
                << _history[i].wear << "%\n";
        }
    }

    /**
     * @brief Выводит сведения обо всём оборудовании.
     */
    void printEquipment() const {
        for (int i = 0; i < _count; i++) {
            std::cout << "Equipment " << i + 1 << ": ";

            if (Ball* ball = dynamic_cast<Ball*>(_equipment[i])) {
                std::cout << *ball;
            }
            else if (JumpRope* rope = dynamic_cast<JumpRope*>(_equipment[i])) {
                std::cout << *rope;
            }
            else if (Dumbbells* dumbbells = dynamic_cast<Dumbbells*>(_equipment[i])) {
                std::cout << *dumbbells;
            }

            std::cout << '\n';
        }
    }

    /**
     * @brief Подсчитывает количество использований, время и наиболее используемый предмет.
     */
    void printStatistics() const {
        std::cout << "\n Equipment Statistics\n";

        int maxUses = 0;
        int mostUsedIndex = -1;

        for (int i = 0; i < _count; i++) {
            int uses = 0;
            int totalMinutes = 0;

            for (int j = 0; j < _historyCount; j++) {
                if (_history[j].equipmentIndex == i) {
                    uses++;
                    totalMinutes += _history[j].minutes;
                }
            }

            std::cout << "\nEquipment " << i + 1 << ": "
                << _equipment[i]->getType() << '\n';

            std::cout << "Uses: " << uses << '\n';
            std::cout << "Total minutes: " << totalMinutes << '\n';

            if (uses > maxUses) {
                maxUses = uses;
                mostUsedIndex = i;
            }
        }

        if (mostUsedIndex != -1) {
            std::cout << "\nMost used equipment: "
                << _equipment[mostUsedIndex]->getType()
                << " (number " << mostUsedIndex + 1 << ")\n";
        }
    }


    /**
     * @brief Проводит тренировку из случайных упражнений и сохраняет их результаты.
     * @param showDetails true — показывать упражнения; false — скрыть подробности.
     */
    void conductTraining(bool showDetails = true) {
        if (_count == 0) {
            std::cout << "Gym is empty.\n";
            return;
        }

        int exercises = MIN_EXERCISES + rand() % (MAX_EXERCISES - MIN_EXERCISES + 1);

        if (showDetails) {
            std::cout << "\nTraining started!\n";
        }
        _trainingsCompleted++;

        for (int i = 0; i < exercises; i++) {
            int index = findAvailableEquipment();

            if (index == -1) {
                std::cout << "No available equipment.\n";
                break;
            }

            int intensity = MIN_INTENSITY + rand() % (MAX_INTENSITY - MIN_INTENSITY + 1);
            int minutes = MIN_MINUTES + rand() % (MAX_MINUTES - MIN_MINUTES + 1);

            if (showDetails) {
                std::cout << "\nExercise " << i + 1 << '\n';
                std::cout << "Equipment: "
                    << _equipment[index]->getType() << '\n';
                std::cout << "Intensity: " << intensity << '\n';
                std::cout << "Duration: " << minutes << " min\n";
            }

            double oldWear = _equipment[index]->getWear();

            _equipment[index]->use(intensity, minutes);

            if (showDetails) {
                _equipment[index]->checkWear();
            }
            double newWear = _equipment[index]->getWear();

            double wear = newWear - oldWear;

            addHistory(index, intensity, minutes, wear);

            if (_equipment[index]->isBroken()) {
                _breakCount[index]++;
                if (showDetails) {
                    std::cout << "Equipment is broken!\n";
                }
            }
        }

        if (showDetails) {
            std::cout << "\nTraining finished!\n";
        }
    }

    /**
     * @brief Ремонтирует предмет по индексу и учитывает стоимость ремонта.
     * @param index Индекс ремонтируемого предмета в массиве.
     */
    void repairEquipment(int index) {
        if (index < 0 || index >= _count) {
            std::cout << "Invalid index.\n";
            return;
        }

        double oldWear = _equipment[index]->getWear();
        if (oldWear == 0) {
            std::cout << "Equipment does not need repair.\n";
            return;
        }

        double cost = _equipment[index]->repair();
        _repairCount[index]++;
        _totalRepairSpent += cost;

        std::cout << "Equipment repaired.\n";
        std::cout << "Repair cost: " << cost << '\n';

        _equipment[index]->checkWear();
    }

    /**
     * @brief Случайно выбирает один исправный предмет.
     * @return Индекс исправного предмета или -1, если таких предметов нет.
     */
    int findAvailableEquipment() const {
        int available[MAX_EQUIPMENT];
        int availableCount = 0;

        for (int i = 0; i < _count; i++) {
            if (!_equipment[i]->isBroken()) {
                available[availableCount] = i;
                availableCount++;
            }
        }

        if (availableCount == 0) {
            return -1;
        }

        int randomIndex = rand() % availableCount;

        return available[randomIndex];
    }
    /**
     * @brief Выводит износ, поломки, ремонты и общие расходы.
     */
    void printReport() const {
        double totalRepairCost = 0;

        std::cout << "\nTraining Report \n";

        for (int i = 0; i < _count; i++) {
            SportEquipment* equipment = _equipment[i];

            std::cout << "\nEquipment " << i + 1 << ": "
                << equipment->getType() << '\n';

            std::cout << "Wear: "
                << equipment->getWear() << "%\n";

            if (equipment->isBroken()) {
                std::cout << "Status: Broken\n";

                double repairCost = equipment->getPrice() * REPAIR_COST_RATE;
                totalRepairCost += repairCost;

                std::cout << "Repair cost: "
                    << repairCost << '\n';
            }

            if (equipment->getWear() > REPLACEMENT_WEAR_LIMIT ||
                _repairCount[i] >= MAX_REPAIRS_BEFORE_REPLACE) {
                std::cout << "Recommendation: Replace equipment\n";
            }
            std::cout << "Breakdowns: " << _breakCount[i] << '\n';
            std::cout << "Repairs: " << _repairCount[i] << '\n';
        }

        std::cout << "\nRepair cost for currently broken items: "
            << totalRepairCost << '\n';
        std::cout << "Total spent on repairs: "
            << _totalRepairSpent << '\n';
        std::cout << "Trainings completed: " << _trainingsCompleted << '\n';
    }

    /**
     * @brief Ремонтирует все сломанные предметы в спортзале.
     */
    void repairBrokenEquipment() {
        for (int i = 0; i < _count; i++) {
            if (_equipment[i]->isBroken()) {
                repairEquipment(i);
            }
        }
    }
    /**
     * @brief Проводит 100 тренировок, ремонтирует поломки и выводит анализ.
     */
    void simulateTrainings() {
        for (int i = 0; i < MAX_TRAININGS; i++) {
            std::cout << "\nTraining " << i + 1 << '\n';

            conductTraining(false);
            repairBrokenEquipment();
        }

        printStatistics();
        printReport();
        planReplacement();
    }
    /**
     * @brief Выводит рекомендации по замене изношенного оборудования.
     */
    void planReplacement() const {
        std::cout << "\n--- Replacement Plan ---\n";

        for (int i = 0; i < _count; i++) {
            double wear = _equipment[i]->getWear();

            if (_equipment[i]->isBroken()) {
                std::cout << _equipment[i]->getType()
                    << ": Urgent replacement\n";
            }
            else if (wear > REPLACEMENT_WEAR_LIMIT ||
                _repairCount[i] >= MAX_REPAIRS_BEFORE_REPLACE) {
                std::cout << _equipment[i]->getType()
                    << ": Replacement recommended (number " << i + 1 << ")\n";
            }
        }
    }
    /**
     * @brief Освобождает динамически созданные предметы, принадлежащие спортзалу.
     */
    ~Gym() {
        for (int i = 0; i < _count; i++) {
            delete _equipment[i];
        }
    }


};


/**
 * @brief Создаёт спортзал, добавляет инвентарь и запускает консольное меню.
 * @return 0 при штатном завершении программы.
 */
int main()
{
    srand((time(nullptr)));

    Gym gym;
    SportEquipment* equipment[] = {
        new Ball(22.0, 80, 1500.0),
        new JumpRope(2.5, 70, 700.0),
        new Dumbbells(5.0, 90, 2500.0),
        new Ball(24.0, 60, 1200.0),
        new JumpRope(3.0, 75, 900.0),
        new Dumbbells(10.0, 95, 4000.0)
    };

    for (int i = 0; i < 6; i++) {
        if (!gym.addEquipment(equipment[i])) {
            delete equipment[i];
            std::cout << "Could not add equipment.\n";
        }
    }
    // Пример перегрузки << для конкретного производного класса.
    Ball exampleBall(22.0, 80, 1500.0);
    std::cout << "Example: " << exampleBall << '\n';

    int choice = -1;

    while (true) {
        std::cout << "\nGYM MENU\n";
        std::cout << "1. Show equipment\n";
        std::cout << "2. Conduct training\n";
        std::cout << "3. Training report\n";
        std::cout << "4. Usage history\n";
        std::cout << "5. Repair equipment\n";
        std::cout << "6. Usage statistics\n";
        std::cout << "7. Replacement plan\n";
        std::cout << "8. Simulate 100 trainings\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";

        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;
            }
            std::cout << "Invalid input.\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 0) {
            break;
        }

        switch (choice) {
        case 1:
            gym.printEquipment();
            break;
        case 2:
            gym.conductTraining();
            gym.printReport();
            break;
        case 3:
            gym.printReport();
            break;
        case 4:
            gym.printHistory();
            break;
        case 5: {
            int number;
            std::cout << "Equipment number: ";
            if (std::cin >> number) {
                gym.repairEquipment(number - 1);
            }
            else {
                std::cout << "Invalid input.\n";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
            }
            break;
        }
        case 6:
            gym.printStatistics();
            break;
        case 7:
            gym.planReplacement();
            break;
        case 8:
            gym.simulateTrainings();
            break;
        default:
            std::cout << "Unknown menu item.\n";
        }
    }

    return 0;
}
