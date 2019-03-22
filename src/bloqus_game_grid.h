
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

#ifndef __BLOQUS_GAME_GRID_H__
#define __BLOQUS_GAME_GRID_H__ 1

#include <QObject>
#include <QPoint>
#include <QVarLengthArray>
#include <QVector>

#include "bloqus_game.h"

namespace Bloqus
{
  namespace Game
  {
    class Piece;
    class Game;
    class Grid;

    class Player_Stats
    {
      friend class Grid;
      public:
        Player_Stats();

        void clear();
        const QVector<QPoint>& usedSquares() const;
        const QVector<QPoint>& allowedSquares() const;
        const QVector<QPoint>& forbiddenSquares() const;

      private:
        QVector<QPoint> used_squares;
        QVector<QPoint> allowed_squares;
        QVector<QPoint> forbidden_squares;
    }; // class Player_Stats

    class Grid_Stats
    {
      friend class Grid;
      public:
        Grid_Stats();

        void clear();
        const QVector<QPoint>& emptySquares() const;
        const Player_Stats& playerStats(Player_Id pid) const;

      private:
        QVector<QPoint> empty_squares;
        Player_Stats players[4];
    }; // class Grid_Stats

    class Grid: public QObject
    {
      Q_OBJECT
      public:
        explicit Grid(QObject* parent);

        Game* game() const;

        void initialize(const unsigned sz);
        void cloneFrom(const Grid& other);
        unsigned size() const;

        quint32 status(const unsigned row,
                       const unsigned col) const;

        bool containsPlayer(Player_Id pid,
                            const unsigned row,
                            const unsigned col) const;

        bool allowedPlayer(Player_Id pid,
                           const unsigned row,
                           const unsigned col) const;

        bool forbiddenPlayer(Player_Id pid,
                             const unsigned row,
                             const unsigned col) const;

        bool isEmpty(const unsigned row,
                     const unsigned col) const;

        const Grid_Stats& getStats() const;

        QVector<QPoint> allowedSquares(Player_Id pid) const;

        bool canPlace(Player_Id pid,
                      const Piece* piece,
                      const unsigned row,
                      const unsigned col,
                      QString* feedback = 0) const;

        void checkPlayerMove(Player_Move& pm) const;

        void findPlace(Player_Id,
                       Piece* piece,
                       QPoint& where) const;

        void findPlace(Player_Id pid,
                       Piece* piece,
                       const QVector<QPoint>& from,
                       QPoint& where) const;

        int countAllMoves(Player_Id pid,
                          const QVector<Piece*>& pieces) const;

        void findAllMoves(Player_Id pid,
                          const QVector<Piece*>& pieces,
                          QVector<Computed_Move>& to) const;
        QVector<Computed_Move> findAllMoves(
            Player_Id pid,
            const QVector<Piece*>& pieces) const;

        bool canPlay(Player_Id pid) const;

        void place(Player_Id pid,
                   const Piece* const piece,
                   const unsigned row,
                   const unsigned col);

      private:
        QVarLengthArray<QVarLengthArray<quint32, MAX_GRID_SIZE>, MAX_GRID_SIZE>
            squares;
        unsigned actual_size;
        mutable Grid_Stats stats;
        mutable bool stats_dirty;

    }; // class Grid

  } // namespace Game
} // namespace Bloqus

#endif // __BLOQUS_GAME_GRID_H__
