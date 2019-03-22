
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

#include <limits>
#include <algorithm>

#include <QCoreApplication>
#include <QRunnable>
#include <QThreadPool>

#include "bloqus.h"
#include "bloqus_game.h"
#include "bloqus_game_game.h"
#include "bloqus_game_grid.h"
#include "bloqus_game_piece.h"
#include "bloqus_game_computer_player.h"

namespace Bloqus
{
  namespace Game
  {
    class Move_Evaluator: public QRunnable
    {
      public:
        Move_Evaluator(const Computer_Player& pl,
                        QVector<Computed_Move>& mvs,
                        int idx_begin,
                        int nb):
          QRunnable(),
          player(pl),
          moves(mvs),
          index_begin(idx_begin),
          nb_to_evaluate(nb)
        {
        }

        virtual void run()
        {
          this->player.evaluatesMoves(this->moves,
                                      this->index_begin,
                                      this->nb_to_evaluate);
        }

      private:
        const Computer_Player& player;
        QVector<Computed_Move>& moves;
        int index_begin;
        int nb_to_evaluate;
    }; // class Move_Evaluator

    // =====================================================================

    Thinking_Thread::Thinking_Thread(QObject* parent):
        QThread(parent),
        player(0),
        found_move(No_Player, No_Shape, -1, -1, false, 0)
    {
    }

    // =====================================================================

    void Thinking_Thread::setup(Computer_Player* pl)
    {
      this->player = pl;
    }

    // =====================================================================

    const Computed_Move& Thinking_Thread::foundMove() const
    {
      return this->found_move;
    }

    // =====================================================================

    void Thinking_Thread::run()
    {
      found_move = player->findMove();
    }

    // =====================================================================

    Computer_Player::Computer_Player(QObject* parent):
        Player(parent),
        thinking(0)
    {
    }

    // =====================================================================

    Computer_Player::~Computer_Player()
    {
      if ( thinking != 0 )
      {
        disconnect(this->thinking,
                   SIGNAL(finished()),
                   this,
                   SLOT(thinkingDone()));
        this->thinking->wait();
      }
    }

    // =====================================================================

    bool Computer_Player::isHuman() const
    {
      return false;
    }

    // =====================================================================

    bool Computer_Player::isLocal() const
    {
      return true;
    }

    // =====================================================================

    void Computer_Player::evaluatesMoves(QVector<Computed_Move>& moves,
                                         const int ind_first,
                                         const int nb) const
    {
      for(int ind_move = 0; ind_move < nb; ++ind_move)
      {
        Grid tmp_grid(0);
        Piece tmp_piece(0);
        Computed_Move& cm = moves[ind_first+ind_move];
        tmp_grid.cloneFrom(this->game()->grid());
        tmp_piece.cloneFrom(*(this->piece(cm.pieceShape())));
        cm.applyOnPiece(&tmp_piece);
        cm.setValue(this->evaluateMove(tmp_grid,
                                       tmp_piece,
                                       cm));
      }
    }

    // =====================================================================

    bool cmp_moves(const Computed_Move& m1,
                   const Computed_Move& m2)
    {
      // sort in reverse order
      return m2.value() < m1.value();
    }

    void Computer_Player::evaluatesAndSortMoves(QVector<Computed_Move>& moves)
    {
      if ( moves.empty() )
      {
        return;
      }
      const int nb_moves = moves.size();
      QThreadPool* pool = QThreadPool::globalInstance();
      const int nb_threads = pool->maxThreadCount();
      const int nb_moves_per_thread = nb_moves / nb_threads;
      for(int ind_thread = 0; ind_thread < nb_threads-1; ++ind_thread)
      {
        pool->start(new Move_Evaluator(*this,
                                       moves,
                                       ind_thread*nb_moves_per_thread,
                                       nb_moves_per_thread));
      }
      pool->start(new Move_Evaluator(
          *this,
          moves,
          (nb_threads-1)*nb_moves_per_thread,
          nb_moves-(nb_threads-1)*nb_moves_per_thread));
      pool->waitForDone();
      std::sort(moves.begin(), moves.end(), cmp_moves);
    }

    // =====================================================================

    void Computer_Player::setCurrent(bool c)
    {
      Player::setCurrent(c);
      if ( this->isCurrent() )
      {
        this->has_been_stopped = false;
        this->thinking = new Thinking_Thread(this);
        this->thinking->setup(this);
        connect(this->thinking,
                SIGNAL(finished()),
                this,
                SLOT(thinkingDone()));
        this->thinking->start();
      }
    }

    // =====================================================================

    void Computer_Player::stop()
    {
      if ( this->thinking != 0 )
      {
        disconnect(this->thinking,
                   SIGNAL(finished()),
                   this,
                   SLOT(thinkingDone()));
      }
      this->has_been_stopped = true;
      if ( this->thinking != 0 )
      {
        this->thinking->wait();
        this->thinking->deleteLater();
        this->thinking = 0;
      }
    }

    // =====================================================================

    void Computer_Player::thinkingDone()
    {
      const Computed_Move& cm = this->thinking->foundMove();
      this->thinking->deleteLater();
      this->thinking = 0;
      Piece* p = this->piece(cm.pieceShape());
      if ( p != 0 )
      {
        if ( cm.withHorizontalFlip() )
        {
          p->flip(Qt::Horizontal);
        }
        for(unsigned rot = 0; rot < cm.nbTrigoRotations(); ++rot)
        {
          p->rotate(Rotate_Trigo);
        }
        Player_Suggest_Move_Event* psm =
          new Player_Suggest_Move_Event(this->id(),
                                        p->pieceShape(),
                                        cm.row(),
                                        cm.col());
        QCoreApplication::postEvent(this->game(), psm);
      }
    }

  } // namespace Game
} // namespace Bloqus
