#include "pinyinform.h"
#include <QPainter>
#include <QFontMetrics>
#include <QDebug>

PinyinForm::PinyinForm(QWidget *parent)
    : QWidget(parent)
{
}
void PinyinForm::setText(const QList<QPair<QString, QString>>& textPairs) {
    m_textPairs = textPairs;
    qDebug() << "m_textPairs size:" << m_textPairs.size();
    update();  // Trigger repaint
}

void PinyinForm::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    QFont hanziFont("Sans", 16);
    QFont pinyinFont("Sans", 10);

    int x = 10;
    int y = 30;
    int spacing = 4;

    painter.setFont(pinyinFont);
    QFontMetrics pinyinMetrics(pinyinFont);
    QFontMetrics hanziMetrics(hanziFont);

    int lineHeight = hanziMetrics.height() + pinyinMetrics.height() + spacing;

    for (const auto& pair : m_textPairs) {
        QString hanzi = pair.first;
        QString pinyin = pair.second;

        int wordWidth = std::max(
            hanziMetrics.horizontalAdvance(hanzi),
            pinyinMetrics.horizontalAdvance(pinyin));

        if (x + wordWidth > width() - 10) {
            x = 10;
            y += lineHeight;
        }

        // Draw Pinyin (above or below based on style)
        painter.setFont(pinyinFont);
        painter.drawText(x, y, wordWidth, pinyinMetrics.height(), Qt::AlignHCenter, pinyin);

        // Draw Hanzi below Pinyin
        painter.setFont(hanziFont);
        painter.drawText(x, y + pinyinMetrics.height() + spacing,
                         wordWidth, hanziMetrics.height(), Qt::AlignHCenter, hanzi);

        x += wordWidth + 10;
    }
}
