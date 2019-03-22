
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

#include "bloqus.h"
#include "bloqus_game_piece.h"
#include "bloqus_game_grid.h"
#include "bloqus_game_game.h"
#include "bloqus_game_player.h"

namespace Bloqus
{
  namespace Game
  {

    // =====================================================================

    Player_Stats::Player_Stats()
    {
    }

    // =====================================================================

    void Player_Stats::clear()
    {
      this->used_squares.clear();
      this->allowed_squares.clear();
      this->forbidden_squares.clear();
    }

    // =====================================================================

    const QVector<QPoint>& Player_Stats::usedSquares() const
    {
      return this->used_squares;
    }

    // =====================================================================

    const QVector<QPoint>& Player_Stats::allowedSquares() const
    {
      return this->allowed_squares;
    }

    // =====================================================================

    const QVector<QPoint>& Player_Stats::forbiddenSquares() const
    {
      return this->forbidden_squares;
    }

    // =====================================================================

    Grid_Stats::Grid_Stats()
    {
    }

    // =====================================================================

    void Grid_Stats::clear()
    {
      this->empty_squares.clear();
      this->players[0].clear();
      this->players[1].clear();
      this->players[2].clear();
      this->players[3].clear();
    }

    // =====================================================================

    const QVector<QPoint>& Grid_Stats::emptySquares() const
    {
      return this->empty_squares;
    }

    // =====================================================================

    const Player_Stats& Grid_Stats::playerStats(Player_Id pid) const
    {
      int idx = playerIdToIndex(pid);
      return this->players[idx];
    }

    // =====================================================================

    Grid::Grid(QObject* parent):
        QObject(parent),
        stats_dirty(true)
    {
    }

    // =====================================================================

    Game* Grid::game() const
    {
      return dynamic_cast<Game*>(this->parent());
    }

    // =====================================================================

    const Grid_Stats& Grid::getStats() const
    {
      if ( this->stats_dirty )
      {
        this->stats.clear();
        for(unsigned r = 0; r < this->actual_size; ++r)
        {
          for(unsigned c = 0; c < this->actual_size; ++c)
          {
            const QPoint sq(c, r);
            if ( this->squares[r][c] == 0 )
            {
              this->stats.empty_squares.append(sq);
            }
            else
            {
              for(int ind_player = 0; ind_player < 4; ++ind_player)
              {
                Player_Id pid = indexToPlayerId(ind_player);
                Player_Stats& pl_stats = this->stats.players[ind_player];
                if ( this->allowedPlayer(pid, r, c) )
                {
                  pl_stats.allowed_squares.append(sq);
                }
                if ( this->forbiddenPlayer(pid, r, c) )
                {
                  pl_stats.forbidden_squares.append(sq);
                }
                if ( this->containsPlayer(pid, r, c) )
                {
                  pl_stats.used_squares.append(sq);
                }
              }
            }
          }
        }
        this->stats_dirty = false;
      }
      return this->stats;
    }

    // =====================================================================

    void Grid::initialize(const unsigned sz)
    {
      this->actual_size = sz;
      this->squares.resize(this->actual_size);
      for(unsigned r = 0; r < this->actual_size; ++r)
      {
        this->squares[r].resize(this->actual_size);
        for(unsigned c = 0; c < this->actual_size; ++c)
        {
          this->squares[r][c] = 0;
        }
      }
      this->squares[0                  ][0]                   =
          Allowed_North_West;
      this->squares[0                  ][this->actual_size-1] =
          Allowed_North_East;
      this->squares[this->actual_size-1][this->actual_size-1] =
          Allowed_South_East;
      this->squares[this->actual_size-1][0]                   =
          Allowed_South_West;
      this->stats_dirty = true;
    }

    // =====================================================================

    void Grid::cloneFrom(const Grid& other)
    {
      this->squares = other.squares;
      this->actual_size = other.actual_size;
      this->stats = other.stats;
      this->stats_dirty = other.stats_dirty;
    }

    // =====================================================================

    unsigned Grid::size() const
    {
      return this->actual_size;
    }

    // =====================================================================

    quint32 Grid::status(const unsigned row,
                         const unsigned col) const
    {
      return this->squares[row][col];
    }

    // =====================================================================

    bool Grid::containsPlayer(Player_Id pid,
                              const unsigned row,
                              const unsigned col) const
    {
      return ( this->status(row, col) bitand containsPlayerMask(pid) ) != 0;
    }

    // =====================================================================

    bool Grid::allowedPlayer(Player_Id pid,
                             const unsigned row,
                             const unsigned col) const
    {
      return ( this->status(row, col) bitand allowedPlayerMask(pid) ) != 0;
    }

    // =====================================================================

    bool Grid::forbiddenPlayer(Player_Id pid,
                               const unsigned row,
                               const unsigned col) const
    {
      return ( this->status(row, col) bitand forbiddenPlayerMask(pid) ) != 0;
    }

    // =====================================================================

    bool Grid::isEmpty(const unsigned row,
                       const unsigned col) const
    {
      return ( this->status(row, col) bitand Mask_Contains ) == 0;
    }

    // =====================================================================

    QVector<QPoint> Grid::allowedSquares(Player_Id pid) const
    {
      const Grid_Stats& st = this->getStats();
      return st.playerStats(pid).allowedSquares();
    } // Grid::allowedSquares

    // =====================================================================

    bool Grid::canPlace(Player_Id pid,
                        const Piece* piece,
                        const unsigned row,
                        const unsigned col,
                        QString* feedback) const
    {
      QString row_col;
      if ( feedback != 0 )
      {
        row_col = QString("(%1, %2)").arg(row+1).arg(col+1);
      }
      if ( (row >= this->size()) or (col >= this->size()) )
      {
        if ( feedback != 0 )
        {
          *feedback = trUtf8("Invalid row or column: %1.",
                             "'%1' will receive (row, col)")
            .arg(row_col);
        }
        return false;
      }
      if ( (row+piece->nbRows()-1) >= this->size() )
      {
        if ( feedback != 0 )
        {
          *feedback = trUtf8("Piece too tall (%1) for %2.",
                             "'%1' is height of piece, '%2' is (row, col")
            .arg(piece->nbRows())
            .arg(row_col);
        }
        return false;
      }
      if ( (col+piece->nbCols()-1) >= this->size() )
      {
        if ( feedback != 0 )
        {
          *feedback = trUtf8("Piece too wide (%1) for %2.",
                             "'%1' is height of piece, '%2' is (row, col")
            .arg(piece->nbCols())
            .arg(row_col);
        }
        return false;
      }
      bool has_allowed = false;
      for(int r = 0; r < piece->nbRows(); ++r)
      {
        for(int c = 0; c < piece->nbCols(); ++c)
        {
          if ( piece->squareFilled(r, c) )
          {
            const unsigned grid_row = row + r;
            const unsigned grid_col = col + c;
            if ( ! this->isEmpty(grid_row, grid_col) )
            {
              if ( feedback != 0 )
              {
                *feedback = trUtf8("Square (%1, %2) is not empty.",
                                   "'%1' is row, '%2' col")
                  .arg(grid_row+1)
                  .arg(grid_col+1);
              }
              return false;
            }
            if ( this->forbiddenPlayer(pid, grid_row, grid_col) )
            {
              if ( feedback != 0 )
              {
                *feedback = trUtf8("Square (%1, %2) is forbidden.",
                                   "'%1' is row, '%2' col")
                  .arg(grid_row+1)
                  .arg(grid_col+1);
              }
              return false;
            }
            if ( ! has_allowed )
            {
              has_allowed = this->allowedPlayer(pid, grid_row, grid_col);
            }
          }
        }
      }
      if ( feedback != 0 )
      {
        if ( has_allowed )
        {
          *feedback = trUtf8("Piece can be placed here.");
        }
        else
        {
          *feedback = trUtf8("No allowed square below piece's squares.");
        }
      }
      return has_allowed;
    } // Grid::canPlace

    // =====================================================================

    void Grid::checkPlayerMove(Player_Move& pm) const
    {
      QString f;
      Player* pl = this->game()->player(pm.player());
      if ( pl == 0 )
      {
        pm.setValid(false);
        pm.setFeedback(trUtf8("Invalid player."));
        return;
      }
      Piece* p = pl->piece(pm.pieceShape());
      if ( p == 0 )
      {
        pm.setValid(false);
        pm.setFeedback(trUtf8("Invalid piece."));
        return;
      }
      pm.setValid(this->canPlace(pl->id(), p, pm.row(), pm.col(), &f));
      pm.setFeedback(f);
    }

    // =====================================================================

    void Grid::findPlace(Player_Id pid,
                         Piece* piece,
                         const QVector<QPoint>& allowed_squares,
                         QPoint& where) const
    {
      where.rx() = -1;
      where.ry() = -1;
      if ( allowed_squares.empty() )
      {
        return;
      }
      Piece tmp_piece(0);
      foreach(QPoint square, allowed_squares)
      {
        tmp_piece.cloneFrom(*piece);
        for(int rot = 0; rot < 4; ++rot)
        {
          tmp_piece.rotate(Rotate_Trigo);
          for(int r = 0; r < tmp_piece.nbRows(); ++r)
          {
            for(int c = 0; c < tmp_piece.nbCols(); ++c)
            {
              if ( this->canPlace(pid,
                                  &tmp_piece,
                                  square.y()-r,
                                  square.x()-c) )
              {
                where.rx() = square.x() - c;
                where.ry() = square.y() - r;
                return;
              }
            }
          }
          tmp_piece.flip(Qt::Horizontal);
          for(int r = 0; r < tmp_piece.nbRows(); ++r)
          {
            for(int c = 0; c < tmp_piece.nbCols(); ++c)
            {
              if ( this->canPlace(pid,
                                  &tmp_piece,
                                  square.y()-r,
                                  square.x()-c) )
              {
                where.rx() = square.x() - c;
                where.ry() = square.y() - r;
                return;
              }
            }
          }
          tmp_piece.flip(Qt::Horizontal);
        }
      }
    } // Grid::findPlace

    // =====================================================================

    void Grid::findPlace(Player_Id pid,
                         Piece* piece,
                         QPoint& where) const
    {
      QVector<QPoint> allowed_squares = this->allowedSquares(pid);
      this->findPlace(pid, piece, allowed_squares, where);
    } // Grid::findPlace

    // =====================================================================

    namespace
    {
      void count_moves_around_squares(Player_Id pid,
                                     Piece* piece,
                                     const Grid& grid,
                                     const QPoint& square,
                                     int& nb)
      {
        for(int r = 0; r < piece->nbRows(); ++r)
        {
          for(int c = 0; c < piece->nbCols(); ++c)
          {
            if ( grid.canPlace(pid,
                               piece,
                               square.y()-r,
                               square.x()-c) )
            {
              ++nb;
            }
          }
        }
      } // count_moves_around_squares
    }

    int Grid::countAllMoves(Player_Id pid,
                            const QVector<Piece*>& pieces) const
    {
      if ( pieces.isEmpty() )
      {
        return 0;
      }
      QVector<QPoint> squares = this->allowedSquares(pid);
      if ( squares.isEmpty() )
      {
        return 0;
      }
      int nb_moves = 0;
      Piece tmp_piece(0);
      foreach(Piece* piece, pieces)
      {
        tmp_piece.cloneFrom(*piece);
        foreach(QPoint square, squares)
        {
          const unsigned max_rots =
              maxNumberOfTrigoRotations(tmp_piece.pieceShape());
          if ( max_rots == 0 )
          {
            count_moves_around_squares(pid,
                                       &tmp_piece,
                                       *this,
                                       square,
                                       nb_moves);
          }
          else
          {
            for(unsigned rot = 0; rot < max_rots; ++rot)
            {
              tmp_piece.rotate(Rotate_Trigo);
              count_moves_around_squares(pid,
                                         &tmp_piece,
                                         *this,
                                         square,
                                         nb_moves);
            }
          }
          if ( isPatternChangedByHorizontalFlip(tmp_piece.pieceShape()) )
          {
            tmp_piece.flip(Qt::Horizontal);
            if ( max_rots == 0 )
            {
              count_moves_around_squares(pid,
                                         &tmp_piece,
                                         *this,
                                         square,
                                         nb_moves);
            }
            else
            {
              for(unsigned rot = 0; rot < max_rots; ++rot)
              {
                tmp_piece.rotate(Rotate_Trigo);
                count_moves_around_squares(pid,
                                           &tmp_piece,
                                           *this,
                                           square,
                                           nb_moves);
              }
            }
            tmp_piece.flip(Qt::Horizontal);
          }
        }
      }
      return nb_moves;
    } // Grid::countAllMoves

    // =====================================================================

    namespace
    {
      void find_moves_around_squares(Player_Id pid,
                                     Piece* piece,
                                     const Grid& grid,
                                     const QPoint& square,
                                     unsigned nb_rots,
                                     bool with_flip,
                                     QVector<Computed_Move>& moves)
      {
        for(int r = 0; r < piece->nbRows(); ++r)
        {
          for(int c = 0; c < piece->nbCols(); ++c)
          {
            if ( grid.canPlace(pid,
                               piece,
                               square.y()-r,
                               square.x()-c) )
            {
              moves.append(Computed_Move(pid,
                                         piece->pieceShape(),
                                         square.y()-r,
                                         square.x()-c,
                                         with_flip,
                                         nb_rots));
            }
          }
        }
      } // find_moves_around_squares
    }

    void Grid::findAllMoves(Player_Id pid,
                            const QVector<Piece*>& pieces,
                            QVector<Computed_Move>& moves) const
    {
      if ( pieces.isEmpty() )
      {
        return;
      }
      QVector<QPoint> squares = this->allowedSquares(pid);
      if ( squares.isEmpty() )
      {
        return;
      }
      moves.reserve(moves.size() + 2*pieces.size()*squares.size());
      Piece tmp_piece(0);
      foreach(Piece* piece, pieces)
      {
        tmp_piece.cloneFrom(*piece);
        foreach(QPoint square, squares)
        {
          const unsigned max_rots =
              maxNumberOfTrigoRotations(tmp_piece.pieceShape());
          if ( max_rots == 0 )
          {
            find_moves_around_squares(pid,
                                      &tmp_piece,
                                      *this,
                                      square,
                                      0,
                                      false,
                                      moves);
          }
          else
          {
            for(unsigned rot = 0; rot < max_rots; ++rot)
            {
              tmp_piece.rotate(Rotate_Trigo);
              find_moves_around_squares(pid,
                                        &tmp_piece,
                                        *this,
                                        square,
                                        (rot+1)%max_rots,
                                        false,
                                        moves);
            }
          }
          if ( isPatternChangedByHorizontalFlip(tmp_piece.pieceShape()) )
          {
            tmp_piece.flip(Qt::Horizontal);
            if ( max_rots == 0 )
            {
              find_moves_around_squares(pid,
                                        &tmp_piece,
                                        *this,
                                        square,
                                        0,
                                        true,
                                        moves);
            }
            else
            {
              for(unsigned rot = 0; rot < max_rots; ++rot)
              {
                tmp_piece.rotate(Rotate_Trigo);
                find_moves_around_squares(pid,
                                          &tmp_piece,
                                          *this,
                                          square,
                                          (rot+1)%max_rots,
                                          true,
                                          moves);
              }
            }
            tmp_piece.flip(Qt::Horizontal);
          }
        }
      }
      return;
    } // Grid::findAllMoves

    // =====================================================================

    QVector<Computed_Move> Grid::findAllMoves(
            Player_Id pid,
            const QVector<Piece*>& pieces) const
    {
      QVector<Computed_Move> moves;
      this->findAllMoves(pid, pieces, moves);
      return moves;
    }

    // =====================================================================

    bool Grid::canPlay(Player_Id pid) const
    {
      Player* player = this->game()->player(pid);
      QVector<Piece*> available_pieces = player->availablePieces();
      if ( available_pieces.empty() )
      {
        return false;
      }
      QVector<QPoint> allowed_squares = this->allowedSquares(pid);
      if ( allowed_squares.isEmpty() )
      {
        return false;
      }
      foreach(Piece* piece, available_pieces)
      {
        QPoint where;
        this->findPlace(pid, piece, allowed_squares, where);
        if ( (where.x() >= 0) and (where.y() >= 0) )
        {
          return true;
        }
      }
      return false;
    } // Grid::canPlay

    // =====================================================================

    void Grid::place(Player_Id pid,
                     const Piece* const piece,
                     const unsigned row,
                     const unsigned col)
    {
      const unsigned contains_mask = playerMask(pid);
      const unsigned allowed_mask = allowedPlayerMask(pid);
      const unsigned forbidden_mask = forbiddenPlayerMask(pid);
      QVector<QPoint> filled;
      QVector<QPoint> adjacent;
      QVector<QPoint> corners;
      piece->getSquares(filled, adjacent, corners);
      // marking newly filled squares
      foreach(QPoint pt, filled)
      {
        const int grid_row = row + pt.y();
        const int grid_col = col + pt.x();
        this->squares[grid_row][grid_col] = contains_mask;
      }
      // marking newly forbidden squares
      foreach(QPoint pt, adjacent)
      {
        const int grid_row = row + pt.y();
        const int grid_col = col + pt.x();
        if ( (grid_row >= 0) and
             (grid_col >= 0) and
             (grid_row < static_cast<int>(this->size())) and
             (grid_col < static_cast<int>(this->size())) )
        {
          if ( this->allowedPlayer(pid, grid_row, grid_col) )
          {
            this->squares[grid_row][grid_col] =
                this->squares[grid_row][grid_col] bitand (compl allowed_mask);
          }
          if ( this->isEmpty(grid_row, grid_col) )
          {
            this->squares[grid_row][grid_col] =
                this->squares[grid_row][grid_col] bitor forbidden_mask;
          }
        }
      }
      // marking newly allowed squares
      foreach(QPoint pt, corners)
      {
        const int grid_row = row + pt.y();
        const int grid_col = col + pt.x();
        if ( (grid_row >= 0) and
             (grid_col >= 0) and
             (grid_row < static_cast<int>(this->size())) and
             (grid_col < static_cast<int>(this->size())) )
        {
          if ( (not this->allowedPlayer(pid, grid_row, grid_col)) and
               (not this->forbiddenPlayer(pid, grid_row, grid_col)) and
               this->isEmpty(grid_row, grid_col) )
          {
            this->squares[grid_row][grid_col] =
                this->squares[grid_row][grid_col] bitor allowed_mask;
          }
        }
      }
      //
      this->stats_dirty = true;
    } // Grid::place

  } // namespace Game
} // namespace Bloqus
