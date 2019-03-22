
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

#include <QtDebug>

#include "bloqus.h"
#include "bloqus_game.h"
#include "bloqus_game_piece.h"

namespace Bloqus
{
  namespace Game
  {

    Player_Id indexToPlayerId(int idx)
    {
      if ( (idx < playerIdToIndex(First_Player)) or
           (idx > playerIdToIndex(Last_Player)) )
      {
        return No_Player;
      }
      else
      {
        return static_cast<Player_Id>(static_cast<int>(First_Player)+idx);
      }
    }

    // =====================================================================

    int playerIdToIndex(Player_Id pid)
    {
      return static_cast<int>(pid) - static_cast<int>(First_Player);
    }

    // =====================================================================

    QPoint startingCorner(Player_Id pid)
    {
      switch ( pid )
      {
        case North_West_Player:
          return QPoint(0, 0);
          break;
        case North_East_Player:
          return QPoint(20, 0);
          break;
        case South_East_Player:
          return QPoint(20, 20);
          break;
        case South_West_Player:
          return QPoint(0, 20);
          break;
        default:
          return QPoint(-1, -1);
      }
    }

    // =====================================================================

    QPoint oppositeCorner(Player_Id pid)
    {
      switch ( pid )
      {
        case North_West_Player:
          return QPoint(20, 20);
          break;
        case North_East_Player:
          return QPoint(0, 20);
          break;
        case South_East_Player:
          return QPoint(0, 0);
          break;
        case South_West_Player:
          return QPoint(20, 0);
          break;
        default:
          return QPoint(-1, -1);
      }
    }

    // =====================================================================

    QPoint leftHandCorner(Player_Id pid)
    {
      switch ( pid )
      {
        case North_West_Player:
          return QPoint(20, 0);
          break;
        case North_East_Player:
          return QPoint(20, 20);
          break;
        case South_East_Player:
          return QPoint(0, 20);
          break;
        case South_West_Player:
          return QPoint(0, 0);
          break;
        default:
          return QPoint(-1, -1);
      }
    }

    // =====================================================================

    QPoint rightHandCorner(Player_Id pid)
    {
      switch ( pid )
      {
        case North_West_Player:
          return QPoint(0, 20);
          break;
        case North_East_Player:
          return QPoint(0, 0);
          break;
        case South_East_Player:
          return QPoint(20, 0);
          break;
        case South_West_Player:
          return QPoint(20, 20);
          break;
        default:
          return QPoint(-1, -1);
      }
    }

    // =====================================================================

    QPoint playerDirection(Player_Id pid)
    {
      switch ( pid )
      {
        case North_West_Player:
          return QPoint(1, 1);
          break;
        case North_East_Player:
          return QPoint(-1, 1);
          break;
        case South_East_Player:
          return QPoint(-1, -1);
          break;
        case South_West_Player:
          return QPoint(1, -1);
          break;
        default:
          return QPoint(0, 0);
      }
    }

    // =====================================================================

    unsigned playerMask(Player_Id pid)
    {
      return (0x00010101 << playerIdToIndex(pid));
    }

    // =====================================================================

    unsigned containsPlayerMask(Player_Id pid)
    {
      return (1 << playerIdToIndex(pid));
    }

    // =====================================================================

    unsigned allowedPlayerMask(Player_Id pid)
    {
      return (1 << playerIdToIndex(pid)) << 8;
    }

    // =====================================================================

    unsigned forbiddenPlayerMask(Player_Id pid)
    {
      return (1 << playerIdToIndex(pid)) << 16;
    }

    // =====================================================================

    Piece_Shape indexToPieceShape(int idx)
    {
      if ( (idx < pieceShapeToIndex(First_Piece_Shape)) or
           (idx > pieceShapeToIndex(Last_Piece_Shape)) )
      {
        return No_Shape;
      }
      else
      {
        return static_cast<Piece_Shape>(static_cast<int>(First_Piece_Shape) + idx);
      }
    }

    // =====================================================================

    int pieceShapeToIndex(Piece_Shape ps)
    {
      return static_cast<int>(ps) - static_cast<int>(First_Piece_Shape);
    }

    // =====================================================================

    namespace
    {
      std::vector<bool>* pattern_changed_by_horiz_flip = 0;
    }

    bool isPatternChangedByHorizontalFlip(Piece_Shape ps)
    {
      if ( pattern_changed_by_horiz_flip == 0 )
      {
        pattern_changed_by_horiz_flip = new std::vector<bool>(21, false);
        (*pattern_changed_by_horiz_flip)[ 0] = false;
        (*pattern_changed_by_horiz_flip)[ 1] = false;
        (*pattern_changed_by_horiz_flip)[ 2] = false;
        (*pattern_changed_by_horiz_flip)[ 3] = true;
        (*pattern_changed_by_horiz_flip)[ 4] = false;
        (*pattern_changed_by_horiz_flip)[ 5] = true;
        (*pattern_changed_by_horiz_flip)[ 6] = false;
        (*pattern_changed_by_horiz_flip)[ 7] = true;
        (*pattern_changed_by_horiz_flip)[ 8] = false;
        (*pattern_changed_by_horiz_flip)[ 9] = true;
        (*pattern_changed_by_horiz_flip)[10] = false;
        (*pattern_changed_by_horiz_flip)[11] = true;
        (*pattern_changed_by_horiz_flip)[12] = true;
        (*pattern_changed_by_horiz_flip)[13] = true;
        (*pattern_changed_by_horiz_flip)[14] = true;
        (*pattern_changed_by_horiz_flip)[15] = false;
        (*pattern_changed_by_horiz_flip)[16] = true;
        (*pattern_changed_by_horiz_flip)[17] = true;
        (*pattern_changed_by_horiz_flip)[18] = false;
        (*pattern_changed_by_horiz_flip)[19] = true;
        (*pattern_changed_by_horiz_flip)[20] = true;
      }
      int idx = pieceShapeToIndex(ps);
      return pattern_changed_by_horiz_flip->at(idx);
    }

    // =====================================================================

    namespace
    {
      std::vector<unsigned>* max_num_of_rots = 0;
    }

    unsigned maxNumberOfTrigoRotations(Piece_Shape ps)
    {
      if ( max_num_of_rots == 0 )
      {
        max_num_of_rots = new std::vector<unsigned>(21, 0);
        (*max_num_of_rots)[ 0] = 0;
        (*max_num_of_rots)[ 1] = 2;
        (*max_num_of_rots)[ 2] = 2;
        (*max_num_of_rots)[ 3] = 4;
        (*max_num_of_rots)[ 4] = 2;
        (*max_num_of_rots)[ 5] = 4;
        (*max_num_of_rots)[ 6] = 0;
        (*max_num_of_rots)[ 7] = 2;
        (*max_num_of_rots)[ 8] = 4;
        (*max_num_of_rots)[ 9] = 4;
        (*max_num_of_rots)[10] = 2;
        (*max_num_of_rots)[11] = 4;
        (*max_num_of_rots)[12] = 4;
        (*max_num_of_rots)[13] = 4;
        (*max_num_of_rots)[14] = 4;
        (*max_num_of_rots)[15] = 4;
        (*max_num_of_rots)[16] = 4;
        (*max_num_of_rots)[17] = 4;
        (*max_num_of_rots)[18] = 0;
        (*max_num_of_rots)[19] = 4;
        (*max_num_of_rots)[20] = 2;
      }
      int idx = pieceShapeToIndex(ps);
      return max_num_of_rots->at(idx);
    }

    // =====================================================================

    Player_Move::Player_Move():
      the_player(No_Player),
      the_shape(No_Shape),
      the_row(-1),
      the_col(-1),
      is_valid(false),
      move_feedback("")
    {
    }

    // =====================================================================

    Player_Move::Player_Move(Player_Id pid,
                             Piece_Shape ps,
                             int row,
                             int col):
      the_player(pid),
      the_shape(ps),
      the_row(row),
      the_col(col),
      is_valid(false),
      move_feedback("")
    {
    }

    // =====================================================================

    Player_Id Player_Move::player() const
    {
      return this->the_player;
    }

    // =====================================================================

    Piece_Shape Player_Move::pieceShape() const
    {
      return this->the_shape;
    }

    // =====================================================================

    int Player_Move::row() const
    {
      return this->the_row;
    }

    // =====================================================================

    int Player_Move::col() const
    {
      return this->the_col;
    }

    // =====================================================================

    bool Player_Move::isValid() const
    {
      return this->is_valid;
    }

    // =====================================================================

    const QString& Player_Move::feedback() const
    {
      return this->move_feedback;
    }

    // =====================================================================

    void Player_Move::setValid(bool v)
    {
      this->is_valid = v;
    }

    // =====================================================================

    void Player_Move::setFeedback(const QString& f)
    {
      this->move_feedback = f;
    }

    // =====================================================================

    Computed_Move::Computed_Move():
        Player_Move(),
        with_flip(false),
        nb_rotations(0),
        val(-std::numeric_limits<double>::max())
    {
    }

    // =====================================================================

    Computed_Move::Computed_Move(Player_Id pid,
                                 Piece_Shape ps,
                                 int row,
                                 int col,
                                 bool horiz_flip,
                                 unsigned nb_rot):
      Player_Move(pid, ps, row, col),
      with_flip(horiz_flip),
      nb_rotations(nb_rot),
      val(-std::numeric_limits<double>::max())
    {
    }

    // =====================================================================

    bool Computed_Move::withHorizontalFlip() const
    {
      return this->with_flip;
    }

    // =====================================================================

    unsigned Computed_Move::nbTrigoRotations() const
    {
      return this->nb_rotations;
    }

    // =====================================================================

    double Computed_Move::value() const
    {
      return this->val;
    }

    // =====================================================================

    void Computed_Move::setValue(double v)
    {
      this->val = v;
    }

    // =====================================================================

    void Computed_Move::applyOnPiece(Piece* p) const
    {
      if ( p == 0 )
      {
        return;
      }
      if ( this->withHorizontalFlip() )
      {
        p->flip(Qt::Horizontal);
      }
      for(unsigned rot = 0; rot < this->nbTrigoRotations(); ++rot)
      {
        p->rotate(Rotate_Trigo);
      }
    }

    // =====================================================================

    Player_Suggest_Move_Event::Player_Suggest_Move_Event(Player_Id pid,
                                                         Piece_Shape ps,
                                                         int row,
                                                         int col):
      QEvent(Player_Suggest_Move_Event_Type),
      Player_Move(pid, ps, row, col)
    {
    }

    // =====================================================================

    Request_Next_Player_Event::Request_Next_Player_Event():
        QEvent(Request_Next_Player_Event_Type)
    {
    }

  } // namespace Game
} // namespace Bloqus
