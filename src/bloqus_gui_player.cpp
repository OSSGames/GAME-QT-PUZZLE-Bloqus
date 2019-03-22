
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

#include <QGraphicsPathItem>
#include <QGraphicsSimpleTextItem>
#include <QFont>
#include <QPen>
#include <QBrush>
#include <QTimer>

#include "bloqus.h"
#include "bloqus_gui_player.h"
#include "bloqus_game_player.h"
#include "bloqus_gui.h"

namespace Bloqus
{
  namespace Gui
  {
    Player::Player(QObject* obj_parent,
                   QGraphicsItem* item_parent):
        QObject(obj_parent),
        QGraphicsItem(item_parent),
        game_player(0),
        the_name(0),
        the_time(0),
        the_points(0),
        the_pieces(0),
        blink_timer(0),
        blink_delta(25),
        name_on_side(true)
    {
    }

    // =====================================================================

    QRectF Player::boundingRect() const
    {
      return this->childrenBoundingRect();
    }

    // =====================================================================

    void Player::paint(QPainter* /*painter*/,
                       const QStyleOptionGraphicsItem* /*option*/,
                       QWidget* /*widget*/)
    {
    }

    // =====================================================================

    void Player::setup(Game::Player* p)
    {
      this->game_player = p;
      if ( this->game_player == 0 )
      {
        return;
      }
      const Game::Player_Id pid = this->game_player->id();
      const QPointF dir = Defaults::playerDirection(pid);
      QFont fnt("FreeSerif", 12);
      // the name
      this->the_name = new QGraphicsSimpleTextItem(
          this->game_player->nickname(),
          this);
      this->the_name->setFont(fnt);
      this->the_name->setPen(QPen(Defaults::playerColor(pid), 0.2));
      this->the_name->setBrush(Defaults::playerBrush(pid));
      const QRectF name_br = this->the_name->boundingRect();
      this->the_name->scale( 4.0 / name_br.height(),
                            13.0 / name_br.width());
      this->the_name->rotate((dir.x() < 0.0) ? 90.0 : -90.0);
      this->the_name->setPos((dir.x() < 0.0) ? 3.75 : -3.75,
                             (dir.x() < 0.0) ?
                                ((dir.y() < 0.0) ? -13.0 : 0.0) :
                                ((dir.y() < 0.0) ? 0.0 : 13.0));
      // the pieces
      QPainterPath pieces_path;
      pieces_path.moveTo( 0.0,  0.0);
      pieces_path.lineTo(13.0,  0.0);
      pieces_path.lineTo(13.0,  4.0);
      pieces_path.lineTo( 5.0,  4.0);
      pieces_path.lineTo( 5.0,  5.0);
      pieces_path.lineTo( 4.0,  5.0);
      pieces_path.lineTo( 4.0, 13.0);
      pieces_path.lineTo( 0.0, 13.0);
      pieces_path.closeSubpath();
      pieces_path.moveTo(0.0, 1.0);
      pieces_path.lineTo(1.0, 1.0);
      pieces_path.lineTo(1.0, 6.0);
      pieces_path.lineTo(4.0, 6.0);
      pieces_path.moveTo(2.0, 0.0);
      pieces_path.lineTo(2.0, 3.0);
      pieces_path.lineTo(1.0, 3.0);
      pieces_path.moveTo(1.0, 2.0);
      pieces_path.lineTo(2.0, 2.0);
      pieces_path.moveTo(6.0, 0.0);
      pieces_path.lineTo(6.0, 1.0);
      pieces_path.lineTo(2.0, 1.0);
      pieces_path.moveTo(0.0, 4.0);
      pieces_path.lineTo(4.0, 4.0);
      pieces_path.lineTo(4.0, 3.0);
      pieces_path.lineTo(7.0, 3.0);
      pieces_path.lineTo(7.0, 2.0);
      pieces_path.lineTo(9.0, 2.0);
      pieces_path.moveTo(7.0, 0.0);
      pieces_path.lineTo(7.0, 1.0);
      pieces_path.lineTo(8.0, 1.0);
      pieces_path.lineTo(8.0, 2.0);
      pieces_path.moveTo(8.0, 4.0);
      pieces_path.lineTo(8.0, 3.0);
      pieces_path.lineTo(9.0, 3.0);
      pieces_path.lineTo(9.0, 1.0);
      pieces_path.lineTo(11.0, 1.0);
      pieces_path.lineTo(11.0, 2.0);
      pieces_path.lineTo(13.0, 2.0);
      pieces_path.moveTo(13.0, 3.0);
      pieces_path.lineTo(11.0, 3.0);
      pieces_path.lineTo(11.0, 4.0);
      pieces_path.moveTo(10.0, 0.0);
      pieces_path.lineTo(10.0, 4.0);
      pieces_path.moveTo(6.0, 4.0);
      pieces_path.lineTo(6.0, 2.0);
      pieces_path.lineTo(5.0, 2.0);
      pieces_path.lineTo(5.0, 1.0);
      pieces_path.moveTo(0.0, 7.0);
      pieces_path.lineTo(3.0, 7.0);
      pieces_path.lineTo(3.0, 2.0);
      pieces_path.lineTo(4.0, 2.0);
      pieces_path.lineTo(4.0, 1.0);
      pieces_path.moveTo(0.0, 11.0);
      pieces_path.lineTo(1.0, 11.0);
      pieces_path.lineTo(1.0, 8.0);
      pieces_path.lineTo(2.0, 8.0);
      pieces_path.lineTo(2.0, 7.0);
      pieces_path.moveTo(1.0, 11.0);
      pieces_path.lineTo(1.0, 12.0);
      pieces_path.lineTo(2.0, 12.0);
      pieces_path.lineTo(2.0, 11.0);
      pieces_path.lineTo(3.0, 11.0);
      pieces_path.moveTo(3.0, 13.0);
      pieces_path.lineTo(3.0, 8.0);
      pieces_path.lineTo(4.0, 8.0);
      pieces_path.moveTo(2.0, 8.0);
      pieces_path.lineTo(2.0, 10.0);
      pieces_path.lineTo(3.0, 10.0);
      this->the_pieces = new QGraphicsPathItem(0);
      this->the_pieces->setBrush(Qt::NoBrush);
      this->the_pieces->setPath(pieces_path);
      this->the_pieces->scale((dir.x() < 0.0) ? -1.0 : 1.0,
                              (dir.y() < 0.0) ? -1.0 : 1.0);
      QPen pieces_pen = this->the_pieces->pen();
      pieces_pen.setColor(Defaults::playerColor(pid));
      this->the_pieces->setPen(pieces_pen);
      this->the_pieces->setParentItem(this);
      // the points
      this->the_points = new QGraphicsSimpleTextItem(
          QString("%1").arg((int)0, (int)3, (int)10, QChar('0')),
          this);
      this->the_points->setFont(fnt);
      this->the_points->setPen(QPen(Defaults::playerColor(pid), 0.2));
      this->the_points->setBrush(Defaults::playerBrush(pid));
      const QRectF points_br = this->the_points->boundingRect();
      this->the_points->scale(1.75 / points_br.height(),
                              4.0 / points_br.width());
      this->the_points->rotate((dir.x() < 0.0) ? -90.0 : 90.0);
      this->the_points->setPos((dir.x() < 0.0) ? -14.5 : 14.5,
                               (dir.x() < 0.0 ) ?
                                  ((dir.y() < 0.0) ? 0.0 : 4.0) :
                                  ((dir.y() < 0.0) ? -4.0 : 0.0));
      // the time
      this->the_time = new QGraphicsSimpleTextItem("05:00", this);
      this->the_time->setFont(fnt);
      this->the_time->setPen(QPen(Defaults::playerColor(pid), 0.2));
      this->the_time->setBrush(Defaults::playerBrush(pid));
      const QRectF time_br = this->the_time->boundingRect();
      this->the_time->scale(4.0 / time_br.width(),
                            1.5 / time_br.height());
      this->the_time->setPos((dir.x() < 0.0) ? -4.0 : 0.0,
                             (dir.y() < 0.0) ? -14.25 : 13.0);
      this->playerTimeChanged();
      //
      connect(this->game_player,
              SIGNAL(pointsChanged()),
              this,
              SLOT(playerPointsChanged()));
      connect(this->game_player,
              SIGNAL(timeChanged()),
              this,
              SLOT(playerTimeChanged()));
      connect(this->game_player,
              SIGNAL(enterCurrentPlayer()),
              this,
              SLOT(enterCurrentPlayer()));
      connect(this->game_player,
              SIGNAL(leaveCurrentPlayer()),
              this,
              SLOT(leaveCurrentPlayer()));
    }

    // =====================================================================

    Game::Player* Player::player() const
    {
      return this->game_player;
    }

    // =====================================================================

    void Player::setNameOnSide(bool nos)
    {
      if ( nos == this->name_on_side )
      {
        return;
      }
      this->name_on_side = nos;
      const QPointF dir = Defaults::playerDirection(this->player()->id());
      QTransform trans;
      this->prepareGeometryChange();
      this->the_name->setTransform(trans, false);
      const QRectF name_br = this->the_name->boundingRect();
      if ( this->name_on_side )
      {
        trans.scale( 4.0 / name_br.height(),
                    13.0 / name_br.width());
        trans.rotate((dir.x() < 0.0) ? 90.0 : -90.0);
        this->the_name->setTransform(trans, false);
        this->the_name->setPos((dir.x() < 0.0) ? 3.75 : -3.75,
                               (dir.x() < 0.0) ?
                                  ((dir.y() < 0.0) ? -13.0 : 0.0) :
                                  ((dir.y() < 0.0) ? 0.0 : 13.0));
      }
      else
      {
        trans.scale(13.0 / name_br.width(),
                     4.0 / name_br.height());
        this->the_name->setTransform(trans, false);
        this->the_name->setPos((dir.x() < 0.0) ? -13.0 : 0.0,
                               (dir.y() < 0.0) ? -0.5 : -3.5);
      }
    }

    // =====================================================================

    void Player::playerPointsChanged()
    {
      if ( this->player() == 0 )
      {
        return;
      }
      this->the_points->setText(
          QString("%1")
            .arg(this->player()->points(),
                 (int)3,
                 (int)10,
                 QChar('0')));
    }

    // =====================================================================

    void Player::playerTimeChanged()
    {
      if ( this->player() == 0 )
      {
        return;
      }
      unsigned rem_time = this->player()->remainingTime();
      this->the_time->setText(
        QString("%1:%2")
          .arg((rem_time/60), (int)2, (int)10, QChar('0'))
          .arg((rem_time%60), (int)2, (int)10, QChar('0')));
      this->the_time->update(this->the_time->boundingRect());
    }

    // =====================================================================

    void Player::enterCurrentPlayer()
    {
      if ( Options::showCurrentPlayerByBlinking() )
      {
        if ( this->blink_timer == 0 )
        {
          this->blink_timer = new QTimer(this);
          this->blink_timer->setSingleShot(false);
          connect(this->blink_timer,
                  SIGNAL(timeout()),
                  this,
                  SLOT(blinkTimerStep()));
        }
        this->blink_delta = 50;
        this->blink_timer->start(100);
      }
    }

    // =====================================================================

    void Player::leaveCurrentPlayer()
    {
      if ( this->blink_timer != 0 )
      {
        this->blink_timer->stop();
      }
      const Game::Player_Id pid = this->player()->id();
      this->the_name->setPen(QPen(Defaults::playerColor(pid), 0.2));
      this->the_name->setBrush(Defaults::playerBrush(pid));
    }

    // =====================================================================

    void Player::blinkTimerStep()
    {
      QBrush b = this->the_name->brush();
      QColor bc = b.color();
      int a = bc.alpha();
      a += this->blink_delta;
      if ( a < 0 )
      {
        a = 0;
        this->blink_delta = -(this->blink_delta);
      }
      else if ( a > 255 )
      {
        a = 255;
        this->blink_delta = -(this->blink_delta);
      }
      bc.setAlpha(a);
      b.setColor(bc);
      this->the_name->setBrush(b);
    }

  } // namespace Gui
} // namespace Bloqus
