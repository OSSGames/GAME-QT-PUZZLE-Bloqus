
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

#ifndef __BLOQUS_H__
#define __BLOQUS_H__

#include <QtDebug>

#include <QObject>
#include <QColor>
#include <QBrush>
#include <QPainterPath>

#ifdef Q_CC_MSVC
  #include <ciso646>
#endif // Q_CC_MSVC

#define BLOQUS_VERSION 0x100000
#define BLOQUS_VERSION_STR "1.0"

namespace Bloqus
{
  struct Compass_Dir
  {
    enum Dir_Name
    {
      North        = 0,
      North_East   = 1,
      East         = 2,
      South_East   = 3,
      South        = 4,
      South_West   = 5,
      West         = 6,
      North_West   = 7,
      No_Direction = 8
    };
    Dir_Name dir;
    Compass_Dir();
    explicit Compass_Dir(Dir_Name d);
    Compass_Dir& operator= (Dir_Name d);
    bool operator== (Dir_Name d) const;
    bool operator== (const Compass_Dir& cd) const;
    bool operator!= (Dir_Name d) const;
  }; // struct Compass_Dir

  struct Compass_Diag_Dir
  {
    enum Dir_Name
    {
      North_East    = 0,
      South_East    = 1,
      South_West    = 2,
      North_West    = 3,
      No_Direction  = 4,
    };
    Dir_Name dir;
    Compass_Diag_Dir();
    explicit Compass_Diag_Dir(Dir_Name d);
    Compass_Diag_Dir& operator= (Dir_Name d);
    bool operator== (Dir_Name d) const;
    bool operator== (const Compass_Diag_Dir& cd) const;
    bool operator!= (Dir_Name d) const;
  }; // struct Compass_Diag_Dir

  struct Compass_Axis_Dir
  {
    enum Dir_Name
    {
      North         = 0,
      East          = 1,
      South         = 2,
      West          = 3,
      No_Direction  = 4,
    };
    Dir_Name dir;
    Compass_Axis_Dir();
    explicit Compass_Axis_Dir(Dir_Name d);
    Compass_Axis_Dir& operator= (Dir_Name d);
    bool operator== (Dir_Name d) const;
    bool operator== (const Compass_Axis_Dir& cd) const;
    bool operator!= (Dir_Name d) const;
  }; // struct Compass_Axis_Dir

  template<typename DIR_TYPE>
  DIR_TYPE Next_Direction(DIR_TYPE d, bool cycle = false);

  template<typename DIR_TYPE>
  DIR_TYPE Previous_Direction(DIR_TYPE d, bool cycle = false);

  template<typename DIR_TYPE>
  QPoint Translate(DIR_TYPE d);

  template<typename DIR_TYPE>
  QPoint Translate(const QPoint& p,
                   DIR_TYPE d);

  template<typename DIR_TYPE>
  DIR_TYPE Opposite(DIR_TYPE d);

  template<typename DIR_TYPE>
  QPoint Top_Right_Corner(DIR_TYPE d);

  template<typename DIR_TYPE>
  QPoint Top_Right_Corner(const QPoint& p,
                          DIR_TYPE d);

  enum Rotation_Direction
  {
    Rotate_Clockwise,
    Rotate_Trigo
  }; // enum Rotation_Direction

  QString colorToCssRgba(QColor c);

  // =======================================================================

  template<typename DIR_TYPE>
  DIR_TYPE Next_Direction(DIR_TYPE d, bool cycle)
  {
    if ( cycle )
    {
      int next = static_cast<int>(d.dir) + 1;
      if ( next >= static_cast<int>(DIR_TYPE::No_Direction) )
      {
        next = 0;
      }
      return DIR_TYPE(static_cast<typename DIR_TYPE::Dir_Name>(next));
    }
    else
    {
      if ( d.dir != DIR_TYPE::No_Direction )
      {
        return DIR_TYPE(static_cast<typename DIR_TYPE::Dir_Name>(
            static_cast<int>(d.dir)+1));
      }
      else
      {
        return d;
      }
    }
  }

  template<typename DIR_TYPE>
  DIR_TYPE Previous_Direction(DIR_TYPE d, bool cycle)
  {
    if ( cycle )
    {
      int prev = static_cast<int>(d.dir) - 1;
      if ( prev < 0 )
      {
        prev = static_cast<int>(DIR_TYPE::No_Direction) - 1;
      }
      return DIR_TYPE(static_cast<typename DIR_TYPE::Dir_Name>(prev));
    }
    else
    {
      if ( d.dir != static_cast<typename DIR_TYPE::Dir_Name>(0) )
      {
        return DIR_TYPE(static_cast<typename DIR_TYPE::Dir_Name>(
            static_cast<int>(d.dir)-1));
      }
      else
      {
        return d;
      }
    }
  }

  template<typename DIR_TYPE>
  QPoint Translate(DIR_TYPE d)
  {
    return QPoint(0, 0);
  }

  template<typename DIR_TYPE>
  QPoint Translate(const QPoint& p,
                   DIR_TYPE d)
  {
    QPoint offset = Translate(d);
    return p + offset;
  }

  template<typename DIR_TYPE>
  DIR_TYPE Opposite(DIR_TYPE d)
  {
    if ( d.dir == DIR_TYPE::No_Direction )
    {
      return d;
    }
    else
    {
      const int val = static_cast<int>(d.dir);
      const int nb = static_cast<int>(DIR_TYPE::No_Direction);
      return DIR_TYPE(static_cast<typename DIR_TYPE::Dir_Name>((val + (nb/2)) % nb));
    }
  }

  template<typename DIR_TYPE>
  QPoint Top_Right_Corner(DIR_TYPE d)
  {
    return QPoint(0, 0);
  }

  template<typename DIR_TYPE>
  QPoint Top_Right_Corner(const QPoint& p,
                          DIR_TYPE d)
  {
    QPoint offset = Top_Right_Corner(d);
    return p + offset;
  }

  QPoint Translate(Compass_Dir d);
  QPoint Translate(Compass_Diag_Dir d);
  QPoint Translate(Compass_Axis_Dir d);
  QPoint Top_Right_Corner(Compass_Axis_Dir d);

} // namespace Bloqus

#endif // __BLOQUS_H__
