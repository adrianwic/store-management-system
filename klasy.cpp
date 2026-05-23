
Blad::Blad(string m) : komunikat(m) {}
void Blad::bladKomunikat() { cout << "BLAD: " << komunikat << endl; }


Produkt::Produkt(string n, int i, double c) : nazwa(n), ilosc(i), cena(c) {}
Produkt::~Produkt() {}
void Produkt::wyswietl() { cout << "Nazwa: " << nazwa << " | Ilosc: " << ilosc << " | Cena: " << cena << " zl"; }
string Produkt::getNazwa() const { return nazwa; }
double Produkt::getCena() const { return cena; }
int Produkt::getIlosc() const { return ilosc; }
bool Produkt::getDoUsuniecia() const { return doUsuniecia; }
void Produkt::ustawIlosc(int arg) { ilosc=arg; }
void Produkt::ustawCene(double arg) { cena=arg; }
string Produkt::przygotujDoZapisu() { return getTyp() + "," + nazwa + "," + to_string(ilosc) + "," + to_string(cena); }
void Produkt::kolejnyDzien() {};

Produkt& operator+=(Produkt& p, int arg) { p.ilosc += arg; return p; }
Produkt& operator-=(Produkt& p, int arg) { p.ilosc -= arg; return p; }
void zmienCene(Produkt& p, double arg)
{
    p.cena*=arg;
    p.cena=round(p.cena * 100) / 100.0;
}


Zywnosc::Zywnosc(string n, int i, double c, int t) : Produkt(n, i, c), termin(t) {}
string Zywnosc::getTyp() { return "Zywnosc"; }
bool Zywnosc::tenSam(Produkt* p)
{
    Zywnosc* inny = dynamic_cast<Zywnosc*>(p);
    return inny && nazwa == p->getNazwa() && cena == p->getCena() && termin == inny->termin;
}
void Zywnosc::wyswietl() { Produkt::wyswietl(); cout << " | Dni do uplywu terminu waznosci: " << termin << endl; }
string Zywnosc::przygotujDoZapisu() { return Produkt::przygotujDoZapisu() + "," + to_string(termin); }
void Zywnosc::kolejnyDzien()
{
    if (termin > 0) termin--;
    if (termin > 0 && termin < 3) cena = round(100 * (0.7 * cena)) / 100.0;
}


Zabawka::Zabawka(string n, int i, double c, int w) : Produkt(n, i, c), wiek(static_cast<Wiek>(w)) {}
string Zabawka::getTyp() { return "Zabawka"; }
bool Zabawka::tenSam(Produkt* p)
{
    Zabawka* inny = dynamic_cast<Zabawka*>(p);
    return inny && nazwa == p->getNazwa() && cena == p->getCena() && wiek == inny->wiek;
}
string Zabawka::wiekString(Wiek w)
{
    switch (w)
    {
        case niemowle: return "niemowle";
        case dziecko: return "dziecko";
        case nastolatek: return "nastolatek";
        case dorosly: return "dorosly";
        default: return "nieznany";
    }
}
void Zabawka::wyswietl() { Produkt::wyswietl(); cout << " | Dla: " << wiekString(wiek) << endl;}
string Zabawka::przygotujDoZapisu() { return Produkt::przygotujDoZapisu() + "," + to_string(wiek); }
void Zabawka::kolejnyDzien() {}

Urzadzenie::Urzadzenie(string n, int i, double c, int g) : Produkt(n, i, c), gwarancja(g) {}
string Urzadzenie::getTyp() { return "Urzadzenie"; }
bool Urzadzenie::tenSam(Produkt* p)
{
    Urzadzenie* inny = dynamic_cast<Urzadzenie*>(p);
    return inny && nazwa == p->getNazwa() && cena == p->getCena() && gwarancja == inny->gwarancja;
}
void Urzadzenie::wyswietl() { Produkt::wyswietl(); cout << " | Gwarancja (miesiace): " << gwarancja << endl;}
string Urzadzenie::przygotujDoZapisu() { return Produkt::przygotujDoZapisu() + "," + to_string(gwarancja); }
void Urzadzenie::kolejnyDzien()
{
    ifstream plik("wycofane.txt");
    if (!plik) return;
    string linia;
    while (getline(plik, linia))
    {
        if (linia == nazwa)
        {
            cout << "Urzadzenie " << nazwa << " zostalo wycofane!\n";
            doUsuniecia = true;
            break;
        }
    }
    plik.close();
}


Sklep::~Sklep() { wyczyscWektor(); }

void Sklep::wyczyscWektor()
{
    for (auto p : wektorProduktow) delete p;
    wektorProduktow.clear();
}

bool Sklep::czyMagazynPusty() { return wektorProduktow.empty(); }

void Sklep::wczytajOstatniDzien()
{
    ifstream plik("przychody.txt");
    if (!plik) { dzien = 1; return; }
    string slowo;
    int ostatni = 0;
    while (plik >> slowo) plik >> ostatni;
    dzien = ostatni + 1;
    plik.close();
}

void Sklep::wczytajPrzychodyDzisiaj()
{
    ifstream plik("przychodyDzisiaj.txt");
    if (!plik)
    {
        przychod = 0;
        return;
    }
    plik >> przychod;
    plik.close();
}

void Sklep::wczytajMagazyn()
{
    wyczyscWektor();
    ifstream plik("magazyn.txt");
    if (!plik) return;
    string t, n, i, c, x;
    while (getline(plik, t, ','))
    {
        getline(plik, n, ',');
        getline(plik, i, ',');
        getline(plik, c, ',');
        getline(plik, x);

        if (t == "Zywnosc") dodajProdukt(new Zywnosc(n, stoi(i), stod(c), stoi(x)));
        else if (t == "Zabawka") dodajProdukt(new Zabawka(n, stoi(i), stod(c), stoi(x)));
        else if (t == "Urzadzenie") dodajProdukt(new Urzadzenie(n, stoi(i), stod(c), stoi(x)));
    }
    plik.close();
}

void Sklep::zapiszPrzychodyDzisiaj()
{
    ofstream plik("przychodyDzisiaj.txt");
    if (!plik)
    {
        cout << "Nie mozna otworzyc przychodow w przychodyDzisiaj.txt!\n";
        return;
    }
    plik << przychod;
    plik.close();
}

void Sklep::zapiszMagazyn()
{
    ofstream plik("magazyn.txt");
    if (!plik) { cout << "Nie mozna otworzyc pliku magazynu do zapisu"; return; }
    for (auto p : wektorProduktow) plik << p->przygotujDoZapisu() << endl;
    plik.close();
}

void Sklep::zapiszPrzychod()
{
    ofstream plik("przychody.txt", ios::app);
    if(!plik) { cout << "Nie mozna otworzyc pliku z przychodami\n"; return; }
    plik << "DZIEN "<< dzien-1 << " | PRZYCHOD " << przychod << "zl" << endl;
}

void Sklep::inicjalizujMagazyn()
{
    cout << "Inicjalizacja magazynu\n";
    dodajProdukt(new Zywnosc("Chleb Razowy", 10, 5.50, 3));
    dodajProdukt(new Zywnosc("Mleko 2%", 20, 3.20, 7));
    dodajProdukt(new Zywnosc("Ser Zolty", 15, 28.90, 14));
    dodajProdukt(new Zabawka("Klocki LEGO", 5, 149.99, 2));
    dodajProdukt(new Zabawka("Gra Planszowa", 8, 89.00, 3));
    dodajProdukt(new Zabawka("Grzechotka", 12, 15.50, 1));
    dodajProdukt(new Urzadzenie("Czajnik", 4, 120.00, 24));
    dodajProdukt(new Urzadzenie("Latarka LED", 10, 45.00, 12));
    dodajProdukt(new Urzadzenie("Powerbank", 7, 99.00, 24));
}

void Sklep::pokazSklep()
{
    cout << "\nDZIEN: " << dzien << " | PRZYCHOD: " << przychod << " zl\n";
    for (auto p : wektorProduktow)
    {
        p->wyswietl();
    }
}

void Sklep::dodajProdukt(Produkt* nowy)
{
    for (auto p : wektorProduktow)
    {
        if (p->getTyp() == nowy->getTyp() && p->tenSam(nowy))
        {
            *p += (nowy->getIlosc());
            delete nowy;
            cout << "Zaktualizowano ilosc istniejacego produktu\n";
            return;
        }
    }
    wektorProduktow.push_back(nowy);
}

void Sklep::interaktywneDodawanie()
{
    int wybor;
    while(true)
    {
        try
        {
            wybor = pobierzDane<int>("Wybierz typ (1. Zywnosc, 2. Zabawka, 3. Urzadzenie, 0. Anuluj): ");
            if (wybor == 0) return;
            if (wybor < 1 || wybor > 3) throw Blad("Nie ma takiego typu!");
            break;
        }
        catch (Blad b) { b.bladKomunikat(); }
    }
    string n = pobierzDane<string>("Nazwa: ");
    int i;
    while(true)
    {
        try
        {
            i = pobierzDane<int>("Ilosc: ");
            if (i <= 0) throw Blad("Ilosc musi byc wieksza od zera!");
            break;
        }
        catch (Blad b) { b.bladKomunikat(); }
    }
    double c;
    while(true)
    {
        try
        {
            c = pobierzDane<double>("Cena: ");
            if (c <= 0) throw Blad("Cena musi byc dodatnia!");
            break;
        }
        catch (Blad b) { b.bladKomunikat(); }
    }
        if (wybor == 1)
        {
            int t;
            while(true)
            {
                try
                {
                    t = pobierzDane<int>("Dni do uplywu daty waznosci: ");
                    if (t < 0) throw Blad("Termin nie moze byc ujemny!");
                    break;
                }
                catch (Blad b) { b.bladKomunikat(); }
            }
            dodajProdukt(new Zywnosc(n, i, c, t));
        }
        else if (wybor == 2)
        {
            int w;
            while(true)
            {
                try
                {
                    w = pobierzDane<int>("Wiek (1-niemowle 2-dziecko 3-nastolatek 4-dorosly): ");
                    if (w < 1 || w > 4) throw Blad("Wybierz wiek z zakresu 1-4!");
                    break;
                }
                catch (Blad b) { b.bladKomunikat(); }
            }
            dodajProdukt(new Zabawka(n, i, c, w));
        }
        else if (wybor == 3)
        {
            int g;
            while(true)
            {
                try
                {
                    g = pobierzDane<int>("Gwarancja (miesiace): ");
                    if (g < 0) throw Blad("Gwarancja nie moze byc ujemna");
                    break;
                }
                catch (Blad b) { b.bladKomunikat(); }
            }
            dodajProdukt(new Urzadzenie(n, i, c, g));
        }
        cout << "Produkt dodany pomyslnie do magazynu\n";
}

void Sklep::szukaj()
{
    string nazwaSzukana = pobierzDane<string>("Podaj nazwe szukanego produktu: ");
    bool znaleziono = false;
    for (auto p : wektorProduktow)
    {
        if (p->getNazwa() == nazwaSzukana)
        {
            p->wyswietl();
            znaleziono = true;
        }
    }
    if (!znaleziono) cout << "Nie znaleziono produktu: " << nazwaSzukana << endl;
}

Produkt* Sklep::wyborProdukt()
{
    string rodzajProduktu;
    int wyborRodzaj;

    while (true)
    {
        try
        {
            wyborRodzaj = pobierzDane<int>("\nWybierz typ (1. Zywnosc, 2. Zabawka, 3. Urzadzenie, 0. Koniec): ");
            if (wyborRodzaj == 0) return nullptr;
            if (wyborRodzaj == 1) { rodzajProduktu = "Zywnosc"; break; }
            if (wyborRodzaj == 2) { rodzajProduktu = "Zabawka"; break; }
            if (wyborRodzaj == 3) { rodzajProduktu = "Urzadzenie"; break; }
            throw Blad("Nieprawidlowy wybor typu! Wybierz liczbe od 0 do 3");
        }
        catch (Blad b) { b.bladKomunikat(); }
    }
    vector<Produkt*> podwektor;
    int licznik = 1;
    cout << "\nDostepne (" << rodzajProduktu << "):\n";
    for (auto p : wektorProduktow)
    {
        if (p->getTyp() == rodzajProduktu)
        {
            cout << licznik << ". "; p->wyswietl();
            podwektor.push_back(p);
            licznik++;
        }
    }
    if (podwektor.empty())
    {
        cout << "Brak produktow tego typu w magazynie.\n";
        return nullptr;
    }
    while (true)
    {
        try
        {
            int nr = pobierzDane<int>("Wybierz nr produktu (lub 0 aby cofnac): ");
            if (nr == 0) return nullptr;
            if (nr < 1 || nr > podwektor.size()) throw Blad("Podano numer spoza listy! Sprobuj ponownie");
            return podwektor[nr - 1];
        }
        catch (Blad b) { b.bladKomunikat(); }
    }
}

void Sklep::sprzedajProdukty()
{
    vector<ElementKoszyka> koszyk;
    while (true)
    {
        Produkt* wybrany = wyborProdukt();
        if (wybrany == nullptr) break;
        while (true)
        {
            try
            {
                int szt = pobierzDane<int>("Ile sztuk: ");
                if (szt <= 0) throw Blad("Ilosc musi byc wieksza od 0!");
                if (szt > wybrany->getIlosc()) throw Blad("Brak wystarczajacej ilosci w magazynie!");
                koszyk.push_back({wybrany, szt});
                break;
            }
            catch (Blad b) { b.bladKomunikat(); }
        }
        char dalej;
        while (true)
        {
            try
            {
                dalej = pobierzDane<char>("Czy dodac kolejny produkt do koszyka? (t/n): ");
                if (dalej == 't' || dalej == 'n') break;
                throw Blad("Nieprawidlowy wybor! Wpisz 't' (TAK) lub 'n' (NIE)");
            }
            catch (Blad b) { b.bladKomunikat(); }
        }
        if (dalej == 'n') break;
    }
    if (koszyk.empty()) return;
    double sumaParagonu = 0;
    cout << "\n==============================\n";
    cout << "           PARAGON            \n";
    cout << "==============================\n";

    for (auto& element : koszyk)
    {
        double koszt = element.wybranaIlosc * element.produkt->getCena();
        *element.produkt -= element.wybranaIlosc;
        przychod += koszt;
        sumaParagonu += koszt;
        cout << element.produkt->getNazwa() << endl;
        cout << "   " << element.wybranaIlosc << " szt. x " << element.produkt->getCena() << " zl = " << koszt << " zl\n";
    }
    cout << "------------------------------\n";
    cout << "SUMA:              " << sumaParagonu << " zl\n";
    cout << "==============================\n";
}

void Sklep::promocja()
{
    string rodzajProduktu;
    int wybor;
    while (true)
    {
        try
        {
            wybor = pobierzDane<int>("\nWybierz typ produktu do promocji (1. Zywnosc, 2. Zabawka, 3. Urzadzenie, 0. Zakoncz): ");

            if (wybor == 0) return;
            if (wybor == 1) { rodzajProduktu = "Zywnosc"; break; }
            if (wybor == 2) { rodzajProduktu = "Zabawka"; break; }
            if (wybor == 3) { rodzajProduktu = "Urzadzenie"; break; }
            throw Blad("Wybor spoza zakresu! Wybierz liczbe od 0 do 3.");
        }
        catch (Blad b) { b.bladKomunikat(); }
    }
    double procent;
    while (true)
    {
        try
        {
            procent = pobierzDane<double>("Podaj o jaki procent obnizyc cene <0-100), 0 aby wyjsc: ");
            if (procent==0) return;
            if (procent > 0 && procent < 100) break;
            throw Blad("Procent musi byc z zakresu 1-99!");
        }
        catch (Blad b) { b.bladKomunikat(); }
    }
    double mnoznik = 1 - 0.01 * procent;
    int licznik = 0;
    for (auto p : wektorProduktow)
    {
        if (p->getTyp() == rodzajProduktu)
        {
            zmienCene(*p, mnoznik);
            licznik++;
        }
    }
    if (licznik > 0) cout << "Przeceniono " << licznik << " produktow typu " << rodzajProduktu << " o " << procent << "%.\n";
    else cout << "Brak produktow typu " << rodzajProduktu << " w magazynie. Nic nie przeceniono.\n";
}

void Sklep::zmien()
{
    Produkt* wybrany = wyborProdukt();
    if (wybrany == nullptr) return;
    while (true)
    {
        cout << "\nWybrano: " << wybrany->getNazwa() << endl;
        int wybor = pobierzDane<int>("1. Zmiana ilosci, 2. Zmiana ceny, 0. Powrot\nWybor: ");
        if (wybor == 0) break;
        if (wybor == 1)
        {
            while (true)
            {
                try
                {
                    int nowaIlosc = pobierzDane<int>("Podaj nowa ilosc (lub -1 aby anulowac): ");
                    if (nowaIlosc == -1) break;
                    if (nowaIlosc < 0) throw Blad("Ilosc nie moze byc ujemna!");
                    wybrany->ustawIlosc(nowaIlosc);
                    cout << "Zaktualizowano ilosc.\n";
                    return;
                }
                catch (Blad b) { b.bladKomunikat(); }
            }
        }
        else if (wybor == 2)
        {
            while (true)
            {
                try
                {
                    double nowaCena = pobierzDane<double>("Podaj nowa cene (lub 0 aby anulowac): ");
                    if (nowaCena == 0) break;
                    if (nowaCena < 0) throw Blad("Cena nie moze byc ujemna!");

                    wybrany->ustawCene(round(nowaCena * 100) / 100.0);
                    cout << "Zaktualizowano cene.\n";
                    return;
                }
                catch (Blad b) { b.bladKomunikat(); }
            }
        }
        else throw Blad("Nieprawidlowa opcja! Wybierz 1, 2 lub 0.");
    }
}

void Sklep::usunPuste()
{
    wektorProduktow.erase(remove_if(wektorProduktow.begin(), wektorProduktow.end(),
            [](Produkt* p) { return p->getIlosc() == 0; }), wektorProduktow.end());
    cout << "Usunieto braki magazynowe\n";
}

void Sklep::sortujCena()
{
    sort(wektorProduktow.begin(), wektorProduktow.end(),
        [](Produkt* a, Produkt* b) { return a->getCena() < b->getCena(); });
    cout << "Posortowano rosnaco po cenie\n";
}

void Sklep::nastepnyDzien()
{
    dzien++;
    for (auto p : wektorProduktow) p->kolejnyDzien();
    auto it = remove_if(wektorProduktow.begin(), wektorProduktow.end(), [](Produkt* p) {
        if (p->getDoUsuniecia())
        {
            delete p;
            return true;
        }
        return false;
    });
    wektorProduktow.erase(it, wektorProduktow.end());
    zapiszPrzychod();
    przychod = 0;
    zapiszPrzychodyDzisiaj();
    zapiszMagazyn();
    cout << "\n--- DZIEN: " << dzien << " ROZPOCZETY ---\n";
}

void Sklep::usunProdukt()
{
    Produkt* usuwany = wyborProdukt();
    if (usuwany == nullptr) return;
    auto it = find(wektorProduktow.begin(), wektorProduktow.end(), usuwany);
    if (it != wektorProduktow.end())
    {
        cout << "Usunieto produkt: " << (*it)->getNazwa() << endl;
        delete *it;
        wektorProduktow.erase(it);
    }
}
