/**
 * @file dbwindow.hpp
 * @brief Окно управления базой данных для серверной части игры "Морской бой"
 * 
 * Этот класс реализует окно для управления базой данных,
 * позволяющее добавлять и удалять записи о полях игры.
 */

#ifndef DBWINDOW_HPP
#define DBWINDOW_HPP

#include <QMainWindow>
#include <qsqldatabase.h>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace DBUi {
    class DBWindow;
} // namespace DBUi

/**
 * @brief Класс окна управления базой данных
 * 
 * Реализует окно для управления базой данных,
 * позволяющее добавлять и удалять записи о полях игры.
 */
class DBWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор
     * @param parent Родительский виджет
     */
    explicit DBWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Деструктор
     */
    ~DBWindow();

private slots:
    /**
     * @brief Обработчик нажатия кнопки добавления
     */
    void on_addButton_clicked();
    
    /**
     * @brief Обработчик нажатия кнопки удаления
     */
    void on_removeButton_clicked();
    
    /**
     * @brief Обработчик клика по таблице
     * @param index Индекс выбранной ячейки
     */
    void on_tableView_clicked(const QModelIndex &index);

private:
    DBUi::DBWindow* ui;     ///< Указатель на интерфейс
    QSqlDatabase db_;       ///< Объект базы данных
    QSqlQuery *query_;      ///< Объект запроса
    QSqlTableModel *model_; ///< Модель таблицы
    int row;                ///< Выбранная строка
};

#endif // DBWINDOW_HPP
