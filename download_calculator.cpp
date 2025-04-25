#include <iostream>
#include <string>
#include <algorithm> // std::transform
#include <cctype> // std::tolower
#include <limits>

using namespace std;

struct Time {

    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    
    void show() const {
        cout << hours << "h, " << minutes << "min and " << seconds << "seconds.\n";
    }

};

void coutWithColor(const string &string, int r, int g, int b){
    cout << "\033[38;2;" << r << ";" << g << ";" << b << "m" << string << "\033[0m";
}

void calculator(Time &time, double file_size, double bitrate){ // On aurait pu faire Time time et mettre &time dans l'appel dans le main
    int total_time = static_cast<int> (file_size, bitrate); /*Pareil que (int) mais plus secure*/
    time.hours = total_time / 3600;
    time.minutes = (total_time % 3600) / 60;
    time.seconds = total_time % 60;
}

void calculateBitrate(double fSize, int seconds){

    int bitrate = fSize / seconds;
    string toStringBitrate = to_string(bitrate);
    coutWithColor("Estimated bitrate : ", 40, 210, 50);
    coutWithColor(toStringBitrate, 40, 210, 50);
    coutWithColor("Mo/s.\n", 40, 210, 50);
}

int convertTimeIntoSeconds(Time t){
    int seconds = ((t.hours * 3600) + (t.minutes * 60) + t.seconds);
    return seconds;
}

double convertGioToGo(double gio){return gio * 1073741824.0 / 1000000000.0;}

double convertMioToMo(double mio){return mio * 1048576.0 / 1000000.0;}

double convertKioToKo(double kio){return kio * 1024.0 / 1000.0;}

void convertFileSizeToMo(double &size, const string unit){ //TODO: Bugged
    if(unit == "go"){size * 1000.0;} // Conversion en Ko
    else if(unit == "mo"){size;} 
    else if(unit == "ko") {size / 1000.0;} 
    
    else {
        if(unit == "gio"){convertGioToGo(size) * 1000.0;} // Conversion en Go puis Mo
        else if(unit == "mio") {convertMioToMo(size);}
        else if(unit == "kio") {convertKioToKo(size) / 1000.0;}
        
        else {throw std::invalid_argument("Cannot resolve unit " + unit + ". Please use a binary or decimal information unit (Go,  Mio...).\n");}
        // Je peux marquer "string" + unit + "string" parce que unit est de type string, 
        // sinon il faut include sstream et faire un std::ostringstream msg dans lequel on met la chaine de caractères + le type
    }
}

void convertBitrateToMosec(double &bitrate, const string unit){ // TODO: Bugged
    if(unit == "go/s"){bitrate * 1000;}
    else if(unit == "mo/s"){bitrate;}
    else if(unit == "ko/s"){bitrate / 1000;}
    
    else {
        if(unit == "gio/s"){bitrate * 1024;}
        else if(unit == "mio/s"){bitrate;}
        else if(unit == "kio/s"){bitrate / 1024;}
        
        else {throw std::invalid_argument("Cannot resolve unit " + unit + ". Please use a binary of decimal bitrate (Go/s, Mio/s...)\n");}
        // Pareil qu'au dessus
    }
}

// Met tout en minuscule (transform : méthode de la library algortihm et tolower de la library cctype)
string makeNonCaseSensitive(string s){
    transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c) { return std::tolower(c); }); 
    return s;
}

void whileNotValueNatural(int value, string name){
    while(value < 0){
        coutWithColor("Invalid value entered for ", 255, 0, 0);
        coutWithColor(name, 40, 210, 50);
        coutWithColor(". Please enter a valid number.\n", 255, 0, 0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> value;
    }
}

int estimatedDownloadTime(){
    double file_size;
    double bitrate;
    Time time;
    string file_size_unit, bitrate_unit;

    cout << "Enter file size (Must be a number > 0) : \n";
    cin >> file_size;
    cout << "\nEnter unit for the file size (Gio/Go, Mio/Mo or Kio/Ko) : \n";
    cin >> file_size_unit;
    whileNotValueNatural(file_size, "file size");

    cout << "\nEnter your bitrate i.e. your internet speed (Must be a number > 0): \n";
    cin >> bitrate;
    cout << "\nEnter the bitrate unit (Example : Mo/s, Gio/s...) : \n";
    cin >> bitrate_unit;
    whileNotValueNatural(bitrate, "bitrate");

    try
    {
        convertFileSizeToMo(file_size, makeNonCaseSensitive(file_size_unit));
        convertBitrateToMosec(bitrate, makeNonCaseSensitive(bitrate_unit));
        cout << "File size unit : " << file_size_unit << " bitrate unit : " << bitrate_unit;
        calculator(time, file_size, bitrate);
    
        cout << "\n\n\tDownload will take around ";
        time.show();
        cout << " at " << bitrate << bitrate_unit;
    }
    catch(const std::invalid_argument& e) {
        cerr << "Error : " << e.what() << '\n';
    }

    string exit;
    cout << "\n\nPress any key to exit.\n>>> ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin >> exit;
    return 0;
}

int estimatedDownloadSpeed(){
    double file_size;
    string file_size_unit;
    Time time;
    int h, m, s, i=0;

    cout << "Enter file size (Must be a number > 0) : \n";
    cin >> file_size;
    cout << "\nEnter unit for the file size (Gio/Go, Mio/Mo or Kio/Ko) : \n";
    cin >> file_size_unit;
    whileNotValueNatural(file_size, "file size");

    cout << "Enter time spent downloading the specified file.\n";
    do {
        try{
            switch (i) {
                case 0:
                    cout << "Enter hours : (if you are unsure or hours are not specified, do not type anything)\n";
                    cin >> h;
                    whileNotValueNatural(h, "hours");
                    break;
                case 1:
                    cout << "Enter minutes : (if you are unsure or hours are not specified, do not type anything)\n";
                    cin >> m;
                    whileNotValueNatural(m, "minutes");
                    break;
                case 2:
                    cout << "Enter seconds : (if you are unsure or hours are not specified, do not type anything)\n";
                    cin >> s;
                    whileNotValueNatural(s, "seconds");
                    break;
                default:
                    coutWithColor("Index out of range. variable 'i' has exceeded excpected range of ", 255, 0, 0);
                    coutWithColor(std::to_string(i), 255, 0, 0);
                    break;
                }
            i++;
        }
        catch(const std::invalid_argument &e){
            cerr << "Error : " << e.what() << '\n'; 
        }

    } while (i != 3);

    time.hours = h;
    time.minutes = m;
    time.seconds = s;
    convertFileSizeToMo(file_size, file_size_unit);
    calculateBitrate(file_size, convertTimeIntoSeconds(time));

    string exit;
    cout << "\n\nPress any key to exit...";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin >> exit;
    return 0;
}

int main(){
    int sel;
    string exit;
    do {
        coutWithColor("\n\n\t\t\t\t\t---- Menu ----\n", 40, 200, 210);
        coutWithColor("\t\t\t1. Calculate estimated download time\n", 40, 200, 210);
        coutWithColor("\t\t\t2. Calculate download speed from file size and time spent downloading\n", 40, 200, 210);
        coutWithColor("\t\t\t3. Quit menu\n", 40, 200, 210);
        cout << ">>> ";
        cin >> sel;

        switch (sel){
            case 1:
                estimatedDownloadTime();
                break;
            case 2:
                estimatedDownloadSpeed();
                break;
            default:
                cout << "Please type a valid number.\n";
                break;
        }

    } while (sel != 3);

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nPress any key to exit...";
    cin >> exit;
    return 0;
}