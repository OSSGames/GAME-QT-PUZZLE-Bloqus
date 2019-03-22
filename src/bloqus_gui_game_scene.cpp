
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

#include <algorithm>

#include <QtDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QCoreApplication>
#include <QMessageBox>
#include <QGraphicsView>

#include "bloqus.h"
#include "bloqus_gui_game_scene.h"
#include "bloqus_gui.h"
#include "bloqus_game_game.h"
#include "bloqus_game_grid.h"
#include "bloqus_gui_grid.h"
#include "bloqus_game_player.h"
#include "bloqus_game_piece.h"
#include "bloqus_gui_piece.h"
#include "bloqus_gui_grid_square.h"
#include "bloqus_gui_player.h"
#include "bloqus_game_human_player.h"
#include "game_results.h"

namespace Bloqus
{
  namespace Gui
  {
    Game_Scene::Game_Scene(QObject* parent):
        QGraphicsScene(parent),
        gui_grid(0),
        game_game(0),
        selected_piece(0),
        current_player(Game::No_Player)
    {
      this->gui_players[0] =
          this->gui_players[1] =
          this->gui_players[2] =
          this->gui_players[3] = 0;
    }

    // =====================================================================

    void Game_Scene::setup(Game::Game* g)
    {
      this->game_game = g;
      this->clear();
      this->gui_grid = 0;
      if ( this->game() != 0 )
      {
        // creating the grid
        this->gui_grid = new Grid(0);
        this->gui_grid->setup(this->game());
        this->addItem(this->gui_grid);
        this->gui_grid->setZValue(0.0);
        for(unsigned ind_player = 0;
            ind_player < this->game()->nbPlayers();
            ++ind_player)
        {
          Game::Player_Id pid = Game::indexToPlayerId(ind_player);
          Game::Player* player = this->game()->player(pid);
          QPointF player_dir = Defaults::playerDirection(pid);
          QPointF player_loc = Defaults::initialLocation(pid);
          // creating the players pieces
          for(int ind_piece = 0; ind_piece < 21; ++ind_piece)
          {
            Game::Piece_Shape ps = Game::indexToPieceShape(ind_piece);
            Game::Piece* piece = player->piece(ps);
            QPointF piece_loc = Defaults::initialLocation(ps);
            if ( player_dir.x() < 0 )
            {
              piece->flip(Qt::Horizontal);
              piece_loc.rx() *= player_dir.x();
              piece_loc.rx() -= piece->nbCols();

            }
            if ( player_dir.y() < 0 )
            {
              piece->flip(Qt::Vertical);
              piece_loc.ry() *= player_dir.y();
              piece_loc.ry() -= piece->nbRows();
            }
            Piece* gui_piece = new Piece(0);
            this->gui_pieces[ind_player][ind_piece] = gui_piece;
            gui_piece->setup(piece);
            gui_piece->setPos(player_loc + piece_loc);
            gui_piece->setZValue(1.0);
            gui_piece->storeDefault();
            this->addItem(gui_piece);
          }
          // creating the GUI players
          Player* p = new Player(0);
          p->setup(player);
          p->setPos(player_loc);
          p->setZValue(0.0);
          this->addItem(p);
          this->gui_players[ind_player] = p;
        }
        //
        connect(this->game_game,
                SIGNAL(gameEnds()),
                this,
                SLOT(gameEnds()));
        connect(this->game_game,
                SIGNAL(playerMoveRejected(const::Bloqus::Game::Player_Move&)),
                this,
                SLOT(playerMoveRejected(const::Bloqus::Game::Player_Move&)));
        connect(this->game_game,
                SIGNAL(playerMoveAccepted(const::Bloqus::Game::Player_Move&)),
                this,
                SLOT(playerMoveAccepted(const::Bloqus::Game::Player_Move&)));
        connect(this->game_game,
                SIGNAL(currentPlayerChanged(::Bloqus::Game::Player_Id)),
                this,
                SLOT(currentPlayerChanged(::Bloqus::Game::Player_Id)));
        connect(this->game_game,
                SIGNAL(playerCantPlay(::Bloqus::Game::Player_Id)),
                this,
                SLOT(playerCantPlay(::Bloqus::Game::Player_Id)));
        this->setHighlightHovered(Options::highlightHoveredPiece());
      }
    } // Game_Scene::setup

    // =====================================================================

    Game::Game* Game_Scene::game() const
    {
      return this->game_game;
    }

    // =====================================================================

    void Game_Scene::setNamesOnSides(bool nos)
    {
      for(unsigned ind_player = 0;
          ind_player < this->game()->nbPlayers();
          ++ind_player)
      {
        this->gui_players[ind_player]->setNameOnSide(nos);
      }
    }

    // =====================================================================

    void Game_Scene::setHighlightHovered(bool s)
    {
      if ( s )
      {
        for(unsigned ind_player = 0; ind_player < 4; ++ind_player)
        {
          for(unsigned ind_piece = 0; ind_piece < 21; ++ind_piece)
          {
            Piece* p = this->gui_pieces[ind_player][ind_piece];
            if ( p != 0 )
            {
              p->setAcceptHoverEvents(true);
            }
          }
        }
      }
      else
      {
        for(unsigned ind_player = 0; ind_player < 4; ++ind_player)
        {
          for(unsigned ind_piece = 0; ind_piece < 21; ++ind_piece)
          {
            Piece* p = this->gui_pieces[ind_player][ind_piece];
            if ( p != 0 )
            {
              if ( p->isHovered() )
              {
                p->setHovered(false);
              }
              p->setAcceptHoverEvents(false);
            }
          }
        }
      }
    }

    // =====================================================================

    void Game_Scene::mouseMoveEvent(QGraphicsSceneMouseEvent* qgsme)
    {
      if ( this->selected_piece != 0 )
      {
        this->selected_piece->setPos(
            this->computePiecePos(this->selected_piece,
                                  qgsme->scenePos()));
        qgsme->accept();
      }
      if ( not qgsme->isAccepted() )
      {
        QGraphicsScene::mouseMoveEvent(qgsme);
      }
    } // Game_Scene::mouseMoveEvent

    // =====================================================================

    void Game_Scene::gameEnds()
    {
    } // Game_Scene::gameEnds

    // =====================================================================

    void Game_Scene::playerMoveRejected(const Game::Player_Move& pm)
    {
      Game::Player* player = this->game()->player(pm.player());
      if ( player->isHuman() )
      {
        QMessageBox msg(QMessageBox::Information,
                        trUtf8("Invalid move"),
                        trUtf8("You can't play here:<br>%1",
                               "'%1' will be the reason (already translated)")
                            .arg(pm.feedback()),
                        QMessageBox::Ok,
                        this->views()[0]);
        msg.setStyleSheet(Defaults::defaultStyleSheet());
        msg.exec();
      }
    } // Game_Scene::playerMoveRejected

    // =====================================================================

    void Game_Scene::playerMoveAccepted(const Game::Player_Move& pm)
    {
      int piece_idx = Game::pieceShapeToIndex(pm.pieceShape());
      int player_idx = Game::playerIdToIndex(pm.player());
      Piece* gui_piece = this->gui_pieces[player_idx][piece_idx];
      gui_piece->setPos(QPointF(10.0, 10.0));
      gui_piece->updateImage();
      gui_piece->setPos(QPointF(pm.col(), pm.row()));
      gui_piece->setZValue(1.0);
      gui_piece->setAllowTransform(false);
      gui_piece->setAcceptHoverEvents(false);
      if ( this->selected_piece != 0 )
      {
        this->selected_piece = 0;
      }
      Game::Request_Next_Player_Event* rnpe =
          new Game::Request_Next_Player_Event();
      QCoreApplication::postEvent(this->game_game, rnpe);
    }

    // =====================================================================

    void Game_Scene::playerCantPlay(::Bloqus::Game::Player_Id pid)
    {
      if ( pid == Game::No_Player )
      {
        return;
      }
      Game::Player* player = this->game()->player(pid);
      if ( player->isHuman() )
      {
        QMessageBox msg(QMessageBox::Information,
                        trUtf8("No place left"),
                        trUtf8("Unfortunately, there's no place left<br>"
                               "for you to play!"),
                        QMessageBox::Ok,
                        this->views()[0]);
        msg.setStyleSheet(Defaults::defaultStyleSheet());
        msg.exec();
      }
      int player_idx = Game::playerIdToIndex(pid);
      for(int ind_piece = 0; ind_piece < 21; ++ind_piece)
      {
        Piece* gui_piece = this->gui_pieces[player_idx][ind_piece];
        if ( gui_piece->gamePiece()->isAvailable() )
        {
          gui_piece->disable();
          this->update(gui_piece->boundingRect());
        }
      }
    }

    // =====================================================================

    void Game_Scene::currentPlayerChanged(::Bloqus::Game::Player_Id pid)
    {
      this->current_player = pid;
      this->update(0.0, 0.0, 20.0, 20.0);
    }

    // =====================================================================

    void Game_Scene::selectPiece(const QPointF& scene_pos)
    {
      QGraphicsItem* item = this->itemAt(scene_pos);
      if ( item != 0 )
      {
        Piece* p = dynamic_cast<Piece*>(item);
        if ( p != 0 )
        {
          Game::Piece* game_piece = p->gamePiece();
          Game::Player* player = game_piece->player();
          if ( (player->id() ==
                this->game_game->currentPlayer()) and
               player->isHuman() and
               player->pieceAvailable(game_piece->pieceShape()) )
          {
            this->selected_piece = p;
            this->selected_piece->setZValue(2.0);
            this->selected_piece->setAllowTransform(true);
            this->selected_piece->setPos(
                this->computePiecePos(this->selected_piece,
                                      scene_pos));
          }
        }
      }
    }

    // =====================================================================

    void Game_Scene::placePiece(const QPointF& /*scene_pos*/)
    {
      QPointF piece_pos = this->selected_piece->pos()+QPointF(0.5, 0.5);
      if ( this->gui_grid->boundingRect().contains(piece_pos) )
      {
        QList<QGraphicsItem*> itms = this->items(piece_pos);
        Grid_Square* square = 0;
        foreach(QGraphicsItem* item, itms)
        {
          square = dynamic_cast<Grid_Square*>(item);
          if ( square != 0 )
            break;
        }
        if ( square == 0 )
        {
          this->selected_piece->restoreDefault();
        }
        else
        {
          Game::Piece* game_piece = this->selected_piece->gamePiece();
          Game::Player* game_player = game_piece->player();
          if ( game_player->isHuman() )
          {
            Game::Human_Player* hp = dynamic_cast<Game::Human_Player*>(game_player);
            hp->guiProposeMove(game_piece->pieceShape(),
                               square->row(),
                               square->col());
          }
        }
      }
      else
      {
        this->selected_piece->setAllowTransform(false);
        this->selected_piece->restoreDefault();
        this->selected_piece = 0;
      }
    }

    // =====================================================================

    QPointF Game_Scene::computePiecePos(Piece* p,
                                        const QPointF& center)
    {
      const QRectF piece_br = p->boundingRect();
      QPointF pos(center.x() - piece_br.width() / 2.0,
                  center.y() - piece_br.height() / 2.0);
      const QRectF scene_rect = this->sceneRect();
      const QRectF good_rect(
          scene_rect.left() + 0.5,
          scene_rect.top() + 0.5,
          scene_rect.width() - (piece_br.width()+1.0),
          scene_rect.height() - (piece_br.height()+1.0));
      pos.rx() = std::max(pos.x(), good_rect.left());
      pos.rx() = std::min(pos.x(), good_rect.right());
      pos.ry() = std::max(pos.y(), good_rect.top());
      pos.ry() = std::min(pos.y(), good_rect.bottom());
      return pos;
    }

    // =====================================================================

    void Game_Scene::mousePressEvent(QGraphicsSceneMouseEvent* qgsme)
    {
      if ( qgsme->button() == Qt::LeftButton )
      {
        if ( this->selected_piece == 0 )
        {
          this->selectPiece(qgsme->scenePos());
          if ( this->selected_piece != 0 )
          {
            qgsme->accept();
          }
        }
        else
        {
          this->placePiece(qgsme->scenePos());
          qgsme->accept();
        }
      }
      else if ( qgsme->button() == Qt::RightButton )
      {
        if ( this->selected_piece != 0 )
        {
          if ( this->selected_piece->allowTransform() )
          {
            if ( this->selected_piece->gamePiece() != 0 )
            {
              this->selected_piece->setPos(10.0, 10.0);
              this->selected_piece->gamePiece()->flip(Qt::Horizontal);
              this->selected_piece->updateImage();
              this->selected_piece->setPos(
                  this->computePiecePos(this->selected_piece,
                                        qgsme->scenePos()));
              qgsme->accept();
            }
          }
        }
      }
      if ( not qgsme->isAccepted() )
      {
        QGraphicsScene::mousePressEvent(qgsme);
      }
    }

    // =====================================================================

    void Game_Scene::wheelEvent(QGraphicsSceneWheelEvent* qgswe)
    {
      if ( this->selected_piece != 0 )
      {
        if ( this->selected_piece->allowTransform() )
        {
          if ( this->selected_piece->gamePiece() != 0 )
          {
            this->selected_piece->setPos(10.0, 10.0);
            if ( qgswe->delta() < 0 )
            {
              this->selected_piece->gamePiece()->rotate(Rotate_Clockwise);
            }
            else
            {
              this->selected_piece->gamePiece()->rotate(Rotate_Trigo);
            }
            this->selected_piece->updateImage();
            this->selected_piece->setPos(
                this->computePiecePos(this->selected_piece,
                                      qgswe->scenePos()));
            qgswe->accept();
          }
        }
      }
      if ( not qgswe->isAccepted() )
      {
        QGraphicsScene::wheelEvent(qgswe);
      }
    }

  } // namespace Gui
} // namespace Bloqus
