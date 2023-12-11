// Tic-Tac-Toe игра
//  ********************
#include <GL/glut.h>  // GLUT библиотека
#include <stdio.h>    // Стандардна I/O билбиотека
#include <stdlib.h>   // Стандардна библиотека
#include <math.h>     // Math библиотека
#include<string.h>    // String библиотека
//  ********************
// Глобални променливи в нашата игра
// Променлива за промяна на екрана, ако е избрана определена опция от менюто
int screen_option = 0;
// Променливи на мишката: Win_x_y = размер на прозорците, mouse_x_y = позиция на курсора, object_select = избрано поле
int mouse_x, mouse_y, Win_x, Win_y, object_select;
// Променлива за преминаване между ортогонален към перспективен изглед или обратно
static int view_state = 0;
// spin = За завъртане на X и O
int spin, spinboxes;
// Tic-Tac-Toe игра
// 'v' = view ortho/perspective // 'l' = lighting on/of
int win;
// Хода на играча се обозначава с променлива 'player', която = -1
// Хода на компютъра се обозначава с променлива 'computer', която е = 1
int player, computer;
// start_game показва, че играта е стартирала
int start_game;
// Указател за изграждане на "Х"
GLUquadricObj * Cylinder;

//  ********************
// Информация и променливи, свързани с матрицата

// На всичките 9 кутии са присвоени номера, които са показани по-долу
// 0 | 1 | 2
// 3 | 4 | 5
// 6 | 7 | 8
// Матрицата ще действа като игрална дъска, където всяка клетка е кутия или квадрат
int box_map[9];
// Всяка клетка/кутия в матрицата може да съхранява 3 различни стойности {-1,0,1}
// -1 = Играчът направи ход в това поле, така че ще се покаже като 'O' на екрана
// 1 = Компютърът направи ход в това поле, така че ще се покаже като 'X' на екрана на дисплея
// 0 = Свободен квадрат

// x,y координати на местоположение за всеки от 9-те квадрата
// Това ще помогне при идентифицирането кой квадрат е избран от позицията на курсора
int object_map[9][2] = {{-6,6},{0,6},{6,6},{-6,0},{0,0},{6,0},{-6,-6},{0,-6},{6,-6}};

// Всички възможни печеливши комбинации в матрицата
static int box[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

//  ********************
// Инциализиране на матрицата
void init_game(void) {
  int i;
  // Изчиства се матрицата за нова игра
  for (i = 0; i < 9; i++) {
    box_map[i] = 0;
  }
  // Първоначално се задава win = 0 защото няма още победител и start_game = 1
  win = 0;
  start_game = 1;
}

// Проверка за три подред елемента
// Връща 1 ако имаме победител, 2 ако е равенство и 0 ако още не е приклщчила играта
int check_move(void) {

  int i, t = 0;
  //Проверка за три елемнта подред
  for (i = 0; i < 8; i++) {
    // Взима се сумата от всичките три позиции в кутията от всичките осем печеливши позиции, съхранени в кутията
    t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]];
    if ((t == 3) || (t == -3)) {
      // Ротират с елементите в печелившата последователност
      spinboxes = i;
      // Връща се 1 ако имаме победител
      return (1);
    }
  }

  t = 0;
  // Проверка за равенство
  for (i = 0; i < 8; i++) {
    t = t + abs(box_map[box[i][0]]) + abs(box_map[box[i][1]]) + abs(box_map[box[i][2]]);
  }
  // Ако е равенство, връщаме 2
  if (t == 24) return (2);

  // Ако нямаме нищо, това означава, че играта не е приключила и връща се 0
  return (0);
}

// Лесен алгоритъм за откриване дали играчът може да спечели в следващия ход, след което го блокира
// Освен това, ако компютърът може да спечели в следващия ход, той ще направи този ход, за да спечели
int blocking_win() {
  int i, t;
  for (i = 0; i < 8; i++) {
    t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]];
    // Търсят се комбинации от всички 8 възможни печеливши последователности, в които две полета са запълнени и едно е свободно
    if ((t == 2) || (t == -2)) {
      // Ако се намери празно, ние присвояваме X в това поле в матрицата
      // Това означава, че компютърът е изиграл този ход в таова конкретн поле
      if (box_map[box[i][0]] == 0) box_map[box[i][0]] = computer;
      if (box_map[box[i][1]] == 0) box_map[box[i][1]] = computer;
      if (box_map[box[i][2]] == 0) box_map[box[i][2]] = computer;
      // Ако компютъра е играл, връща се 1
      return (1);
    }
  }
  // Ако компютъра не е играл, връща се 0
  return (0);
}

// Проверка за свободно място в ъгъла
int check_corner(void) {
  int i;
  if (box_map[0] == 0) {
    box_map[0] = computer;
    i = 1;
    return (1);
  }
  if (box_map[2] == 0) {
    box_map[2] = computer;
    i = 1;
    return (1);
  }
  if (box_map[6] == 0) {
    box_map[6] = computer;
    i = 1;
    return (1);
  }
  if (box_map[8] == 0) {
    box_map[8] = computer;
    i = 1;
    return (1);
  }
  return (0);
}

// Проверка за свободно място в реда
int check_row(void) {
  // Първо правиме проверка за центъра
  if (box_map[4] == 0) {
    box_map[4] = computer;
    return (1);
  }
  // След това проверяваме за останалите
  if (box_map[1] == 0) {
    box_map[1] = computer;
    return (1);
  }
  if (box_map[3] == 0) {
    box_map[3] = computer;
    return (1);
  }
  if (box_map[5] == 0) {
    box_map[5] = computer;
    return (1);
  }
  if (box_map[7] == 0) {
    box_map[7] = computer;
    return (1);
  }
  // Ако не намериме свободна позиция
  return (0);
}

// Логиката за действието на компютъра
int computer_move() {
  if (blocking_win() == 1) return (1);
  if (check_corner() == 1) return (1);
  if (check_row() == 1) return (1);
  return (0);
}

// Използваме тази функция за принтиране на екрана
void Sprint(int x, int y, char * st) {
  int l, i;
  // Гледаме колко знака ще има в низа
  l = strlen(st);
  // Място за започване на отпечатването на текста
  glRasterPos2i(x, y);
  for (i = 0; i < l; i++){
    // Отпечатваме знака на екрана
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
  }
}

// Функция, която създава завъртането на куба
static void TimeEvent(int te) {
  // Увеличаваме завъртането на куба с 1
  spin++;
  // Ако е над 360 градуса, започваме обратно от нула
  if (spin > 360) spin = 180;
  // Актуализация на екрана
  glutPostRedisplay();
  // Reset на таймера
  glutTimerFunc(8, TimeEvent, 1);
}

// Настройка на света
void init(void) {

  // Задаване на ClearColor
  glClearColor(0.0, 0.0, 0.0, 0.0);
  // Задаване на променливите на нашата играта
  start_game = 0;
  win = 0;

  // Create a new quadric
  // За изобразяване на "X"
  Cylinder = gluNewQuadric();
  gluQuadricDrawStyle(Cylinder, GLU_FILL);
  gluQuadricNormals(Cylinder, GLU_SMOOTH);
  gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
}

// Функция за изчертаване на "О" на екрана което се ротира когато играча спечели
void Draw_O(int x, int y, int z, int a) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(a, 0, 1, 0);
  glutSolidTorus(0.5, 2.0, 20, 16);
  glPopMatrix();
}

// Функция за изчертаване на "Х" на екрана което се ротира когато компютъра спечели
void Draw_X(int x, int y, int z, int a) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glPushMatrix();
  glRotatef(a, 0, 1, 0);
  glRotatef(90, 0, 1, 0);
  glRotatef(45, 1, 0, 0);
  glTranslatef(0, 0, -3);
  gluCylinder(Cylinder, 0.5, 0.5, 6, 16, 16);
  glPopMatrix();
  glPushMatrix();
  glRotatef(a, 1, 0, 0);
  glRotatef(90, 0, 1, 0);
  glRotatef(315, 1, 0, 0);
  glTranslatef(0, 0, -3);
  gluCylinder(Cylinder, 0.5, 0.5, 6, 16, 16);
  glPopMatrix();
  glPopMatrix();
}

// Функция за представяне на нашия свят на екрана
void display(void) {

  // Ако "Help" опцията е избрана
  if (screen_option == 3) {
    //Изчистване на екрана и задаване на цвета на фона
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    // Превключване към Project Matrix Work за показване на промените в екрана на дисплея
    glMatrixMode(GL_PROJECTION);
    // Изчистваме матрицата, за да премахнеме предишните промени
    glLoadIdentity();
    // Настройка на ортографският изглед
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);

    // Превключване към Model Matrix Work за рисуване
    glMatrixMode(GL_MODELVIEW);
    // Изчистваме матрицата, за да премахнеме предишните промени
    glLoadIdentity();
    // Деактивиране за избягване на цвета на материала за проследяване на текущия цвят (цвят на фона)
    glDisable(GL_COLOR_MATERIAL);
    // Задаваме цвета на шрифта
    glColor3f(0.0, 1.0, 1.0);
    // Показване на текста на екрана
    Sprint(-2, 2, "To Start the game:");
    Sprint(-2,1,"Right button => 'Start Game'");
    Sprint(-2,0,"---------------------");
    Sprint(-2,-1,"To Quit the game:");
    Sprint(-2,-2,"Right button => 'Quit'");
    // Разменяне на съдържанието на предния буфер със задния буфер
    glutSwapBuffers();
  }

  // Начална страница
  else if (screen_option == 0) {

    //Изтриваме всичко на екрана
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Превключване към Project Matrix Work за показване на промените в екрана на дисплея
    glMatrixMode(GL_PROJECTION);
    // Изчистваме матрицата, за да премахнеме предишните промени
    glLoadIdentity();
    // Настройка на ортографският изглед
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);

    // Превключване към Project Matrix Work за показване на промените в екрана на дисплея
    glMatrixMode(GL_MODELVIEW);
    // Изчистваме матрицата, за да премахнеме предишните промени
    glLoadIdentity();
    // Деактивиране за избягване на цвета на материала за проследяване на текущия цвят (цвят на фона)
    glDisable(GL_COLOR_MATERIAL);
    // Задаваме цвета на шрифта
    glColor3f(1.0, 1.0, 1.0);
    // Показване на текста на екрана
    Sprint(-2, 3,  "Tic-Tac-Toe Game");
    Sprint(-4, 2,  "Game was created by: Viktor Yordanov");
    Sprint(-3, 1, "Faculty Number: 123220015");
    Sprint(-3, -2, "Right Click to Start the Game");

    // Разменяне на съдържанието на предния буфер със задния буфер
    glutSwapBuffers();
  }

  else {
    int ix, iy;
    int i; // За for цикъла
    int j; // За настройка на въртенето на обектите
    // Почистване на екрана
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Превключване към Project Matrix Work за показване на промените в екрана на дисплея
    glMatrixMode(GL_PROJECTION);
    // Изчистваме матрицата, за да премахнеме предишните промени
    glLoadIdentity();
    // Настройка на ортографският изглед
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);
    // Превключване към Project Matrix Work за чертане
    glMatrixMode(GL_MODELVIEW);
    // Изчистваме матрицата, за да премахнеме предишните промени
    glLoadIdentity();

    // Деактивиране за избягване на цвета на материала за проследяване на текущия цвят (цвят на фона)
    glDisable(GL_COLOR_MATERIAL);
    // Задаваме цвета на шрифта
    glColor3f(1.0, 0.0, 0.3);

    // Какво ще се покаже на екрана при победа/загуба/равенство
    if (win == 1) Sprint(0, 0, " :)");
    if (win == -1) Sprint(0, 0, ":( ");
   // if (win == 2) Sprint(-2, 1, "It's a Tie");

    // Настройка на изгледа
    // За перспективния изглед
    if (view_state == 1) {
      glColor3f(1.0, 0.0, 0.3);
      Sprint(-2, 8, "Perspective View");
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(70, 1, 1, 30);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    }
    // За ортографския изглед
    else {
      glColor3f(1.0, 0.0, 0.3);
      Sprint(-1, 8, "Ortho View");
    }

    // За да получиме препратка към центъра на екрана на дисплея
    gluLookAt(0, 0, 20,
              0, 0, 0,
              0, 1, 0);
    // Изчертаване на мрежа за играта
    // 4 линии вертикално
    for (ix = 0; ix < 4; ix++) {
      glPushMatrix();
      glColor3f(1, 1, 1);
      glBegin(GL_LINES);
        glVertex2i(-9, -9 + ix * 6);
        glVertex2i(9, -9 + ix * 6);
      glEnd();
      glPopMatrix();
    }
    // 4 линии хоризонтално
    for (iy = 0; iy < 4; iy++) {
      glPushMatrix();
      glColor3f(1, 1, 1);
      glBegin(GL_LINES);
        glVertex2i(-9 + iy * 6, 9);
        glVertex2i(-9 + iy * 6, -9);
      glEnd();
      glPopMatrix();
    }

    // Изчертаване на обекти в квадратчетата
    for (i = 0; i < 9; i++) {
      j = 0;
      // Първо разглеждаме дали играта е готова или не
      if (abs(win) == 1) {
        // Проверяваме дали i е печеливша позиция в някоя от 8-те възможни печеливши последователности
        if ((i == box[spinboxes][0]) || (i == box[spinboxes][1]) || (i == box[spinboxes][2])) {
          j = spin;
        } else j = 0;
      }
      // В противен случай рисуваме или "Х" или "О" на екрана
      if (box_map[i] == 1) Draw_X(object_map[i][0], object_map[i][1], -1, j);
      if (box_map[i] == -1) Draw_O(object_map[i][0], object_map[i][1], -1, j);
    }

    // Разменяване на съдържанието на предния буфер със задния буфер
    glutSwapBuffers();
  }
}

// Функция, която се вика когато прозореца е преоразмерен
void reshape(int w, int h) {
  Win_x = w;
  Win_y = h;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

// За четене на въведеното от клавиатурата
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    // За превключване между изгледите
    case 'v':
      view_state = abs(view_state - 1);
      break;
    // За излизане от играта
    case 27:
      exit(0);
      break;
    default:
      break;
  }
}

// Работа с мишката
void mouse(int button, int state, int x, int y) {
  // Преобразуване на координатите на мишката на Windows във външни координати на openGL
  mouse_x = (18 * (float)((float) x / (float) Win_x)) / 6;
  mouse_y = (18 * (float)((float) y / (float) Win_y)) / 6;
  // Определяме номера на избраното поле
  object_select = mouse_x + mouse_y * 3;

  // Ако играта не е започнала
  if (start_game == 0) {
    // При натискане на левия бутон на мишката
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
      player = -1;
      computer = 1;
      init_game();
      return;
    }
  }
  // Ако играта продължава в момента
  if (start_game == 1) {
    // При натискане на левият бутон на мишката
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
      // Ако никой не е спечелил играта
      if (win == 0) {
        // Правиме проверка дали квадратчето е избрано или не
        if (box_map[object_select] == 0) {
          // Просвояваме "О" на това поле на дъската
          box_map[object_select] = player;
          // Проверка дали играчът е спечелил или не
          win = check_move();
          // Проверка дали играчът е спечелил
          if (win == 1) {
            start_game = 0;
            return;
          }
          // Ред на компютъра
          computer_move();
          // Проверка дали е спечелил или не
          win = check_move();
          if (win == 1) {
            win = -1;
            start_game = 0;
          }
        }
      }
    }
    }
  // Проверка дали мача е равен
  if (win == 2) start_game = 0;
}

// Настройка на менюто
void menu(int choice) {
  switch (choice) {
  // За стартиране на играта
  case 1:
    screen_option = 1;
    glutMouseFunc(mouse);
    break;
  // За промяна на viewport
  case 2:
    view_state = abs(view_state - 1);
    break;
  // За приказ на Help екрана
  case 3:
    screen_option = 3;
    glutMouseFunc(mouse);
    break;
  // Излизаме от играта
  case 4:
    exit(0);
    break;
  }
}

//  ********************
// Main програма
int main(int argc, char ** argv) {

  glutInit( & argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(850, 600);
  glutInitWindowPosition(10, 10);
  glutCreateWindow(argv[0]);
  glutSetWindowTitle("Tic-Tac-Toe Game Course Project CG");
  init();

  // Настройваме менюто за избор
  glutCreateMenu(menu);
  glutAddMenuEntry("Start Game", 1);
  glutAddMenuEntry("Prespective View", 2);
  glutAddMenuEntry("Help", 3);
  glutAddMenuEntry("Quit", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // Извикване на основните функции
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // Таймер функция за правилно въртене
  glutTimerFunc(50, TimeEvent, 1);

  glutMainLoop();
  return 0;
}

