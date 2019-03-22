
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

#ifndef __BLOQUS_GAME_H__
#define __BLOQUS_GAME_H__ 1

#include <QObject>
#include <QPoint>
#include <QEvent>

namespace Bloqus
{
  namespace Game
  {
    const unsigned int MAX_GRID_SIZE = 20;

    class Piece;

    enum Player_Id
    {
      No_Player = 0,
      North_West_Player = 1,
      North_East_Player = 2,
      South_East_Player = 3,
      South_West_Player = 4,
      First_Player = North_West_Player,
      Last_Player = South_West_Player
    }; // enum Player_Id

    Player_Id indexToPlayerId(int idx);
    int playerIdToIndex(Player_Id pid);

    QPoint startingCorner(Player_Id pid);
    QPoint oppositeCorner(Player_Id pid);
    QPoint leftHandCorner(Player_Id pid);
    QPoint rightHandCorner(Player_Id pid);
    QPoint playerDirection(Player_Id pid);

    enum Square_Status
    {
      Contains_North_West  = 0x00000001,
      Contains_North_East  = 0x00000002,
      Contains_South_East  = 0x00000004,
      Contains_South_West  = 0x00000008,
      Mask_Contains        = 0x000000FF,
      Allowed_North_West   = 0x00000100,
      Allowed_North_East   = 0x00000200,
      Allowed_South_East   = 0x00000400,
      Allowed_South_West   = 0x00000800,
      Mask_Allowed         = 0x0000FF00,
      Forbidden_North_West = 0x00010000,
      Forbidden_North_East = 0x00020000,
      Forbidden_South_East = 0x00040000,
      Forbidden_South_West = 0x00080000,
      Mask_Forbidden       = 0x00FF0000
    }; // enum Square_Status

    unsigned playerMask(Player_Id pid);
    unsigned containsPlayerMask(Player_Id pid);
    unsigned allowedPlayerMask(Player_Id pid);
    unsigned forbiddenPlayerMask(Player_Id pid);

    enum Piece_Shape
    {
      No_Shape =  0,
      Mono     =  1,
      Duo      =  2,
      Trio_I   =  3,
      Trio_L   =  4,
      Tetra_I  =  5,
      Tetra_L  =  6,
      Tetra_O  =  7,
      Tetra_S  =  8,
      Tetra_T  =  9,
      Penta_F  = 10,
      Penta_I  = 11,
      Penta_L  = 12,
      Penta_N  = 13,
      Penta_P  = 14,
      Penta_T  = 15,
      Penta_U  = 16,
      Penta_V  = 17,
      Penta_W  = 18,
      Penta_X  = 19,
      Penta_Y  = 20,
      Penta_Z  = 21,
      Unknown_Shape = 22,
      First_Piece_Shape = Mono,
      Last_Piece_Shape = Penta_Z
    }; // enum Piece_Shape

    Piece_Shape indexToPieceShape(int idx);
    int pieceShapeToIndex(Piece_Shape ps);

    bool isPatternChangedByHorizontalFlip(Piece_Shape ps);
    unsigned maxNumberOfTrigoRotations(Piece_Shape ps);

    class Player_Move
    {
      public:
        Player_Move();
        Player_Move(Player_Id pid,
                    Piece_Shape ps,
                    int row,
                    int col);

        Player_Id player() const;
        Piece_Shape pieceShape() const;
        int row() const;
        int col() const;

        bool isValid() const;
        const QString& feedback() const;

        void setValid(bool v);
        void setFeedback(const QString& f);

      public:
        Player_Id the_player;
        Piece_Shape the_shape;
        int the_row;
        int the_col;
        bool is_valid;
        QString move_feedback;
    }; // class Player_Move

    class Computed_Move: public Player_Move
    {
      public:
        Computed_Move();
        Computed_Move(Player_Id pid,
                      Piece_Shape ps,
                      int row,
                      int col,
                      bool horiz_flip,
                      unsigned nb_rot);

        bool withHorizontalFlip() const;
        unsigned nbTrigoRotations() const;

        double value() const;
        void setValue(double v);

        void applyOnPiece(Piece* p) const;

      private:
        bool with_flip;
        unsigned nb_rotations;
        double val;
    }; // class Computed_Move

    const QEvent::Type Player_Suggest_Move_Event_Type =
        static_cast<QEvent::Type>(static_cast<int>(QEvent::User) + 1);
    class Player_Suggest_Move_Event: public QEvent, public Player_Move
    {
      public:
        Player_Suggest_Move_Event(Player_Id pid,
                                  Piece_Shape ps,
                                  int row,
                                  int col);
    }; // class Player_Suggest_Move_Event

    const QEvent::Type Request_Next_Player_Event_Type =
        static_cast<QEvent::Type>(static_cast<int>(QEvent::User) + 2);
    class Request_Next_Player_Event: public QEvent
    {
      public:
        Request_Next_Player_Event();
    }; // class Request_Next_Player_Event

  } // namespace Game
} // namespace Bloqus

#endif // __BLOQUS_GAME_H__
