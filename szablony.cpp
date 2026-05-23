
template <typename T>
T pobierzDane(string komunikat)
{
    T wartosc;
    while (true)
    {
        cout << komunikat;
        if (cin >> wartosc)
        {
            char nastepny = cin.peek();
            if (nastepny != '\n' && nastepny != ' ' && nastepny != '\t' && nastepny != EOF)
            {
                cout << "Nieprawidlowy format! Nie uzywaj kropek ani liter w tym polu\n";
                cin.clear();
                cin.ignore(100, '\n');
                continue;
            }
            cin.ignore(100, '\n');
            return wartosc;
        }
        else
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Nieprawidlowy format! Sprobuj ponownie.\n";
        }
    }
}

template <>
string pobierzDane<string>(string komunikat)
{
    string wartosc;
    cout << komunikat;
    getline(cin >> ws, wartosc);
    return wartosc;
}





