
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
#include <cmath>

#include <QtDebug>

#include "bloqus_game_computer_player_strength_4.h"
#include "bloqus.h"
#include "bloqus_game.h"
#include "bloqus_game_game.h"
#include "bloqus_game_grid.h"
#include "bloqus_game_piece.h"

namespace Bloqus
{
  namespace Game
  {
    Computer_Player_Strength_4::Computer_Player_Strength_4(QObject* parent):
        Computer_Player(parent),
        target(0.0, 0.0),
        nb_rounds(0),
        current_nb_moves(0)
    {
    }

    // =====================================================================

    namespace
    {
      double distance_to_target(const Piece& p,
                                Player_Id pid,
                                const QPoint& from,
                                const QPointF& target)
      {
        const QPoint dir = playerDirection(pid);
        const QPoint piece_corner((dir.x() < 0) ? 0 : p.nbCols()-1,
                                  (dir.y() < 0) ? 0 : p.nbRows()-1);
        QPoint corner_square;
        const int diag = std::min(p.nbCols(), p.nbRows());
        for(int k = 0; k < diag; ++k)
        {
          corner_square.rx() = piece_corner.x() - k*dir.x();
          corner_square.ry() = piece_corner.y() - k*dir.y();
          if ( p.squareFilled(corner_square) )
          {
            break;
          }
        }
        const QPointF corner(from.x()+corner_square.x(),
                             from.y()+corner_square.y());
        const QPointF vec = target - corner;
        const double d = std::sqrt(vec.x()*vec.x()+vec.y()*vec.y());
        return d;
      }
    }

    double Computer_Player_Strength_4::evaluateMove(Grid& working_grid,
                                                    Piece& working_piece,
                                                    const Computed_Move& cm) const
    {
      working_grid.place(this->id(),
                         &working_piece,
                         cm.row(),
                         cm.col());
      int new_nb_moves =
        working_grid.countAllMoves(this->id(),
                                   this->availablePieces());
      const int delta = new_nb_moves - this->current_nb_moves;
      if ( this->nb_rounds < 4 )
      {
        const double d = distance_to_target(working_piece,
                                            this->id(),
                                            QPoint(cm.col(), cm.row()),
                                            this->target);
        return static_cast<double>(working_piece.value()) +
            0.01*static_cast<double>(delta) - d;
      }
      else if ( this->nb_rounds < 8 )
      {
        return static_cast<double>(working_piece.value()) +
            0.5*static_cast<double>(delta);
      }
      else
      {
        return static_cast<double>(working_piece.value()) +
            0.05*static_cast<double>(delta);
      }
    }

    // =====================================================================

    Computed_Move Computer_Player_Strength_4::findMove()
    {
      const Grid& grid = this->game()->grid();
      QVector<Computed_Move> moves;
      grid.findAllMoves(this->id(),
                        this->availablePieces(),
                        moves);
      if ( moves.empty() )
      {
        Computed_Move cm(No_Player, No_Shape, -1, -1, false, 0);
        cm.setValid(false);
        return cm;
      }
      this->current_nb_moves = moves.size();
      this->evaluatesAndSortMoves(moves);
      QVector<Computed_Move>::iterator move_iter = moves.begin();
      ++move_iter;
      while ( (move_iter != moves.end()) and
              (std::fabs(move_iter->value()-moves.begin()->value()) < 0.25) )
      {
        ++move_iter;
      }
      std::random_shuffle(moves.begin(), move_iter);
      const Computed_Move& cm = *(moves.begin());
      ++(this->nb_rounds);
      return cm;
    }

    // =====================================================================

    void Computer_Player_Strength_4::initialize(Game* g,
                                                Player_Id pid,
                                                const QString& p,
                                                const unsigned max_time)
    {
      this->Computer_Player::initialize(g, pid, p, max_time);
      this->target = oppositeCorner(this->id());
    }

  } // namespace Game
} // namespace Bloqus
