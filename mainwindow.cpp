#include <QKeyEvent>
#include <QClipboard>
#include <QGuiApplication>
#include <QImage>
#include <QMimeData>
#include <QDebug>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->clipboard_text->setAlignment(Qt::AlignCenter);
    ui->clipboard_text->setWordWrap(true);
    ui->clipboard_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MainWindow::~MainWindow()
{
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

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_V) {
        const QClipboard *clipboard = QGuiApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();
        lastPastedImage = QPixmap();

        if (mimeData->hasImage()) {
            QImage image = qvariant_cast<QImage>(mimeData->imageData());
            qDebug() << "Image pasted. Size:" << image.size();
            // You can now display, save, or process the image
            QPixmap pixmap = QPixmap::fromImage(image);
            lastPastedImage = pixmap;
            ui->clipboard_text->setPixmap(pixmap.scaled(
                ui->clipboard_text->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            ));
        }
        else if (mimeData->hasText()) {
            QString text = mimeData->text();
            qDebug() << "Text pasted:" << text;
            // Handle text
            ui->clipboard_text->setText(text);
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
    QMainWindow::resizeEvent(event);
    if (!lastPastedImage.isNull()) {
        ui->clipboard_text->setPixmap(lastPastedImage.scaled(
            ui->clipboard_text->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        ));
    }
}
