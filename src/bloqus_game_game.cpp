
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

#include "bloqus.h"
#include "bloqus_game_game.h"
#include "bloqus_game_player.h"
#include "bloqus_game_grid.h"
#include "bloqus_game_piece.h"
#include "bloqus_game_human_player.h"
/*
#include "bloqus_game_moron_player.h"
#include "bloqus_game_basic_player.h"
#include "bloqus_game_halfwit_player.h"
#include "bloqus_game_smart_player.h"
#include "bloqus_game_annoying_player.h"
  */
#include "bloqus_game_computer_player_strength_0.h"
#include "bloqus_game_computer_player_strength_1.h"
#include "bloqus_game_computer_player_strength_2.h"
#include "bloqus_game_computer_player_strength_3.h"
#include "bloqus_game_computer_player_strength_4.h"
#include "bloqus_game_computer_player_strength_5.h"

namespace Bloqus
{
  namespace Game
  {
    Game::Game(QObject* parent):
        QObject(parent),
        the_grid(0),
        nb_players(0),
        game_running(false),
        current_player(No_Player),
        prev_current_player(No_Player),
        has_been_stopped(false)
    {
      std::fill(this->players, this->players+4, (Player*)0);
    }

    // =====================================================================

    Game::~Game()
    {
      this->clear();
    }

    // =====================================================================

    Player* Game::createHumanPlayer(Player_Id pid,
                                    const QString& pseudo,
                                    const unsigned max_time)
    {
      int idx = playerIdToIndex(pid);
      if ( this->players[idx] != 0 )
      {
        delete this->players[idx];
        this->players[idx] = 0;
      }
      this->players[idx] = new Human_Player(this);
      this->can_play[idx] = true;
      this->players[idx]->initialize(this, pid, pseudo, max_time);
      return this->players[idx];
    }

    // =====================================================================

    Player* Game::createComputerPlayer(Player_Id pid,
                                       const QString& pseudo,
                                       int strength,
                                       const unsigned max_time)
    {
      int idx = playerIdToIndex(pid);
      if ( this->players[idx] != 0 )
      {
        delete this->players[idx];
        this->players[idx] = 0;
      }
      switch ( strength )
      {
        case 0:
          this->players[idx] = new Computer_Player_Strength_0(this);
          break;
        case 1:
          this->players[idx] = new Computer_Player_Strength_1(this);
          break;
        case 2:
          this->players[idx] = new Computer_Player_Strength_2(this);
          break;
        case 3:
          this->players[idx] = new Computer_Player_Strength_3(this);
          break;
        case 4:
          this->players[idx] = new Computer_Player_Strength_4(this);
          break;
        case 5:
          this->players[idx] = new Computer_Player_Strength_5(this);
          break;
        default:
          this->players[idx] = new Computer_Player_Strength_0(this);
          break;
      }
      this->can_play[idx] = true;
      this->players[idx]->initialize(this, pid, pseudo, max_time);
      return this->players[idx];
    }

    // =====================================================================

    void Game::initialize(const unsigned grid_sz,
                          const unsigned nb_plyrs)
    {
      this->clear();
      this->the_grid = new Grid(this);
      this->the_grid->initialize(grid_sz);
      this->nb_players = nb_plyrs;
      this->game_running = false;
      this->current_player = No_Player;
    }

    // =====================================================================

    const Grid& Game::grid() const
    {
      return *(this->the_grid);
    }

    // =====================================================================

    unsigned Game::nbPlayers() const
    {
      return this->nb_players;
    }

    // =====================================================================

    Player* Game::player(Player_Id pid) const
    {
      int index = playerIdToIndex(pid);
      if ( (index < 0) or (index > 3) )
      {
        return 0;
      }
      else
      {
        return this->players[index];
      }
    }

    // =====================================================================

    bool Game::canPlay(Player_Id pid) const
    {
      Player* pl = this->player(pid);
      if ( pl == 0 )
      {
        return false;
      }
      int idx = playerIdToIndex(pid);
      bool newly_cant_play = false;
      if ( this->can_play[idx] )
      {
        if ( pl->remainingTime() == 0 )
        {
          this->can_play[idx] = false;
        }
        else
        {
          this->can_play[idx] = this->the_grid->canPlay(pid);
        }
        newly_cant_play = (not this->can_play[idx]);
      }
      if ( newly_cant_play )
      {
        emit playerCantPlay(pid);
      }
      return this->can_play[idx];
    }

    // =====================================================================

    Player_Id Game::currentPlayer() const
    {
      return this->current_player;
    }

    // =====================================================================

    void Game::startGame()
    {
      this->game_running = true;
      this->has_been_stopped = false;
      this->current_player = First_Player;
      Player* pl = this->player(this->current_player);
      if ( pl != 0 )
      {
        pl->setCurrent(true);
      }
      emit currentPlayerChanged(this->current_player);
    } // Game::startGame

    // =====================================================================

    void Game::stopGame()
    {
      for(unsigned ind_player = 0;
          ind_player < 4;
          ++ind_player)
      {
        this->players[ind_player]->stop();
      }
      this->game_running = false;
      this->has_been_stopped = true;
      this->current_player = No_Player;
      emit gameStopped();
    }

    // =====================================================================

    const QVector<QPoint>& Game::movesHistory() const
    {
      return this->history;
    }

    // =====================================================================

    void Game::customEvent(QEvent* qe)
    {
      if ( qe->type() == Player_Suggest_Move_Event_Type )
      {
        Player_Suggest_Move_Event* psme =
            dynamic_cast<Player_Suggest_Move_Event*>(qe);
        if ( psme != 0 )
        {
          this->the_grid->checkPlayerMove(*psme);
          if ( psme->isValid() )
          {
            Player* move_player = this->player(psme->player());
            Piece* move_piece = move_player->piece(psme->pieceShape());
            this->the_grid->place(move_player->id(),
                                  move_piece,
                                  psme->row(),
                                  psme->col());
            move_player->playPiece(move_piece->pieceShape());
            this->prev_current_player = this->current_player;
            this->current_player = No_Player;
            move_player->setCurrent(false);
            // history record a filled square
            QPoint corner_square;
            const int diag = std::min(move_piece->nbCols(),
                                      move_piece->nbRows());
            for(int k = 0; k < diag; ++k)
            {
              corner_square.rx() = k;
              corner_square.ry() = k;
              if ( move_piece->squareFilled(corner_square) )
              {
                break;
              }
            }
            this->history.push_back(
                QPoint(psme->col(), psme->row())+corner_square);
            // signals
            emit playerMoveAccepted(*psme);
            emit currentPlayerChanged(this->current_player);
          }
          else
          {
            emit playerMoveRejected(*psme);
          }
          psme->setAccepted(true);
        }
      }
      else if ( qe->type() == Request_Next_Player_Event_Type )
      {
        Player* pl = this->player(this->current_player);
        if ( pl != 0 )
        {
          pl->setCurrent(false);
        }
        int player_idx = playerIdToIndex(this->prev_current_player);
        if ( player_idx < 0 )
        {
          player_idx = 0;
        }
        else
        {
          player_idx = (player_idx+1) % 4;
        }
        int nb_tries = 0;
        while ( (nb_tries < 4) and
                (not this->canPlay(indexToPlayerId(player_idx))) )
        {
          player_idx = (player_idx+1) % 4;
          ++nb_tries;
        }
        if ( nb_tries >= 4 )
        {
          this->current_player = No_Player;
          emit gameEnds();
        }
        else
        {
          this->current_player = indexToPlayerId(player_idx);
          pl = this->player(this->current_player);
          pl->setCurrent(true);
          emit currentPlayerChanged(this->current_player);
        }
        qe->setAccepted(true);
      }
      if ( not qe->isAccepted() )
      {
        QObject::customEvent(qe);
      }
    } // Game::customEvent

    // =====================================================================

    void Game::clear()
    {
      if ( this->the_grid != 0 )
      {
        delete this->the_grid;
        this->the_grid = 0;
      }
      for(unsigned ind_player = 0; ind_player < 4; ++ind_player)
      {
        if ( this->players[ind_player] != 0 )
        {
          delete this->players[ind_player];
          this->players[ind_player] = 0;
        }
      }
    }

  } // namespace Game
} // namespace Bloqus
