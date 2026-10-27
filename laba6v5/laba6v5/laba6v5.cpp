#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <utility>

/** Абстрактный базовый класс геометрических фигур. */
class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void printInfo() const = 0;
    virtual ~Shape() = default;

protected:
    virtual void printHeader() const = 0;
};

void Shape::printInfo() const {
    printHeader();
    std::cout << " | Area: " << area()
        << " | Perimeter: " << perimeter() << '\n';
}

/** Интерфейс для изменения линейных размеров. */
class IScalable {
public:
    /**
     * @brief Масштабирует фигуру.
     * @param factor Положительный коэффициент масштабирования.
     */
    virtual void scale(double factor) = 0;
    virtual ~IScalable() = default;
};

class Circle : public Shape, public IScalable {
private:
    const double PI = 3.141592653589793;
    double _radius;

public:
    /** @brief Создаёт круг. @param radius Радиус круга. */
    Circle(double radius) {
        if (!std::isfinite(radius) || radius <= 0) {
            std::cout << "Invalid radius. Using 1.0.\n";
            radius = 1.0;
        }
        _radius = radius;
    }

    /** @brief Вычисляет площадь круга. @return Площадь. */
    double area() const override {
        return PI * _radius * _radius;
    }

    /** @brief Вычисляет длину окружности. @return Периметр. */
    double perimeter() const override {
        return 2 * PI * _radius;
    }

    /** @brief Выводит характеристики круга через общую реализацию. */
    void printInfo() const override {
        Shape::printInfo();
    }

    /** @brief Меняет радиус круга. @param factor Коэффициент масштабирования. */
    void scale(double factor) override {
        if (!std::isfinite(factor) || factor <= 0 || !std::isfinite(_radius * factor)) {
            std::cout << "Invalid scale factor.\n";
            return;
        }
        _radius *= factor;
    }

protected:
    /** @brief Выводит тип и радиус круга. */
    void printHeader() const override {
        std::cout << "Circle | Radius: " << _radius;
    }
};

class Rectangle : public Shape, public IScalable {
private:
    double _width;
    double _height;

public:
    /**
     * @brief Создаёт прямоугольник.
     * @param width Ширина.
     * @param height Высота.
     */
    Rectangle(double width, double height) {
        if (!std::isfinite(width) || width <= 0) {
            std::cout << "Invalid width. Using 1.0.\n";
            width = 1.0;
        }
        if (!std::isfinite(height) || height <= 0) {
            std::cout << "Invalid height. Using 1.0.\n";
            height = 1.0;
        }
        _width = width;
        _height = height;
    }

    /** @brief Вычисляет площадь прямоугольника. @return Площадь. */
    double area() const override {
        return _width * _height;
    }

    /** @brief Вычисляет периметр прямоугольника. @return Периметр. */
    double perimeter() const override {
        return 2 * (_width + _height);
    }

    /** @brief Выводит характеристики прямоугольника через общую реализацию. */
    void printInfo() const override {
        Shape::printInfo();
    }

    /** @brief Меняет ширину и высоту. @param factor Коэффициент масштабирования. */
    void scale(double factor) override {
        if (!std::isfinite(factor) || factor <= 0 ||
            !std::isfinite(_width * factor) || !std::isfinite(_height * factor)) {
            std::cout << "Invalid scale factor.\n";
            return;
        }
        _width *= factor;
        _height *= factor;
    }

protected:
    /** @brief Выводит тип и размеры прямоугольника. */
    void printHeader() const override {
        std::cout << "Rectangle | Width: " << _width
            << " | Height: " << _height;
    }
};

class Triangle : public Shape {
private:
    double _sideA;
    double _sideB;
    double _sideC;

public:
    /**
     * @brief Создаёт треугольник и проверяет существование сторон.
     * @param a Первая сторона.
     * @param b Вторая сторона.
     * @param c Третья сторона.
     */
    Triangle(double a, double b, double c) {
        if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c) ||
            a <= 0 || b <= 0 || c <= 0 ||
            a + b <= c || a + c <= b || b + c <= a) {
            std::cout << "Invalid triangle. Using sides 3, 4, 5.\n";
            a = 3;
            b = 4;
            c = 5;
        }
        _sideA = a;
        _sideB = b;
        _sideC = c;
    }

    /** @brief Вычисляет периметр. @return Сумма трёх сторон. */
    double perimeter() const override {
        return _sideA + _sideB + _sideC;
    }

    /** @brief Вычисляет площадь по формуле Герона. @return Площадь. */
    double area() const override {
        double p = perimeter() / 2;
        return std::sqrt(p * (p - _sideA) * (p - _sideB) * (p - _sideC));
    }

    /** @brief Выводит характеристики треугольника через общую реализацию. */
    void printInfo() const override {
        Shape::printInfo();
    }

protected:
    /** @brief Выводит тип и стороны треугольника. */
    void printHeader() const override {
        std::cout << "Triangle | Sides: " << _sideA << ", " << _sideB
            << ", " << _sideC;
    }
};

class ShapeGroup {
private:
    std::vector<std::unique_ptr<Shape>> _shapes;

public:
    /**
     * @brief Добавляет фигуру в группу и принимает владение ею.
     * @param s Указатель на динамически созданную фигуру.
     */
    void addShape(Shape* s) {
        if (s != nullptr) {
            _shapes.emplace_back(s);
        }
    }

    /** @brief Вычисляет суммарную площадь. @return Сумма площадей фигур. */
    double totalArea() const {
        double total = 0;
        for (const auto& shape : _shapes) {
            total += shape->area();
        }
        return total;
    }

    /** @brief Вычисляет суммарный периметр. @return Сумма периметров. */
    double totalPerimeter() const {
        double total = 0;
        for (const auto& shape : _shapes) {
            total += shape->perimeter();
        }
        return total;
    }

    /** @brief Выводит информацию обо всех фигурах. */
    void printAll() const {
        for (std::size_t i = 0; i < _shapes.size(); i++) {
            std::cout << "Shape " << i + 1 << ": ";
            _shapes[i]->printInfo();
        }
    }

    /**
     * @brief Масштабирует выбранную фигуру, если она поддерживает IScalable.
     * @param index Индекс фигуры, начиная с нуля.
     * @param factor Коэффициент масштабирования.
     */
    void scaleShape(int index, double factor) {
        if (index < 0 || static_cast<std::size_t>(index) >= _shapes.size() ||
            !std::isfinite(factor) || factor <= 0) {
            std::cout << "Invalid index or scale factor.\n";
            return;
        }

        IScalable* scalable = dynamic_cast<IScalable*>(_shapes[index].get());
        if (scalable == nullptr) {
            std::cout << "This shape does not support scaling.\n";
            return;
        }

        scalable->scale(factor);
        std::cout << "Scaling attempted.\n";
    }

    /** @brief Проводит три этапа масштабирования и пересчитывает параметры. */
    void simulateScaling() {
        const double FACTORS[3] = { 2.0, 0.5, 1.5 };
        for (int step = 0; step < 3; step++) {
            std::cout << "\nSimulation step " << step + 1
                << " | Factor: " << FACTORS[step] << '\n';

            for (const auto& shape : _shapes) {
                IScalable* scalable = dynamic_cast<IScalable*>(shape.get());
                if (scalable != nullptr) {
                    scalable->scale(FACTORS[step]);
                }
            }
            printAll();
            std::cout << "Total area: " << totalArea() << '\n';
            std::cout << "Total perimeter: " << totalPerimeter() << '\n';
        }
    }
};

/** @brief Создаёт фигуры и запускает консольное меню. @return 0 при завершении. */
int main() {
    ShapeGroup group;
    group.addShape(new Circle(5));
    group.addShape(new Rectangle(4, 6));
    group.addShape(new Triangle(3, 4, 5));
    group.addShape(new Circle(3));
    group.addShape(new Rectangle(2, 8));

    while (true) {
        std::cout << "\nSHAPE MENU\n";
        std::cout << "1. Show all shapes\n";
        std::cout << "2. Total area and perimeter\n";
        std::cout << "3. Scale one shape\n";
        std::cout << "4. Simulate scaling\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";

        int choice;
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
            group.printAll();
            break;
        case 2:
            std::cout << "Total area: " << group.totalArea() << '\n';
            std::cout << "Total perimeter: " << group.totalPerimeter() << '\n';
            break;
        case 3: {
            int number;
            double factor;
            std::cout << "Shape number: ";
            if (!(std::cin >> number)) {
                std::cout << "Invalid input.\n";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                break;
            }
            std::cout << "Scale factor: ";
            if (!(std::cin >> factor)) {
                std::cout << "Invalid input.\n";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                break;
            }
            group.scaleShape(number - 1, factor);
            break;
        }
        case 4:
            group.simulateScaling();
            break;
        default:
            std::cout << "Unknown menu item.\n";
        }
    }
    return 0;
}
