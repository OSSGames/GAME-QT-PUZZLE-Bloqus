
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

#include <QTimer>

#include "bloqus.h"
#include "bloqus_game_piece.h"
#include "bloqus_game_player.h"

namespace Bloqus
{
  namespace Game
  {

    Player::Player(QObject* parent):
      QObject(parent),
      the_game(0),
      player_id(No_Player),
      nick(),
      pieces(),
      played_pieces(),
      pts(0),
      is_current(false),
      countdown(0),
      chrono(),
      rem_time(300)
    {
    }

    // =====================================================================

    void Player::initialize(Game* g,
                            Player_Id pid,
                            const QString& p,
                            const unsigned max_time)
    {
      this->the_game = g;
      this->player_id = pid;
      const int nb_pieces_shapes = static_cast<int>(Penta_Z) -
                                   static_cast<int>(Mono) + 1;
      this->nick = p;
      this->pieces.resize(nb_pieces_shapes);
      this->played_pieces.fill(false, nb_pieces_shapes);
      for(int ind_piece_shape = 0;
          ind_piece_shape < nb_pieces_shapes;
          ++ind_piece_shape)
      {
        Piece_Shape ps = indexToPieceShape(ind_piece_shape);
        Piece* p = Piece::generate(this, ps);
        this->pieces[ind_piece_shape] = p;
      }
      this->pts = 0;
      this->rem_time = max_time;
      if ( this->countdown == 0 )
      {
        this->countdown = new QTimer(this);
        this->countdown->setSingleShot(false);
        connect(this->countdown,
                SIGNAL(timeout()),
                this,
                SLOT(countdownTimeout()));
      }
    }

    // =====================================================================

    Game* Player::game() const
    {
      return this->the_game;
    }

    // =====================================================================

    Player_Id Player::id() const
    {
      return this->player_id;
    }

    // =====================================================================

    const QString& Player::nickname() const
    {
      return this->nick;
    }

    // =====================================================================

    unsigned Player::points() const
    {
      return this->pts;
    }

    // =====================================================================

    unsigned Player::remainingTime() const
    {
      return this->rem_time;
    }

    // =====================================================================

    bool Player::isCurrent() const
    {
      return this->is_current;
    }

    // =====================================================================

    Piece* Player::piece(Piece_Shape pc) const
    {
      int idx = pieceShapeToIndex(pc);
      return this->pieces[idx];
    }

    // =====================================================================

    void Player::playPiece(Piece_Shape pc)
    {
      int idx = pieceShapeToIndex(pc);
      Piece* p = this->pieces[idx];
      p->setAvailable(false);
      this->pts += p->value();
      this->played_pieces.setBit(idx, true);
      emit pointsChanged();
    }

    // =====================================================================

    bool Player::pieceAvailable(Piece_Shape pc) const
    {
      int idx = pieceShapeToIndex(pc);
      return (not this->played_pieces.testBit(idx));
    }

    // =====================================================================

    QVector<Piece*> Player::availablePieces() const
    {
      QVector<Piece*> av;
      av.reserve(this->pieces.size()/2);
      Piece_Shape ps = Mono;
      while ( ps != Unknown_Shape )
      {
        if ( this->pieceAvailable(ps) )
        {
          av.append(this->piece(ps));
        }
        ps = static_cast<Piece_Shape>(static_cast<int>(ps)+1);
      }
      return av;
    }

    // =====================================================================

    void Player::setCurrent(bool c)
    {
      if ( c != this->is_current )
      {
        if ( this->is_current )
        {
          this->countdown->stop();
          this->countdownTimeout();
          if ( this->rem_time == this->saved_rem_time )
          {
            --(this->rem_time);
            emit timeChanged();
          }
          emit leaveCurrentPlayer();
        }
        this->is_current = c;
        if ( this->is_current )
        {
          this->countdown->start(1000);
          this->chrono.restart();
          this->saved_rem_time = this->rem_time;
          emit enterCurrentPlayer();
        }
      }
    }

    // =====================================================================

    void Player::stop()
    {
    }

    // =====================================================================

    void Player::countdownTimeout()
    {
      int nb_sec = qRound(
          static_cast<double>(this->chrono.elapsed()) / 1000.0);
      if ( nb_sec > 0 )
      {
        if ( static_cast<unsigned>(nb_sec) > this->rem_time )
        {
          this->rem_time = 0;
        }
        else
        {
          this->rem_time -= nb_sec;
        }
        this->chrono.restart();
        emit timeChanged();
      }
    }

  } // namespace Game
} // namespace Bloqus
