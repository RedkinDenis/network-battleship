/**
 * @file controller.hpp
 * @brief Контроллер для клиентской части игры "Морской бой"
 * 
 * Этот класс связывает модель и представление, обрабатывает пользовательский ввод
 * и управляет взаимодействием с сервером.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QTcpSocket>
#include <QLabel>
#include <QRegularExpression>
#include <QMessageBox>
#include <QFile>
#include <QtMultimedia>
#include <QMediaPlayer>
#include <QMap>
#include "config.hpp"
#include "constants.hpp"
#include "model.hpp"
#include "util/PlaySound.h"

/**
 * @brief Результаты игры
 */
enum GameResult
{
    GR_NONE = 0 ,  ///< Игра не завершена
    GR_WON  = 1 ,  ///< Победа
    GR_LOST = -1   ///< Поражение
};

/**
 * @brief Коды ошибок игры
 */
enum GameErrorMessage
{
    GEM_UNKNOWN                 = -1,  ///< Неизвестная ошибка
//    GEM_NONE                    = 0 ,  ///< Нет ошибки
//    GEM_ALREADY_CONNECTED       ,     ///< Уже подключен
//    GEM_WRONG_FIELD            ,     ///< Неверное поле
//    GEM_WRONG_USER             ,     ///< Неверный пользователь
//    GEM_SERVER_ERROR           ,     ///< Ошибка сервера
//    GEM_SERVER_CONNECTION_REFUSED,   ///< Отказ в подключении
//    GEM_SERVER_CONNECTION_TIMEOUT,   ///< Таймаут подключения
//    GEM_SERVER_UNAVAILABLE     ,     ///< Сервер недоступен
};

/**
 * @brief Класс контроллера
 * 
 * Связывает модель и представление, обрабатывает пользовательский ввод
 * и взаимодействие с сервером.
 */
class Controller: public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор
     * @param model Указатель на модель
     * @param socket Указатель на сокет
     */
    Controller(Model* model, QTcpSocket* socket);
    
    /**
     * @brief Деструктор
     */
    ~Controller();
    
    /**
     * @brief Обработчик нажатия мыши
     * @param position Позиция курсора
     * @param event Событие мыши
     * @param applyIsOkLabel Метка для отображения корректности поля
     * @param applyIsNotOkLabel Метка для отображения некорректности поля
     * @param applyFieldButton Кнопка применения поля
     */
    void onMousePressed(const QPoint& position, QMouseEvent* event, QLabel* applyIsOkLabel, QLabel* applyIsNotOkLabel, QPushButton* applyFieldButton);

    /**
     * @brief Воспроизвести звук
     * @param soundName Имя звука
     */
    void playSound(QString soundName);
    
    /**
     * @brief Остановить звук
     * @param sound_name Имя звука
     */
    void stopSound(QString sound_name);
    
    /**
     * @brief Обновить громкость
     * @param volume Новое значение громкости
     */
    void updateVolume(int volume);
    
    /**
     * @brief Загрузить звуки
     */
    void loadSounds();

public:
    int volume_;  ///< Текущая громкость

private:
    /**
     * @brief Обработчик изменения состояния медиаплеера
     * @param status Новое состояние
     */
    void on_mediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    bool isLoaded_;                ///< Флаг загрузки звуков
    QTcpSocket* socket_;           ///< Сокет для коммуникации с сервером
    Model* model_;                 ///< Указатель на модель
    QMap<QString, PlaySound*> sounds_;  ///< Карта звуков
};

#endif // CONTROLLER_H
