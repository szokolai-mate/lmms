#pragma once

#include <QtWidgets>

class NoteField : public QWidget
{
  Q_OBJECT
  private:
    QPushButton * deleteButton;

  protected slots:
    void deleteButtonPressed()
    {
      emit deleteSelf(this);
    }

  public:
    int index;

    NoteField(std::string label, int index)
    {
      this->index = index;
      deleteButton = new QPushButton( "X", this);
      deleteButton->setCursor( QCursor( Qt::PointingHandCursor ) );
      connect( deleteButton, SIGNAL( clicked() ),
            this, SLOT( deleteButtonPressed() ) );

      QHBoxLayout * layout = new QHBoxLayout;
      this->setLayout(layout);
      layout->addWidget(new QLabel(QString(label.c_str())));
      layout->addWidget(deleteButton);
      deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      layout->setMargin(0);
      layout->setSpacing(0);
    }

  signals:
    void deleteSelf(NoteField * self);
};