
#include <QtDebug>
#include <QGraphicsRectItem>
#include <QRadialGradient>

#include "bloqus.h"
#include "bloqus_block.h"
#include "bloqus_grid.h"

namespace Bloqus
{
  const unsigned GRID_SQUARE_ROW_DATA_ID = 0;
  const unsigned GRID_SQUARE_COL_DATA_ID = 1;

  Grid::Grid(QWidget* parent):
    QGraphicsView(parent)
  {
    this->setScene(new QGraphicsScene(this));
    this->setRenderHints(QPainter::HighQualityAntialiasing |
                         QPainter::TextAntialiasing |
                         QPainter::SmoothPixmapTransform);
    this->init(20);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }

  void Grid::init(unsigned sz)
  {
    QGraphicsScene* sc = this->scene();
    // building the grid itself
    for(unsigned row = 0; row < sz; ++row)
    {
      for(unsigned col = 0; col < sz; ++col)
      {
        QGraphicsRectItem* it =
          sc->addRect(static_cast<double>(col),
                      static_cast<double>(row),
                      1.0, 1.0,
                      QPen(Qt::black, 0.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
                      QBrush(Qt::white));
        it->setData(GRID_SQUARE_ROW_DATA_ID, row);
        it->setData(GRID_SQUARE_COL_DATA_ID, col);
        this->squares.append(it);
      }
    }
    QRadialGradient grad;
    grad.setRadius(1.0);

    grad.setCenter(this->squares[0]->rect().center());
    grad.setFocalPoint(grad.center());
    grad.setColorAt(0.0, Datas::playerColor(Player_Blue).lighter());
    grad.setColorAt(0.5, Qt::white);
    this->squares[0]->setBrush(grad);

    grad.setCenter(this->squares[sz-1]->rect().center());
    grad.setFocalPoint(grad.center());
    grad.setColorAt(0.0, Datas::playerColor(Player_Yellow).lighter());
    grad.setColorAt(0.5, Qt::white);
    this->squares[sz-1]->setBrush(grad);

    grad.setCenter(this->squares[sz*sz-1]->rect().center());
    grad.setFocalPoint(grad.center());
    grad.setColorAt(0.0, Datas::playerColor(Player_Red).lighter());
    grad.setColorAt(0.5, Qt::white);
    this->squares[sz*sz-1]->setBrush(grad);

    grad.setCenter(this->squares[sz*(sz-1)]->rect().center());
    grad.setFocalPoint(grad.center());
    grad.setColorAt(0.0, Datas::playerColor(Player_Green).lighter());
    grad.setColorAt(0.5, Qt::white);
    this->squares[sz*(sz-1)]->setBrush(grad);

    this->fitInView(sc->sceneRect(), Qt::KeepAspectRatio);

    // creating initial blocks for players
    for(int ind_player = 0; ind_player < 4; ++ind_player)
    {
      Player_Color pc = static_cast<Player_Color>(static_cast<int>(Player_Blue)+ind_player);
      QPointF offset = Datas::playerInitialLocation(pc);
      QPointF dir = Datas::playerInitialDirection(pc);
      for(int ind_block = 0; ind_block < 21; ++ind_block)
      {
        Block_Shape bs = static_cast<Block_Shape>(static_cast<int>(Mono)+ind_block);
        Block* bl = new Block;
        bl->setBlockShape(bs);
        bl->setPen(QPen(Datas::playerColor(pc), 0.1));
        bl->setBrush(Datas::playerBrush(pc));
        bl->setZValue(1.0);
        QPointF init_loc = Datas::initialLocation(bs);
        if ( dir.x() < 0.0 )
        {
          init_loc.rx() = - init_loc.x();
          bl->scale(-1.0, 1.0);
        }
        if ( dir.y() < 0.0 )
        {
          init_loc.ry() = - init_loc.y();
          bl->scale(1.0, -1.0);
        }
        bl->setPos(offset+init_loc);
        this->scene()->addItem(bl);
      }
    }
  }

  void Grid::resizeEvent(QResizeEvent* qre)
  {
    this->fitInView(this->scene()->sceneRect(), Qt::KeepAspectRatio);
    this->QGraphicsView::resizeEvent(qre);
  }

} // namespace Bloqus
