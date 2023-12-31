// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "vector.h"

using namespace std;

void print_greetings();
void open_file(ifstream& fin, string& file_name, Vector<string>& words);
void initialize_N(int& n);
bool initialize_num(int& n, const int& N);
void build_map(Map<Vector<string>, Vector<string>>& mp, Vector<string>& words, const int& n);
void generate_text(const Map<Vector<string>, Vector<string>>& mp, const int& N, const int& words_num);

int main() {
    // TODO: Finish the program!

    // welcome information
    print_greetings();

    // open_file;
    ifstream fin;
    string file_name;
    Vector<string> words;
    open_file(fin, file_name, words);

    // initialize N;
    int N;
    initialize_N(N);

    // num of words
    int words_num;

    // build maps
    Map<Vector<string>, Vector<string>> mp;
    build_map(mp, words, N);

    // generate text
    while (initialize_num(words_num, N))
        generate_text(mp, N, words_num);



    fin.close();
    cout << "Exiting." << endl;
    return 0;
}

void print_greetings() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams').\n"
         << "This program makes random text based on a document.\n"
         << "Give me an input file and an 'N' value for groups \n"
         << "of words, and I'll create random text for you.\n\n";
}

void open_file(ifstream& fin, string& file_name, Vector<string>& words) {
    while (true) {
        cout << "Input file name? ";
        getline(cin, file_name);
        fin.open(file_name);
        if (fin.is_open())
            break;
        cout << "Unable to open that file.  Try again.\n";
    }
    string word;
    while (fin >> word)
        words.push_back(word);
}

void initialize_N(int& n) {
    string tmp;
    long long num;
    while (true) {
        cout << "Value of N? ";
        getline(cin, tmp);
        if (tmp.size() && isalpha(tmp[0])) {
            cout << "Illegal integer format. Try again.\n";
            continue;
        }
        else if (num = stoi(tmp), num < 2) {
            cout << "N must be 2 or greater.\n";
            continue;
        }
        else
            break;
    }
    n = stoi(tmp);
}

bool initialize_num(int& n, const int& N) {
    string tmp;
    long long num;
    while (true) {
        cout << "\n# of random words to generate (0 to quit)? ";
        getline(cin, tmp);
        if (tmp.size() && isalpha(tmp[0])) {
            cout << "Illegal integer format. Try again. ";
            continue;
        }
        else if (num = stoi(tmp), num < N) {
            if (!num)
                return false;
            cout << "Must be at least " << N << " words.";
            continue;
        }
        else
            break;
    }
    n = stoi(tmp);
    if (!n)
        return false;
    return true;
}

void build_map(Map<Vector<string>, Vector<string>>& mp, Vector<string>& words, const int& n) {
    int len = words.size();
    Vector<string> cp = words;
    for (int i = 0; i < len; i ++)
        words.push_back(cp[i]);

    Vector<string> window;
    for (int i = 0; i < n - 1; i ++)
        window.push_back(words[i]);
    int left = 0, right = n - 1;
    while (left < len) {
        mp[window].push_back(words[right]);
        window.remove(0);
        left ++;
        window.push_back(words[right ++]);
    }
}

void generate_text(const Map<Vector<string>, Vector<string>>& mp, const int& N, const int& words_num) {
    Vector<string> win = randomKey(mp);
    Vector<string> text = win;
    int cnt = N - 1;
    while (cnt < words_num) {
        string suffix = randomElement(mp[win]);
        text.push_back(suffix);
        win.remove(0);
        win.push_back(suffix);
        cnt ++;
    }
    cout << "...";
    for (string word : text)
        cout << ' ' << word;
    cout << " ...\n";
}
