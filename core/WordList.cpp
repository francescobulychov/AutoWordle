#include "WordList.h"
#include <fstream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <algorithm>

bool WordList::isValidString(const string& string) {

    for (auto c : string) {
        if (static_cast<unsigned char>(c) < 97 || static_cast<unsigned char>(c) > 122) {
            return false;
        }
    }
    return true;
}

list<string> WordList::getWordList() const {

    return words;
}

void WordList::generateWordList(const string& fileName) {

    words.clear();

    ifstream file(fileName);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.size() == 5 && isValidString(line)) {
                transform(line.begin(), line.end(), line.begin(), ::toupper);
                words.push_back(line);
            }
        }
        file.close();

        sortWordList();
    }

    else
        exit(0);
}

void WordList::sortWordList() {

    unordered_map<char, float> frequencyChars;

    float totalChars = 0;
    for (string word : words) {
        for (char c : word) {
            frequencyChars[c]++;
            totalChars++;
        }
    }

    for (auto it = frequencyChars.begin(); it != frequencyChars.end(); it++)
        it->second = static_cast<float>(it->second) / totalChars;

    map<float, vector<string>> sortedWordsMap;

    for (string word : words) {
        unordered_set<char> uniqueChars(word.begin(), word.end());

        float totalWeight = 0;
        for (char c : uniqueChars)
            totalWeight += frequencyChars[c];

        sortedWordsMap[totalWeight].push_back(word);
    }

    list<string> sortedWords;

    for (auto it = sortedWordsMap.begin(); it != sortedWordsMap.end(); it++) {
        for (auto word : it->second)
            sortedWords.push_front(word);
    }

    words = sortedWords;
}

string WordList::getBestWord() const {

    return words.front();
}

int WordList::getNumberRemainingWords() const {

    return words.size();
}

void WordList::manageGreenChars(const string& greenChars) {

    for (int i = 0; i < greenChars.size(); i++) {

        if (greenChars[i] != '-') {

            for (auto it = words.begin(); it != words.end();) {
                if (greenChars[i] != it->at(i))
                    it = words.erase(it);
                else
                    ++it;
            }
        }
    }
}

void WordList::manageOrangeChars(const string& orangeChars, const string& greenChars) {

    for (int i = 0; i < orangeChars.size(); i++) {

        if (orangeChars[i] != '-') {

            for (auto it = words.begin(); it != words.end();) {
                if (orangeChars[i] == it->at(i))
                    it = words.erase(it);
                else
                    ++it;
            }

            for (auto it = words.begin(); it != words.end();) {
                bool flag = false;
                for (int j = 0; j < greenChars.size(); j++) {
                    if (greenChars[j] == '-') {
                        if (orangeChars[i] == it->at(j)) {
                            ++it;
                            flag = true;
                            break;
                        }
                    }
                    else
                        continue;
                }
                if (flag == false)
                    it = words.erase(it);
            }
        }
    }
}

void WordList::manageGreyChars(const string& greyChars, const string& greenChars) {

    for (char c : greyChars) {

        if (greenChars.find(c) != string::npos) {
            for (int i = 0; i < greenChars.size(); i++) {
                if (greenChars[i] == '-') {
                    for (auto it = words.begin(); it != words.end();) {
                        if (c == it->at(i))
                            it = words.erase(it);
                        else
                            ++it;
                    }
                }
            }
        }
        else {
            for (auto it = words.begin(); it != words.end();) {
                if (it->find(c) != string::npos)
                    it = words.erase(it);
                else
                    ++it;
            }
        }
    }
}