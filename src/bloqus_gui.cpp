
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
#include <QSettings>
#include <QRadialGradient>

#include "bloqus.h"
#include "bloqus_gui.h"
#include "bloqus_game_piece.h"

namespace Bloqus
{

  namespace Gui
  {
    // =====================================================================

    // static
    QString Defaults::playerName(Game::Player_Id pid)
    {
      switch ( pid )
      {
        case Game::No_Player:
          return trUtf8("[not a player]",
                        "Default name for non-player");
          break;

        case Game::North_West_Player:
          return trUtf8("North-West player",
                        "Default name for \"north-west\" player");
          break;

        case Game::North_East_Player:
          return trUtf8("North-East player",
                        "Default name for \"north-east\" player");
          break;

        case Game::South_East_Player:
          return trUtf8("South-East player",
                        "Default name for \"south-east\" player");
          break;

        case Game::South_West_Player:
          return trUtf8("South-West player",
                        "Default name for \"south-west\" player");
          break;
        default:
          return trUtf8("Invalid Player_Id");
      }
    }

    // =====================================================================

    // static
    QString Defaults::playerPropertyPrefix(Game::Player_Id pid)
    {
      switch ( pid )
      {
        case Game::North_West_Player:
          return QString("north_west");
          break;
        case Game::North_East_Player:
          return QString("north_east");
          break;
        case Game::South_East_Player:
          return QString("south_east");
          break;
        case Game::South_West_Player:
          return QString("south_west");
          break;
        default:
          return QString("no_player");
      }
    }

    // =====================================================================

    // static
    QColor Defaults::playerGuiGradientStart(Game::Player_Id pid)
    {
      switch ( pid )
      {
        case Game::North_West_Player:
          return QColor(112, 112, 255, 192);
          break;
        case Game::North_East_Player:
          return QColor(224, 224, 96, 192);
          break;
        case Game::South_East_Player:
          return QColor(255, 128, 128, 192);
          break;
        case Game::South_West_Player:
          return QColor(128, 255, 128, 192);
          break;
        default:
          return Qt::white;
      }
    }

    // =====================================================================

    // static
    QColor Defaults::playerGuiGradientEnd(Game::Player_Id pid)
    {
      switch ( pid )
      {
        case Game::North_West_Player:
          return QColor(224, 224, 255, 64);
          break;
        case Game::North_East_Player:
          return QColor(255, 255, 224, 64);
          break;
        case Game::South_East_Player:
          return QColor(255, 224, 224, 64);
          break;
        case Game::South_West_Player:
          return QColor(224, 255, 224, 64);
          break;
        default:
          return Qt::white;
      }
    }

    // =====================================================================

    // static
    QColor Defaults::playerColor(Game::Player_Id pid)
    {
      switch ( pid )
      {
        case Game::North_West_Player:
          return QColor(64, 64, 255);
          break;
        case Game::North_East_Player:
          return QColor(192, 192, 0);
          break;
        case Game::South_East_Player:
          return QColor(255, 0, 0);
          break;
        case Game::South_West_Player:
          return QColor(0, 208, 0);
          break;
        default:
          return Qt::white;
      }
    }

    // =====================================================================

    // static
    QBrush Defaults::playerBrush(Game::Player_Id pid)
    {
      switch ( pid )
      {
        case Game::North_West_Player:
          return QBrush(QColor(64, 64, 255, 176));
          break;
        case Game::North_East_Player:
          return QBrush(QColor(208, 208, 0, 176));
          break;
        case Game::South_East_Player:
          return QBrush(QColor(255, 0, 0, 176));
          break;
        case Game::South_West_Player:
          return QBrush(QColor(0, 224, 0, 176));
          break;
        default:
          return QBrush(Qt::white);
      }
    }

    // =====================================================================

    // static
    QBrush Defaults::playerTextureBrush(Game::Player_Id pid)
    {
      static QBrush* nw_brush = 0;
      static QBrush* ne_brush = 0;
      static QBrush* sw_brush = 0;
      static QBrush* se_brush = 0;
      if ( nw_brush == 0 )
      {
        QRadialGradient nw_grad(1.0, 1.0, 1.0, 1.0, 1.0);
        nw_grad.setSpread(QGradient::ReflectSpread);
        nw_grad.setColorAt(0.0, QColor(64, 64, 255, 176));
        nw_grad.setColorAt(1.0, QColor(128, 128, 255, 208));
        nw_brush = new QBrush(nw_grad);

        QRadialGradient ne_grad(0.9, 0.9, 1.0, 0.9, 0.9);
        ne_grad.setSpread(QGradient::ReflectSpread);
        ne_grad.setColorAt(0.0, QColor(200, 200, 0, 176));
        ne_grad.setColorAt(1.0, QColor(232, 232, 64, 208));
        ne_brush = new QBrush(ne_grad);

        QRadialGradient sw_grad(0.9, 0.9, 1.0, 0.9, 0.9);
        sw_grad.setSpread(QGradient::ReflectSpread);
        sw_grad.setColorAt(0.0, QColor(0, 224, 0, 176));
        sw_grad.setColorAt(1.0, QColor(96, 255, 96, 208));
        sw_brush = new QBrush(sw_grad);

        QRadialGradient se_grad(0.9, 0.9, 1.0, 0.9, 0.9);
        se_grad.setSpread(QGradient::ReflectSpread);
        se_grad.setColorAt(0.0, QColor(255, 0, 0, 176));
        se_grad.setColorAt(1.0, QColor(255, 64, 64, 208));
        se_brush = new QBrush(se_grad);
      }
      switch ( pid )
      {
        case Game::North_West_Player:
          return *nw_brush;
          break;
        case Game::North_East_Player:
          return *ne_brush;
          break;
        case Game::South_East_Player:
          return *se_brush;
          break;
        case Game::South_West_Player:
          return *sw_brush;
          break;
        default:
          return QBrush(Qt::white);
      }
    }

    // =====================================================================

    // static
    QString Defaults::shapeName(Game::Piece_Shape ps)
    {
      switch ( ps )
      {
        case Game::Mono:
          return trUtf8("Monomino");
          break;
        case Game::Duo:
          return trUtf8("Domino");
          break;
        case Game::Trio_I:
          return trUtf8("Tromino I");
          break;
        case Game::Trio_L:
          return trUtf8("Tromino L");
          break;
        case Game::Tetra_I:
          return trUtf8("Tetromino I");
          break;
        case Game::Tetra_L:
          return trUtf8("Tetromino L");
          break;
        case Game::Tetra_O:
          return trUtf8("Tetromino O");
          break;
        case Game::Tetra_S:
          return tr("Tetromino S");
          break;
        case Game::Tetra_T:
          return trUtf8("Tetromino T");
          break;
        case Game::Penta_F:
          return trUtf8("Pentomino F");
          break;
        case Game::Penta_I:
          return trUtf8("Pentomino I");
          break;
        case Game::Penta_L:
          return trUtf8("Pentomino L");
          break;
        case Game::Penta_N:
          return trUtf8("Pentomino N");
          break;
        case Game::Penta_P:
          return trUtf8("Pentomino P");
          break;
        case Game::Penta_T:
          return trUtf8("Pentomino T");
          break;
        case Game::Penta_U:
          return trUtf8("Pentomino U");
          break;
        case Game::Penta_V:
          return trUtf8("Pentomino V");
          break;
        case Game::Penta_W:
          return trUtf8("Pentomino W");
          break;
        case Game::Penta_X:
          return trUtf8("Pentomino X");
          break;
        case Game::Penta_Y:
          return trUtf8("Pentomino Y");
          break;
        case Game::Penta_Z:
          return trUtf8("Pentomino Z");
          break;
        default:
          return trUtf8("Unknown shape");
          break;
      }
    }

    // =====================================================================

    // static
    QPointF Defaults::initialLocation(Game::Piece_Shape ps)
    {
      switch ( ps )
      {
        case Game::Mono:
          return QPointF(1.0, 2.0);
          break;
        case Game::Duo:
          return QPointF(11.0, 3.0);
          break;
        case Game::Trio_I:
          return QPointF(0.0, 1.0);
          break;
        case Game::Trio_L:
          return QPointF(0.0, 0.0);
          break;
        case Game::Tetra_I:
          return QPointF(2.0, 0.0);
          break;
        case Game::Tetra_L:
          return QPointF(8.0, 1.0);
          break;
        case Game::Tetra_O:
          return QPointF(1.0, 4.0);
          break;
        case Game::Tetra_S:
          return QPointF(6.0, 2.0);
          break;
        case Game::Tetra_T:
          return QPointF(7.0, 0.0);
          break;
        case Game::Penta_F:
          return QPointF(3.0, 1.0);
          break;
        case Game::Penta_I:
          return QPointF(3.0, 8.0);
          break;
        case Game::Penta_L:
          return QPointF(0.0, 7.0);
          break;
        case Game::Penta_N:
          return QPointF(2.0, 6.0);
          break;
        case Game::Penta_P:
          return QPointF(10.0, 0.0);
          break;
        case Game::Penta_T:
          return QPointF(10.0, 1.0);
          break;
        case Game::Penta_U:
          return QPointF(0.0, 11.0);
          break;
        case Game::Penta_V:
          return QPointF(0.0, 4.0);
          break;
        case Game::Penta_W:
          return QPointF(3.0, 3.0);
          break;
        case Game::Penta_X:
          return QPointF(5.0, 0.0);
          break;
        case Game::Penta_Y:
          return QPointF(1.0, 8.0);
          break;
        case Game::Penta_Z:
          return QPointF(1.0, 1.0);
          break;
        default:
          return QPointF(5.5, 5.5);
          break;
      }
    }

    // =====================================================================

    // static
    QPointF Defaults::initialLocation(Game::Player_Id pid)
    {
      switch ( pid )
      {
        case Game::North_West_Player:
          return QPointF(-5.25, -5.25);
          break;
        case Game::North_East_Player:
          return QPointF(25.25, -5.25);
          break;
        case Game::South_East_Player:
          return QPointF(25.25, 25.25);
          break;
        case Game::South_West_Player:
          return QPointF(-5.25, 25.25);
          break;
        default:
          return QPointF(0.0, 0.0);
      }
    }

    // =====================================================================

    // static
    QPointF Defaults::playerDirection(Game::Player_Id pid)
    {
      switch ( pid )
      {
        case Game::North_West_Player:
          return QPointF(1.0, 1.0);
          break;
        case Game::North_East_Player:
          return QPointF(-1.0, 1.0);
          break;
        case Game::South_East_Player:
          return QPointF(-1.0, -1.0);
          break;
        case Game::South_West_Player:
          return QPointF(1.0, -1.0);
          break;
        default:
          return QPointF(0.0, 0.0);
      }
    }

    // =====================================================================

    QPainterPath generatePath(Game::Piece* p)
    {
      QPainterPath pp;
      if ( p == 0 )
        return pp;
      // find starting square
      QPoint grid_pos(0, 0);
      while ( not p->squareFilled(grid_pos) )
      {
        grid_pos.rx() += 1;
        if ( grid_pos.x() >= p->nbCols() )
        {
          grid_pos.rx() = 0;
          grid_pos.ry() += 1;
        }
      }
      // first wall
      pp.moveTo(grid_pos.x(), grid_pos.y()+1);
      pp.lineTo(grid_pos.x(), grid_pos.y());
      // now follow walls by left-hand
      const QPoint start_grid_pos = grid_pos;
      Compass_Axis_Dir dir(Compass_Axis_Dir::North);
      const Compass_Axis_Dir start_dir = dir;
      do
      {
        QPoint new_grid_pos = Translate(grid_pos, dir);
        bool has_wall = (new_grid_pos.x() < 0) or
                        (new_grid_pos.y() < 0) or
                        (new_grid_pos.x() >= p->nbCols()) or
                        (new_grid_pos.y() >= p->nbRows());
        if ( not has_wall )
        {
          has_wall = not p->squareFilled(new_grid_pos);
        }
        if ( has_wall )
        {
          QPoint corner = Top_Right_Corner(grid_pos, dir);
          pp.lineTo(corner.x(), corner.y());
          dir = Next_Direction(dir, true);
        }
        else
        {
          grid_pos = new_grid_pos;
          dir = Previous_Direction(dir, true);
        }
      }
      while ( not ( (dir == start_dir) and
                    (grid_pos == start_grid_pos) ) );
      pp.closeSubpath();
      return pp;
    }

    // =====================================================================

    // static
    bool Defaults::showAllowedSquares()
    {
      return true;
    }

    // =====================================================================

    // static
    bool Defaults::showForbiddenSquares()
    {
      return false;
    }

    // =====================================================================

    // static
    bool Defaults::showCurrentPlayerByBlinking()
    {
      return true;
    }

    // =====================================================================

    // static
    bool Defaults::highlightHoveredPiece()
    {
      return true;
    }

    // =====================================================================

    // static
    QString Defaults::defaultStyleSheet()
    {
      return
          "QWidget { "
          "   background-image: url(:/images/grey_bg); "
          "} "
          ;
    }

    // =====================================================================

    // static
    QString Defaults::gameViewStyleSheet()
    {
      return
          "QWidget { "
          "   background-image: url(:/images/yellow_bg); "
          "}";
     }

    // =====================================================================

    // static
    QString Defaults::strengthName(int s)
    {
      switch ( s )
      {
        case 0:
          return trUtf8("Moron", "player strength 0");
          break;
        case 1:
          return trUtf8("Hurry", "player strength 1");
          break;
        case 2:
          return trUtf8("Greedy", "player strength 2");
          break;
        case 3:
          return trUtf8("Basic", "player strength 3");
          break;
        case 4:
          return trUtf8("Thinking", "player strength 4");
          break;
        case 5:
          return trUtf8("Annoying", "player strength 4");
          break;
        default:
          return "";
          break;
      }
    }

    // static
    void Options::init()
    {
      Options::show_allowed =
          Options::settings().value(
              "gui_setup/show_allowed_squares",
              Defaults::showAllowedSquares()).toBool();
      Options::show_forbidden =
          Options::settings().value(
              "gui_setup/show_forbidden_squares",
              Defaults::showForbiddenSquares()).toBool();
      Options::current_player_by_blinking =
          Options::settings().value(
              "gui_setup/current_player_by_blinking",
              Defaults::showCurrentPlayerByBlinking()).toBool();
    }

    // =====================================================================

    // static
    bool Options::showAllowedSquares()
    {
      return Options::show_allowed;
    }

    // =====================================================================

    // static
    bool Options::showForbiddenSquares()
    {
      return Options::show_forbidden;
    }

    // =====================================================================

    // static
    bool Options::showCurrentPlayerByBlinking()
    {
      return Options::current_player_by_blinking;
    }

    // =====================================================================

    // static
    bool Options::highlightHoveredPiece()
    {
      return Options::highlight_hovered;
    }

    // =====================================================================

    // static
    QSettings& Options::settings()
    {
      if ( Options::user_settings == 0 )
      {
        Options::user_settings =
            new QSettings(QSettings::IniFormat,
                          QSettings::UserScope,
                          "Bloqus",
                          "Bloqus");
      }
      return *(Options::user_settings);
    }

    // =====================================================================

    // static
    bool Options::show_allowed =
        Defaults::showAllowedSquares();

    // static
    bool Options::show_forbidden =
        Defaults::showForbiddenSquares();

    // static
    bool Options::current_player_by_blinking =
        Defaults::showCurrentPlayerByBlinking();

    // static
    bool Options::highlight_hovered =
        Defaults::highlightHoveredPiece();

    // static
    QSettings* Options::user_settings = 0;

  } // namespace Gui
} // namespace Bloqus
