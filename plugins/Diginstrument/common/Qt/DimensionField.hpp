#pragma once

#include <QtWidgets>

#include "../Dimension.h"

//TODO: add default value as parameter

/**
 * @brief A QWidget for the display and construction of a Dimension.
 */
class DimensionField : public QWidget
{
  Q_OBJECT
private:
  QLineEdit *label, *value, *minValue, *maxValue;
  QPushButton *deleteButton;
  bool editable = true;

protected slots:
  void deleteButtonPressed()
  {
    emit deleteSelf(this);
  }

public:
  DimensionField()
  {
    label = new QLineEdit();
    value = new QLineEdit();
    minValue = new QLineEdit();
    maxValue = new QLineEdit();
    deleteButton = new QPushButton("X", this);
    deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteButtonPressed()));

    QVBoxLayout *leftLayout = new QVBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    leftLayout->addWidget(new QLabel("Label"));
    leftLayout->addWidget(label);
    leftLayout->addWidget(new QLabel("Current value"));
    leftLayout->addWidget(value);
    rightLayout->addWidget(new QLabel("minimum value"));
    rightLayout->addWidget(minValue);
    rightLayout->addWidget(new QLabel("maximum value"));
    rightLayout->addWidget(maxValue);
    QWidget *leftContainer = new QWidget;
    QWidget *rightContainer = new QWidget;
    leftContainer->setLayout(leftLayout);
    rightContainer->setLayout(rightLayout);
    layout->addWidget(leftContainer);
    layout->addWidget(rightContainer);
    layout->addWidget(deleteButton);
    deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->setMargin(0);
    layout->setSpacing(0);
  }

  std::pair<std::string, float> getCoordinate() const
  {
    return std::make_pair(label->text().toStdString(), value->text().toDouble());
  }

  Diginstrument::Dimension getDimension() const
  {
    return Diginstrument::Dimension(label->text().toStdString(), minValue->text().toDouble(), maxValue->text().toDouble());
  }

  void lock()
  {
    deleteButton->setEnabled(false);
    label->setReadOnly(true);
    minValue->setReadOnly(true);
    maxValue->setReadOnly(true);
  }

  void unlock()
  {
    deleteButton->setEnabled(true);
    label->setReadOnly(false);
    minValue->setReadOnly(false);
    maxValue->setReadOnly(false);
  }

signals:
  void deleteSelf(DimensionField *self);
};