#include <QKeyEvent>
#include <QClipboard>
#include <QGuiApplication>
#include <QImage>
#include <QMimeData>
#include <QDebug>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "settingsform.h"
#include "pinyinform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tokenizer(new Tokenizer())
{
    ui->setupUi(this);

    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::openSettings);

    //ui->clipboard_text->setAlignment(Qt::AlignCenter);
    //ui->clipboard_text->setWordWrap(true);
    //ui->clipboard_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //ui->clipboard_text->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    pinyin = new PinyinWrapper();
    pinyinForm = new PinyinForm(this);
    pinyinForm->show();

    updatePinyinFormGeometry();
}

MainWindow::~MainWindow()
{
    delete pinyin;
    delete ui;
}

void MainWindow::toggleVisibility() {
    if (isVisible()) {
        hide();
    } else {
        showNormal();
        raise();
        activateWindow();
    }
}

void MainWindow::openSettings() {
    // Open your settings dialog here, or just test:
    SettingsForm *settings = new SettingsForm(this);
    settings->setAttribute(Qt::WA_DeleteOnClose); // Clean up on close
    settings->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_V) {
        const QClipboard *clipboard = QGuiApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();
        lastPastedImage = QPixmap();

        if (mimeData->hasImage()) {
            QImage image = qvariant_cast<QImage>(mimeData->imageData());
            qDebug() << "Image pasted. Size:" << image.size();
            // handle image, save it to cache to handle resizeEvent
            QPixmap pixmap = QPixmap::fromImage(image);
            lastPastedImage = pixmap;
            // ui->clipboard_text->setPixmap(pixmap.scaled(
            //     ui->clipboard_text->size(),
            //     Qt::KeepAspectRatio,
            //     Qt::SmoothTransformation
            // ));
        }
        else if (mimeData->hasText()) {
            QString text = mimeData->text();
            qDebug() << "Text pasted:" << text;
            std::vector<std::string> tokens = tokenizer->tokenize(text.toStdString());
            QList<QPair<QString, QString>> hanziPinyinList;
            for (const auto& token : tokens) {
              std::string pinyinResult = pinyin->convert(token);
              QString qToken = QString::fromStdString(token);
              QString qPinyin = QString::fromStdString(pinyinResult);

              hanziPinyinList.append(qMakePair(qToken, qPinyin));
            }
            pinyinForm->setText(hanziPinyinList);
            pinyinForm->setMinimumSize(400, 300);
        }
        else {
            qDebug() << "Clipboard contains unsupported data";
        }

        event->accept();
        return;
    }

    // Default handler for other keys
    QMainWindow::keyPressEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // Pass forward to default handler
    QMainWindow::resizeEvent(event);

    int windowWidth = this->width();
    int windowHeight = this->height();

    // Use the smaller dimension to determine font size
    int baseSize = std::min(windowHeight, windowWidth);
    updatePinyinFormGeometry();

    // Calculate font size, adjust divisor to taste
    int newFontSize = std::clamp(baseSize / 25, 10, 28);

    // Only update if changed
    // static int currentFontSize = -1;
    // if (newFontSize != currentFontSize) {
    //     currentFontSize = newFontSize;
    //     QFont font = ui->clipboard_text->font();
    //     font.setPointSize(newFontSize);
    //     ui->clipboard_text->setFont(font);
    // }

    // // Rescale image if one is present
    // if (!lastPastedImage.isNull()) {
    //     ui->clipboard_text->setPixmap(lastPastedImage.scaled(
    //         ui->clipboard_text->size(),
    //         Qt::KeepAspectRatio,
    //         Qt::SmoothTransformation
    //     ));
    // }
}

void MainWindow::updatePinyinFormGeometry()
{
    int windowWidth = this->width();
    int windowHeight = this->height();

    int marginX = 30;
    int marginY = 100;
    int newWidth = windowWidth - 2 * marginX;
    int newHeight = windowHeight - marginY;

    pinyinForm->setGeometry(marginX, marginY / 2, newWidth, newHeight);
    pinyinForm->update();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    this->hide();  // Just hide the window
    event->ignore();  // Prevent the app from quitting
}
