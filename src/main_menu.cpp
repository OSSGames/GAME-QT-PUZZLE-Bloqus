
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

#include <QtDebug>
#include <QTranslator>
#include <QMessageBox>
#include <QIcon>
#include <QPixmap>
#include <QSettings>
#include <QApplication>

#include "bloqus.h"
#include "main_menu.h"
#include "ui_main_menu.h"
#include "bloqus_gui.h"
#include "game_setup.h"
#include "bloqus_game_game.h"
#include "bloqus_gui_game_view.h"
#include "game_window.h"
#include "gui_setup.h"
#include "bloqus_game_player.h"
#include "game_results.h"

namespace Bloqus
{
  namespace Gui
  {
    Main_Menu::Main_Menu(QWidget* parent):
        QDialog(parent),
        ui(new Ui::Main_Menu),
        trans_en(0),
        trans_fr(0),
        game_game(0),
        game_win(0)
    {
      this->ui->setupUi(this);
      this->setWindowTitle(QString("Bloqus v. %1").arg(BLOQUS_VERSION_STR));
      this->ui->bt_en->setIcon(QPixmap(":/images/us_uk"));
      this->ui->bt_fr->setIcon(QPixmap(":/images/france"));

      Options::init();

      connect(this->ui->bt_en,
              SIGNAL(clicked()),
              this,
              SLOT(languageEn()));

      connect(this->ui->bt_fr,
              SIGNAL(clicked()),
              this,
              SLOT(languageFr()));

      connect(this->ui->bt_new_game,
              SIGNAL(clicked()),
              this,
              SLOT(newGame()));

      connect(this->ui->bt_options,
              SIGNAL(clicked()),
              this,
              SLOT(options()));

      connect(this->ui->bt_about,
              SIGNAL(clicked()),
              this,
              SLOT(about()));

      QString lang = Options::settings().value("language", "en").toString();
      if ( lang == QString("en") )
      {
        languageEn();
      }
      else if ( lang == QString("fr") )
      {
        languageFr();
      }
    }

    // =====================================================================

    bool Main_Menu::event(QEvent* qe)
    {
      if ( qe->type() == QEvent::LanguageChange )
      {
        this->ui->retranslateUi(this);
        qe->accept();
        return true;
      }
      else
      {
        return QDialog::event(qe);
      }
    }

    // =====================================================================

    void Main_Menu::languageEn()
    {
      if ( this->trans_fr != 0 )
      {
        QCoreApplication::removeTranslator(this->trans_fr);
        delete this->trans_fr;
        this->trans_fr = 0;
      }
      if ( this->trans_en == 0 )
      {
        trans_en = new QTranslator(qApp);
        trans_en->load(":/i18n/bloqus_en");
        QCoreApplication::installTranslator(this->trans_en);
      }
      Options::settings().setValue("language", "en");
      Options::settings().sync();
    }

    // =====================================================================

    void Main_Menu::languageFr()
    {
      if ( this->trans_en != 0 )
      {
        QCoreApplication::removeTranslator(this->trans_en);
        delete this->trans_en;
        this->trans_en = 0;
      }
      if ( this->trans_fr == 0 )
      {
        trans_fr = new QTranslator(qApp);
        trans_fr->load(":/i18n/bloqus_fr");
        QCoreApplication::installTranslator(this->trans_fr);
      }
      Options::settings().setValue("language", "fr");
      Options::settings().sync();
    }

    // =====================================================================

    void Main_Menu::newGame()
    {
      if ( this->game_win != 0 )
      {
        this->game_win->hide();
        this->game_win->deleteLater();
        this->game_win = 0;
      }
      if ( this->game_game != 0 )
      {
        this->game_game->deleteLater();
        this->game_game = 0;
      }
      Game_Setup gs(this);
      if ( gs.exec() == QDialog::Accepted )
      {
        this->game_game = new Game::Game(this);
        connect(this->game_game,
                SIGNAL(gameEnds()),
                this,
                SLOT(gameEnds()));
        connect(this->game_game,
                SIGNAL(gameStopped()),
                this,
                SLOT(gameStopped()));
        this->game_game->initialize(20, 4);
        for(int ind_player = 0; ind_player < 4; ++ind_player)
        {
          Game::Player_Id pid = Game::indexToPlayerId(ind_player);
          if ( gs.isHumanPlayer(pid) )
          {
            this->game_game->createHumanPlayer(
                pid,
                gs.playerName(pid),
                gs.allowedTime());
          }
          else if ( gs.isComputerPlayer(pid) )
          {
            this->game_game->createComputerPlayer(
                pid,
                gs.playerName(pid),
                gs.computerStrength(pid),
                gs.allowedTime());
          }
        }
        this->game_win = new Gui::Game_Window(0);
        this->game_win->setup(this->game_game);
        this->game_win->showMaximized();
        this->game_game->startGame();
      }
    }

    // =====================================================================

    namespace
    {
      bool cmp_players(Game::Player* p1, Game::Player* p2)
      {
        return p1->points() < p2->points();
      }
    }
    void Main_Menu::gameEnds()
    {
      Game::Player* players[4] = {0, 0, 0, 0};
      for(int ind_player = 0; ind_player < 4; ++ind_player)
      {
        Game::Player_Id pid = Game::indexToPlayerId(ind_player);
        players[ind_player] = this->game_game->player(pid);
      }
      std::sort(&(players[0]), &(players[0])+4, cmp_players);
      Game_Results* gr = new Game_Results(this);
      gr->setStyleSheet(Defaults::defaultStyleSheet());
      gr->setWinner(players[3]);
      gr->setChallenger(players[2]);
      gr->setThird(players[1]);
      gr->setLooser(players[0]);
      gr->setGameWindow(this->game_win);
      int result = gr->exec();
      delete gr;
      gr = 0;
      if ( result == QDialog::Accepted )
      {
        this->newGame();
      }
      else
      {
        qApp->quit();
      }
    }

    // =====================================================================

    void Main_Menu::gameStopped()
    {
      this->game_win->deleteLater();
      this->game_win = 0;
      this->game_game->deleteLater();
      this->game_game = 0;
    }

    // =====================================================================

    void Main_Menu::options()
    {
      Gui_Setup gs(this);
      gs.show();
      gs.exec();
    }

    // =====================================================================

    void Main_Menu::about()
    {
      QMessageBox msg(
          QMessageBox::NoIcon,
          trUtf8("About Bloqus"),
          trUtf8(
              "<table border=0 width=100%>\n"
              "  <tr>\n"
              "    <td width=20%>\n"
              "       <p align=\"center\"><img src=\":/images/bloqus_icon_64\"/></p>"
              "    </td>\n"
              "    <td>\n"
              "       <p align=\"center\"><b><big>Bloqus</big></b></p>\n"
              "       <p align=\"center\"><i>v. %1</i></p>\n"
              "    </td>\n"
              "    <td width=20%>\n"
              "       <p align=\"center\"><img src=\":/images/bloqus_icon_64\"/></p>"
              "    </td>\n"
              "  </tr>\n"
              "</table>\n"
              "<p align=\"center\">&copy; Yves Bailly 2009</p>\n"
              "<p align=\"center\"><tt>yves.bailly@laposte.net</tt></p>\n"
              "<p align=\"center\">Inspired by <b>Blokus&reg;</b>: "
              "<a href=\"http://www.blokus.com/\">"
              "http://www.blokus.com/</a></p>\n"
              "<p>The author is not related in any way with "
              "<a href=\"http://www.blokus.com/en/equipe.html?pays=us\">"
              "Sekkoïa</a>, the editor of the official Blokus&reg;.</p>\n"
              "<p>Distributed under the <a href=\"http://www.cecill.info\">"
              "CeCILL</a> licence.</p>",
              "'%1' is the version number (such as '0.8')")
            .arg(BLOQUS_VERSION_STR),
          QMessageBox::Ok,
          this);
      msg.exec();
    }

  } // namespace Gui
} // namespace Bloqus
