#include "MainWindow.h"
#include <QMenuBar>
#include <QHBoxLayout>
#include <QSplitter>
#include <QApplication>
#include <QFileDialog>

MainWindow::MainWindow(WordList& wordList, QWidget *parent) : QMainWindow(parent), wordList(wordList) {;

    adjustSize();

    createMenu();

    QSplitter* splitter = new QSplitter(this);
    setCentralWidget(splitter);

    splitter->addWidget(wordListWidget());
    splitter->addWidget(wordlePanelWidget());

    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
}

void MainWindow::createMenu() {

    QMenu* fileMenu;
    fileMenu = menuBar()->addMenu("File");

    fileMenu->addAction(new QAction("Open", fileMenu));
    fileMenu->actions().at(0)->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(fileMenu->actions().at(0), &QAction::triggered, this, &MainWindow::openFile);

    fileMenu->addSeparator();

    fileMenu->addAction(new QAction("Quit", fileMenu));
    fileMenu->actions().at(2)->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(fileMenu->actions().at(2), &QAction::triggered, this, &QApplication::quit);
}

QWidget* MainWindow::wordListWidget() {

    QWidget* widget = new QWidget(this);

    QVBoxLayout* vBox = new QVBoxLayout(widget);
    vBox->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    possibleWordsLabel = new QLabel("Possible words: ", this);
    possibleWordsLabel->setText("Possible words: " + QString::number(wordList.getNumberRemainingWords()));

    vBox->addWidget(possibleWordsLabel);

    vBox->addSpacing(5);

    listWidget = new QListWidget(this);

    vBox->addWidget(listWidget);

    return widget;
}

QWidget* MainWindow::wordlePanelWidget() {

    QWidget* widget = new QWidget(this);

    QVBoxLayout* vBoxPanel = new QVBoxLayout(widget);
    vBoxPanel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    vBoxPanel->addWidget(wordButtonsWidget());

    QHBoxLayout* hBox = new QHBoxLayout(widget);
    vBox->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    confirmButton = new QPushButton("Confirm");
    confirmButton->setEnabled(false);
    connect(confirmButton, &QPushButton::clicked, this, &MainWindow::confirmChoices);
    hBox->addWidget(confirmButton);

    resetButton = new QPushButton("Reset");
    resetButton->setEnabled(false);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::startNewWord);
    hBox->addWidget(resetButton);

    vBoxPanel->addLayout(hBox);

    return widget;
}

QWidget* MainWindow::wordButtonsWidget() {

    QWidget* widget = new QWidget(this);

    vBox = new QVBoxLayout(widget);
    vBox->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    for (int i = 0; i < 6; i++) {

        QHBoxLayout* hBox = new QHBoxLayout();
        hBox->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        vBox->addLayout(hBox);

        for (int j = 0; j < 5; j++) {
            QPushButton* button = new QPushButton("");
            button->setFixedSize(QSize(50, 50));
            button->setEnabled(false);
            button->setStyleSheet("QPushButton { font: bold 13px; color: white; background-color : grey }");

            hBox->addWidget(button);
        }
    }

    return widget;
}

void MainWindow::openFile() {

    path = QFileDialog::getOpenFileName(this, "Open file", "./", "Plain text *.txt");
    if (path.isEmpty()) return;

    startNewWord();
}

void MainWindow::startNewWord() {

    currentRaw = 0;
    resetButtons();

    wordList.generateWordList(path.toStdString());

    showWordList();
    setWordButtons();

    confirmButton->setEnabled(true);
    resetButton->setEnabled(true);
}

void MainWindow::resetButtons() {

    while (!vBox->isEmpty()) {

        QLayout* hBox = vBox->itemAt(0)->layout();

        while (!hBox->isEmpty()) {
            delete hBox->takeAt(0);
        }
        delete hBox;
    }

    for (int i = 0; i < 6; i++) {

        QHBoxLayout* hBox = new QHBoxLayout();
        hBox->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        vBox->addLayout(hBox);

        for (int j = 0; j < 5; j++) {
            QPushButton* button = new QPushButton("");
            button->setFixedSize(QSize(50, 50));
            button->setEnabled(false);
            button->setStyleSheet("QPushButton { font: bold 13px; color: white; background-color : grey }");

            hBox->addWidget(button);
        }
    }

}

void MainWindow::showWordList() {

    possibleWordsLabel->setText("Possible words: " + QString::number(wordList.getNumberRemainingWords()));

    listWidget->clear();

    list<string> words = wordList.getWordList();
    for (string word : words) {
        listWidget->addItem(QString::fromStdString(word));
    }
}

void MainWindow::setWordButtons() {

    QLayout* hBox = vBox->itemAt(currentRaw)->layout();

    for (int i = 0; i < 5; i++) {
        QPushButton* button = dynamic_cast<QPushButton*>(hBox->itemAt(i)->widget());
        button->setText(QString::fromStdString(wordList.getBestWord())[i]);

        if (wordList.getNumberRemainingWords() == 1) {
            button->setStyleSheet("QPushButton{ font: bold 13px; color: white; background-color : green }");
            confirmButton->setEnabled(false);
        }
        else {
            button->setEnabled(true);
            connect(button, &QPushButton::clicked, this, [this, button]() {
                emit MainWindow::changeButtonColor(button);
                });
        }
    }
}

void MainWindow::changeButtonColor(QPushButton* button) {

    if (button->styleSheet().contains("background-color : grey")) {
        button->setStyleSheet("QPushButton{ font: bold 13px; color: white; background-color : green }");
    }

    else if (button->styleSheet().contains("background-color : green")) {
        button->setStyleSheet("QPushButton{ font: bold 13px; color: white; background-color : orange }");
    }

    else if (button->styleSheet().contains("background-color : orange")) {
        button->setStyleSheet("QPushButton{ font: bold 13px; color: white; background-color : grey }");
    }
}

void MainWindow::confirmChoices() {

    QLayout* hBox = vBox->itemAt(currentRaw)->layout();
    string greenChars = "-----";
    string orangeChars = "-----";
    string greyChars = "";

    for (int i = 0; i < 5; i++) {

        QPushButton* button = dynamic_cast<QPushButton*>(hBox->itemAt(i)->widget());
        button->setEnabled(false);

        if (button->styleSheet().contains("background-color : green")) {
            greenChars[i] = button->text().toStdString()[0];
        }

        else if (button->styleSheet().contains("background-color : orange")) {
            orangeChars[i] = button->text().toStdString()[0];
        }

        else if (button->styleSheet().contains("background-color : grey")) {
            greyChars += button->text().toStdString()[0];
        }
    }

    wordList.manageGreenChars(greenChars);
    wordList.manageOrangeChars(orangeChars, greenChars);
    wordList.manageGreyChars(greyChars, greenChars);
    wordList.sortWordList();

    showWordList();

    if (greenChars.find('-') == string::npos || wordList.getNumberRemainingWords() == 0)
        confirmButton->setEnabled(false);

    else {
        currentRaw++;

        if (currentRaw == 6)
            confirmButton->setEnabled(false);

        setWordButtons();
    }
}




