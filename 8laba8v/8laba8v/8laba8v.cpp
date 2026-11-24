#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


/** @brief Интерфейс заметки. */
class INote {
public:
    virtual int getId() const = 0;
    virtual std::string getTitle() const = 0;
    virtual std::string getBody() const = 0;
    virtual const std::vector<std::string>& getTags() const = 0;

    virtual ~INote() = default;
};
/** @brief Класс заметки. */
class Note : public INote {
private:
    int id;
    std::string title;
    std::string body;
    std::vector<std::string> tags;

public:
    /**
     * @brief Создаёт заметку.
     * @param id Идентификатор заметки.
     * @param title Заголовок заметки.
     * @param body Текст заметки.
     */
    Note(int id, const std::string& title, const std::string& body)
        : id(id), title(title), body(body) {
    }
    int getId() const override {
        return id;
    }

    std::string getTitle() const override {
        return title;
    }
    std::string getBody() const override {
        return body;
    }

    /**
     * @brief Возвращает список тегов.
     * @return Константная ссылка на вектор тегов.
     */
    const std::vector<std::string>& getTags() const override {
        return tags;
    }

    /**
     * @brief Добавляет тег без повторений.
     * @param tag Название добавляемого тега.
     */
    void addTag(const std::string& tag) {
        if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
            tags.push_back(tag);
        }
    }

    /**
 * @brief Заменяет старый тег новым без повторений.
 * @param oldTag Старый тег.
 * @param newTag Новый тег.
 * @return true, если старый тег найден.
 */
    bool replaceTag(const std::string& oldTag, const std::string& newTag) {
        auto it = std::find(tags.begin(), tags.end(), oldTag);

        if (it == tags.end()) {
            return false;
        }

        if (oldTag == newTag) {
            return true;
        }

        if (std::find(tags.begin(), tags.end(), newTag) != tags.end()) {
            tags.erase(it);
        }
        else {
            *it = newTag;
        }

        return true;
    }








};
/** @brief Интерфейс хранилища заметок. */
class INoteRepo {
public:
    virtual bool addNote(const Note& note) = 0;
    virtual bool removeNote(int id) = 0;
    virtual const INote* findById(int id) const = 0;
    virtual std::vector<Note> findByTag(const std::string& tag) const = 0;

    virtual ~INoteRepo() = default;
};

/** @brief Хранилище заметок с индексом тегов. */
class NoteRepo : public INoteRepo {
private:
    std::vector<Note> notes;
    std::map<std::string, std::vector<int>> tagIndex;

public:

    /**
     * @brief Добавляет заметку и обновляет индекс тегов.
     * @param note Добавляемая заметка.
     * @return true, если заметка добавлена.
     */
    bool addNote(const Note& note) override {
        for (const auto& current : notes) {
            if (current.getId() == note.getId()) {
                return false;
            }
        }

        notes.push_back(note);

        for (const auto& tag : note.getTags()) {
            tagIndex[tag].push_back(note.getId());
        }

        return true;
    }

    /**
 * @brief Ищет заметку по идентификатору.
 * @param id Идентификатор заметки.
 * @return Указатель на найденную заметку или nullptr.
 */
    const INote* findById(int id) const override {
        for (const auto& note : notes) {
            if (note.getId() == id) {
                return &note;
            }
        }

        return nullptr;
    }


    /**
 * @brief Ищет все заметки по тегу.
 * @param tag Искомый тег.
 * @return Вектор найденных заметок.
 */
    std::vector<Note> findByTag(const std::string& tag) const override {
        std::vector<Note> result;

        auto it = tagIndex.find(tag);

        if (it == tagIndex.end()) {
            return result;
        }

        for (int id : it->second) {
            const INote* note = findById(id);

            if (note != nullptr) {
                for (const auto& current : notes) {
                    if (current.getId() == id) {
                        result.push_back(current);
                        break;
                    }
                }
            }
        }

        return result;
    }

    /**
 * @brief Удаляет заметку и обновляет индекс тегов.
 * @param id Идентификатор заметки.
 * @return true, если заметка удалена.
 */
    bool removeNote(int id) override {
        for (auto it = notes.begin(); it != notes.end(); ++it) {
            if (it->getId() == id) {

                for (const auto& tag : it->getTags()) {
                    auto indexIt = tagIndex.find(tag);

                    if (indexIt != tagIndex.end()) {
                        auto& ids = indexIt->second;

                        ids.erase(
                            std::remove(ids.begin(), ids.end(), id),
                            ids.end()
                        );

                        if (ids.empty()) {
                            tagIndex.erase(indexIt);
                        }
                    }
                }

                notes.erase(it);
                return true;
            }
        }

        return false;
    }


    /**
 * @brief Заменяет тег во всех заметках и обновляет индекс.
 * @param oldTag Старый тег.
 * @param newTag Новый тег.
 * @return Количество изменённых заметок.
 */
    int replaceTagAll(const std::string& oldTag,
        const std::string& newTag) {
        if (oldTag == newTag) {
            return 0;
        }

        int count = 0;

        for (auto& note : notes) {
            if (note.replaceTag(oldTag, newTag)) {
                ++count;
            }
        }

        if (count > 0) {
            tagIndex.clear();

            for (const auto& note : notes) {
                for (const auto& tag : note.getTags()) {
                    tagIndex[tag].push_back(note.getId());
                }
            }
        }

        return count;
    }
};


/** @brief Пользовательские настройки. */
class UserSettings {
private:
    std::string username;

public:
    /**
     * @brief Создаёт настройки пользователя.
     * @param name Имя пользователя.
     */
    UserSettings(const std::string& name) : username(name) {}

    /**
     * @brief Возвращает имя пользователя.
     * @return Имя пользователя.
     */
    std::string getUsername() const {
        return username;
    }
};
/** @brief Блокнот, объединяющий репозиторий и настройки. */
class Notebook {
private:
    NoteRepo& repo;
    UserSettings& settings;

public:
    /**
     * @brief Создаёт блокнот.
     * @param r Репозиторий заметок.
     * @param s Пользовательские настройки.
     */
    Notebook(NoteRepo& r, UserSettings& s)
        : repo(r), settings(s) {
    }

    /** @brief Выводит информацию о пользователе. */
    void showInfo() const {
        std::cout << "Notebook owner: "
            << settings.getUsername() << '\n';
    }

    /**
     * @brief Заменяет тег во всех заметках.
     * @param oldTag Старый тег.
     * @param newTag Новый тег.
     * @return Количество изменённых заметок.
     */
    int replaceTags(const std::string& oldTag,
        const std::string& newTag) {
        return repo.replaceTagAll(oldTag, newTag);
    }
};



int main() {
    NoteRepo repo;

    Note note1(1, "C++", "Learn containers");
    note1.addTag("study");
    note1.addTag("cpp");

    Note note2(2, "SQL", "Learn databases");
    note2.addTag("study");
    note2.addTag("database");

    repo.addNote(note1);
    repo.addNote(note2);

    std::cout << "Search by ID:\n";
    const INote* found = repo.findById(1);

    if (found != nullptr) {
        std::cout << found->getTitle() << '\n';
    }

    std::cout << "\nSearch by tag:\n";

    for (const auto& note : repo.findByTag("study")) {
        std::cout << note.getId() << ": "
            << note.getTitle() << '\n';
    }

    std::cout << "\nDelete note:\n";

    if (repo.removeNote(1)) {
        std::cout << "Note deleted.\n";
    }

    std::cout << "\nSearch after deletion:\n";

    for (const auto& note : repo.findByTag("study")) {
        std::cout << note.getId() << ": "
            << note.getTitle() << '\n';
    }

    UserSettings settings("Dasha");

    Notebook notebook(repo, settings);

    notebook.showInfo();

    int count = notebook.replaceTags("study", "education");

    std::cout << "Changed notes: " << count << '\n';

    return 0;
}