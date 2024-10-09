#include "conf.h"

/**
 * @brief La estuctura que define un bloque (ladrillo)
 *
 * Este consta de un sprite que es el dibujo en pantalla, un nivel de "dureza"
 * (cantidad de veces que debe ser golpeado para desaparecer) y un estado
 * destruido que indica si ya no debe ser considerado para ser dibujado y en las
 * colisiones.
 */
struct Brick {
  Sprite spr;
  int lvl;
  bool esta_destruido;
};

/*
 * Indica si el juego está en pausa, se modifica en un evento
 */
bool esta_pausado = true;
bool es_reset = false;

void leer_eventos(RenderWindow &ventana);

enum borde_t { SUPERIOR, INFERIOR, IZQUIERDO, DERECHO };
bool colision_con_ventana(Sprite &s, borde_t borde);

void cargar_textura(Texture &texture, string path);
void inicializar_pelota(Sprite &pelota, Texture &ark_texture);
void inicializar_bloques(Brick bricks[][BRICK_LINE], Texture &ark_texture);
void inicializar_jugador(Sprite &jugador, Texture &ark_texture);
void inicializar_leyenda(Sprite &leyenda, Texture &tex_leyendas,
                         IntRect recorte);
void inicializar_sonido(string path, SoundBuffer &buff, Sound &sound,
                        float volume);
void inicializar_musica(string path, Music &musica, float volumen, bool loop);

int main() {
  srand(time(NULL));
  RenderWindow ventana =
      RenderWindow({ANCHO_VENT, ALTO_VENT}, "Arkanoid - UTN-INSPT");
  ventana.setFramerateLimit(FRAMERATE);
  ventana.setMouseCursorVisible(false);

  // Texturas del Arkanoid (ladrillos, pelota y jugador)
  Texture ark_texture;
  cargar_textura(ark_texture, ASSETS_PATH + ARK_TEXTURES);
  // Texturas de Pausa, Ganaste y Perdiste
  Texture tex_leyendas;
  cargar_textura(tex_leyendas, ASSETS_PATH + LYND_TEXTURES);

  Brick bricks[BRICK_ROWS][BRICK_LINE];
  inicializar_bloques(bricks, ark_texture);
  Sprite ball;
  inicializar_pelota(ball, ark_texture);
  Sprite player;
  inicializar_jugador(player, ark_texture);

  Sprite perdiste;
  inicializar_leyenda(perdiste, tex_leyendas,
                      IntRect(PERDISTE_OFFSET.x, PERDISTE_OFFSET.y,
                              PERDISTE_SIZE_X, LYN_OFFSET_Y));
  Sprite ganaste;
  inicializar_leyenda(ganaste, tex_leyendas,
                      IntRect(GANASTE_OFFSET.x, GANASTE_OFFSET.y,
                              GANASTE_SIZE_X, LYN_OFFSET_Y));
  Sprite pausa;
  inicializar_leyenda(
      pausa, tex_leyendas,
      IntRect(PAUSA_OFFSET.x, PAUSA_OFFSET.y, PAUSA_SIZE_X, LYN_OFFSET_Y));

  SoundBuffer sbuffrebote;
  Sound rebote_sound;
  inicializar_sonido(ASSETS_PATH + SND_REBOTE, sbuffrebote, rebote_sound,
                     VOLUMEN_FX);
  SoundBuffer sbuffperdiste;
  Sound perdiste_sound;
  inicializar_sonido(ASSETS_PATH + SND_PERDISTE, sbuffperdiste, perdiste_sound,
                     VOLUMEN_FX);
  SoundBuffer sbuffpausa;
  Sound pausa_sound;
  inicializar_sonido(ASSETS_PATH + SND_PAUSA, sbuffpausa, pausa_sound,
                     VOLUMEN_FX * .25f);
  Music music;
  inicializar_musica(ASSETS_PATH + MUSIC, music, VOLUMEN_MUSICA, true);

  // ángulo en el que se desplaza la pelota.
  float angulo_mov = M_PI_4 - rand() * M_PI_4 * pow(-1, rand() % 2);
  // movimiento de la pelota (inicial random)
  Vector2f diff = {cosf(angulo_mov) * VELOCIDAD,
                   -abs(sinf(angulo_mov) * VELOCIDAD)};

  // Normalización del ángulo para cuando la pelota toca al jugador.
  const float NORM = (player.getGlobalBounds().height / 2.f);

  // Cantidad de bloques restantes (cuando es 0 ganaste)
  size_t bloques_restantes = CANT_BRICKS;

  bool esta_finalizado = false, es_ganador = false;
  // para indicar si se puso o salio de pausa y reproducir el sonido
  bool last_pausa = esta_pausado;
  while (ventana.isOpen()) {
    leer_eventos(ventana);
    // Si pasa de un estado a otro se reproduce el sonido
    if (last_pausa != esta_pausado) {
      last_pausa = esta_pausado;
      pausa_sound.play();
    }
    if (!esta_pausado && !esta_finalizado) {
      // No está en pausa y no se finalizó el juego, se sigue jugando:
      // Lectura de los movimientos del jugador
      if (Keyboard::isKeyPressed(TEC_IZQUIERDA)) {
        player.move({-VELOCIDAD, 0});
      } else if (Keyboard::isKeyPressed(TEC_DER)) {
        player.move({VELOCIDAD, 0});
      }
      // Se desplaza la pelota
      ball.move(diff);

      // Detección de colisiones del jugador con los bordes
      if (colision_con_ventana(player, IZQUIERDO)) {
        player.move({VELOCIDAD, 0});
      } else if (colision_con_ventana(player, DERECHO)) {
        player.move({-VELOCIDAD, 0});
      }

      // Detección de colisiones de la pelota con el jugador:
      
      if (player.getGlobalBounds().intersects(ball.getGlobalBounds())) {
        rebote_sound.play();
        angulo_mov =
            M_PI_2 -
            (M_PI_4 / 3.f) *
                ((ball.getPosition().x - player.getPosition().x) / NORM);
        diff = {cosf(angulo_mov) * VELOCIDAD,
                -abs(sinf(angulo_mov) * VELOCIDAD)};
      } else {
        // Detección de colisiones de la pelota con los bordes de la ventana
        bool esta_rebotando = false;
        if (colision_con_ventana(ball, IZQUIERDO)) {
          esta_rebotando = true;
          diff.x = abs(diff.x);
        } else if (colision_con_ventana(ball, DERECHO)) {
          esta_rebotando = true;
          diff.x = -abs(diff.x);
        }
        if (colision_con_ventana(ball, SUPERIOR)) {
          esta_rebotando = true;
          diff.y = abs(diff.y);
        } else if (colision_con_ventana(ball, INFERIOR)) {
          perdiste_sound.play();
          esta_finalizado = true;
        }
        if (esta_rebotando) {
          rebote_sound.play();
        }

        // Detección de colisiones de la pelota con los ladrillos:
        for (int i = 0; i < BRICK_ROWS; i++) {
          for (int j = 0; j < BRICK_LINE; j++) {
            if (!bricks[i][j].esta_destruido) {
              Sprite &brickSprite = bricks[i][j].spr;
              if (ball.getGlobalBounds().intersects(
                      brickSprite.getGlobalBounds())) {
                rebote_sound.play();
                float ballBottom =
                    ball.getGlobalBounds().top + ball.getGlobalBounds().height;
                float ballTop = ball.getGlobalBounds().top;
                float ballRight =
                    ball.getGlobalBounds().left + ball.getGlobalBounds().width;
                float ballLeft = ball.getGlobalBounds().left;

                float brickBottom = brickSprite.getGlobalBounds().top +
                                    brickSprite.getGlobalBounds().height;
                float brickTop = brickSprite.getGlobalBounds().top;
                float brickRight = brickSprite.getGlobalBounds().left +
                                   brickSprite.getGlobalBounds().width;
                float brickLeft = brickSprite.getGlobalBounds().left;

                // Comparar la posición relativa de la pelota y el ladrillo
                // para determinar el lado
                if (ballBottom > brickTop && ballTop < brickTop) {
                  // Colisión en la parte superior del ladrillo
                  diff.y = -abs(diff.y); // Rebotar hacia arriba
                } else if (ballTop < brickBottom && ballBottom > brickBottom) {
                  // Colisión en la parte inferior del ladrillo
                  diff.y = abs(diff.y); // Rebotar hacia abajo
                } else if (ballRight > brickLeft && ballLeft < brickLeft) {
                  // Colisión en el lado izquierdo del ladrillo
                  diff.x = -abs(diff.x); // Rebotar hacia la izquierda
                } else if (ballLeft < brickRight && ballRight > brickRight) {
                  // Colisión en el lado derecho del ladrillo
                  diff.x = abs(diff.x); // Rebotar hacia la derecha
                }
                bricks[i][j].lvl--;
                if (bricks[i][j].lvl < 0) {
                  bloques_restantes--;
                  // Si no quedan bloques se ganó el juego.
                  if (bloques_restantes == 0) {
                    es_ganador = true;
                    esta_finalizado = true;
                  }
                  bricks[i][j].esta_destruido = true;
                } else {
                  bricks[i][j].spr.setTextureRect(IntRect(
                      IntRect(BRICK_SIZE.x *
                                  (((BRICK_ROWS - 1) - bricks[i][j].lvl) / 3),
                              BRICK_SIZE.y *
                                  (((BRICK_ROWS - 1) - bricks[i][j].lvl) % 3),
                              BRICK_SIZE.x, BRICK_SIZE.y)));
                }
                // break;
              }
            }
          }
        }
      }
    }

    /*
     * Se muestra la ventana:
     */
    ventana.clear();
    // primero se muestran los ladrillos (los que no estén destruidos)
    for (int i = 0; i < BRICK_ROWS; i++) {
      for (int j = 0; j < BRICK_LINE; j++) {
        if (!bricks[i][j].esta_destruido)
          ventana.draw(bricks[i][j].spr);
      }
    }
    // Se dibuja la pelota y el jugador:
    ventana.draw(ball);
    ventana.draw(player);
    // Se dibuja el texto "Pausa" si corresponde
    if (esta_pausado && !esta_finalizado) {
      ventana.draw(pausa);
    }
    // Se dibuja el texto "Ganaste" o "Perdiste" según corresponda
    if (esta_finalizado) {
      ventana.draw((es_ganador) ? ganaste : perdiste);
    }
    // Se muestra la pantalla con los elemnetos dibujados sobre ella:
    ventana.display();
  }
}

void leer_eventos(RenderWindow &ventana) {
  Event event = Event();
  while (ventana.pollEvent(event)) {
    if (event.type == Event::Closed) {
      ventana.close();
    } else if (event.type == Event::KeyPressed) {
      if (event.key.code == TEC_PAUSA) {
        esta_pausado = !esta_pausado;
      } else if (event.key.code == TEC_EXIT) {
        ventana.close();
      }
    }
  }
}

bool colision_con_ventana(Sprite &s, borde_t borde) {
  switch (borde) {
  case SUPERIOR:
    return s.getGlobalBounds().top <= 0;
  case INFERIOR:
    return (s.getGlobalBounds().top + s.getGlobalBounds().height) >= ALTO_VENT;
  case IZQUIERDO:
    return s.getGlobalBounds().left <= 0;
  case DERECHO:
    return (s.getGlobalBounds().left + s.getGlobalBounds().width) >= ANCHO_VENT;
  }
  return false;
}

void cargar_textura(Texture &texture, string path) {
  if (!texture.loadFromFile(path)) {
    cerr << "No se pudieron cargar las texturas '" << path
         << "' cerrando el programa." << endl;
    exit(-1);
  }
}

void inicializar_bloques(Brick bricks[][BRICK_LINE], Texture &ark_texture) {
  for (int i = 0; i < BRICK_ROWS; i++) {
    for (int j = 0; j < BRICK_LINE; j++) {
      bricks[i][j].spr.setTexture(ark_texture);
      bricks[i][j].spr.setTextureRect(IntRect(BRICK_SIZE.x * (i / 3),
                                              BRICK_SIZE.y * (i % 3),
                                              BRICK_SIZE.x, BRICK_SIZE.y));
      bricks[i][j].lvl = (BRICK_ROWS - 1) - i;
      bricks[i][j].spr.setPosition(
          {BRICK_SIZE.x * j, TOP_GAP + BRICK_SIZE.y * i});
      bricks[i][j].esta_destruido = false;
    }
  }
}

void inicializar_pelota(Sprite &pelota, Texture &ark_texture) {
  pelota.setTexture(ark_texture);
  pelota.setTextureRect(
      IntRect(BALL_OFFSET.x, BALL_OFFSET.y, BALL_SIZE.x, BALL_SIZE.y));
  pelota.setOrigin(BALL_SIZE / 2.f);
  pelota.setPosition(ANCHO_VENT / 2.f,
                     ALTO_VENT - (PLYR_SIZE.y + BALL_SIZE.y) * PLY_BLL_SCALE);
  pelota.setScale({PLY_BLL_SCALE, PLY_BLL_SCALE});
}

void inicializar_jugador(Sprite &jugador, Texture &ark_texture) {
  jugador.setTexture(ark_texture);
  jugador.setTextureRect(
      IntRect(PLYR_OFFSET.x, PLYR_OFFSET.y, PLYR_SIZE.x, PLYR_SIZE.y));
  jugador.setOrigin(PLYR_SIZE.x / 2.f, PLYR_SIZE.y / 2.f);
  jugador.setPosition(ANCHO_VENT / 2.f,
                      ALTO_VENT - (PLYR_SIZE.y * PLY_BLL_SCALE));
  jugador.setScale({PLY_BLL_SCALE, PLY_BLL_SCALE});
}

void inicializar_leyenda(Sprite &leyenda, Texture &tex_leyendas,
                         IntRect recorte) {
  leyenda.setTexture(tex_leyendas);
  leyenda.setTextureRect(recorte);
  leyenda.setOrigin({leyenda.getGlobalBounds().width / 2.f,
                     leyenda.getGlobalBounds().height / 2.f});
  leyenda.setPosition({ANCHO_VENT / 2.f, ALTO_VENT / 4.f});
}

void inicializar_sonido(string path, SoundBuffer &buff, Sound &sound,
                        float volume) {
  if (!buff.loadFromFile(path)) {
    cerr << "No se pudo cargar sonido " << path << " ..." << endl;
  } else {
    sound.setBuffer(buff);
    sound.setVolume(volume);
  }
}

void inicializar_musica(string path, Music &musica, float volumen, bool loop) {
  if (!musica.openFromFile(path)) {
    cerr << "No se pudo cargar la música " << path << "..." << endl;
  } else {
    musica.setLoop(loop);
    musica.setVolume(volumen);
    musica.play();
  }
}
