/**
 * @file controller.cpp
 * @brief Реализация класса Controller для игры "Морской бой".
 * 
 * Этот файл содержит реализацию класса Controller, который обрабатывает пользовательский ввод,
 * управляет игровыми событиями и координирует взаимодействие между Model и View компонентами.
 * Обрабатывает события мыши, управляет звуковыми эффектами и переходами между состояниями игры.
 */

#include "controller.hpp"
#include <QMouseEvent>

/**
 * @brief Конструктор.
 * @param model Указатель на объект модели
 * @param socket Указатель на сокет для сетевого взаимодействия
 * 
 * Инициализирует контроллер с указанными моделью и сокетом,
 * устанавливает начальную громкость и загружает звуковые эффекты.
 */
Controller::Controller(Model* model, QTcpSocket* socket):
    model_(model),
    socket_(socket),
    volume_(50),
    isLoaded_(false)
{
//    backgroundMusicPlayer_ = new QMediaPlayer;
//    backgroundMusicPlayer_->setMedia(QUrl("qrc:/sounds/background.mp3"));
//    backgroundMusicPlayer_->setVolume(volume_);
//    backgroundMusicPlayer_->play();
//    connect(backgroundMusicPlayer_, &QMediaPlayer::mediaStatusChanged, this, &Controller::on_mediaStatusChanged);

    loadSounds();
}

/**
 * @brief Деструктор.
 * 
 * Освобождает ресурсы, занятые звуковыми эффектами.
 */
Controller::~Controller()
{
//    backgroundMusicPlayer_->stop();
//    delete backgroundMusicPlayer_;
}

/**
 * @brief Преобразование координат мыши в координаты поля.
 * @param pos Позиция мыши
 * @param owner Владелец поля (MY_FIELD или ENEMY_FIELD)
 * @return Координаты на поле или (-1, -1) если позиция вне поля
 * 
 * Преобразует координаты мыши в координаты на игровом поле с учетом смещения.
 */
QPoint getFieldCoord(const QPoint& pos, Field::Owner owner)
{
    QPoint res;
    res.setX(-1);
    res.setY(-1);

    int shift_x = 0;
    int shift_y = 0;

    if (owner == Field::MY_FIELD)
    {
        shift_x = MYFIELD_IMG_X;
        shift_y = MYFIELD_IMG_Y;
    }
    else
    {
        shift_x = ENEMYFIELD_IMG_X;
        shift_y = ENEMYFIELD_IMG_Y;
    }

    if(pos.x() < shift_x || pos.x() > (shift_x + FIELD_IMG_WIDTH_DEFAULT ) ||
       pos.y() < shift_y || pos.y() > (shift_y + FIELD_IMG_HEIGHT_DEFAULT)   )
        return res;

    res.setX(1.0*FIELD_WIDTH_DEFAULT *(pos.x()-shift_x)/FIELD_IMG_WIDTH_DEFAULT );
    res.setY(1.0*FIELD_HEIGHT_DEFAULT*(pos.y()-shift_y)/FIELD_IMG_HEIGHT_DEFAULT);

    qDebug() << "X: " << pos.x() - shift_x;
    qDebug() << "Y: " << pos.y() - shift_y;

    return res;
}

/**
 * @brief Обработка нажатия кнопки мыши.
 * @param pos Позиция мыши
 * @param event Событие мыши
 * @param applyIsOkLabel Метка для отображения корректности размещения
 * @param applyIsNotOkLabel Метка для отображения некорректности размещения
 * @param applyFieldButton Кнопка подтверждения размещения
 * 
 * Обрабатывает нажатия кнопок мыши в зависимости от текущего состояния игры:
 * - При размещении кораблей: левая кнопка - разместить корабль, правая - убрать
 * - При ходе: левая кнопка - сделать выстрел, правая - поставить метку
 */
void Controller::onMousePressed(const QPoint& pos, QMouseEvent* event, QLabel* applyIsOkLabel, QLabel* applyIsNotOkLabel, QPushButton* applyFieldButton)
{
    ModelState state = model_->getState();

    if(state == ST_PLACING_SHIPS ||
       state == ST_GAME_NSTARTED   )
//                                 ||
//       state == ST_WAITING_PLACING )
    {
        QPoint point = getFieldCoord(pos, Field::MY_FIELD);

        if( point.x() == -1 || point.y() == -1 )
            return;

        qDebug() << "Ship at" << point.x() << point.y();

        if (event->button() == Qt::LeftButton)
        {
            model_->setMyDrawCell(point.x(), point.y(), CELL_LIVE);
            model_->setMyStateCell(point.x(), point.y(), CL_ST_UNDEFINED);
            qDebug() << "Press on left button -> place a ship";
        }
        if (event->button() == Qt::RightButton)
        {
            model_->setMyDrawCell(point.x(), point.y(), CELL_EMPTY);
            model_->setMyStateCell(point.x(), point.y(), CL_ST_EMPTY);
            qDebug() << "Press on left button -> remove a ship";
        }
        else
            qDebug() << "nothing to do on this button click";

//        emit stateChanged();

//        if (model_->getState() == ST_PLACING_SHIPS)
//        {
            if (model_->isMyFieldCorrect())
            {
                qDebug() << "Placement is correct";
                applyIsOkLabel->setVisible(true);
                applyIsNotOkLabel->setVisible(false);
            }
            else
            {
                qDebug() << "Placement is incorrect";
                applyIsOkLabel->setVisible(false);
                applyIsNotOkLabel->setVisible(true);
                // incorrect placement
            }
//        }

        qDebug() << "field after click: " << model_->getMyFieldStr();
//        applyFieldButton->setStyleSheet("background-color: gray;");

        return;
    }

    if(model_->getState() == ST_MAKING_STEP)
    {
        QPoint point = getFieldCoord(pos, Field::ENEMY_FIELD);
        if(point.x() == -1 || point.y() == -1)
            return;



        if (event->button() == Qt::LeftButton)
        {
            CellDraw cell = model_->getEnemyCell(point.x(), point.y());
            if (cell != CELL_EMPTY && cell != CELL_MARK)
            {
                qDebug() << "Already shot";
                return;
            }

            QString shotMessage = "GAME:" + QString::number(model_->getGameId()) + ":" + model_->getLogin() + ":" + "SHOT:" + QString::number(point.x()) + ":" + QString::number(point.y()); // GAME:<gameId>:<my_login>:SHOT:<x>:<y>

            socket_->write((shotMessage+"@").toUtf8());
            qDebug() << shotMessage;
        }
        else if (event->button() == Qt::RightButton)
        {
            CellDraw cellSt = model_->getEnemyCell(point.x(), point.y());

            if (cellSt == CELL_MARK)
            {
                model_->setEnemyCell(point.x(), point.y(), CELL_EMPTY);
                qDebug() << "Press on right button -> place a mark";
                return;
            }

            if (cellSt != CELL_EMPTY)
            {
                qDebug() << "This cell if already played, cannot mark it";
                return;
            }

            model_->setEnemyCell(point.x(), point.y(), CELL_MARK);
            return;
        }
        else
        {
            qDebug() << "nothing to do on this button click";
            return;
        }

        qDebug() << "Going to" << point.x() << point.y();

        // TODO: make step

//        emit stateChanged();
        return;
    }

    if(model_->getState() == ST_WAITING_STEP)
    {
        QPoint point = getFieldCoord(pos, Field::ENEMY_FIELD);
        if(point.x() == -1 || point.y() == -1)
            return;

        if (event->button() == Qt::RightButton)
        {
            CellDraw cellSt = model_->getEnemyCell(point.x(), point.y());

            if (cellSt == CELL_MARK)
            {
                model_->setEnemyCell(point.x(), point.y(), CELL_EMPTY);
                qDebug() << "Press on right button -> place a mark";
                return;
            }

            if (cellSt != CELL_EMPTY)
            {
                qDebug() << "This cell if already played, cannot mark it";
                return;
            }

            model_->setEnemyCell(point.x(), point.y(), CELL_MARK);
            return;
        }
        else
        {
            qDebug() << "nothing to do on this button click";
            return;
        }
    }

}

//void Controller::playMissSound()
//{
//    qDebug() << "miss sound";
//    backgroundMusicPlayer_->setMedia(QUrl("qrc:/sounds/miss.wav"));
//    backgroundMusicPlayer_->setVolume(volume_);
//    backgroundMusicPlayer_->play();
//}

//void Controller::playHitSound()
//{
//    qDebug() << "hit sound";
//    backgroundMusicPlayer_->setMedia(QUrl("qrc:/sounds/hit.wav"));
//    backgroundMusicPlayer_->setVolume(volume_);
//    backgroundMusicPlayer_->play();
//}

//void Controller::playClickSound()
//{
//    qDebug() << "click sound";
//    backgroundMusicPlayer_->setMedia(QUrl("qrc:/sounds/click.wav"));
//    backgroundMusicPlayer_->setVolume(volume_);
//    backgroundMusicPlayer_->play();
//}

//void Controller::playNewMessageSound()
//{
//    qDebug() << "new message sound";

//    backgroundMusicPlayer_->setMedia(QUrl("qrc:/sounds/new_msg.wav"));
//    backgroundMusicPlayer_->setVolume(volume_);
//    backgroundMusicPlayer_->play();
//}

#define LOAD_SOUND_WAV(name_str) sounds_.insert(name_str, new PlaySound(":" SOUNDS_DIRECTORY_PATH name_str ".wav")); \
                                 qDebug() << name_str ".wav is loaded";

/**
 * @brief Загрузка звуковых эффектов.
 * 
 * Загружает все необходимые звуковые эффекты для игры.
 */
void Controller::loadSounds()
{
    if (isLoaded_)
        return;
    LOAD_SOUND_WAV("intro_music")
    LOAD_SOUND_WAV("background")
    LOAD_SOUND_WAV("field_music")
    LOAD_SOUND_WAV("victory_sound")
    LOAD_SOUND_WAV("defeat_sound")

    LOAD_SOUND_WAV("click"      )
    LOAD_SOUND_WAV("you_hit"    )
    LOAD_SOUND_WAV("enemy_hit"  )
    LOAD_SOUND_WAV("you_miss"   )
    LOAD_SOUND_WAV("enemy_miss" )
    LOAD_SOUND_WAV("you_kill"   )
    LOAD_SOUND_WAV("enemy_kill" )
    LOAD_SOUND_WAV("new_msg"    )
    isLoaded_ = true;
}

#undef LOAD_SOUND_WAV

/**
 * @brief Воспроизведение звукового эффекта.
 * @param sound_name Имя звукового эффекта
 * 
 * Воспроизводит указанный звуковой эффект.
 * Выбрасывает исключение, если эффект не найден.
 */
void Controller::playSound(QString sound_name)
{
    qDebug() << "Play sound: " << sound_name;

    QMap<QString, PlaySound*>::iterator sndIt = sounds_.find(sound_name);

    if(sndIt == sounds_.end())
        throw 1;

    sndIt.value()->play();
}

/**
 * @brief Остановка звукового эффекта.
 * @param sound_name Имя звукового эффекта
 * 
 * Останавливает воспроизведение указанного звукового эффекта.
 * Выбрасывает исключение, если эффект не найден.
 */
void Controller::stopSound(QString sound_name)
{
    qDebug() << "Play sound: " << sound_name;

    QMap<QString, PlaySound*>::iterator sndIt = sounds_.find(sound_name);

    if(sndIt == sounds_.end())
        throw 1;

    sndIt.value()->stop();
}

/**
 * @brief Обновление громкости.
 * @param volume Новая громкость (0-100)
 * 
 * Устанавливает новую громкость для всех звуковых эффектов.
 */
void Controller::updateVolume(int volume)
{
    if (volume < 0)
        volume = 0;

    volume_ = volume;
//    backgroundMusicPlayer_->setVolume(volume_);
//    soundPlayer_->setVolume(volume_);
}

/**
 * @brief Обработка изменения состояния медиаплеера.
 * @param status Новое состояние медиаплеера
 * 
 * Обрабатывает события изменения состояния медиаплеера.
 */
void Controller::on_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
//    qDebug() << "end of the player_";

    if (status == QMediaPlayer::EndOfMedia)
    {
//        backgroundMusicPlayer_->setPosition(0);
//        backgroundMusicPlayer_->play();
    }
}
