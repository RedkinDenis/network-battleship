#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QImage>
#include <QStringList>
#include <QListWidget>
#include <QTextBrowser>
#include <QStackedWidget>
#include "./config.h"
#include "field.h"
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Перечисление, определяющее состояния клиента
 * 
 * Клиент может находиться в одном из следующих состояний:
 * - ST_DISCONNECTED: Клиент не подключен к серверу
 * - ST_CONNECTED: Клиент подключен к серверу, но не авторизован
 * - ST_AUTHORIZED: Клиент авторизован на сервере
 * - ST_READY: Клиент готов к игре
 */
enum ClientState
{
    ST_DISCONNECTED = 0,  // Клиент не подключен к серверу
    ST_CONNECTED,         // Клиент подключен к серверу, но не авторизован
    ST_AUTHORIZED,        // Клиент авторизован на сервере
    ST_READY,             // Клиент готов к игре
    // Дополнительные состояния могут быть добавлены здесь
};

/**
 * @brief Главное окно приложения
 * 
 * Класс MainWindow является основным интерфейсом игры. Он:
 * - Управляет подключением к серверу
 * - Обрабатывает авторизацию пользователя
 * - Отображает игровое поле
 * - Обеспечивает чат между игроками
 * - Обрабатывает игровые события
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор
     * @param ip IP-адрес сервера
     * @param port Порт сервера
     * @param parent Родительский виджет
     */
    MainWindow(QString ip, quint16 port, QWidget *parent = nullptr);
    
    /**
     * @brief Деструктор
     */
    ~MainWindow();
    
    /**
     * @brief Обновить экран
     * Перерисовывает игровое поле и другие элементы интерфейса
     */
    void screenUpdate();
    
    /**
     * @brief Обновить состояние клиента
     * @param new_state Новое состояние клиента
     */
    void stateUpdate(ClientState new_state);
    
    /**
     * @brief Обновить все элементы интерфейса
     * Вызывает screenUpdate, stateUpdate и другие методы обновления
     */
    void updateAll();
    
    /**
     * @brief Выйти с сервера
     * Отправляет сообщение о выходе на сервер и закрывает соединение
     */
    void exitFromServer();

private:
    ClientState state_;    // Текущее состояние клиента
    QString ip_;           // IP-адрес сервера
    int port_;             // Порт сервера
    QTcpSocket* socket_;   // Сокет для взаимодействия с сервером
    QByteArray data_;      // Данные, полученные от сокета и для отправки в сокет
    QString login_;        // Логин пользователя
    Model* model_;         // Модель игры
    int timerId_;          // Идентификатор таймера для периодических действий

protected:
    /**
     * @brief Обработчик события таймера
     * @param event Событие таймера
     */
    void timerEvent(QTimerEvent *event) override;
    
    /**
     * @brief Обработчик события отрисовки
     * @param event Событие отрисовки
     */
    void paintEvent(QPaintEvent* event) override;
    
    /**
     * @brief Обработчик события закрытия окна
     * @param event Событие закрытия
     */
    void closeEvent(QCloseEvent* event) override;

public:
    QStringList userLogins_;  // Список логинов пользователей
    
    /**
     * @brief Подключить пользователя к серверу
     * Устанавливает соединение с сервером по указанному IP и порту
     */
    void connectUser();
    
    /**
     * @brief Авторизовать пользователя на сервере
     * Отправляет запрос на авторизацию с логином пользователя
     */
    void authenticateUser();
    
    /**
     * @brief Обработать данные, полученные от сервера
     * Анализирует полученные данные и вызывает соответствующие обработчики
     */
    void handleData();
    
    /**
     * @brief Обновить список пользователей
     * Запрашивает у сервера актуальный список пользователей
     */
    void usersListUpdate();
    
    /**
     * @brief Отправить сообщение
     * Отправляет сообщение выбранному пользователю или всем пользователям
     */
    void sendMessage();
    
    /**
     * @brief Обработать запрос сообщения
     * Обрабатывает полученное сообщение и отображает его в чате
     */
    void handleMessageRequest();
    
    /**
     * @brief Обработать запрос списка пользователей
     * Обрабатывает полученный список пользователей и обновляет интерфейс
     */
    void handleUsersRequest();
    
    /**
     * @brief Обработать запрос пинга
     * Отправляет ответ на пинг от сервера
     */
    void handlePingRequest();
    
    /**
     * @brief Обработать запрос выхода
     * Обрабатывает сообщение о выходе пользователя
     */
    void handleExitRequest();
    
    /**
     * @brief Обновить чаты
     * Обновляет список чатов с пользователями
     */
    void updateChats();
    
    /**
     * @brief Остановить клиент
     * @param msg Сообщение о причине остановки
     * Останавливает клиент и отображает сообщение
     */
    void stopClient(QString msg);

    /**
     * @brief Получить изображение игрового поля
     * @param field Игровое поле
     * @return Изображение игрового поля
     */
    QImage getFieldImage(const Field& field) const;

public slots:
    /**
     * @brief Обработчик события подключения к серверу
     */
    void on_sockConnect();
    
    /**
     * @brief Обработчик события получения данных от сервера
     */
    void on_receiveData();
    
    /**
     * @brief Обработчик события отключения от сервера
     */
    void on_sockDisconnect();
    
    /**
     * @brief Обработчик события ошибки сокета
     * @param error Код ошибки
     */
    void on_sockError(QAbstractSocket::SocketError error);

private slots:
    /**
     * @brief Обработчик нажатия кнопки подключения к серверу
     */
    void on_connectToServerButton_clicked();
    
    /**
     * @brief Обработчик изменения позиции курсора в поле логина
     * @param arg1 Старая позиция
     * @param arg2 Новая позиция
     */
    void on_loginLabel_cursorPositionChanged(int arg1, int arg2);
    
    /**
     * @brief Обработчик нажатия кнопки обновления
     */
    void on_updateButton_clicked();
    
    /**
     * @brief Обработчик нажатия кнопки отправки сообщения
     */
    void on_sendMessageButton_clicked();
    
    /**
     * @brief Обработчик изменения выбранного элемента в списке получателей
     */
    void on_messageRecieversOptionList_itemSelectionChanged();
    
    /**
     * @brief Создать текстовый браузер для чата с пользователем
     * @param user Элемент списка пользователей
     */
    void createTextBrowser(QListWidgetItem* user);

private:
    QListWidget* receiversListWidget;           // Список пользователей, которым можно отправить сообщение
    QStackedWidget* receiverBrowserStackedWidget;  // Стек браузеров (чатов) с пользователями
    QMap<QListWidgetItem*, QTextBrowser*> browserMap;  // Карта соответствия пользователей и их чатов

private:
    Ui::MainWindow* ui;  // Указатель на пользовательский интерфейс
};

#endif // MAINWINDOW_H
