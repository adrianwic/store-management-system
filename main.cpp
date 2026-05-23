#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

#include "szablony.cpp"
#include "klasy.h"
#include "klasy.cpp"

int main()
{
    Sklep mojSklep;

    mojSklep.wczytajOstatniDzien();
    mojSklep.wczytajPrzychodyDzisiaj();
    mojSklep.wczytajMagazyn();
    if (mojSklep.czyMagazynPusty()) mojSklep.inicjalizujMagazyn();

    int menu = 1;
    while (menu != 0)
    {
        cout << "\n1. LISTA | 2. DODAJ | 3. SZUKAJ | 4. ZMIANA(CENY/ILOSCI) | 5. SPRZEDAJ | 6. PROMOCJA\n";
        cout << "7. USUN BRAKI | 8. SORTUJ | 9. NASTEPNY DZIEN | 10. USUN PRODUKT | 0. KONIEC\n";

        try
        {
            menu = pobierzDane<int>("Wybor: ");

            switch (menu)
            {
                case 1 : mojSklep.pokazSklep(); break;
                case 2 : mojSklep.interaktywneDodawanie(); break;
                case 3 : mojSklep.szukaj(); break;
                case 4 : mojSklep.zmien(); break;
                case 5 : mojSklep.sprzedajProdukty(); break;
                case 6 : mojSklep.promocja(); break;
                case 7 : mojSklep.usunPuste(); break;
                case 8 : mojSklep.sortujCena(); break;
                case 9 : mojSklep.nastepnyDzien(); break;
                case 10: mojSklep.usunProdukt(); break;
                case 0 : cout << "Zamykanie"; break;
                default: cout << "Nie ma takiej opcji\n"; break;
            }
        }
        catch (Blad b) { b.bladKomunikat(); }
        catch (...) { cout << "Nieoczekiwany blad\n"; }
    }
    mojSklep.zapiszMagazyn();
    mojSklep.zapiszPrzychodyDzisiaj();
    return 0;
}
