#ifndef MESHPAINTUI_H
#define MESHPAINTUI_H

#include <QFrame>
#include <QColor>
#include <QPainter>
#include <QLine>
#include <QMouseEvent>
#include <QAction>
#include <QColor>
#include <QObject>
#include <QtPlugin>
#include <MeshPaintPluginMacros.h>
#include <PluginBase/RadiumPluginInterface.hpp>
#include <QMessageBox>

namespace UI
{
    class MeshPaintUI;
}

namespace MeshPaintPlugin
{
    class MeshPaintPluginC;
}

namespace Ui
{
    class MeshPaintUI;
}

class MeshPaintUI : public QFrame
{
    Q_OBJECT

        friend class MeshPaintPlugin::MeshPaintPluginC;

public:
    explicit MeshPaintUI(QColor m_color,int m_sizePixel, QWidget* parent = 0);
    const QColor& getColor() const;
    const int& getSizePixel() const;
    const bool& getPaint() const;
    const bool& getFill() const;
    void setSizePixel(int sizeMin,int sizeMax);
    void setValueSizePixel(int sizePixel);
     const bool& getchoseColor() const;
     const bool& getBoundaryFill() const;
     const bool& getBoundary() const;
    void unCheckedFill();
    ~MeshPaintUI();

//signals:
   // void paintColor(bool);
   // void colorChanged(const QColor&);

private slots:
    void on_paintColor_rb_toggled(bool checked);
    //void on_changeColor_pb_clicked();
    void on_fillColor();
    void on_colorBlack();
    void on_colorWhite();
    void on_boundaryFill(bool checked);
    void on_boundary();
private:
    Ui::MeshPaintUI* ui;
    QColor _color;
    int _sizePixel;
    bool _paint=0;
    bool _fill=0;
    bool _choseColor=0;
    bool _boundaryFill=0;
    bool _boundary=0;
};

#endif // MESHPAINTUI_H#pragma once
