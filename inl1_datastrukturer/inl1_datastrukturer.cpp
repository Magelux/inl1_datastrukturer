#include <iostream> // för cout, cin
#include <fstream> // för ifstream DVS filhantering
#include <vector> // för vector
#include <string> // för string
#include <sstream> // för stringstream
#include <iomanip> // för setprecision


using namespace std;

// Konstruktor för lådorna som innehåller kolasnoren

struct Box
{
    // Namn på lådan
    string name;

    // Maximal längd på lådan
    double maxLength;

    // Maximal antal kolasnoren i lådan
    int maxCount;

    // Innehållet i lådan
    vector<double> contents;
};

// Funktion för att läsa innehållet i en CSV-fil
vector<double> readCSV(const string& filename)
{
    vector<double> lengths;

    ifstream file(filename);
    // Kontroll för att se om filen öppnas korrekt

    // Kontroll för att se om filen öppnas korrekt
    if (!file.is_open())
    {
        cerr << "FEL: Kunde inte öppna filen " << filename << "\n";
        return lengths;
        // Returnerar en tom vektor om filen inte kan öppnas
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        // En kontroll för att se om filen är tom
        float length;


        while (ss >> length)
        {
            lengths.push_back(length);
            if (ss.peek() == ',') ss.ignore(); // Ignorera kommatecken
        }

        // En kontroll för att se om det finns ogiltiga data i filen
        if (ss.fail() && !ss.eof())
        {
            cout << "FEL: Ogiltig data hittad i filen: " << filename << "|" << "\n";

            cout << ss.str() << "\n";
            cout << "|============================================================|\n";
            cout << " *omvandlar resterande data till double.*. *.* .*. .*. *.* .*.\n";
            cout << "|============================================================|\n";

            ss.clear(); // rensa felmeddelanden
            ss.str(""); // rensa strängen
            ss << line; // sätta strängen till den aktuella raden


            ss.clear(); // en funktion för att rensa felmeddelanden
        }
    }

    file.close();
    // Kontroll för att se om filen är tom

    return lengths;
}

// Heapify funktion för att skapa max heap
void heapify(vector<double>& arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Heap sort algoritm implementation
void heapSort(vector<double>& arr)
{
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--)
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Funktion för att gruppera längder i lådor
void groupIntoBoxes(const vector<double>& lengths, vector<Box>& boxes, vector<double>& discarded)
{
    vector<bool> used(lengths.size(), false);

    // for loop för att iterera genom lådorna

    for (auto& box : boxes)
    {
        // while loop för att fylla lådan med längder

        while (box.contents.size() < box.maxCount)
        {
            double closestFit = -1;

            // closestfit är en variabel som används för att hålla reda på den längd som passar bäst i lådan

            size_t closestIndex = -1;

            // closestindex är en variabel som används för att hålla reda på indexet
            // dvs positionen i vektorn för den längd som passar bäst i lådan

            // For-loop för att hitta den längd som passar bäst i lådan
            for (size_t i = 0; i < lengths.size(); ++i)
            {
                if (!used[i] && lengths[i] <= box.maxLength)
                {
                    if (closestFit == -1 || (box.maxLength - lengths[i]) < (box.maxLength - closestFit))
                    {
                        closestFit = lengths[i];
                        closestIndex = i;
                    }
                }
            }

            if (closestFit == -1) break; // break för att sluta om det inte finns fler passande längder

            box.contents.push_back(closestFit);
            used[closestIndex] = true;
        }
    }

    for (size_t i = 0; i < lengths.size(); ++i)
    {
        if (!used[i])
        {
            discarded.push_back(lengths[i]);
        }
    }
}

// Funktion till att skriva ut resultaten
void printResults(const vector<Box>& boxes, const vector<double>& discarded)
{
    cout << "==================== Slutgiltigt resultat ====================\n";
    cout << "                        Packade Boxar:                        \n";
    for (const auto& box : boxes)
    {
        if (!box.contents.empty())
        {
            cout << "  " << box.name << " (" << box.contents.size() << "/" << box.maxCount << "): ";
            for (size_t i = 0; i < box.contents.size(); ++i)
            {
                cout << fixed << setprecision(1) << box.contents[i];
                if (i != box.contents.size() - 1) cout << ", ";
            }
            cout << "\n";
        }
    }

    cout << "\n|====================Kasserade kolasnoren====================|""\n" << "                    (Totalt: " <<
        discarded.size() << " st ): ";
    cout << " " << "                                                         " << "\n";
    cout << "|============================================================|\n\n";
    if (!discarded.empty())
    {
        for (size_t i = 0; i < discarded.size(); ++i)
        {
            cout << "";
            cout << fixed << setprecision(1) << discarded[i];
            cout << "   ";
            if (i != discarded.size() - 1) cout << ", ";
        }
    }
    else
    {
        // implementerade det endast för att ha en sk. "failsafe"
        // OM det skulle vara att det inte finns några kasserade kolasnoren
        cout << "Inga kasserade kolasnoren";
    }
    cout << "\n<=================================================>\n";
}

int main()
{
    vector<Box> boxes = {
        {"Mini", 20.0, 35, {}},
        {"Medel", 30.0, 30, {}},
        {"Maxi", 45.0, 10, {}}
    };
    // Lådor med olika maxlängd och maxantal kolasnoren
    //vectorn lengths 
    vector<double> lengths = readCSV("kolasnoren.csv");


    heapSort(lengths);

    vector<double> discarded;
    groupIntoBoxes(lengths, boxes, discarded);

    printResults(boxes, discarded);
    cout << "\nTotalt antal kolasnoren ur maskinen: " << lengths.size() << "\n";
    cout << "Totalt antal kasserade kolasnoren: " << discarded.size() << "\n";
    cout << "Totalt antal packade kolasnoren: " << (lengths.size() - discarded.size()) << "\n\n";

    cout << "|=================================================|\n";
    cout << "|Kod skriven av: [Mathias D]|\n";
    cout << "|=================================================|\n";
    cout << "Tryck på ENTER tangenten för att avsluta programmet...";
    cin.get(); // Väntar på användarens input innan programmet avslutas
    
    return 0;
}
