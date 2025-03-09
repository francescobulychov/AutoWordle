#pragma once
#include <QMainWindow>
#include "../core/WordList.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WordList& wordList, QWidget *parent = nullptr);

private:
    WordList& wordList;
    void createMenu();
    QWidget* wordListWidget();
    QWidget* wordlePanelWidget();
    QWidget* wordButtonsWidget();
    QVBoxLayout* vBox;
    QListWidget* listWidget;
    QLabel* possibleWordsLabel;
    QPushButton* confirmButton;
    QPushButton* resetButton;
    QString path;
    int currentRaw = 0;
    int possibleWords = 0;

private slots:
    void openFile();
    void startNewWord();
    void showWordList();
    void setWordButtons();
    void changeButtonColor(QPushButton* button);
    void resetButtons();
    void confirmChoices();
};
