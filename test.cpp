/* Compilación: g++ -g -o img test.cpp
 * Ejecución: ./img
 *
 * Luego de la ejecución se generarán 3 imagines: imagen.png,
 * imagen_desplazada_500.png e imagen_desplazada_600.png
 */

#include "moving_image.h"
#include<unistd.h> // para sleep (linux). Usar  #include<windows.h> para Windows

int main() {
  moving_image im;

  im.draw("1_imagen.png");
  im.move_left(300);
  sleep(1);
  im.draw("2_imagen_desplazada_300left.png");
  im.move_right(300);
  sleep(1);
  im.draw("3_imagen_desplazada_300right.png");
  im.move_up(300);
  sleep(1);
  im.draw("4_imagen_desplazada_300up.png");
  im.move_down(300);
  sleep(1);
  im.draw("5_imagen_desplazada_300down.png");
  im.rotate();
  sleep(1);
  im.draw("6_imagen_rotada.png");
  
  // Test undo
  im.undo();
  sleep(1);
  im.draw("7_imagen_undo.png");

  // Test redo
  im.redo();
  sleep(1);
  im.draw("8_imagen_redo.png");

  // Test repeat
  im.repeat();
  sleep(1);
  im.draw("9_imagen_repeat.png");

  // Test repeat_all
  im.repeat_all();

  /* NOTA 1: Si usan el mismo nombre para las imágenes, entonces cada llamada al
  método draw() sobreescribirá a la imagen */

  /* NOTA 2: Si usan un ciclo while(1) y usan el mismo nombre de imagen,
  entonces se puede generar un efecto de desplazamiento continuo (algo así como
  un video =D ) */
  return 0;
}
