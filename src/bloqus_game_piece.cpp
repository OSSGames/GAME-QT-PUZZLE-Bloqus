
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

#include "bloqus.h"
#include "bloqus_game_piece.h"
#include "bloqus_game_player.h"

namespace Bloqus
{
  namespace Game
  {

    // static
    Piece::Squares_Map_Cache* Piece::filled_cache = 0;
    // static
    Piece::Squares_Map_Cache* Piece::adjacent_cache = 0;
    // static
    Piece::Squares_Map_Cache* Piece::corners_cache = 0;

    // =====================================================================

    Piece::Piece(QObject* parent,
                 Piece_Shape ps,
                 const char* m):
        QObject(parent),
        shape(ps),
        nb_rows(0),
        nb_cols(0),
        squares(),
        val(0),
        is_available(true)
    {
      if ( m[0] != '\0' )
      {
        this->nb_rows = 1;
        unsigned m_pos = 0;
        while ( m[m_pos] != '\0' )
        {
          if ( m[m_pos] == '\n' )
          {
            if ( m[m_pos+1] != '\0' )
            {
              ++this->nb_rows;
            }
          }
          else
          {
            if ( this->nb_rows == 1 )
            {
              ++this->nb_cols;
            }
            if ( QChar(m[m_pos]).isSpace() )
            {
              this->squares.append(' ');
            }
            else
            {
              this->squares.append('@');
              ++(this->val);
            }
          }
          ++m_pos;
        }
      }
      this->storeDefault();
      // filling static special squares
      QVector<QPoint> filled;
      QVector<QPoint> adjacent;
      QVector<QPoint> corners;
      this->getSquares(filled, adjacent, corners);
    }

    // =====================================================================

    void Piece::cloneFrom(const Piece& other)
    {
      this->shape = other.shape;
      this->nb_rows = other.nb_rows;
      this->nb_cols = other.nb_cols;
      this->squares = other.squares;
    }

    // =====================================================================

    Piece_Shape Piece::pieceShape() const
    {
      return this->shape;
    }

    // =====================================================================

    QByteArray Piece::pattern() const
    {
      QByteArray pat = this->squares;
      pat.append('0'+this->nbRows());
      pat.append('0'+this->nbCols());
      return pat;
    }

    // =====================================================================

    int Piece::nbRows() const
    {
      return this->nb_rows;
    }

    // =====================================================================

    int Piece::nbCols() const
    {
      return this->nb_cols;
    }

    // =====================================================================

    unsigned Piece::value() const
    {
      return this->val;
    }

    // =====================================================================

    bool Piece::contains(int row,
                         int col) const
    {
      if ( row < 0 )
        return false;
      if ( col < 0 )
        return false;
      if ( row >= this->nbRows() )
        return false;
      if ( col >= this->nbCols() )
        return false;
      return true;
    }

    // =====================================================================

    bool Piece::contains(const QPoint& p) const
    {
      return this->contains(p.y(), p.x());
    }

    // =====================================================================

    bool Piece::squareFilled(int pos) const
    {
      return (this->squares.at(pos) != ' ');
    }

    // =====================================================================

    bool Piece::squareFilled(const QPoint& p) const
    {
      return this->squareFilled(p.y(), p.x());
    }

    // =====================================================================

    bool Piece::squareFilled(int row,
                             int col) const
    {
      return squareFilled(row*this->nb_cols+col);
    }

    // =====================================================================

    void Piece::getSquares(QVector<QPoint>& filled,
                           QVector<QPoint>& adjacent,
                           QVector<QPoint>& corners) const
    {
      const QByteArray patt = this->pattern();

      if ( Piece::filled_cache == 0 )
      {
        Piece::filled_cache = new Piece::Squares_Map_Cache;
      }
      if ( Piece::adjacent_cache == 0 )
      {
        Piece::adjacent_cache = new Piece::Squares_Map_Cache;
      }
      if ( Piece::corners_cache == 0 )
      {
        Piece::corners_cache = new Piece::Squares_Map_Cache;
      }

      Piece::Squares_Map_Cache::const_iterator filled_iter =
          Piece::filled_cache->find(patt);
      if ( filled_iter == Piece::filled_cache->end() )
      {
        adjacent.reserve(this->nbCols()+this->nbRows());
        filled.reserve((this->nbCols()+this->nbRows())/2);
        corners.reserve(4);
        for(int r = 0; r < this->nbRows(); ++r)
        {
          for(int c = 0; c < this->nbCols(); ++c)
          {
            if ( this->squareFilled(r, c) )
            {
              const QPoint pt(c, r);
              filled.append(pt);
              Compass_Dir dir(Compass_Dir::North_West);
              do
              {
                const Compass_Dir prev_dir = Previous_Direction(dir, true);
                const Compass_Dir next_dir = Next_Direction(dir, true);
                const QPoint prev_pt = Translate(pt, prev_dir);
                const QPoint next_pt = Translate(pt, next_dir);
                bool adjacent_empty = true;
                if ( this->contains(prev_pt) )
                {
                  if ( this->squareFilled(prev_pt) )
                  {
                    adjacent_empty = false;
                  }
                  else
                  {
                    if ( adjacent.indexOf(prev_pt) < 0 )
                      adjacent.append(prev_pt);
                  }
                }
                else
                {
                  if ( adjacent.indexOf(prev_pt) < 0 )
                    adjacent.append(prev_pt);
                }
                if ( this->contains(next_pt) )
                {
                  adjacent_empty and_eq (not this->squareFilled(next_pt));
                }
                if ( adjacent_empty )
                {
                  const QPoint corner_pt = Translate(pt, dir);
                  if ( corners.indexOf(corner_pt) < 0 )
                    corners.append(corner_pt);
                }
                dir = Next_Direction(next_dir, true);
              }
              while ( dir != Compass_Dir::North_West);
            }
          }
        }
        Piece::filled_cache->insert(patt, filled);
        Piece::adjacent_cache->insert(patt, adjacent);
        Piece::corners_cache->insert(patt, corners);
      }
      else
      {
        filled = filled_iter.value();
        adjacent = Piece::adjacent_cache->value(patt);
        corners = Piece::corners_cache->value(patt);
      }
    }

    // =====================================================================

    void Piece::storeDefault()
    {
      this->default_nb_cols = this->nb_cols;
      this->default_nb_rows = this->nb_rows;
      this->default_squares = this->squares;
    }

    // =====================================================================

    void Piece::flip(Qt::Orientation orient)
    {
      switch ( orient )
      {
        case Qt::Horizontal:
          {
            for(int row = 0; row < this->nb_rows; ++row)
            {
              for(int col = 0; col < this->nb_cols/2; ++col)
              {
                std::swap(this->squares.data()[row*this->nb_cols+col],
                          this->squares.data()[row*this->nb_cols+(this->nb_cols-col-1)]);
              }
            }
          }
          break;
        case Qt::Vertical:
          {
            for(int col = 0; col < this->nb_cols; ++col)
            {
              for(int row = 0; row < this->nb_rows/2; ++row)
              {
                std::swap(this->squares.data()[row*this->nb_cols+col],
                          this->squares.data()[(this->nb_rows-row-1)*this->nb_cols+col]);
              }
            }
          }
          break;
      }
    }

    // =====================================================================

    void Piece::rotate(Rotation_Direction dir)
    {
      QByteArray tmp;
      tmp.resize(this->squares.size());
      switch ( dir )
      {
        case Rotate_Clockwise:
          {
            for(int row = 0; row < this->nb_rows; ++row)
            {
              for(int col = 0; col < this->nb_cols; ++col)
              {
                tmp[col*this->nb_rows+(this->nb_rows-row-1)] =
                  this->squares[row*this->nb_cols+col];
              }
            }
          }
          break;
        case Rotate_Trigo:
          {
            for(int row = 0; row < this->nb_rows; ++row)
            {
              for(int col = 0; col < this->nb_cols; ++col)
              {
                tmp[(this->nb_cols-col-1)*this->nb_rows+row] =
                  this->squares[row*this->nb_cols+col];
              }
            }
          }
          break;
      }
      this->squares = tmp;
      std::swap(this->nb_rows, this->nb_cols);
    }

    // =====================================================================

    void Piece::restoreDefault()
    {
      this->nb_rows = this->default_nb_rows;
      this->nb_cols = this->default_nb_cols;
      this->squares = this->default_squares;
    }

    // =====================================================================

    Player* Piece::player() const
    {
      return dynamic_cast<Player*>(this->parent());
    }

    // =====================================================================

    bool Piece::isAvailable() const
    {
      return this->is_available;
    }

    // =====================================================================

    void Piece::setAvailable(bool v)
    {
      this->is_available = v;
    }

    // =====================================================================

    // static
    Piece* Piece::generate(QObject* parent,
                           Piece_Shape ps)
    {
      switch ( ps )
      {
        case Mono:
          // 0
          return new Piece(parent, ps, "@");
          break;
        case Duo:
          // 1
          return new Piece(parent, ps, "@@");
          break;
        case Trio_I:
          // 2
          return new Piece(parent, ps, "@\n"
                                       "@\n"
                                       "@");
          break;
        case Trio_L:
          // 3
          return new Piece(parent, ps, "@@\n"
                                       " @");
          break;
        case Tetra_I:
          // 4
          return new Piece(parent, ps, "@@@@");
          break;
        case Tetra_L:
          // 5
          return new Piece(parent, ps, " @\n"
                                       " @\n"
                                       "@@\n");
          break;
        case Tetra_O:
          // 6
          return new Piece(parent, ps, "@@\n"
                                       "@@");
          break;
        case Tetra_S:
          // 7
          return new Piece(parent, ps, " @@\n"
                                       "@@ ");
          break;
        case Tetra_T:
          // 8
          return new Piece(parent, ps, "@@@\n"
                                       " @ ");
          break;
        case Penta_F:
          // 9
          return new Piece(parent, ps, " @ \n"
                                       "@@@\n"
                                       "@  ");
          break;
        case Penta_I:
          // 10
          return new Piece(parent, ps, "@\n"
                                       "@\n"
                                       "@\n"
                                       "@\n"
                                       "@");
          break;
        case Penta_L:
          // 11
          return new Piece(parent, ps, "@@\n"
                                       "@ \n"
                                       "@ \n"
                                       "@ ");
          break;
        case Penta_N:
          // 12
          return new Piece(parent, ps, " @\n"
                                       "@@\n"
                                       "@ \n"
                                       "@ ");
          break;
        case Penta_P:
          // 13
          return new Piece(parent, ps, "@@@\n"
                                       " @@");
          break;
        case Penta_T:
          // 14
          return new Piece(parent, ps, "@  \n"
                                       "@@@\n"
                                       "@  ");
          break;
        case Penta_U:
          // 15
          return new Piece(parent, ps, "@ @\n"
                                       "@@@");
          break;
        case Penta_V:
          // 16
          return new Piece(parent, ps, "@  \n"
                                       "@  \n"
                                       "@@@");
          break;
        case Penta_W:
          // 17
          return new Piece(parent, ps, " @@\n"
                                       "@@ \n"
                                       "@  ");
          break;
        case Penta_X:
          // 18
          return new Piece(parent, ps, " @ \n"
                                       "@@@\n"
                                       " @ ");
          break;
        case Penta_Y:
          // 19
          return new Piece(parent, ps, "@ \n"
                                       "@ \n"
                                       "@@\n"
                                       "@ ");
          break;
        case Penta_Z:
          // 20
          return new Piece(parent, ps, " @@\n"
                                       " @ \n"
                                       "@@ ");
          break;
        default:
          return new Piece(parent, ps, "@");
          break;
      }
    }

    Piece::Piece(QObject* parent):
      QObject(parent)
    {
      // should never be called
    }

  } // namespace Game
} // namespace Bloqus
