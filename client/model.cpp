/**
 * @file model.cpp
 * @brief Реализация класса Model для игры "Морской бой".
 * 
 * Этот файл содержит реализацию класса Model, который управляет состоянием игры,
 * полями игроков и игровой логикой. Является центральным компонентом архитектуры MVC,
 * отвечающим за данные и бизнес-логику.
 */

#include "./model.hpp"
#include <QMessageBox>

/**
 * @brief Конструктор по умолчанию.
 * 
 * Инициализирует модель с начальным состоянием и создает пустые поля для обоих игроков.
 */
Model::Model() :
    state_(ST_GAME_NSTARTED),
    myField_(new Field(Field::MY_FIELD)),
    enemyField_(new Field(Field::ENEMY_FIELD)),
    gameId_(-1),
    login_("")
{
}

/**
 * @brief Деструктор.
 * 
 * Освобождает память, занятую полями игроков, и устанавливает финальное состояние.
 */
Model::~Model()
{
    if (myField_)
        delete myField_;

    if (enemyField_)
        delete enemyField_;

    updateState(ST_GAME_FINISHED);
}

/**
 * @brief Получение состояния ячейки на своем поле.
 * @param x Координата X
 * @param y Координата Y
 * @return Состояние ячейки
 * 
 * Возвращает текущее состояние указанной ячейки на поле игрока.
 */
CellDraw Model::getMyCell(int x, int y) const
{
    return myField_->getCell(x, y);
}

/**
 * @brief Получение состояния ячейки на поле противника.
 * @param x Координата X
 * @param y Координата Y
 * @return Состояние ячейки
 * 
 * Возвращает текущее состояние указанной ячейки на поле противника.
 */
CellDraw Model::getEnemyCell(int x, int y) const
{
    return enemyField_->getCell(x, y);
}

/**
 * @brief Установка состояния ячейки на своем поле.
 * @param x Координата X
 * @param y Координата Y
 * @param state Новое состояние ячейки
 * 
 * Устанавливает новое состояние для указанной ячейки на поле игрока.
 */
void Model::setMyDrawCell(int x, int y, CellDraw state)
{
    myField_->setCell(x, y, state);
}

/**
 * @brief Установка состояния ячейки на поле противника.
 * @param x Координата X
 * @param y Координата Y
 * @param state Новое состояние ячейки
 * 
 * Устанавливает новое состояние для указанной ячейки на поле противника.
 */
void Model::setEnemyCell(int x, int y, CellDraw state)
{
    enemyField_->setCell(x, y, state);
}

/**
 * @brief Установка состояния ячейки на своем поле.
 * @param x Координата X
 * @param y Координата Y
 * @param state Новое состояние ячейки
 * 
 * Устанавливает новое состояние для указанной ячейки на поле игрока.
 */
void Model::setMyStateCell(int x, int y, CellState state)
{
    myField_->setState(x, y, state);
}

/**
 * @brief Установка состояния ячейки на поле противника.
 * @param x Координата X
 * @param y Координата Y
 * @param state Новое состояние ячейки
 * 
 * Устанавливает новое состояние для указанной ячейки на поле противника.
 */
void Model::setEnemyStateCell(int x, int y, CellState state)
{
    enemyField_->setState(x, y, state);
}

/**
 * @brief Получение строкового представления своего поля.
 * @return Строковое представление поля игрока
 * 
 * Возвращает строковое представление поля игрока для отладки.
 */
QString Model::getMyFieldStr() const
{
    QString result;
    for(int i = 0; i < FIELD_WIDTH; i++)
    {
        for(int j = 0; j < FIELD_HEIGHT; j++)
        {
            switch(myField_->getCell(i, j))
            {
                case CellDraw::CELL_EMPTY:
                    result += ".";
                    break;
                case CellDraw::CELL_LIVE:
                    result += "O";
                    break;
                case CellDraw::CELL_DAMAGED:
                    result += "X";
                    break;
                case CellDraw::CELL_DESTROYED:
                    result += "#";
                    break;
                case CellDraw::CELL_MISS:
                    result += "*";
                    break;
                default:
                    result += "?";
            }
        }
        result += "\n";
    }
    return result;
}

/**
 * @brief Получение строкового представления поля противника.
 * @return Строковое представление поля противника
 * 
 * Возвращает строковое представление поля противника для отладки.
 */
QString Model::getEnemyFieldStr() const
{
    QString result;
    for(int i = 0; i < FIELD_WIDTH; i++)
    {
        for(int j = 0; j < FIELD_HEIGHT; j++)
        {
            switch(enemyField_->getCell(i, j))
            {
                case CellDraw::CELL_EMPTY:
                    result += ".";
                    break;
                case CellDraw::CELL_LIVE:
                    result += "O";
                    break;
                case CellDraw::CELL_DAMAGED:
                    result += "X";
                    break;
                case CellDraw::CELL_DESTROYED:
                    result += "#";
                    break;
                case CellDraw::CELL_MISS:
                    result += "*";
                    break;
                default:
                    result += "?";
            }
        }
        result += "\n";
    }
    return result;
}

/**
 * @brief Получение объекта поля игрока.
 * @return Объект поля игрока
 */
Field Model::getMyField() const
{
    return *myField_;
}

/**
 * @brief Получение объекта поля противника.
 * @return Объект поля противника
 */
Field Model::getEnemyField() const
{
    return *enemyField_;
}

/**
 * @brief Установка поля игрока из вектора состояний для отображения.
 * @param field Вектор состояний ячеек
 */
void Model::setMyField(QVector<CellDraw> field)
{
    myField_->setDrawField(field);
}

/**
 * @brief Установка поля игрока из вектора внутренних состояний.
 * @param field Вектор внутренних состояний ячеек
 */
void Model::setMyField(QVector<CellState> field)
{
    myField_->setStateField(field);
}

/**
 * @brief Установка поля игрока из строки.
 * @param field Строка, представляющая состояние поля
 */
void Model::setMyField(QString field)
{
    myField_->setStateField(field);
}

/**
 * @brief Инициализация поля игрока для отображения.
 * 
 * Преобразует внутреннее состояние поля в состояние для отображения.
 */
void Model::initMyDrawField()
{
    myField_->initMyDrawField();
}

/**
 * @brief Очистка поля игрока.
 * 
 * Устанавливает все ячейки поля в пустое состояние.
 */
void Model::clearMyField()
{
    myField_->clear();
}

/**
 * @brief Получение текущего состояния игры.
 * @return Текущее состояние игры
 */
ModelState Model::getState() const
{
    return state_;
}

/**
 * @brief Обновление состояния поля игрока для отображения.
 * @param field Вектор новых состояний ячеек
 */
void Model::updateMyFieldDraw(QVector<CellDraw>& field)
{
    myField_->setDrawField(field);
}

/**
 * @brief Обновление состояния поля противника для отображения.
 * @param field Вектор новых состояний ячеек
 * 
 * Обновляет только те ячейки, которые были подбиты или промахнуты.
 */
void Model::updateEnemyFieldDraw(QVector<CellDraw>& field)
{
    QVector<CellDraw> updatedField = enemyField_->getDrawField();
    int area = updatedField.size();

    for (int i = 0; i < area; i++)
    {
        if (field[i] == CellDraw::CELL_DAMAGED ||
            field[i] == CellDraw::CELL_DOT     ||
            field[i] == CellDraw::CELL_KILLED    )
        {
            updatedField[i] = field[i];
        }
    }

    enemyField_->setDrawField(updatedField);
}

/**
 * @brief Обновление состояния игры.
 * @param state Новое состояние игры
 * 
 * Устанавливает новое состояние игры и выводит отладочное сообщение.
 */
void Model::updateState(ModelState state)
{
    if (state == ST_GAME_NSTARTED)
        qDebug() << "state updated to ST_GAME_NSTARTED";
    else if (state == ST_PLACING_SHIPS)
        qDebug() << "state updated to ST_PLACING_SHIPS";
    else if (state == ST_WAITING_PLACING)
        qDebug() << "state updated to ST_WAITING_PLACING";
    else if (state == ST_MAKING_STEP)
        qDebug() << "state updated to ST_MAKING_STEP";
    else if (state == ST_WAITING_STEP)
        qDebug() << "state updated to ST_WAITING_STEP";
    else if (state == ST_GAME_FINISHED)
        qDebug() << "state updated to ST_GAME_FINISHED";
    else {}

    state_ = state;
}

/**
 * @brief Переключение хода между игроками.
 * 
 * Меняет состояние игры между ST_MAKING_STEP и ST_WAITING_STEP.
 */
void Model::switchStep()
{
    if (state_ == ST_MAKING_STEP)
        updateState(ST_WAITING_STEP);
    else if (state_ == ST_WAITING_STEP)
        updateState(ST_MAKING_STEP);
}

/**
 * @brief Установка логина игрока.
 * @param login Логин игрока
 */
void Model::setLogin(const QString& login)
{
    login_ = login;
}

/**
 * @brief Получение логина игрока.
 * @return Логин игрока
 */
QString Model::getLogin() const
{
    return login_;
}

/**
 * @brief Получение ID текущей игры.
 * @return ID игры
 */
int Model::getGameId() const
{
    return gameId_;
}

/**
 * @brief Проверка корректности размещения кораблей на поле игрока.
 * @return true если размещение корректно, false в противном случае
 */
bool Model::isMyFieldCorrect() const
{
    return myField_->isCorrect();
}

/**
 * @brief Начало новой игры.
 * @param enemy_login Логин противника
 * @param gameId ID игры
 * 
 * Инициализирует новую игру с указанным противником и ID.
 */
void Model::startGame(QString enemy_login, int gameId)
{
    gameId_ = gameId;
    enemyLogin_ = enemy_login;
    updateState(ST_PLACING_SHIPS);
}

/**
 * @brief Завершение текущей игры.
 * 
 * Очищает поля игроков и сбрасывает состояние игры.
 */
void Model::finishGame()
{
    myField_->clear();
    enemyField_->clear();
    updateState(ST_GAME_FINISHED);
    updateState(ST_GAME_NSTARTED);
}

/**
 * @brief Начало боя.
 * 
 * Определяет, кто делает первый ход, и устанавливает соответствующее состояние.
 */
void Model::startFight()
{
    qDebug() << "HERE!!!!!!!";

    if (amIStarted_)
    {
        updateState(ST_MAKING_STEP);
        qDebug() << "1st step is mine";
    }
    else
    {
        updateState(ST_WAITING_STEP);
        qDebug() << "1st step isn't mine";
    }
}

/**
 * @brief Генерация случайного поля для игрока.
 * 
 * Создает случайное размещение кораблей на поле игрока.
 */
void Model::generateMyField()
{
    myField_->generate();
}

/**
 * @brief Установка флага первого хода.
 * @param val true если игрок ходит первым, false в противном случае
 */
void Model::setStartedFlag(bool val)
{
    amIStarted_ = val;
}

/**
 * @brief Получение флага первого хода.
 * @return true если игрок ходит первым, false в противном случае
 */
bool Model::getStartedFlag()
{
    return amIStarted_;
}

/**
 * @brief Установка логина противника.
 * @param login Логин противника
 */
void Model::setEnemyLogin(QString login)
{
    enemyLogin_ = login;
}

/**
 * @brief Получение логина противника.
 * @return Логин противника
 */
QString Model::getEnemyLogin()
{
    return enemyLogin_;
}
