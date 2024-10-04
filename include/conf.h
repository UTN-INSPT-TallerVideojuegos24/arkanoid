#ifndef __CONF__
#define __CONF__

#include "sfml-utn-inspt.h"

/*
 * Carpeta de los "recursos" (texturas y sonidos)
 */
const string ASSETS_PATH = "../assets/";
const string ARK_TEXTURES = "textures.png";
const string LYND_TEXTURES = "leyendas.png";
const string SND_PAUSA = "pausa.ogg";
const string SND_PERDISTE = "perdiste.ogg";
const string SND_REBOTE = "rebote.ogg";
const string MUSIC = "lose-your-head.mp3";

/*
 * Cantidad de bloques a dibujar (de esto depende el tamaño de la ventana)
 */
const unsigned int BRICK_LINE = 14;
const unsigned int BRICK_ROWS = 6; // Siempre menor igual que 6!!
const size_t CANT_BRICKS = BRICK_LINE * BRICK_ROWS;

/*
 * Offsets de los sprites para las texturas del Arkanoid
 */
const Vector2f BRICK_SIZE = {64.f, 32.f};
const Vector2f PLYR_OFFSET = {129, 24};
const Vector2f PLYR_SIZE = {104, 24};
const Vector2f BALL_OFFSET = {129, 48};
const Vector2f BALL_SIZE = {22, 22};
// Escalado del jugador y la pelota, por estética, la paleta y la pelota
// podrían ser más chicas o grandes respecto de los ladrillos según la preferencia.
const float PLY_BLL_SCALE = .75f;

/*
 * Offsets para las texturas de los textos
 */
const int LYN_OFFSET_Y = 117;
const Vector2i PERDISTE_OFFSET = {0, LYN_OFFSET_Y * 0};
const int PERDISTE_SIZE_X = 617;
const Vector2i GANASTE_OFFSET = {0, LYN_OFFSET_Y * 1};
const int GANASTE_SIZE_X = 573;
const Vector2i PAUSA_OFFSET = {0, LYN_OFFSET_Y * 2};
const int PAUSA_SIZE_X = 433;

/*
 * Dimensiones de la ventana
 */
const unsigned int TOP_GAP = BRICK_SIZE.y * 2;
const unsigned int BOTTOM_GAP = BRICK_SIZE.y * 10;
const unsigned int ANCHO_VENT = BRICK_SIZE.x * BRICK_LINE;
const unsigned int ALTO_VENT = TOP_GAP + BRICK_SIZE.y * BRICK_ROWS + BOTTOM_GAP;
const unsigned int FRAMERATE = 60;

/*
 * Teclas del juego
 */
const Keyboard::Key TEC_IZQUIERDA = Keyboard::A;
const Keyboard::Key TEC_DER = Keyboard::D;
const Keyboard::Key TEC_PAUSA = Keyboard::Space;
const Keyboard::Key TEC_EXIT = Keyboard::Escape;

/*
 * Velocidad de la pelota y el jugador
 */
const float VELOCIDAD = 7.5f;

/*
 * Volúmen de la música y los efectos de sonido
 */
const float VOLUMEN_MUSICA = 35.f; // 0.f a 100.f
const float VOLUMEN_FX = 100.f;

#endif // __CONF__
