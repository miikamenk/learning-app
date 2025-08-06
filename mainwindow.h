#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pinyin.h"
#include "pinyinform.h"
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
    PinyinForm *pinyinForm;

    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void updatePinyinFormGeometry();

private slots:
    void openSettings();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
