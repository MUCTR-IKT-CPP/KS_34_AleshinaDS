#include <iostream>
#include <utility>

struct Texture { int width; int height; };
struct Mesh { int vertices_count; };
struct Sound { float duration; };

enum class ResourceType {
    TEXTURE,
    MESH,
    SOUND
};

class Resource {
private:
    void* _p_data;
    ResourceType _type;
    bool _is_loaded;
    static void* copyData(const Resource& other);
    void freeData();

public:
    Resource();
    Resource(const Texture& texture);
    Resource(const Mesh& mesh);
    Resource(const Sound& sound);

    Resource(const Resource& other);
    Resource(Resource&& other) noexcept;

    Resource& operator=(const Resource& other);
    Resource& operator=(Resource&& other) noexcept;

    ~Resource();
    void unload();
    void print() const;
    bool isLoaded() const;
    ResourceType getType() const;
};


void* Resource::copyData(const Resource& other) {
    if (other._p_data == nullptr) {
        return nullptr;
    }

    switch (other._type) {
    case ResourceType::TEXTURE:
        return new Texture(
            *static_cast<const Texture*>(other._p_data)
        );

    case ResourceType::MESH:
        return new Mesh(
            *static_cast<const Mesh*>(other._p_data)
        );

    case ResourceType::SOUND:
        return new Sound(
            *static_cast<const Sound*>(other._p_data)
        );
    }

    return nullptr;
}


void Resource::freeData() {
    if (_p_data == nullptr) {
        _is_loaded = false;
        return;
    }

    switch (_type) {
    case ResourceType::TEXTURE:
        delete static_cast<Texture*>(_p_data);
        break;

    case ResourceType::MESH:
        delete static_cast<Mesh*>(_p_data);
        break;

    case ResourceType::SOUND:
        delete static_cast<Sound*>(_p_data);
        break;
    }

    _p_data = nullptr;
    _is_loaded = false;
}

Resource::Resource()
    : _p_data(nullptr),
    _type(ResourceType::TEXTURE),
    _is_loaded(false) {
}

Resource::Resource(const Texture& texture)
    : _p_data(new Texture(texture)),
    _type(ResourceType::TEXTURE),
    _is_loaded(true) {

    std::cout << "Resource: Texture constructor\n";
}

Resource::Resource(const Mesh& mesh)
    : _p_data(new Mesh(mesh)),
    _type(ResourceType::MESH),
    _is_loaded(true) {

    std::cout << "Resource: Mesh constructor\n";
}

Resource::Resource(const Sound& sound)
    : _p_data(new Sound(sound)),
    _type(ResourceType::SOUND),
    _is_loaded(true) {

    std::cout << "Resource: Sound constructor\n";
}

// Копирующий конструктор
Resource::Resource(const Resource& other)
    : _p_data(copyData(other)),
    _type(other._type),
    _is_loaded(other._is_loaded) {

    std::cout << "Resource: copy constructor\n";
}

// Перемещающий конструктор
Resource::Resource(Resource&& other) noexcept
    : _p_data(other._p_data),
    _type(other._type),
    _is_loaded(other._is_loaded) {

    other._p_data = nullptr;
    other._is_loaded = false;

    std::cout << "Resource: move constructor\n";
}

Resource& Resource::operator=(const Resource& other) {
    if (this == &other) {
        return *this;
    }

    void* newData = copyData(other);
    freeData();

    _p_data = newData;
    _type = other._type;
    _is_loaded = other._is_loaded;

    std::cout << "Resource: copy assignment\n";

    return *this;
}

Resource& Resource::operator=(Resource&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    freeData();

    _p_data = other._p_data;
    _type = other._type;
    _is_loaded = other._is_loaded;

    other._p_data = nullptr;
    other._is_loaded = false;

    std::cout << "Resource: move assignment\n";

    return *this;
}

Resource::~Resource() {
    if (_p_data != nullptr) {
        std::cout << "Resource: destructor\n";
    }

    freeData();
}

void Resource::unload() {
    freeData();
}

void Resource::print() const {
    if (!_is_loaded) {
        std::cout << "Resource not loaded\n";
        return;
    }

    switch (_type) {
    case ResourceType::TEXTURE: {
        const Texture* p = static_cast<const Texture*>(_p_data);

        std::cout << "Texture: " << p->width
            << " x " << p->height << "\n";
        break;
    }

    case ResourceType::MESH: {
        const Mesh* p = static_cast<const Mesh*>(_p_data);

        std::cout << "Mesh: " << p->vertices_count
            << " vertices\n";
        break;
    }

    case ResourceType::SOUND: {
        const Sound* p = static_cast<const Sound*>(_p_data);

        std::cout << "Sound: " << p->duration
            << " seconds\n";
        break;
    }
    }
}

bool Resource::isLoaded() const {
    return _is_loaded;
}

ResourceType Resource::getType() const {
    return _type;
}

class ResourceManager {
private:
    Resource* _resources;
    int _count;
    int _capacity;
    void resize(int newCapacity);

public:
    ResourceManager();
    ResourceManager(int capacity);

    ResourceManager(const ResourceManager& other);
    ResourceManager(ResourceManager&& other) noexcept;

    ResourceManager& operator=(const ResourceManager& other);
    ResourceManager& operator=(ResourceManager&& other) noexcept;

    ~ResourceManager();

    void addResource(const Resource& resource);
    void unloadResource(int index);
    int findByType(ResourceType type) const;

    void printAll() const;
    void printStatistics() const;
};

ResourceManager::ResourceManager()
    : _resources(nullptr),
    _count(0),
    _capacity(0) {

    std::cout << "Manager: default constructor\n";
}

ResourceManager::ResourceManager(int capacity)
    : _resources(nullptr),
    _count(0),
    _capacity(capacity > 0 ? capacity : 0) {

    if (_capacity > 0) {
        _resources = new Resource[_capacity];
    }

    std::cout << "Manager: parameterized constructor\n";
}


ResourceManager::ResourceManager(const ResourceManager& other)
    : _resources(nullptr),
    _count(0),
    _capacity(other._capacity) {

    if (_capacity > 0) {
        _resources = new Resource[_capacity];

        try {
            for (int i = 0; i < other._count; i++) {
                _resources[i] = other._resources[i];
                _count++;
            }
        }
        catch (...) {
            delete[] _resources;
            throw;
        }
    }

    std::cout << "Manager: copy constructor\n";
}

ResourceManager::ResourceManager(ResourceManager&& other) noexcept
    : _resources(other._resources),
    _count(other._count),
    _capacity(other._capacity) {

    other._resources = nullptr;
    other._count = 0;
    other._capacity = 0;

    std::cout << "Manager: move constructor\n";
}

ResourceManager& ResourceManager::operator=(const ResourceManager& other) {
    if (this == &other) {
        return *this;
    }

    Resource* newResources = nullptr;

    if (other._capacity > 0) {
        newResources = new Resource[other._capacity];

        try {
            for (int i = 0; i < other._count; i++) {
                newResources[i] = other._resources[i];
            }
        }
        catch (...) {
            delete[] newResources;
            throw;
        }
    }

    delete[] _resources;

    _resources = newResources;
    _count = other._count;
    _capacity = other._capacity;

    std::cout << "Manager: copy assignment\n";

    return *this;
}

ResourceManager& ResourceManager::operator=(ResourceManager&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    delete[] _resources;

    _resources = other._resources;
    _count = other._count;
    _capacity = other._capacity;

    other._resources = nullptr;
    other._count = 0;
    other._capacity = 0;

    std::cout << "Manager: move assignment\n";

    return *this;
}


ResourceManager::~ResourceManager() {
    delete[] _resources;

    std::cout << "Manager: destructor\n";
}


void ResourceManager::resize(int newCapacity) {
    if (newCapacity <= _capacity) {
        return;
    }

    Resource* newResources = new Resource[newCapacity];

    for (int i = 0; i < _count; i++) {
        newResources[i] = std::move(_resources[i]);
    }

    delete[] _resources;

    _resources = newResources;
    _capacity = newCapacity;

    std::cout << "Manager: resized to " << _capacity << "\n";
}


void ResourceManager::addResource(const Resource& resource) {
    if (_count >= _capacity) {
        int newCapacity = (_capacity == 0) ? 2 : _capacity * 2;

        resize(newCapacity);
    }

    _resources[_count] = resource;
    _count++;
}

void ResourceManager::unloadResource(int index) {
    if (index < 0 || index >= _count) {
        std::cout << "Invalid resource index\n";
        return;
    }

    _resources[index].unload();
}

int ResourceManager::findByType(ResourceType type) const {
    int found = 0;

    for (int i = 0; i < _count; i++) {
        if (_resources[i].isLoaded() &&
            _resources[i].getType() == type) {

            std::cout << "Found at index " << i << ": ";
            _resources[i].print();

            found++;
        }
    }

    return found;
}

void ResourceManager::printAll() const {
    std::cout << "Resources: " << _count
        << ", capacity: " << _capacity << "\n";

    for (int i = 0; i < _count; i++) {
        std::cout << "[" << i << "] ";
        _resources[i].print();
    }
}


int main() {
    ResourceManager manager(2);
    int choice = -1;

    do {
        std::cout << "\nМе\n";
        std::cout << "1. Load resource\n";
        std::cout << "2. Unload resource\n";
        std::cout << "3. Find resources by type\n";
        std::cout << "4. Print resource report\n";
        std::cout << "0. Exit\n";
        std::cout << "Your choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input\n";
            return 1;
        }

        switch (choice) {
        case 1: {
            int type;
            std::cout << "Resource type (1 - Texture, 2 - Mesh, 3 - Sound): ";
            if (!(std::cin >> type)) return 1;

            switch (type) {
            case 1: {
                int width, height;
                std::cout << "Width and height: ";
                if (!(std::cin >> width >> height)) return 1;
                if (width <= 0 || height <= 0) {
                    std::cout << "Dimensions must be positive\n";
                    break;
                }
                Texture texture{ width, height };
                Resource resource(texture);
                manager.addResource(resource);
                break;
            }
            case 2: {
                int vertices;
                std::cout << "Number of vertices: ";
                if (!(std::cin >> vertices)) return 1;
                if (vertices <= 0) {
                    std::cout << "Number of vertices must be positive\n";
                    break;
                }
                Mesh mesh{ vertices };
                Resource resource(mesh);
                manager.addResource(resource);
                break;
            }
            case 3: {
                float duration;
                std::cout << "Sound duration: ";
                if (!(std::cin >> duration)) return 1;
                if (!(duration > 0.0f)) {
                    std::cout << "Duration must be positive\n";
                    break;
                }
                Sound sound{ duration };
                Resource resource(sound);
                manager.addResource(resource);
                break;
            }
            default:
                std::cout << "Unknown resource type\n";
                break;
            }
            break;
        }
        case 2: {
            int index;
            std::cout << "Index to unload: ";
            if (!(std::cin >> index)) return 1;
            manager.unloadResource(index);
            break;
        }
        case 3: {
            int type;
            std::cout << "Find type (1 - Texture, 2 - Mesh, 3 - Sound): ";
            if (!(std::cin >> type)) return 1;

            ResourceType selected;
            switch (type) {
            case 1: selected = ResourceType::TEXTURE; break;
            case 2: selected = ResourceType::MESH; break;
            case 3: selected = ResourceType::SOUND; break;
            default:
                std::cout << "Unknown resource type\n";
                break;
            }
            if (type >= 1 && type <= 3) {
                int found = manager.findByType(selected);
                std::cout << "Found: " << found << "\n";
            }
            break;
        }
        case 4:
            manager.printAll();
            break;
        case 0:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Unknown menu option\n";
            break;
        }
    } while (choice != 0);

    return 0;
}