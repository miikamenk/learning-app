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

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QList<QPair<QString, QString>> m_textPairs;
};

#endif // PINYINFORM_H
