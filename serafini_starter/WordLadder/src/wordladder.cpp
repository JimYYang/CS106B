// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "lexicon.h"
#include "set.h"
#include "map.h"
#include "queue.h"
#include "stack.h"
#include "simpio.h"
#include <unordered_set>

using namespace std;
void print_greetings();
void open_file(ifstream& fin, Lexicon& dict);
bool prompt_words(string& start_word, string& end_word);
bool valid_format(const string& w1, const string& w2, const Lexicon& dict);
void find_word_ladder(const string& start_word, const string& end_word, const Lexicon& dict);
Stack<string> find_neighbors(const string& word, const Lexicon& dict);
void promptDictionary(fstream& inFile, string& fileName, Lexicon& dictionary);

int main() {
    // TODO: Finish the program!

    // greetings
    print_greetings();

    // open file
    fstream fin;
    Lexicon dict;
    // open_file(fin, dict);
    string filename;
    promptDictionary(fin, filename, dict);


    // prompt words
    string start_word, end_word;

    while (prompt_words(start_word, end_word))
        find_word_ladder(start_word, end_word, dict);

    fin.close();

    cout << "Have a nice day." << endl;
    return 0;
}

void print_greetings() {
    cout << "Welcome to CS 106B Word Ladder.\n"
         << "Please give me two English words, and I will change the\n"
         << "first into the second by changing one letter at a time.\n\n";
}


void open_file(ifstream& fin, Lexicon& dict) {
    promptUserForFile(fin, "please input the dictionary file name?");
    if (fin.is_open())
        Lexicon dict(fin);
}

void promptDictionary(fstream& inFile, string& fileName, Lexicon& dictionary) {
    while (true) {
        cout << "Dictionary file name? ";
        getline(cin, fileName);
        inFile.open(fileName);
        if (!inFile.fail()) break;
        cout << "Unable to open that file.  Try again." << endl;
    }
    dictionary.addWordsFromFile(inFile);
}


bool prompt_words(string& start_word, string& end_word) {
    cout << "Word #1 (or Enter to quit): ";
    // getLine(start_word);
    start_word = getLine();
    // cin.clear();
    // getline(cin, start_word);
    if (start_word.empty())
        return false;
    cout << "Word #2 (or Enter to quit): ";
    end_word = getLine();
    // cin.clear();
    // getline(cin, end_word);
    // getLine(end_word);
    if (end_word.empty())
        return false;
    start_word = toLowerCase(start_word);
    end_word = toLowerCase(end_word);
    return true;
}


bool valid_format(const string& w1, const string& w2, const Lexicon& dict) {
    if (!dict.contains(w1) || !dict.contains(w2)) {
        cout << "The two words must be found in the dictionary.\n\n";
        return false;
    }
    else if (w1.size() != w2.size()){
        cout << "The twowords must be the same length.\n\n";
        return false;
    }
    else if (w1 == w2) {
        cout << "The two words must be different.\n\n";
        return false;
    }
    return true;
}


void find_word_ladder(const string& start_word, const string& end_word, const Lexicon& dict) {
    if (!valid_format(start_word, end_word, dict))
        return;
    Queue<Stack<string>> que;
    unordered_set<string> used;
    Stack<string> st;
    st.add(start_word);
    que.enqueue(st);
    used.insert(start_word);

    while (!que.isEmpty()) {
        Stack<string> cur = que.front();
        que.dequeue();
        string cur_word = cur.peek();

        Stack<string> neighbors = find_neighbors(cur_word, dict);

        while (!neighbors.isEmpty()) {
            string neighbor = neighbors.peek();
            neighbors.pop();

            // don't use duplicate words
            // because if the word has been used before, the length of this word in is shortest.
            if (used.count(neighbor))
                continue;
            used.insert(neighbor);

            if (neighbor == end_word) {
                cout << "A ladder from " << end_word << " to " <<  start_word << ":\n";
                cout << end_word << ' ';
                while (!cur.isEmpty()) {
                    cout << cur.peek() << ' ';
                    cur.pop();
                }
                cout << endl << endl;
                return;
            }
            else {
                Stack<string> cp = cur;
                cp.add(neighbor);
                que.add(cp);
            }
        }
    }
    cout << "No word ladder found from "<< end_word << " to " << start_word << ".\n\n";
}

Stack<string> find_neighbors(const string& word, const Lexicon& dict) {
    int n = word.size();
    string tmp = word;
    Stack<string> neighbors;
    for (int i = 0; i < n; i++) {
        for (char c = 'a'; c <= 'z'; c ++) {
            tmp[i] = c;
            if (tmp == word)
                continue;
            if (dict.contains(tmp))
                neighbors.add(tmp);
        }
        tmp = word;
    }
    return neighbors;
}
