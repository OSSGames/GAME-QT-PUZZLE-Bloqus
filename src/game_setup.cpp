
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

#include <QSettings>

#include "bloqus.h"
#include "game_setup.h"
#include "ui_game_setup.h"
#include "bloqus_gui.h"
#include "player_setup.h"

namespace Bloqus
{
  namespace Gui
  {

    Game_Setup::Game_Setup(QWidget* parent):
        QDialog(parent),
        ui(new Ui::Game_Setup)
    {
      ui->setupUi(this);
      ui->north_west_setup->setPlayer(Game::North_West_Player);
      ui->north_east_setup->setPlayer(Game::North_East_Player);
      ui->south_west_setup->setPlayer(Game::South_West_Player);
      ui->south_east_setup->setPlayer(Game::South_East_Player);

      connect(this->ui->allowed_time,
              SIGNAL(valueChanged(int)),
              this,
              SLOT(changedAllowedTime(int)));
      this->ui->allowed_time->setValue(10);
      this->changedAllowedTime(10);
    }

    // =====================================================================

    void Game_Setup::accept()
    {
      Options::settings().setValue(
          "game_setup/north_west_name",
          this->playerName(Game::North_West_Player));
      Options::settings().setValue(
          "game_setup/south_west_name",
          this->playerName(Game::South_West_Player));
      Options::settings().setValue(
          "game_setup/north_east_name",
          this->playerName(Game::North_East_Player));
      Options::settings().setValue(
          "game_setup/south_east_name",
          this->playerName(Game::South_East_Player));

      Options::settings().setValue(
          "game_setup/north_west_is_human",
          this->isHumanPlayer(Game::North_West_Player));
      Options::settings().setValue(
          "game_setup/south_west_is_human",
          this->isHumanPlayer(Game::South_West_Player));
      Options::settings().setValue(
          "game_setup/north_east_is_human",
          this->isHumanPlayer(Game::North_East_Player));
      Options::settings().setValue(
          "game_setup/south_east_is_human",
          this->isHumanPlayer(Game::South_East_Player));

      Options::settings().setValue(
          "game_setup/north_west_is_computer",
          this->isComputerPlayer(Game::North_West_Player));
      Options::settings().setValue(
          "game_setup/south_west_is_computer",
          this->isComputerPlayer(Game::South_West_Player));
      Options::settings().setValue(
          "game_setup/north_east_is_computer",
          this->isComputerPlayer(Game::North_East_Player));
      Options::settings().setValue(
          "game_setup/south_east_is_computer",
          this->isComputerPlayer(Game::South_East_Player));

      Options::settings().setValue(
          "game_setup/north_west_is_network",
          this->isNetworkPlayer(Game::North_West_Player));
      Options::settings().setValue(
          "game_setup/south_west_is_network",
          this->isNetworkPlayer(Game::South_West_Player));
      Options::settings().setValue(
          "game_setup/north_east_is_network",
          this->isNetworkPlayer(Game::North_East_Player));
      Options::settings().setValue(
          "game_setup/south_east_is_network",
          this->isNetworkPlayer(Game::South_East_Player));

      Options::settings().setValue(
          "game_setup/north_west_computer_strength",
          this->computerStrength(Game::North_West_Player));
      Options::settings().setValue(
          "game_setup/north_east_computer_strength",
          this->computerStrength(Game::North_East_Player));
      Options::settings().setValue(
          "game_setup/south_west_computer_strength",
          this->computerStrength(Game::South_West_Player));
      Options::settings().setValue(
          "game_setup/south_east_computer_strength",
          this->computerStrength(Game::South_East_Player));

      Options::settings().sync();
      QDialog::accept();
    }

    // =====================================================================

    QString Game_Setup::playerName(Game::Player_Id pid) const
    {
      Player_Setup* ps = this->playerSetup(pid);
      if ( ps != 0 )
      {
        return ps->nickname();
      }
      else
      {
        return QString("");
      }
    }

    // =====================================================================

    bool Game_Setup::isHumanPlayer(Game::Player_Id pid) const
    {
      Player_Setup* ps = this->playerSetup(pid);
      if ( ps != 0 )
      {
        return ps->isHuman();
      }
      else
      {
        return false;
      }
    }

    // =====================================================================

    bool Game_Setup::isComputerPlayer(Game::Player_Id pid) const
    {
      Player_Setup* ps = this->playerSetup(pid);
      if ( ps != 0 )
      {
        return ps->isComputer();
      }
      else
      {
        return false;
      }
    }

    // =====================================================================

    bool Game_Setup::isNetworkPlayer(Game::Player_Id pid) const
    {
      Player_Setup* ps = this->playerSetup(pid);
      if ( ps != 0 )
      {
        return ps->isNetwork();
      }
      else
      {
        return false;
      }
    }

    // =====================================================================

    int Game_Setup::computerStrength(Game::Player_Id pid) const
    {
      Player_Setup* ps = this->playerSetup(pid);
      if ( ps != 0 )
      {
        return ps->strength();
      }
      else
      {
        return -1;
      }
    }

    int Game_Setup::allowedTime()
    {
      return this->ui->allowed_time->value() * 30;
    }

    // =====================================================================

    void Game_Setup::changedAllowedTime(int val)
    {
      int minutes = val/2;
      int sec = ((val % 2) == 0) ? 0 : 30;
      this->ui->lcd_time->display(
          QString("%1:%2")
          .arg((int)minutes, (int)2, (int)10, QChar('0'))
          .arg((int)sec, (int)2, (int)10, QChar('0')));
    }

    // =====================================================================

    Player_Setup* Game_Setup::playerSetup(Game::Player_Id pid) const
    {
      switch ( pid )
      {
        case Game::North_West_Player:
          return this->ui->north_west_setup;
          break;
        case Game::North_East_Player:
          return this->ui->north_east_setup;
          break;
        case Game::South_West_Player:
          return this->ui->south_west_setup;
          break;
        case Game::South_East_Player:
          return this->ui->south_east_setup;
          break;
        default:
          return 0;
      }
    }

  } // namespace Gui
} // namespace Bloqus
