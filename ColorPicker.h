#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QColor>
#include <QVector>

class ColorPicker
{
public:
    ColorPicker(size_t defaultSize)
    {
        colorList.reserve(defaultSize);
    }

    void addColor(const QColor& c)
    {
        colorList.push_back(c);
    }

    const QColor& randomColor(){
        return colorList[rand() % colorList.size()];
    }

    const QColor& operator[](int index)
    {
        return colorList.at(index);
    }

private:
    QVector<QColor> colorList;
};

#endif // COLORPICKER_H
