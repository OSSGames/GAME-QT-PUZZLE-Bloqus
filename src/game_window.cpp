
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
#include <QPalette>
#include <QPixmap>
#include <QBrush>
#include <QPainter>
#include <QFileDialog>
#include <QGraphicsSimpleTextItem>

#include "bloqus_game_game.h"
#include "bloqus_game_player.h"
#include "game_window.h"
#include "ui_game_window.h"
#include "bloqus_gui.h"
#include "gui_setup.h"
#include "bloqus_gui_game_scene.h"

namespace Bloqus
{
  namespace Gui
  {
    Game_Window::Game_Window(QWidget* parent):
        QDialog(parent),
        ui(new Ui::Game_Window),
        game_game(0)
    {
      this->ui->setupUi(this);
      QPalette pal;

      pal = this->ui->net_chat->palette();
      pal.setBrush(QPalette::Base,
                   QBrush(QPixmap(":/images/marble_black_128")));
      pal.setBrush(QPalette::Text,
                   QBrush(QPixmap(":/images/marble_white_64")));
      this->ui->net_chat->setPalette(pal);
      this->ui->net_chat->setFont(QFont("FreeSerif", 12));

      pal = this->ui->right_frame->palette();
      pal.setBrush(QPalette::Window,
                   QBrush(QPixmap(":/images/grey_bg")));
      this->ui->right_frame->setPalette(pal);

      connect(this->ui->bt_screenshot,
              SIGNAL(clicked()),
              this,
              SLOT(screenshot()));
      connect(this->ui->bt_options,
              SIGNAL(clicked()),
              this,
              SLOT(options()));
      connect(this->ui->bt_close,
              SIGNAL(clicked()),
              this,
              SLOT(closeWin()));
    }

    // =====================================================================

    void Game_Window::setup(Game::Game* g)
    {
      this->ui->grid_view->setup(g);
      this->game_game = g;
      this->ui->splitter->setSizes(QList<int>() << 1000 << 10);
      connect(g,
              SIGNAL(playerMoveAccepted(const::Bloqus::Game::Player_Move&)),
              this,
              SLOT(playerMoveAccepted(const::Bloqus::Game::Player_Move&)));
    }

    // =====================================================================

    void Game_Window::screenshot()
    {
      const QVector<QPoint>& history = this->game_game->movesHistory();
      QVector<QGraphicsSimpleTextItem*> moves_nums;
      if ( not history.empty() )
      {
        const QFont fnt("DejaVu Serif", 12, QFont::Normal, true);
        const double txt_height = 0.65;
        QRectF br;
        for(int ind_move = 0; ind_move < history.size(); ++ind_move)
        {
          QGraphicsSimpleTextItem* gsti = new QGraphicsSimpleTextItem(
            QString::number(ind_move+1));
          gsti->setZValue(100.0);
          this->ui->grid_view->scene()->addItem(gsti);
          gsti->setFont(fnt);
          br = gsti->boundingRect();
          gsti->scale(((ind_move < 9) ? txt_height/2.0 : txt_height)/br.width(),
                      txt_height/br.height());
          br = gsti->boundingRect();
          gsti->setPos(double(history[ind_move].x()) +
                        ((ind_move < 9) ? 0.32 : 0.16),
                       double(history[ind_move].y()) +
                        0.85 - txt_height);
          moves_nums.push_back(gsti);
        }
      }
      QPixmap img(this->ui->grid_view->size());
      QPainter painter(&img);
      painter.setRenderHints(QPainter::HighQualityAntialiasing bitor
                             QPainter::SmoothPixmapTransform bitor
                             QPainter::TextAntialiasing bitor
                             QPainter::Antialiasing);
      painter.fillRect(img.rect(), QBrush(QPixmap(":/images/yellow_bg")));
      this->ui->grid_view->render(&painter);
      painter.end();
      QString selected_filter;
      QString filename = QFileDialog::getSaveFileName(
          this,
          tr("Saving screenshot..."),
          QDir::homePath(),
          tr("PNG images (*.png);;JPEG images (*.jpg *.jpeg)"),
          &selected_filter);
      if ( not filename.isEmpty() )
      {
        if ( selected_filter.contains("*.png") )
        {
          if ( not filename.endsWith(".png", Qt::CaseInsensitive) )
          {
            filename.append(".png");
          }
          img.save(filename, "PNG", 99);
        }
        else if ( selected_filter.contains("*.jpeg") )
        {
          if ( not (filename.endsWith(".jpeg", Qt::CaseInsensitive) or
                    filename.endsWith(".jpg", Qt::CaseInsensitive)) )
          {
            filename.append(".jpeg");
          }
          img.save(filename, "JPEG", 99);
        }
      }
      foreach(QGraphicsSimpleTextItem* gsti, moves_nums)
      {
        delete gsti;
      }
    }

    // =====================================================================

    void Game_Window::options()
    {
      Gui_Setup gs(this);
      gs.show();
      gs.exec();
      Game_Scene* game_scene =
          dynamic_cast<Game_Scene*>(this->ui->grid_view->scene());
      if ( game_scene != 0 )
      {
        game_scene->setHighlightHovered(Options::highlightHoveredPiece());
      }
    }

    // =====================================================================

    void Game_Window::closeWin()
    {
      this->game_game->stopGame();
      this->close();
    }

    // =====================================================================

    void Game_Window::playerMoveAccepted(const::Bloqus::Game::Player_Move& pm)
    {
      if ( not pm.isValid() )
      {
        return;
      }
      Game::Player* player = this->game_game->player(pm.player());
      QString piece_name = Defaults::shapeName(pm.pieceShape());
      QColor col = Defaults::playerColor(player->id());
      col = col.lighter(125);
      QChar symbol;
      if ( player->isHuman() )
      {
        symbol = QChar(0x26A4);
      }
      else if ( player->isLocal() )
      {
        symbol = QChar(0x2699);
      }
      else
      {
        symbol = QChar(0x21AF);
      }
      QString msg =
          tr("<font color=%5><b>%6</b>&nbsp;%1 plays <i>%2</i> at (%3, %4)</font>",
             "'%1' is the player's name, "
             "'%2' is the piece's name (translated already), "
             "'%3' is the row, '%4' is the column, "
             "'%5' is the color, don't care about it, "
             "'%6' is a symbol, don't care about it.")
          .arg(player->nickname())
          .arg(piece_name)
          .arg(pm.row()+1)
          .arg(pm.col()+1)
          .arg(col.name())
          .arg(symbol);
      this->ui->net_chat->append(msg);
    }

} // namespace Gui
} // namespace Bloqus

