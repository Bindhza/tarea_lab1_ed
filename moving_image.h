#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <stack>
#include <queue>
#include <string>
#include <cstdio>


// --------------------
// Integrantes:
// Benjamin Poblete
// Khristian Villalobos
// --------------------


// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image {
private:
  enum ActionType {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    ROTATE,
    UNDO_ACT,
    REDO_ACT,
    REPEAT_ACT
  };

  struct Action {
    ActionType type;
    int d;
  };

  std::stack<Action> undo_stack;
  std::stack<Action> redo_stack;
  std::queue<Action> history_queue;

  unsigned char **red_layer; // Capa de tonalidades rojas
  unsigned char **green_layer; // Capa de tonalidades verdes
  unsigned char **blue_layer; // Capa de tonalidades azules

public:
  // Constructor de la imagen. Se crea una imagen por defecto
  moving_image() {
    // Reserva de memoria para las 3 matrices RGB
    red_layer = new unsigned char*[H_IMG];
    green_layer = new unsigned char*[H_IMG];
    blue_layer = new unsigned char*[H_IMG];
    
    for(int i=0; i < H_IMG; i++) {
      red_layer[i] = new unsigned char[W_IMG];
      green_layer[i] = new unsigned char[W_IMG];
      blue_layer[i] = new unsigned char[W_IMG];
    }

    // Llenamos la imagen con su color de fondo
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
	red_layer[i][j] = DEFAULT_R;
	green_layer[i][j] = DEFAULT_G;
	blue_layer[i][j] = DEFAULT_B;
      }

    // Dibujamos el objeto en su posición inicial
    for(int i=0; i < 322; i++)
      for(int j=0; j < 256; j++) {
	if(!s_R[i][j] && !s_G[i][j] && !s_B[i][j]) {
	  red_layer[INIT_Y+i][INIT_X+j] = DEFAULT_R;
	  green_layer[INIT_Y+i][INIT_X+j] = DEFAULT_G;
	  blue_layer[INIT_Y+i][INIT_X+j] = DEFAULT_B;
	} else {
	  red_layer[INIT_Y+i][INIT_X+j] = s_R[i][j];
	  green_layer[INIT_Y+i][INIT_X+j] = s_G[i][j];
	  blue_layer[INIT_Y+i][INIT_X+j] = s_B[i][j];
	}
      }   
  }

  // Destructor de la clase
  ~moving_image() {
    for(int i=0; i < H_IMG; i++) {
      delete red_layer[i];
      delete green_layer[i];
      delete blue_layer[i];
    }

    delete red_layer;
    delete green_layer;
    delete blue_layer;
  }

  // Función utilizada para guardar la imagen en formato .png
  void draw(const char* nb) {
    _draw(nb);
  }

  // Función que similar desplazar la imagen, de manera circular, d pixeles a la izquierda
  void move_left(int d, bool track = true) {
    if (track) {
      undo_stack.push({MOVE_LEFT, d});
      while (!redo_stack.empty()) redo_stack.pop();
      history_queue.push({MOVE_LEFT, d});
    }
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	tmp_layer[i][j] = red_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = red_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	tmp_layer[i][j] = green_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = green_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	tmp_layer[i][j] = blue_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = blue_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	blue_layer[i][j] = tmp_layer[i][j];
  }
  //funcion para desplazar a la derecha
  void move_right(int d, bool track = true) {
    if (track) {
      undo_stack.push({MOVE_RIGHT, d});
      while (!redo_stack.empty()) redo_stack.pop();
      history_queue.push({MOVE_RIGHT, d});
    }
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
        tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG-d; j++)
            tmp_layer[i][j+d] = red_layer[i][j];      
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < d; j++)
            tmp_layer[i][j] = red_layer[i][W_IMG-d+j];      

    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG-d; j++)
            tmp_layer[i][j+d] = green_layer[i][j];      
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < d; j++)
            tmp_layer[i][j] = green_layer[i][W_IMG-d+j];      

    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG-d; j++)
            tmp_layer[i][j+d] = blue_layer[i][j];      
    
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < d; j++)
            tmp_layer[i][j] = blue_layer[i][W_IMG-d+j];      

    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            blue_layer[i][j] = tmp_layer[i][j];
}

  //mover hacia arriba
  void move_up(int d, bool track = true) {
    if (track) {
      undo_stack.push({MOVE_UP, d});
      while (!redo_stack.empty()) redo_stack.pop();
      history_queue.push({MOVE_UP, d});
    }
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
	      tmp_layer[i][j] = red_layer[i+d][j];      
    
    for(int i=0, k = H_IMG-d; i < d; i++,k++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[k][j] = red_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
	      tmp_layer[i][j] = green_layer[i+d][j];      
    
    for(int i=0, k = H_IMG-d; i < d; i++,k++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[k][j] = green_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
	      tmp_layer[i][j] = blue_layer[i+d][j];      
    
    for(int i=0, k = H_IMG-d; i < d; i++,k++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[k][j] = blue_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      blue_layer[i][j] = tmp_layer[i][j];

  }

  void move_down(int d, bool track = true) {
    if (track) {
      undo_stack.push({MOVE_DOWN, d});
      while (!redo_stack.empty()) redo_stack.pop();
      history_queue.push({MOVE_DOWN, d});
    }
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=d; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      tmp_layer[i][j] = red_layer[i-d][j];      
    
    for(int i=0, k = H_IMG-d; i < d; i++,k++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[i][j] = red_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=d; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      tmp_layer[i][j] = green_layer[i-d][j];      
    
    for(int i=0, k = H_IMG-d; i < d; i++,k++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[i][j] = green_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=d; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      tmp_layer[i][j] = blue_layer[i-d][j];      
    
    for(int i=0, k = H_IMG-d; i < d; i++,k++)
      for(int j=0; j < W_IMG; j++)
    	  tmp_layer[i][j] = blue_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      blue_layer[i][j] = tmp_layer[i][j];

  }

  //rota en 90 grados la imagen en sentido horario
  void rotate(bool track = true) {
    if (track) {
      undo_stack.push({ROTATE, 0});
      while (!redo_stack.empty()) redo_stack.pop();
      history_queue.push({ROTATE, 0});
    }
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
        tmp_layer[i] = new unsigned char[W_IMG];
    
    // Rotar la capa roja
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            tmp_layer[H_IMG-1-j][i] = red_layer[i][j];

    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            red_layer[i][j] = tmp_layer[i][j];

    // Rotar la capa verde
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            tmp_layer[H_IMG-1-j][i] = green_layer[i][j];

    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            green_layer[i][j] = tmp_layer[i][j];

    // Rotar la capa azul
    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            tmp_layer[H_IMG-1-j][i] = blue_layer[i][j];

    for(int i=0; i < H_IMG; i++)
        for(int j=0; j < W_IMG; j++)
            blue_layer[i][j] = tmp_layer[i][j];
  }

  void undo(bool track = true) {
    if (undo_stack.empty()) return;
    if (track) history_queue.push({UNDO_ACT, 0});
    Action act = undo_stack.top();
    undo_stack.pop();
    redo_stack.push(act);

    switch (act.type) {
      case MOVE_LEFT: move_right(act.d, false); break;
      case MOVE_RIGHT: move_left(act.d, false); break;
      case MOVE_UP: move_down(act.d, false); break;
      case MOVE_DOWN: move_up(act.d, false); break;
      case ROTATE: 
        rotate(false);
        rotate(false);
        rotate(false);
        break;
    }
  }

  void redo(bool track = true) {
    if (redo_stack.empty()) return;
    if (track) history_queue.push({REDO_ACT, 0});
    Action act = redo_stack.top();
    redo_stack.pop();
    undo_stack.push(act);

    switch (act.type) {
      case MOVE_LEFT: move_left(act.d, false); break;
      case MOVE_RIGHT: move_right(act.d, false); break;
      case MOVE_UP: move_up(act.d, false); break;
      case MOVE_DOWN: move_down(act.d, false); break;
      case ROTATE: rotate(false); break;
    }
  }

  void repeat(bool track = true) {
    if (undo_stack.empty()) return;
    if (track) history_queue.push({REPEAT_ACT, 0});
    Action act = undo_stack.top();

    undo_stack.push(act);
    while (!redo_stack.empty()) redo_stack.pop();

    switch (act.type) {
      case MOVE_LEFT: move_left(act.d, false); break;
      case MOVE_RIGHT: move_right(act.d, false); break;
      case MOVE_UP: move_up(act.d, false); break;
      case MOVE_DOWN: move_down(act.d, false); break;
      case ROTATE: rotate(false); break;
    }
  }

  void repeat_all() {
    moving_image temp_img;
    std::queue<Action> temp_queue = history_queue;
    int count = 1;
    while (!temp_queue.empty()) {
      Action act = temp_queue.front();
      temp_queue.pop();

      switch (act.type) {
        case MOVE_LEFT: temp_img.move_left(act.d, true); break;
        case MOVE_RIGHT: temp_img.move_right(act.d, true); break;
        case MOVE_UP: temp_img.move_up(act.d, true); break;
        case MOVE_DOWN: temp_img.move_down(act.d, true); break;
        case ROTATE: temp_img.rotate(true); break;
        case UNDO_ACT: temp_img.undo(false); break;
        case REDO_ACT: temp_img.redo(false); break;
        case REPEAT_ACT: temp_img.repeat(false); break;
      }

      char filename[50];
      sprintf(filename, "movie_%d.png", count++);
      temp_img.draw(filename);
    }
  }

private:
  // Función privada que guarda la imagen en formato .png
  void _draw(const char* nb) {
    //    unsigned char rgb[H_IMG * W_IMG * 3], *p = rgb;
    unsigned char *rgb = new unsigned char[H_IMG * W_IMG * 3];
    unsigned char *p = rgb;
    unsigned x, y;

    // La imagen resultante tendrá el nombre dado por la variable 'nb'
    FILE *fp = fopen(nb, "wb");

    // Transformamos las 3 matrices en una arreglo unidimensional
    for (y = 0; y < H_IMG; y++)
        for (x = 0; x < W_IMG; x++) {
            *p++ = red_layer[y][x];    /* R */
            *p++ = green_layer[y][x];    /* G */
            *p++ = blue_layer[y][x];    /* B */
        }
    // La función svpng() transforma las 3 matrices RGB en una imagen PNG 
    svpng(fp, W_IMG, H_IMG, rgb, 0);
    fclose(fp);
}

  
};

#endif
