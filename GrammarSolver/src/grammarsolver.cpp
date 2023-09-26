/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains grammar generating code for CS106B.
 */
#include "grammarsolver.h"

using namespace std;

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
void getGrammmer(istream& fin, Map<string, Vector<Vector<string>>>& grammers);
string generateOne(string symbol, Map<string, Vector<Vector<string>>>& grammers);
Vector<string> generateAll(string symbol, int times, Map<string, Vector<Vector<string>>>& grammers);

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    // TODO: write this function
    Map<string, Vector<Vector<string>>> grammers;
    getGrammmer(input, grammers);
    return generateAll(symbol, times, grammers);           // This is only here so it will compile
}

void getGrammmer(istream& fin, Map<string, Vector<Vector<string>>>& grammers) {
    string line;
    while (getline(fin, line)) {
        int pos = line.find(":");
        string key = line.substr(0, pos);
        Vector<string> rules = stringSplit(line.substr(pos + 3), "|");
        for (auto rule : rules) {
            Vector<string> r = stringSplit(rule, " ");
            grammers[key].push_back(r);
        }
    }
}

string generateOne(string symbol, Map<string, Vector<Vector<string>>>& grammers) {
    if (!grammers.containsKey(symbol))
        return symbol;
    Vector<Vector<string>> grammer = grammers[symbol];
    Vector<string> rule = randomElement(grammer);
    string ans;
    for (auto r : rule) {
        if (!grammers.containsKey(r))
            ans = ans + " " +  r;
        else
            ans += generateOne(r, grammers);
    }
    return ans;
}

Vector<string> generateAll(string symbol, int times, Map<string, Vector<Vector<string>>>& grammers) {
    Vector<string> res;
    for (int i = 0; i < times; i++) {
        res.push_back(generateOne(symbol, grammers));
    }
    return res;
}
