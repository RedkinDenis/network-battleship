/**
 * @file dbcontroller.hpp
 * @brief Контроллер базы данных для игры "Морской бой"
 * 
 * Этот класс управляет взаимодействием с базой данных SQLite,
 * обеспечивая хранение и получение информации о полях игры.
 */

#ifndef DBCONTROLLER_HPP
#define DBCONTROLLER_HPP

#include <QDebug>
#include <QDialog>
#include <qsqldatabase.h>
#include <QSqlQuery>
#include <QSqlTableModel>

/**
 * @brief Класс контроллера базы данных
 * 
 * Реализует взаимодействие с базой данных SQLite для хранения
 * и получения информации о полях игры.
 */
class DBController : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор
     * @param parent Родительский виджет
     */
    explicit DBController(QWidget* parent = nullptr);
    
    /**
     * @brief Деструктор
     */
    ~DBController();

    /**
     * @brief Подключиться к базе данных
     * @param dbName Имя базы данных
     */
    void connectDatabase(const QString& dbName);
    
    /**
     * @brief Отключиться от базы данных
     */
    void disconnectDatabase();
    
    /**
     * @brief Выполнить SQL-запрос
     * @param queryStr Строка запроса
     */
    void runQuery(QString queryStr);
    
    /**
     * @brief Создать таблицу
     * @param tableName Имя таблицы
     * @param tableFormat Формат таблицы
     */
    void createTable(QString tableName, QString tableFormat);

    /**
     * @brief Вывести содержимое таблицы
     * @param tableName Имя таблицы
     */
    void printTable(const QString& tableName);
    
    /**
     * @brief Получить количество записей в таблице
     * @param tableName Имя таблицы
     * @return Количество записей
     */
    int tableLen(const QString& tableName);
    
    /**
     * @brief Очистить базу данных
     */
    void clearDatabase();

    /**
     * @brief Получить случайное поле из базы данных
     * @return Строка с полем
     */
    QString getRandomField();
    
    /**
     * @brief Добавить новое поле в базу данных
     * @param field Строка с полем
     */
    void addNewPlacement(QString field);

private:
    QSqlDatabase db_;        ///< Объект базы данных
    QSqlQuery *query_;       ///< Объект запроса
    QSqlTableModel *model_;  ///< Модель таблицы
};

#endif // DBCONTROLLER_HPP
