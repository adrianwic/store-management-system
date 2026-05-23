
class Blad
{
private:
    string komunikat;
public:
    Blad(string m);
    void bladKomunikat();
};

class Produkt
{
protected:
    string nazwa;
    int ilosc;
    double cena;
    bool doUsuniecia = false;
public:
    Produkt(string n, int i, double c);
    virtual ~Produkt();
    virtual void wyswietl();
    string getNazwa() const;
    double getCena() const;
    bool getDoUsuniecia() const;
    int getIlosc() const;
    void ustawIlosc(int arg);
    void ustawCene (double arg);
    virtual string getTyp() = 0;
    virtual bool tenSam(Produkt* p) = 0;
    virtual string przygotujDoZapisu();
    virtual void kolejnyDzien() = 0;

    friend Produkt& operator+=(Produkt& p, int arg);
    friend Produkt& operator-=(Produkt& p, int arg);
    friend void zmienCene(Produkt& p, double arg);
};

class Zywnosc : public Produkt
{
    int termin;
public:
    Zywnosc(string n, int i, double c, int t);
    string getTyp();
    bool tenSam(Produkt* p);
    void wyswietl();
    string przygotujDoZapisu();
    void kolejnyDzien();
};

class Zabawka : public Produkt
{
private:
    enum Wiek {niemowle=1, dziecko=2, nastolatek=3, dorosly=4};
    Wiek wiek;
public:
    Zabawka(string n, int i, double c, int w);
    string getTyp();
    bool tenSam(Produkt* p);
    string wiekString(Wiek w);
    void wyswietl();
    string przygotujDoZapisu();
    void kolejnyDzien();
};

class Urzadzenie : public Produkt
{
    int gwarancja;
public:
    Urzadzenie(string n, int i, double c, int g);
    string getTyp();
    bool tenSam(Produkt* p);
    void wyswietl();
    string przygotujDoZapisu();
    void kolejnyDzien();
};

struct ElementKoszyka
{
    Produkt* produkt;
    int wybranaIlosc;
};

class Sklep
{
private:
    vector<Produkt*> wektorProduktow;
    int dzien = 1;
    double przychod = 0;
public:
    ~Sklep();
    void wyczyscWektor();
    void dodajProdukt(Produkt* nowy);
    void inicjalizujMagazyn();
    void interaktywneDodawanie();
    void sprzedajProdukty();
    void usunPuste();
    void sortujCena();
    void szukaj();
    void zapiszMagazyn();
    void wczytajMagazyn();
    void nastepnyDzien();
    void pokazSklep();
    bool czyMagazynPusty();
    void promocja();
    Produkt* wyborProdukt();
    void usunProdukt();
    void zmien();
    void zapiszPrzychod();
    void wczytajOstatniDzien();
    void zapiszPrzychodyDzisiaj();
    void wczytajPrzychodyDzisiaj();
};



