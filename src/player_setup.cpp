
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
#include "player_setup.h"
#include "ui_player_setup.h"
#include "bloqus_gui.h"

namespace Bloqus
{
  namespace Gui
  {
    Player_Setup::Player_Setup(QWidget* parent):
        QGroupBox(parent),
        ui(new Ui::Player_Setup),
        player_id(Game::No_Player)
    {
      ui->setupUi(this);
    }

    // =====================================================================

    void Player_Setup::setPlayer(Game::Player_Id pid)
    {
      if ( pid == this->player_id )
      {
        return;
      }
      this->player_id = pid;
      this->setTitle(Defaults::playerName(this->player_id));
      QString css = this->styleSheet();
      const QColor grad_start = Defaults::playerGuiGradientStart(pid);
      const QColor grad_end = Defaults::playerGuiGradientEnd(pid);
      css.append(QString(
          "\nQGroupBox {\n"
          "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
          "stop: 0 %1, "
          "stop: 1 %2);\n"
          "}")
          .arg(colorToCssRgba(grad_start))
          .arg(colorToCssRgba(grad_end)));
      this->setStyleSheet(css);
      const QString setting_prefix("game_setup/");
      this->ui->player_name->setText(
          Options::settings().value(
              setting_prefix+Defaults::playerPropertyPrefix(pid)+"_name",
              Defaults::playerName(pid)).toString());

      connect(this->ui->player_is_human,
              SIGNAL(clicked()),
              this,
              SLOT(playerTypeChanged()));
      connect(this->ui->player_is_computer,
              SIGNAL(clicked()),
              this,
              SLOT(playerTypeChanged()));
/*
      connect(this->ui->player_is_network,
              SIGNAL(clicked()),
              this,
              SLOT(playerTypeChanged()));
*/
      connect(this->ui->player_strength,
              SIGNAL(valueChanged(int)),
              this,
              SLOT(strengthChanged(int)));

      bool is_human =
          Options::settings().value(
              setting_prefix+Defaults::playerPropertyPrefix(pid)+"_is_human",
              true).toBool();
      const bool is_computer =
          Options::settings().value(
              setting_prefix+Defaults::playerPropertyPrefix(pid)+"_is_computer",
              false).toBool();
      const bool is_network =
          Options::settings().value(
              setting_prefix+Defaults::playerPropertyPrefix(pid)+"_is_network",
              false).toBool();
      if ( not (is_human or is_computer or is_network) )
      {
        is_human = true;
      }
      if ( is_human )
      {
        this->ui->player_is_human->setChecked(true);
        this->ui->player_is_computer->setChecked(false);
//        this->ui->player_is_network->setChecked(false);
        this->ui->player_is_human->click();
      }
      else if ( is_computer )
      {
        this->ui->player_is_computer->setChecked(true);
        this->ui->player_is_human->setChecked(false);
//        this->ui->player_is_network->setChecked(false);
        this->ui->player_is_computer->click();
      }
      else
      {
//        this->ui->player_is_network->setChecked(true);
        this->ui->player_is_computer->setChecked(false);
        this->ui->player_is_human->setChecked(false);
//        this->ui->player_is_network->click();
      }

      this->ui->player_strength->setValue(
          Options::settings().value(
              setting_prefix+Defaults::playerPropertyPrefix(pid)+"_computer_strength",
              0).toInt());
    }

    // =====================================================================

    Game::Player_Id Player_Setup::player() const
    {
      return this->player_id;
    }

    // =====================================================================

    QString Player_Setup::nickname() const
    {
      return this->ui->player_name->text();
    }

    // =====================================================================

    bool Player_Setup::isHuman() const
    {
      return this->ui->player_is_human->isChecked();
    }

    // =====================================================================

    bool Player_Setup::isComputer() const
    {
      return this->ui->player_is_computer->isChecked();
    }

    // =====================================================================

    bool Player_Setup::isNetwork() const
    {
      return false;
 //     return this->ui->player_is_network->isChecked();
    }

    // =====================================================================

    int Player_Setup::strength() const
    {
      return this->ui->player_strength->value();
    }

    // =====================================================================

    void Player_Setup::playerTypeChanged()
    {
      if ( this->isHuman() )
      {
        this->ui->player_sub_setup->setCurrentIndex(0);
      }
      else if ( this->isComputer() )
      {
        this->ui->player_sub_setup->setCurrentIndex(1);
      }
      else if ( this->isNetwork() )
      {
        this->ui->player_sub_setup->setCurrentIndex(2);
      }
    }

    // =====================================================================

    void Player_Setup::strengthChanged(int val)
    {
      this->ui->player_strength_name->setText(
          Defaults::strengthName(val));
    }

  } // namespace Gui
} // namespace Bloqus

