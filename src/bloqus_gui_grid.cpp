
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

#include <QGraphicsSimpleTextItem>
#include <QFont>

#include "bloqus.h"
#include "bloqus_gui_grid.h"
#include "bloqus_game_grid.h"
#include "bloqus_game_game.h"
#include "bloqus_gui_grid_square.h"

namespace Bloqus
{
  namespace Game
  {
    class Grid;
  }

  namespace Gui
  {
    Grid::Grid(QGraphicsItem * parent):
        QGraphicsItem(parent),
        game_game(0)
    {
      std::fill(&(this->squares[0][0]),
                &(this->squares[0][0])+Game::MAX_GRID_SIZE*Game::MAX_GRID_SIZE,
                (Grid_Square*)0);
    }

    // =====================================================================

    QRectF Grid::boundingRect() const
    {
      if ( this->game_game != 0 )
      {
        return QRectF(-1.0,
                      -1.0,
                      static_cast<qreal>(this->gameGrid().size())+2.0,
                      static_cast<qreal>(this->gameGrid().size())+2.0);
      }
      else
      {
        return QRectF(0.0, 0.0, 0.0, 0.0);
      }
    }

    // =====================================================================

    void Grid::paint(QPainter* /*painter*/,
                     const QStyleOptionGraphicsItem* /*option*/,
                     QWidget* /*widget*/)
    {
    }

    // =====================================================================

    void Grid::setup(Game::Game* g)
    {
      this->game_game = g;
      const Game::Grid& gr = this->game_game->grid();
      for(unsigned row = 0; row < gr.size(); ++row)
      {
        for(unsigned col = 0; col < gr.size(); ++col)
        {
          if ( this->squares[row][col] == 0 )
          {
            this->squares[row][col] = new Grid_Square(this);
          }
          this->squares[row][col]->setSquare(row, col);
          this->squares[row][col]->setZValue(0.0);
          this->squares[row][col]->setVisible(true);
        }
        QGraphicsSimpleTextItem* gsti = 0;
        QRectF br;
        const QFont fnt("DejaVu Serif", 12);
        const double txt_height = 0.5;
        // top row
        gsti = new QGraphicsSimpleTextItem(
            QString::number(row+1), this);
        gsti->setFont(fnt);
        br = gsti->boundingRect();
        gsti->scale(((row < 9) ? 0.25 : 0.5)/br.width(),
                    txt_height/br.height());
        br = gsti->boundingRect();
        gsti->setPos(row+((row < 9) ? 0.375 : 0.25),
                     -txt_height);
        // bottom row
        gsti = new QGraphicsSimpleTextItem(
            QString::number(row+1), this);
        gsti->setFont(fnt);
        br = gsti->boundingRect();
        gsti->scale(((row < 9) ? 0.25 : 0.5)/br.width(),
                    txt_height/br.height());
        br = gsti->boundingRect();
        gsti->setPos(row+((row < 9) ? 0.375 : 0.25),
                     20.1);
        // left column
        gsti = new QGraphicsSimpleTextItem(
            QString::number(row+1), this);
        gsti->setFont(fnt);
        br = gsti->boundingRect();
        gsti->scale(((row < 9) ? 0.25 : 0.5)/br.width(),
                    txt_height/br.height());
        br = gsti->boundingRect();
        gsti->setPos(-0.275-((row < 9) ? 0.125 : 0.375),
                     row+(0.55-txt_height/2.0));
        // right column
        gsti = new QGraphicsSimpleTextItem(
            QString::number(row+1), this);
        gsti->setFont(fnt);
        br = gsti->boundingRect();
        gsti->scale(((row < 9) ? 0.25 : 0.5)/br.width(),
                    txt_height/br.height());
        br = gsti->boundingRect();
        gsti->setPos(19.875+((row < 9) ? 0.25 : 0.25),
                     row+(0.55-txt_height/2.0));
      }
      this->prepareGeometryChange();
    }

    // =====================================================================

    const Game::Grid& Grid::gameGrid() const
    {
      return this->game_game->grid();
    }

    // =====================================================================

    Grid_Square* Grid::square(unsigned row,
                              unsigned col)
    {
      return this->squares[row][col];
    }

  } // namespace Gui
} // namespace Bloqus
