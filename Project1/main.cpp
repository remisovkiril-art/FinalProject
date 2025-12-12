#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Element {
protected:
    string imya; // имя элемента
public:
    Element(const string& n) : imya(n) {}
    virtual ~Element() {}

    string GetName() const { return imya; }
    void Rename(const string& n) { imya = n; }
    virtual int GetSize() const = 0;
    virtual void Show(int otstup) const = 0;
    virtual bool IsFolder() const = 0;
    // Копирование базового типа
    virtual Element* Copy() const = 0;
};
class File : public Element {
    int razmer;
public:
    File(const string& n, int r) : Element(n), razmer(r) {}
    int GetSize() const override { return razmer; }
    void Show(int otstup) const override {
        for (int i = 0; i < otstup; i++) cout << " ";
        cout << "[Файл] " << imya << " (" << razmer << " байт)\n";
    }
    bool IsFolder() const override { return false; }
    // Реализация метода копирования файла
    Element* Copy() const override {
        return new File(imya, razmer);
    }
};
class Folder : public Element {
    vector<Element*> spisok;
public:
    Folder(const string& n) : Element(n) {}

    ~Folder() {
        for (auto p : spisok) delete p;
    }
    bool IsFolder() const override { return true; }
    void Show(int otstup) const override {
        for (int i = 0; i < otstup; i++) cout << " ";
        cout << "[Папка] " << imya << "\n";

        for (auto p : spisok)
            p->Show(otstup + 2);
    }
    int GetSize() const override {
        int sum = 0;
        for (auto p : spisok) sum += p->GetSize();
        return sum;
    }
    // Создать файл
    void AddFile(const string& n, int r) {
        spisok.push_back(new File(n, r));
    }
    // Создать папку
    Folder* AddFolder(const string& n) {
        Folder* f = new Folder(n);
        spisok.push_back(f);
        return f;
    }
    // Удаление
    void Delete(const string& n) {
        for (int i = 0; i < spisok.size(); i++) {
            if (spisok[i]->GetName() == n) {
                delete spisok[i];
                spisok.erase(spisok.begin() + i);
                cout << "Удалено: " << n << "\n";
                return;
            }
        }
        cout << "Не найдено.\n";
    }
    // Переименование
    void RenameElement(const string& old, const string& nov) {
        for (auto p : spisok)
            if (p->GetName() == old) {
                p->Rename(nov);
                cout << "Готово.\n";
                return;
            }
        cout << "Не найдено.\n";
    }
    // Поиск по маске
    void Search(const string& maska) const {
        for (auto p : spisok) {
            if (p->GetName().find(maska) != string::npos)
                cout << "Найдено: " << p->GetName() << "\n";

            if (p->IsFolder())
                ((Folder*)p)->Search(maska);
        }
    }
    // Копирование
    void CopyElement(const string& name) {
        for (auto p : spisok) {
            if (p->GetName() == name) {
                Element* kopya = p->Copy();
                kopya->Rename(name + "_copy");
                spisok.push_back(kopya);
                cout << "Сделано: копия создана.\n";
                return;
            }
        }
        cout << "Не найдено.\n";
    }// Перемещение
    void MoveElement(const string& name, Folder* newPlace) {
        for (int i = 0; i < spisok.size(); i++) {
            if (spisok[i]->GetName() == name) {
                newPlace->spisok.push_back(spisok[i]);
                spisok.erase(spisok.begin() + i);
                cout << "Элемент перемещён.\n";
                return;
            }
        }
        cout << "Не найдено.\n";
    }
    Element* Copy() const override {
        Folder* f = new Folder(imya);
        for (auto p : spisok)
            f->spisok.push_back(p->Copy());
        return f;
    }
};
class Manager {
    Folder* disk;
public:
    Manager() { disk = new Folder("DISK_C"); }
    ~Manager() { delete disk; }
    void Menu() {
        int vibor;
        while (true) {
            cout << "\nФайловый менеджер\n";
            cout << "1. Показать содержимое\n";
            cout << "2. Создать папку\n";
            cout << "3. Создать файл\n";
            cout << "4. Удалить\n";
            cout << "5. Переименовать\n";
            cout << "6. Поиск по маске\n";
            cout << "7. Показать размер\n";
            cout << "8. Копировать\n";
            cout << "9. Переместить\n";
            cout << "0. Выход\n";
            cout << "Выбор: ";
            cin >> vibor;
            if (vibor == 0) break;
            string n, n2;
            int r;
            if (vibor == 1) disk->Show(0);
            else if (vibor == 2) {
                cout << "Имя: "; cin >> n;
                disk->AddFolder(n);
            }
            else if (vibor == 3) {
                cout << "Имя: "; cin >> n;
                cout << "Размер: "; cin >> r;
                disk->AddFile(n, r);
            }
            else if (vibor == 4) {
                cout << "Имя: "; cin >> n;
                disk->Delete(n);
            }
            else if (vibor == 5) {
                cout << "Имя: "; cin >> n;
                cout << "Новое имя: "; cin >> n2;
                disk->RenameElement(n, n2);
            }
            else if (vibor == 6) {
                cout << "Маска: "; cin >> n;
                disk->Search(n);
            }
            else if (vibor == 7) {
                cout << "Размер: " << disk->GetSize() << " байт\n";
            }
            else if (vibor == 8) {
                cout << "Имя: "; cin >> n;
                disk->CopyElement(n);
            }
            else if (vibor == 9) {
                cout << "Имя: "; cin >> n;
                disk->MoveElement(n, disk);
            }
        }
    }
};
int main() {
    setlocale(LC_ALL, "Russian");
    Manager m;
    m.Menu();
    return 0;
}
