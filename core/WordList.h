#include<list>
#include<string>

using namespace std;

class WordList {

private:
	list<string> words;
	static bool isValidString(const string& string);

public:
	list<string> getWordList() const;
	void generateWordList(const string& fileName);
	void sortWordList();
	string getBestWord() const;
	int getNumberRemainingWords() const;
	void manageGreenChars( const string& greenChars);
	void manageOrangeChars(const string& orangeChars, const string& greenChars);
	void manageGreyChars(const string& greyChars, const string& greenChars);
};

