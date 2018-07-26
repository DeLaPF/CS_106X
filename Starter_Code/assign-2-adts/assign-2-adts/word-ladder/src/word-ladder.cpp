/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "strlib.h"
#include "simpio.h"

#define ALPHABET_SIZE (26)
#define LOWER_CASE_A (97)

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

static Vector<string> generateLadder(const Lexicon& english, const string& start, const string& end) {
    Vector<string> wordLadder;
    wordLadder.add(start);

    Queue<Vector<string>> possiblePaths;
    possiblePaths.enqueue(wordLadder);

    Set<string> visited;
    visited.add(start);

    while (!possiblePaths.isEmpty()) {
        Vector<string> path = possiblePaths.dequeue();
        string lastWord = path[path.size() - 1];
        if (lastWord == end) {
            return path;
        }

        for (int i = 0; i < lastWord.length(); i++) {
            char c = lastWord[i];
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                lastWord[i] = LOWER_CASE_A + j; // How would I got about checking all other chars?

                if (visited.contains(lastWord)) {
                    continue;
                }
                if (!english.contains(lastWord)) {
                    continue;
                }

                Vector<string> newPath = path;
                newPath.add(lastWord);
                visited.add(lastWord);
                possiblePaths.enqueue(newPath);
            }
            lastWord[i] = c;
        }
    }

    Vector<string> empty;
    return empty;
}

static const string kEnglishLanguageDatafile = "dictionary.txt";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        Vector<string> ladder = generateLadder(english, start, end);
        cout << ladder << endl;
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
