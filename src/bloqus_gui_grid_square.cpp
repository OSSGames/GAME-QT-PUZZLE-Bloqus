
// -- utf8 --
/*
  Bloqus
  Copyright Yves Bailly 2009
  yves.bailly@laposte.net

Ce logiciel est un programme informatique permettant de jouer au jeu
Blokus, s'inspirant du produit de la société Sekkoïa : http://www.blokus.com/

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

À cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/
/*
  Bloqus
  Copyright Yves Bailly 2009
  yves.bailly@laposte.net

This software is a computer program whose purpose is to play the game Blokus,
inpired by the product from the society Sekkoïa: http://www.blokus.com/

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCIL license and that you accept its terms.
*/

#include <QtDebug>
#include <QPainter>
#include <QRadialGradient>

#include "bloqus.h"
#include "bloqus_game.h"
#include "bloqus_gui.h"
#include "bloqus_gui_grid_square.h"
#include "bloqus_gui_grid.h"
#include "bloqus_game_grid.h"
#include "bloqus_game_game.h"

namespace Bloqus
{
  namespace
  {
    QRadialGradient* square_grad = 0;
  }

  namespace Gui
  {
    Grid_Square::Grid_Square(Grid* parent):
        QGraphicsRectItem(0.0, 0.0, 1.0, 1.0, parent),
        grid_row(-1),
        grid_col(-1)
    {
      this->setPen(QPen(Qt::black,
                        0.0,
                        Qt::SolidLine,
                        Qt::RoundCap,
                        Qt::RoundJoin));
      if ( square_grad == 0 )
      {
        square_grad = new QRadialGradient(0.9, 0.9, 1.0, 0.9, 0.9);
        square_grad->setSpread(QGradient::ReflectSpread);
        square_grad->setColorAt(0.0, Qt::white);
        square_grad->setColorAt(1.0, QColor(200, 200, 200));
      }
      this->setBrush(QBrush(*square_grad));
    }

    // =====================================================================

    const Game::Grid& Grid_Square::gameGrid() const
    {
      Grid* gr = dynamic_cast<Grid*>(this->parentItem());
      return gr->gameGrid();
    }

    // =====================================================================

    void Grid_Square::setSquare(unsigned r,
                                unsigned c)
    {
      this->prepareGeometryChange();
      this->grid_row = r;
      this->grid_col = c;
      this->setRect(0.0,
                    0.0,
                    1.0,
                    1.0);
      this->setPos(static_cast<qreal>(c),
                   static_cast<qreal>(r));
    }

    // =====================================================================

    int Grid_Square::row() const
    {
      return this->grid_row;
    }

    // =====================================================================

    int Grid_Square::col() const
    {
      return this->grid_col;
    }

    // =====================================================================

    void Grid_Square::paint(QPainter* painter,
                            const QStyleOptionGraphicsItem* option,
                            QWidget* widget)
    {
      if ( (this->row() < 0) or
           (this->col() < 0) )
      {
        return;
      }
      QGraphicsRectItem::paint(painter, option, widget);
      if ( Options::showAllowedSquares() or
           Options::showForbiddenSquares() )
      {
        const Game::Grid& game_grid = this->gameGrid();
        if ( (game_grid.status(this->row(), this->col()) == 0) or
             (not game_grid.isEmpty(this->row(), this->col())) )
        {
          return;
        }
        Game::Game* game_game = game_grid.game();
        Game::Player_Id pid = game_game->currentPlayer();
        if ( Options::showAllowedSquares() )
        {
          if ( game_grid.allowedPlayer(pid,
                                       this->row(),
                                       this->col()) )
          {
            QRadialGradient grad(0.5, 0.5, 0.4, 0.5, 0.5);
            grad.setColorAt(0.0, Defaults::playerColor(pid));
            grad.setColorAt(1.0, Qt::white);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(grad));
            painter->drawEllipse(QRectF(0.1, 0.1, 0.8, 0.8));
          }
        }
        if ( Options::showForbiddenSquares() )
        {
          if ( game_grid.forbiddenPlayer(pid,
                                         this->row(),
                                         this->col()) )
          {
            QRadialGradient grad(0.5, 0.5, 0.5, 0.5, 0.5);
            grad.setColorAt(0.0, Defaults::playerColor(pid));
            grad.setColorAt(1.0, Qt::white);
            painter->setPen(QPen(QBrush(grad), 0.1));
            painter->drawLine(QPointF(0.1, 0.1), QPointF(0.9, 0.9));
            painter->drawLine(QPointF(0.1, 0.9), QPointF(0.9, 0.1));
          }
        }
      }
    } // Grid_Square::paint

  } // namespace Gui
} // namespace Bloqus
