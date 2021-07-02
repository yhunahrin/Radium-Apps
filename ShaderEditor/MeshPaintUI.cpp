#include "MeshPaintUI.h"

#include <iostream>

#include <QColorDialog>

#include "ui_MeshPaintUI.h"

MeshPaintUI::MeshPaintUI(QColor m_color,int m_sizePixel,QWidget* parent) : QFrame(parent), ui(new Ui::MeshPaintUI),_color(m_color),_sizePixel(m_sizePixel){

    ui->setupUi(this);
    ui->spinSizePixel->setEnabled(false);
    ui->colorBlack->setEnabled(false);
    ui->colorWhite->setEnabled(false);
    ui->fillColor->setEnabled(false);
    ui->boundaryDraw->setEnabled(false);
    connect(ui->colorBlack, &QCheckBox::clicked,this, &MeshPaintUI::on_colorBlack);
    connect(ui->colorWhite, &QCheckBox::clicked,this, &MeshPaintUI::on_colorWhite);
    connect(ui->fillColor, &QCheckBox::clicked,this, &MeshPaintUI::on_fillColor);
    connect(ui->boundaryFill, &QCheckBox::clicked,this, &MeshPaintUI::on_boundaryFill);
    connect(ui->boundaryDraw, &QCheckBox::clicked,this, &MeshPaintUI::on_boundary);
}
MeshPaintUI::~MeshPaintUI() {
    delete ui;
}

const QColor &MeshPaintUI::getColor()const{
    return _color;
}

const int& MeshPaintUI::getSizePixel() const{
    return _sizePixel;
}
void MeshPaintUI::setValueSizePixel(int sizePixel){
    sizePixel = ui->spinSizePixel->value();
    _sizePixel = sizePixel;
}
const bool& MeshPaintUI::getPaint() const{
    return _paint;
}
const bool& MeshPaintUI::getFill() const{
    return _fill;
}
void MeshPaintUI::setSizePixel(int sizeMin, int sizeMax){
    ui->spinSizePixel->setMinimum(sizeMin);
    ui->spinSizePixel->setMaximum(sizeMax);
} 
/*void MeshPaintUI::on_changeColor_pb_clicked() {
    QColor color = QColorDialog::getColor();
    if (color.isValid())
    {
        ui->changeColor_pb->setPalette(QPalette(color));
        emit colorChanged(color);
        _color = color;
    }
    //_pickingManager->get
}*/
void MeshPaintUI::on_paintColor_rb_toggled(bool checked) {
    ui->spinSizePixel->setEnabled(checked);
    ui->colorBlack->setEnabled(checked);
    ui->colorWhite->setEnabled(checked);
    _paint = true;
    _boundaryFill=false;
    this->setSizePixel(1,256);
   //emit paintColor(checked);
}
void MeshPaintUI::on_fillColor(){

    if(ui->fillColor->checkState()==Qt::Checked){
        QMessageBox msgBox;
        msgBox.setText("Please select a spot in the area of boundary or boundary need close!!!");
        msgBox.exec();
        _fill =true;
        _boundary =false;
        ui->boundaryDraw->setChecked(false);
        if(_choseColor==false){
            QMessageBox msgBox4;
            msgBox4.setText("Please choose color");
            msgBox4.exec();
        }
    }
    else if(ui->fillColor->checkState()==Qt::Unchecked){
        _fill =false;
    }
}
const bool& MeshPaintUI::getBoundaryFill() const{
    return _boundaryFill;
}
const bool& MeshPaintUI::getBoundary() const{
    return _boundary;
}
void MeshPaintUI::on_colorBlack(){
    if(ui->colorBlack->checkState()==Qt::Checked){
            _color=Qt::black;
            _choseColor=true;
            ui->colorWhite->setChecked(false);
    }
    else if(ui->colorBlack->checkState()==Qt::Unchecked){
        _choseColor=false;
    }
}
void MeshPaintUI::on_colorWhite(){
    if(ui->colorWhite->checkState()==Qt::Checked){
            _color=Qt::white;
            _choseColor=true;
            ui->colorBlack->setChecked(false);
    }
    else if(ui->colorWhite->checkState()==Qt::Unchecked){
        _choseColor=false;
    }
}
void MeshPaintUI::on_boundaryFill(bool checked){
     ui->fillColor->setEnabled(checked);
     ui->boundaryDraw->setEnabled(checked);
     ui->spinSizePixel->setEnabled(checked);
     _boundaryFill = true;
     _paint = false;
     this->setSizePixel(2,256);
     ui->boundaryDraw->setChecked(true);
     _boundary =true;
    // emit paintColor(checked);
}
void MeshPaintUI::on_boundary(){
    if(ui->boundaryDraw->checkState()==Qt::Checked){
            _boundary=true;
            _fill =false;
            ui->fillColor->setChecked(false);

    }
    else if(ui->boundaryDraw->checkState()==Qt::Unchecked){
        _boundary=false;
    }
}
const bool& MeshPaintUI::getchoseColor() const{
    return _choseColor;
}

void MeshPaintUI::unCheckedFill(){
    ui->fillColor->setChecked(false);
    _fill =false;
    _boundary =true;
    ui->boundaryDraw->setChecked(true);
}
