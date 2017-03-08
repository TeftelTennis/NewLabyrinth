#include "joinserver.h"
#include "ui_joinserver.h"

JoinServer::JoinServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinServer)
{
    ui->setupUi(this);
}

JoinServer::~JoinServer()
{
    delete ui;
}

void JoinServer::on_returnButton_clicked()
{
    hide();
    parentWidget()->show();
}

void JoinServer::on_connectButton_clicked()
{
    GameWindow *game = new GameWindow(this);
    try {
        // game->getParamsFromServer() -- берем инфу с сервака по айпишнику
        // game->setPlayerCoors() -- как-то так наверное
        game->setWindowTitle("Labyrinth");
        game->setModal(true);
        hide();
        game->exec();
        show();
    } catch (...) {
        // если соснули на гетпарамс, то можно показать окошко типа соснули
    }
}
