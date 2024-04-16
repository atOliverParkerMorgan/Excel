#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <memory>

// Levely jsou 0 az 6
#define LEVEL 0
// Vypis obsahu Bagu ano/ne
#define PRINT 1

/*
TODO: class Item
 - konstruktor (jmeno, vaha)
 - metoda weight() vraci vahu
 - operator << pro vypis do streamu

TODO: class Bag
 - konstruktor (jmeno, max. vaha, max. pocet polozek), samotny bag ma vahu 0
 - metoda weight() vraci vahu celeho Bagu vcetne vnorenych Bagu
 - metoda insert(Item/Bag) vlozi do Bagu typ Item nebo Bag
 - metoda contains() vraci true/false, pokud Item/Bag je uvnitr
 - operator << pro vypis do streamu v podobnem formatu jako tento (vcetne odsazeni):
skrin (items: 1/5, weight: 23/100)
  kufr (items: 2/3, weight: 23/40)
    slanina (weight: 8)
    sportovni taska (items: 2/3, weight: 15/30)
      tuzka (weight: 5)
      lahev rumu (weight: 10)
*/

class Thing {
protected:
    std::string _name;
public:
    Thing(const std::string &name) : _name(name) {};

    virtual ~Thing() {};

    virtual size_t weight() const = 0;

    virtual void print(std::ostream &os) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Thing &t) {
        t.print(os);
        return os;
    }

    virtual std::shared_ptr<Thing> clone() const = 0;
};

class Item : public Thing {
protected:
    size_t _weight;
public:
    Item(const std::string &name, size_t weight) : Thing(name), _weight(weight) {};

    size_t weight() const override { return _weight; }

    void print(std::ostream &os) const override {
        os << _name << "( weight: " << _weight << ")" << std::endl;
    }

    std::shared_ptr<Thing> clone() const override {
        return std::make_shared<Item>(*this);
    }
};

class Bag : public Thing {

protected:
    size_t max_weight, max_items;
    std::list<std::shared_ptr<Thing>> _items;
public:
    Bag(const std::string &name, size_t max_w, size_t max_i) : Thing(name), max_weight(max_w), max_items(max_i) {}

    bool insert(const Thing &t) {
        _items.push_back(t.clone());
        return true;
    }

    size_t weight() const override {
        size_t w = 0;
        for (const auto &item: _items) {
            w += item->weight();
        }
        return w;
    }

    std::shared_ptr<Thing> clone() const override {
        return std::make_shared<Bag>(*this);
    }

    void print(std::ostream &os) const override {
        os << _name << "( weight: " << weight() << "/" << max_weight << ", " << _items.size() << "/" << max_items << ")"
           << std::endl;
        for (const auto &item: _items) {
            item->print(os);
        }

    };
};

int main() {

#if LEVEL >= 0
    std::cout << "--- LEVEL 0 ---" << std::endl;
    Item tuzka("tuzka", 5);
    Item lahev("lahev rumu", 10);
    Item slanina("slanina", 8);
    Bag sportovni_taska("sportovni taska", 30, 3);
    Bag kufr("kufr", 40, 3);
    Bag skrin("skrin", 100, 5);
    sportovni_taska.insert(tuzka);
    sportovni_taska.insert(lahev);
    kufr.insert(slanina);
    if (PRINT) {
        std::cout << lahev;
        std::cout << sportovni_taska;
        std::cout << kufr;
        std::cout << skrin;
    }
    std::cout << "LEVEL 0 OK" << std::endl;
#endif

#if LEVEL >= 1
    std::cout << "--- LEVEL 1 ---" << std::endl;
    kufr.insert(sportovni_taska);
    skrin.insert(kufr);
    if (PRINT)
        std::cout << skrin;

    std::cout << "LEVEL 1 OK" << std::endl;
#endif

#if LEVEL >= 2
    std::cout << "--- LEVEL 2 ---" << std::endl;

    assert(sportovni_taska.contains("lahev rumu"));
    assert(!sportovni_taska.contains("lahev mineralky"));

    assert(kufr.contains("slanina"));
    assert(kufr.contains("tuzka"));

    assert(skrin.contains("kufr"));
    assert(skrin.contains("sportovni taska"));
    assert(skrin.contains("lahev rumu"));
    assert(!skrin.contains("door to narnia"));

    std::cout << "LEVEL 2 OK" << std::endl;

#endif

#if LEVEL >= 3
    std::cout << "--- LEVEL 3 ---" << std::endl;
    Bag batoh("batoh", 25, 5);
    {
        Item ponozky("ponozky", 5);
        Item trenky("trenky", 5);
        Item tuzka("tuzka", 5);
        Bag pytlik("pytlik", 10, 2);
        Bag krabice("krabice", 15, 3);
        pytlik.insert(ponozky);
        pytlik.insert(trenky);
        krabice.insert(tuzka);
        krabice.insert(pytlik);
        batoh.insert(krabice);
    }
    assert(batoh.contains("trenky"));
    assert(batoh.contains("pytlik"));
    if (PRINT)
        std::cout << batoh;
    std::cout << "LEVEL 3 OK" << std::endl;
#endif

#if LEVEL >= 4
    std::cout << "--- LEVEL 4 ---" << std::endl;

    assert(slanina.weight() == 8);

    assert(sportovni_taska.weight() == 15);
    assert(skrin.weight() == 23);
    assert(batoh.weight() == 15);

    skrin.insert(batoh);
    assert(skrin.weight() == 38);
    if (PRINT)
        std::cout << skrin;

    skrin.insert(skrin);
    assert(skrin.weight() == 76);
    assert(skrin.cont        virtual std::shared_ptr<Thing> clone() const {
        return std::make_shared<Bag>(*this);
    }ains("skrin"));
    assert(skrin.contains("ponozky"));
    if (PRINT)
        std::cout << skrin;
    std::cout << "LEVEL 4 OK" << std::endl;
#endif

#if LEVEL >= 5
    std::cout << "--- LEVEL 5 ---" << std::endl;

    Item kovadlina("kovadlina", 50);
    Item mala_kovadlina("mala kovadlina", 15);
    assert(!kufr.insert(kovadlina));
    assert(!kufr.contains("kovadlina"));
    assert(kufr.insert(mala_kovadlina));
    assert(kufr.contains("mala kovadlina"));
    assert(kufr.weight() == 38);
    if (PRINT)
        std::cout << kufr;

    Item pirko("pirko", 1);
    assert(!kufr.insert(pirko));
    assert(kufr.weight() == 38);
    assert(!kufr.contains("pirko"));
    if (PRINT)
        std::cout << kufr;
    std::cout << "LEVEL 5 OK" << std::endl;
#endif

#if LEVEL >= 6
    std::cout << "--- LEVEL 6 ---" << std::endl;

    Bag igelitka("igelitka", 20, 5);
    Bag basa_piv("basa piv", 20, 20);
    Bag nakupni_kosik("Nakupni kosik", 45, 5);
    Item lahvace("lahvace", 20);
    Item branik("branik PET", 5);
    Item kockolit("kockolit", 15);
    assert(basa_piv.insert(lahvace));
    assert(igelitka.insert(branik));
    assert(igelitka.insert(kockolit));
    assert(nakupni_kosik.insert(basa_piv));
    assert(nakupni_kosik.insert(pirko));
    assert(nakupni_kosik.insert(igelitka));
    assert(!nakupni_kosik.insert(basa_piv));

    if (PRINT)
        std::cout << nakupni_kosik;
    std::cout << "LEVEL 6 OK" << std::endl;
#endif
    return 0;
}
