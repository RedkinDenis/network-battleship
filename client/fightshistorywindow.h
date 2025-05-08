/**
 * @file fightshistorywindow.h
 * @brief Окно истории боев для игры "Морской бой"
 * 
 * Этот класс реализует окно, отображающее историю завершенных игр,
 * включая информацию о победителях и проигравших.
 */

#ifndef FIGHTSHISTORYWINDOW_H
#define FIGHTSHISTORYWINDOW_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class FightsHistoryWindow;
}

/**
 * @brief Класс окна истории боев
 * 
 * Реализует диалоговое окно для отображения истории завершенных игр,
 * включая информацию о победителях и проигравших.
 */
class FightsHistoryWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор
     * @param parent Родительский виджет
     */
    explicit FightsHistoryWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Деструктор
     */
    ~FightsHistoryWindow();
    
    /**
     * @brief Добавить строку с информацией о завершении игры
     * @param gameEndingStr Строка с информацией о завершении игры
     * @param rowIndex Индекс строки в таблице
     */
    void addGameEmdingStr(QString& gameEndingStr, int rowIndex);
    
    /**
     * @brief Заполнить таблицу информацией о завершенных играх
     * @param gameEndingsStrList Список строк с информацией о завершении игр
     */
    void fillTable(QStringList& gameEndingsStrList);

private:
    Ui::FightsHistoryWindow *ui;  ///< Указатель на интерфейс
};

#endif // FIGHTSHISTORYWINDOW_H
