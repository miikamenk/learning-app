#ifndef PINYINFORM_H
#define PINYINFORM_H

#include <QWidget>
#include <QList>
#include <QPair>
#include <QString>

class PinyinForm : public QWidget
{
    Q_OBJECT

public:
    explicit PinyinForm(QWidget *parent = nullptr);

    void setText(const QList<QPair<QString, QString>>& textPairs);
    QSize computeMinimumContentSize(int maxWidth = 800) const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent *event);

private:
    QList<QPair<QString, QString>> m_textPairs;
    QFont hanziFont;
    QFont pinyinFont;

    struct FontSizing {
        QFont hanziFont;
        QFont pinyinFont;
    };

    FontSizing computeFontSizes(QSize availableSize, int wordCount) const;
};

#endif // PINYINFORM_H
