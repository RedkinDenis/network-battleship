/**
 * @file images.hpp
 * @brief Менеджер изображений для игры "Морской бой"
 * 
 * Этот класс управляет загрузкой и хранением изображений,
 * используемых в графическом интерфейсе игры.
 */

#ifndef IMAGES_H
#define IMAGES_H

#include <QMap>
#include <QImage>
#include <QString>
#include <QDebug>
#include "config.hpp"
#include "constants.hpp"

/**
 * @brief Класс менеджера изображений
 * 
 * Реализует загрузку и хранение изображений, используемых
 * в графическом интерфейсе игры.
 */
class Images
{
public:
    /**
     * @brief Конструктор
     */
    Images();
    
    /**
     * @brief Деструктор
     */
    ~Images();

    /**
     * @brief Загрузить все изображения
     */
    void load();
    
    /**
     * @brief Получить изображение по имени
     * @param imgName Имя изображения
     * @return Ссылка на изображение
     */
    QImage& get(const QString& imgName);
    
    /**
     * @brief Проверить, загружены ли изображения
     * @return true, если изображения загружены
     */
    bool isLoaded();

private:
    QMap<QString, QImage> images_;  ///< Карта изображений
    bool isLoaded_;                 ///< Флаг загрузки изображений
};

extern Images pictures;  ///< Глобальный экземпляр менеджера изображений

#endif // IMAGES_H
