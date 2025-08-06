#include "pinyinform.h"
#include <QPainter>
#include <QFontMetrics>
#include <QDebug>

PinyinForm::PinyinForm(QWidget *parent)
    : QWidget(parent)
{
}

void PinyinForm::setText(const QList<QPair<QString, QString>> &textPairs)
{
    m_textPairs = textPairs;

    // Compute and store font sizes
    auto fonts = computeFontSizes(size(), m_textPairs.size());
    hanziFont = fonts.hanziFont;
    pinyinFont = fonts.pinyinFont;

    // Compute minimum content size and apply it
    QSize minSize = computeMinimumContentSize(width());
    setMinimumSize(minSize);

    QWidget *win = window();
    if (win) {
        QSize currentSize = win->size();
        if (currentSize.width() < minSize.width() || currentSize.height() < minSize.height()) {
            win->resize(minSize);  // resize() not setSize()
        }
        win->setMinimumSize(minSize);
    }

    update();
}

PinyinForm::FontSizing PinyinForm::computeFontSizes(QSize availableSize, int wordCount) const
{
    int base = std::min(availableSize.width(), (availableSize.width() + availableSize.height()) / 2);

    int hanziSize = std::clamp(base / 30, 14, 48);
    int pinyinSize = std::clamp(static_cast<int>(hanziSize / 1.6), 10, 28);

    int lineHeight = QFontMetrics(QFont("Sans", hanziSize)).height()
                   + QFontMetrics(QFont("Sans", pinyinSize)).height() + 6;

    int estimatedLines = std::max(1, wordCount / 6); // rough estimate
    int totalHeight = estimatedLines * lineHeight;

    if (totalHeight > availableSize.height()) {
        float scaleFactor = static_cast<float>(availableSize.height()) / totalHeight;
        hanziSize = std::max(12, static_cast<int>(hanziSize * scaleFactor));
        pinyinSize = std::max(8, static_cast<int>(pinyinSize * scaleFactor));
    }

    return {
        QFont("Sans", hanziSize),
        QFont("Sans", pinyinSize)
    };
}

QSize PinyinForm::computeMinimumContentSize(int maxWidthHint) const
{
    const int maxAllowedWidth = 1500;
    const int maxAllowedHeight = 800;

    int baseWidth = std::min(maxWidthHint, maxAllowedWidth);

    // Use cached fonts
    QFontMetrics hanziMetrics(hanziFont);
    QFontMetrics pinyinMetrics(pinyinFont);

    int spacing = 6;
    int wordSpacing = 10;

    int currentLineWidth = 0;
    int maxLineWidth = 0;
    int lines = 1;

    for (const auto &pair : m_textPairs) {
        int wordWidth = std::max(
            hanziMetrics.horizontalAdvance(pair.first),
            pinyinMetrics.horizontalAdvance(pair.second)
        );

        if (currentLineWidth + wordWidth + wordSpacing > baseWidth - 40) {
            lines++;
            maxLineWidth = std::max(maxLineWidth, currentLineWidth);
            currentLineWidth = 0;
        }

        currentLineWidth += wordWidth + wordSpacing;
    }

    maxLineWidth = std::max(maxLineWidth, currentLineWidth);

    int totalHeight = lines * (hanziMetrics.height() + pinyinMetrics.height() + spacing);

    int finalWidth = std::clamp(maxLineWidth + 40, 150, maxAllowedWidth) - 60;
    int finalHeight = std::clamp(totalHeight + 40, 100, maxAllowedHeight) - 100;

    return QSize(finalWidth, finalHeight);
}

void PinyinForm::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), palette().window()); // clear background

    // Use cached fonts only — no recalculation here
    painter.setFont(pinyinFont);
    QFontMetrics pinyinMetrics(pinyinFont);
    QFontMetrics hanziMetrics(hanziFont);

    int spacing = std::max(hanziFont.pointSize() / 6, 4);
    int wordSpacing = spacing * 2;

    int lineHeight = pinyinMetrics.height() + spacing + hanziMetrics.height();

    QVector<QVector<QPair<QString, QString>>> lines;
    QVector<QPair<QString, QString>> currentLine;
    int lineWidth = 0;
    int windowWidth = width();
    int windowHeight = height();

    for (const auto& pair : m_textPairs) {
        int wordWidth = std::max(
            hanziMetrics.horizontalAdvance(pair.first),
            pinyinMetrics.horizontalAdvance(pair.second));

        if (lineWidth + wordWidth > windowWidth - 20 && !currentLine.isEmpty()) {
            lines.append(currentLine);
            currentLine.clear();
            lineWidth = 0;
        }

        currentLine.append(pair);
        lineWidth += wordWidth + wordSpacing;
    }
    if (!currentLine.isEmpty()) {
        lines.append(currentLine);
    }

    int totalHeight = lines.size() * lineHeight;
    int y = (windowHeight - totalHeight) / 2;
    if (y < 10) y = 10; // minimal top margin

    for (const auto& line : lines) {
        int totalLineWidth = 0;
        QVector<int> wordWidths;
        for (const auto& pair : line) {
            int wordWidth = std::max(
                hanziMetrics.horizontalAdvance(pair.first),
                pinyinMetrics.horizontalAdvance(pair.second));
            wordWidths.append(wordWidth);
            totalLineWidth += wordWidth + wordSpacing;
        }
        totalLineWidth -= wordSpacing;

        int x = (windowWidth - totalLineWidth) / 2;
        if (x < 10) x = 10; // minimal left margin

        for (int i = 0; i < line.size(); ++i) {
            const QString& hanzi = line[i].first;
            const QString& pinyin = line[i].second;
            int wordWidth = wordWidths[i];

            painter.setFont(pinyinFont);
            painter.drawText(x, y, wordWidth, pinyinMetrics.height(), Qt::AlignHCenter, pinyin);

            painter.setFont(hanziFont);
            painter.drawText(x, y + pinyinMetrics.height() + spacing,
                             wordWidth, hanziMetrics.height(), Qt::AlignHCenter, hanzi);

            x += wordWidth + wordSpacing;
        }

        y += lineHeight;
    }
}

void PinyinForm::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    auto fonts = computeFontSizes(size(), m_textPairs.size());
    hanziFont = fonts.hanziFont;
    pinyinFont = fonts.pinyinFont;
    QSize minSize = computeMinimumContentSize(width());
    setMinimumSize(minSize);

    QWidget *win = window();
    if (win) {
        QSize currentSize = win->size();
        if (currentSize.width() < minSize.width() || currentSize.height() < minSize.height()) {
            win->resize(minSize);  // resize() not setSize()
        }
        win->setMinimumSize(minSize);
    }
    update();
}
