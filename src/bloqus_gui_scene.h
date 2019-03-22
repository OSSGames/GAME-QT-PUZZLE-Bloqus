
#ifndef __BLOQUS_GRID_H__
#define __BLOQUS_GRID_H__

#include <QGraphicsView>

class QGraphicsRectItem;

namespace Bloqus
{
  namespace Game
  {
    class Grid;
  }

  namespace Gui
  {
    class Grid: public QGraphicsView
    {
      Q_OBJECT
      public:
        Grid(QWidget* parent = 0);

        void initialize(unsigned sz);

      protected:
        virtual void resizeEvent(QResizeEvent* qre);

      private:
        QList<QGraphicsRectItem*> squares;
    }; // class Grid

  } // namespace Gui
} // namespace Bloqus

#endif // __BLOQUS_GRID_H__
