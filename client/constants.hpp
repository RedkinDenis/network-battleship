/**
 * @file constants.hpp
 * @brief Константы для игры "Морской бой"
 * 
 * Этот файл содержит определения констант, используемых в игре,
 * таких как размеры поля, координаты изображений и параметры кораблей.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/// Ширина игрового поля по умолчанию
const int FIELD_WIDTH_DEFAULT  = 10;

/// Высота игрового поля по умолчанию
const int FIELD_HEIGHT_DEFAULT = 10;

/// Ширина изображения поля по умолчанию
const int FIELD_IMG_WIDTH_DEFAULT  = 216;

/// Высота изображения поля по умолчанию
const int FIELD_IMG_HEIGHT_DEFAULT = 217;

/// X-координата изображения поля игрока
const int MYFIELD_IMG_X = 40;

/// Y-координата изображения поля игрока
const int MYFIELD_IMG_Y = 127;

/// Относительная X-координата изображения поля игрока
const int MYFIELD_IMG_REL_X = MYFIELD_IMG_X;

/// Относительная Y-координата изображения поля игрока
const int MYFIELD_IMG_REL_Y = 39;

/// X-координата изображения поля противника
const int ENEMYFIELD_IMG_X = 322;

/// Y-координата изображения поля противника
const int ENEMYFIELD_IMG_Y = 127;

/// Относительная X-координата изображения поля противника
const int ENEMYFIELD_IMG_REL_X = ENEMYFIELD_IMG_X;

/// Относительная Y-координата изображения поля противника
const int ENEMYFIELD_IMG_REL_Y = 39;

/// Максимальная длина корабля
const int SHIP_MAXLEN = 4;

/// Количество кораблей длиной 1
const int SHIP1_NUM = 4;

/// Количество кораблей длиной 2
const int SHIP2_NUM = 3;

/// Количество кораблей длиной 3
const int SHIP3_NUM = 2;

/// Количество кораблей длиной 4
const int SHIP4_NUM = 1;

#endif // CONSTANTS_H
