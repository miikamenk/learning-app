#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pinyin.h"
#include "tokenizer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void toggleVisibility();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPixmap lastPastedImage;
    PinyinWrapper *pinyin;
    Tokenizer *tokenizer;

    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void openSettings();
};
#endif // MAINWINDOW_H
