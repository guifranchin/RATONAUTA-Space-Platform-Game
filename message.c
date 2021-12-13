#define TFT_DC    7
#define TFT_RST   8
#define SCR_WD   240
#define SCR_HT   240
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <EEPROM.h>

Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);
void menu();
void jogar();
void historia();
void setup(void)
{

  Serial.begin(9600);       
  lcd.init(SCR_WD, SCR_HT); 
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}



int botao1, botao2, botao3, botao1a, botao2a, botao3a, cont = 1, menuAlrPant = 0;

void loop() {  
   if (!menuAlrPant) {
    menuDrawn();
    cont = 1;
    menu(cont);
    menuAlrPant = 1;
  }


  botao1 = digitalRead(3);
  botao2 = digitalRead(4);
  botao3 = digitalRead(5);
  delay(1);
  botao1a = digitalRead(3);
  botao2a = digitalRead(4);
  botao3a = digitalRead(5);

  if (botao2 != botao2a) {
    cont++;
    if (cont == 5) {
      cont = 1;
    }
    menu(cont);
  }

  if (botao1 != botao1a) {
    cont--;
    if (cont < 1) {
      cont = 4;
    }
    menu(cont);
  }



  if (botao3 != botao3a) {
    menuAlrPant = 0;
    switch (cont) {
      case 1:
        jogar();
        break;
      case 2:
        historia();
        break;
      case 3:
        ranking();
        break;
      case 4:
        creditos();
        break;
    }
  }
}

void menu(int cont) {
  writeMenu("JOGAR", 75, 120, 3.5, 1 - cont);
  writeMenu("HISTORIA", 50, 145, 3.5, 2 - cont);
  writeMenu("RANKING", 60, 170, 3.5, 3 - cont);
  writeMenu("CREDITOS", 50, 195, 3.5, 4 - cont);
}

void writeMenu(char *text, int X, int Y, int FontSize, int Blue) {
  lcd.setCursor(X, Y);
  if (!Blue) {
    lcd.setTextColor(RGBto565(161, 190, 247));
  } else {
    lcd.setTextColor(RGBto565(105, 105, 105));
  }
  lcd.setTextSize(FontSize);
  lcd.println(text);
}

void historia() {
  lcd.fillRect(0, 0, 240, 240, RGBto565(0, 0, 0));
  write("Um adoravel ratinho que, apos se jogar  ao espaco, ira ter  sua aventura. O ratinho entao reune seus esforcos para desviar de meteoros. Conseguira o pobre ratinho sobriver Lua?", 0, 60, 2, 1);
  while (1) {
    botao2 = digitalRead(4);
    delay(1);
    botao2a = digitalRead(4);

    if (botao2 != botao2a)
      break;
  }
}


void write(char *text, int X, int Y, int FontSize, int Selected) {
  lcd.setCursor(X, Y);
  if (Selected == 0) {
    lcd.setTextColor(RGBto565(140, 97, 97));
  } else {
    lcd.setTextColor(WHITE);
  }
  lcd.setTextSize(FontSize);
  lcd.println(text);
}

int endereco1, endereco2, ponto;
void ranking() {
  typedef struct {
    char nome[4];
    int ponto;
  } User;

  lcd.fillRect(0, 0, 240, 240, RGBto565(255, 255, 255));
  lcd.fillRect(7, 7, 226, 226, RGBto565(13, 26, 77));

  int z = 30, y = 40;
  User Users[2];
  int pos = 0;
  int ponto;

  for (int i = 0; i < 15; i += 5) {
    for (int j = 0; j < 3; j++) {
      Users[pos].nome[j] = EEPROM.read(i + j);
    }
    Users[pos].nome[3] = '\0';

    endereco1 = EEPROM.read(i + 3);
    endereco2 = EEPROM.read(i + 4);
    ponto = (endereco1 * 256) + endereco2;

    write("Nome: ", z, y, 2.3, 0);
    write("Pontos:", z, y + 30, 2.3, 0);
    writeNome(Users[pos].nome, z + 70, y, 2.3);
    writeRanking(ponto, z + 80, y + 30, 2.3);
    y += 60;
    pos++;
  }

  while (1) {
    botao2 = digitalRead(4);
    delay(1);
    botao2a = digitalRead(4);

    if (botao2 != botao2a)
      break;
  }
}

void writeNome(char *Name, int X, int Y, int FontSize) {
  lcd.setCursor(X, Y);
  lcd.setTextColor(BLACK);
  lcd.setTextSize(FontSize);
  lcd.println(Name);
}


void writeRanking(int Value, int X, int Y, int FontSize) {
  lcd.setCursor(X, Y);
  lcd.setTextColor(RGBto565(90, 90, 90));
  lcd.setTextSize(FontSize);
  lcd.println(Value);
}


void creditos() {
  lcd.fillRect(0, 0, 240, 240, RGBto565(255, 255, 255));
  write("Quatro--", 50, 40, 3.5, 0);
  write("Emilly", 50, 80, 3.5, 0);
  write("Guilherme", 50, 120, 3.5, 0);
  write("Maria", 50, 160, 3.5, 0);

  while (1) {
    botao2 = digitalRead(4);
    delay(1);
    botao2a = digitalRead(4);
    if (botao2 != botao2a)
      break;
  }

}



void jogar() {
  int pontuacao = 0;
  int x = 125;
  int y = 200;
  int tb;
  int obsy = 0;
  int obs = random(0, 240);
  int ta = millis();
  lcd.fillRect(0, 0, 240, 240, RGBto565(0, 0, 0));
  rato(x);
  while (1) {

    lcd.setTextColor(RGBto565(255, 255, 255));
    lcd.setCursor(200, 10);
    lcd.print(pontuacao);
    tb = millis();
    if ((tb - ta ) > 50) { //ja passou 50 MS?
      ta = tb;
      obsy += 5;
      lcd.fillCircle(obs, obsy - 5, 5, RGBto565(0, 0, 0));
      lcd.fillCircle(obs, obsy, 5, RGBto565(255, 255, 255));
      if (obsy > 240) {
        obsy = 0;
        obs = random(0, 240);
        pontuacao++;
      }

      if (obs >= x - 20 && obs <= x + 23 && obsy >= 155) {
        //lcd.drawLine(x - 20, 0, x - 20, 240, RGBto565(255, 255, 255));
        //lcd.drawLine(x + 23, 0, x + 23, 240, RGBto565(255, 255, 255));
        //lcd.drawLine(0, 155, 240, 155, RGBto565(255, 255, 255));
        //Debug
        lcd.fillRect(0, 0, 240, 240, RGBto565(255, 0, 0));
        lcd.setTextColor(RGBto565(0, 0, 0));
        lcd.setCursor(40, 90);
        lcd.print("GAME");

        lcd.setCursor(100, 120);
        lcd.print("OVER!");
        lcd.setCursor(200, 10);
        lcd.print(pontuacao);
        delay(2000);
        rankingInsert(pontuacao);
        break;
      }
      lcd.fillRect(200, 0, 240, 35, RGBto565(0, 0, 0));
    }


    botao1 = digitalRead(3);
    botao2 = digitalRead(4);
    botao3 = digitalRead(5);
    delay(1);
    botao3a = digitalRead(5);
    botao2a = digitalRead(4);
    botao1a = digitalRead(3);


    if (x > 220) {
      x = 220;
    }
    if (x < 10) {
      x = 20;
    }

    if ( botao1 != botao1a) {
      x -= 15;
      lcd.fillRect(x - 15, y - 55, 60, 90, RGBto565(0, 0, 0));
      rato(x);

    }


    if ( botao2 != botao2a) {
      x += 15;
      lcd.fillRect(x - 45, y - 55, 60, 90, RGBto565(0, 0, 0));
      rato(x);
    }


  }
}


void rankingInsert(int pontuacao) {

  int x = 35, y = 70, Cursor = 0, letra = 64;
  char nick[4];
  nick[3] = '\0';
  lcd.fillRect(0, 0, 240, 240, RGBto565(134, 128, 249));
  lcd.fillRect(20, 115, 60, 5, RGBto565(255, 255, 255));
  lcd.fillRect(90, 115, 60, 5, RGBto565(255, 255, 255));
  lcd.fillRect(160, 115, 60, 5, RGBto565(255, 255, 255));
  while (1) {
    botao1 = digitalRead(3);
    botao2 = digitalRead(4);
    botao3 = digitalRead(5);


    if (botao2 == LOW) {
      x += 68;
      letra = 64;
      Cursor++;
      if (Cursor == 3) {     
        rankingUpdate(pontuacao, nick);
        break;
      }
    }

    if (botao1 == LOW) {
      x -= 68;
      letra = 64;
      Cursor--;
      if (Cursor < 0) {
        Cursor = 0;
        x += 68;
      }
    }


    if (botao3 == LOW) {
      letra++;
      lcd.fillRect(x, y, 30, 40, RGBto565(134, 128, 249));
      lcd.setCursor(x, y);
      lcd.setTextSize(5);
      lcd.println((char) letra);
      nick[Cursor] = letra;
    }

    delay(100);

  }
}


typedef struct {
  char nome[3];
  int ponto;
} User;

void rankingUpdate(int pontuacao, char nome[]) {
  int NUM_USERS = 3;
  User Users[NUM_USERS];
  int aux, pos = 0;

  for (int i = 0; i < 15; i += 5) {
    for (int j = 0; j < NUM_USERS; j++) {
      Users[pos].nome[j] = EEPROM.read(i + j);
    }
    pos++;
  }
  pos = 0;
  for (int i = 3; i < 15; i += 5) {
    endereco1 = EEPROM.read(i);
    endereco2 = EEPROM.read(i + 1);
    Users[pos].ponto = (int)(endereco1 << 8) | endereco2;
    pos++;
  }

  int posInsert = 100;
  for (int i = 0; i < NUM_USERS; i ++) {
    if (pontuacao > Users[i].ponto) {
      posInsert = i;
      break;
    }
  }
  if (posInsert < NUM_USERS) {
    for (int i = NUM_USERS - 1; i >= posInsert; i--) {
      Users[i + 1].ponto = Users[i].ponto;
      mystrcpy(Users[i + 1].nome, Users[i].nome, 3);
    }
    mystrcpy(Users[posInsert].nome, nome, 3);
    Users[posInsert].ponto = pontuacao;
  }


  pos = 0;
  for (int i = 0; i < 15; i += 5) {
    for (int j = 0; j < NUM_USERS; j++) {
      for (int k = 0; k < 3; k++) {
        EEPROM.write(i + k, Users[pos].nome[k]);
      }
      EEPROM.write(i + 3, (Users[pos].ponto * 256));
      EEPROM.write(i + 4, (Users[pos].ponto % 256));
    }
    pos++;
  }
}

void mystrcpy(char str1[], char str2[], int len) {
  for (int i = 0; i < len; i++) {
    str2[i] = str1[i];
  }
}


void menuDrawn() {
  lcd.fillRect(7, 7, 232, 232, RGBto565(0, 0, 0));       
  lcd.fillCircle(120, 7, 100, RGBto565(138, 81, 11));  
  lcd.fillCircle(29, 107, 4, RGBto565(255, 255, 255));
  lcd.fillCircle(215, 71, 3, RGBto565(255, 255, 255));
  lcd.fillCircle(210, 210, 1, RGBto565(255, 255, 255));
  lcd.fillCircle(180, 130, 2, RGBto565(255, 255, 255));
  lcd.fillCircle(42, 182, 2, RGBto565(255, 255, 255));


  lcd.drawLine(41, 91, 63, 67, RGBto565(255, 255, 255));
  lcd.drawLine(209, 20, 175, 51, RGBto565(255, 255, 255));
  lcd.drawLine(142, 85, 103, 120, RGBto565(255, 255, 255));
  lcd.drawLine(152, 21, 116, 62, RGBto565(255, 255, 255));
  lcd.drawLine(100, 192, 81, 211, RGBto565(255, 255, 255));
  lcd.drawLine(220, 142, 200, 162, RGBto565(255, 255, 255));
  lcd.drawLine(49, 14, 34, 30, RGBto565(255, 255, 255));



  lcd.setCursor(43, 35);
  lcd.setTextColor(RGBto565(248, 215, 154));
  lcd.setTextSize(3);
  lcd.println("RATONAUTA");

  lcd.fillRect(0, 0, 7, 240, RGBto565(13, 36, 71));    
  lcd.fillRect(0, 0, 240, 7, RGBto565(13, 36, 71));
  lcd.fillRect(233, 0, 240, 240, RGBto565(13, 36, 71));
  lcd.fillRect(0, 233, 240, 240, RGBto565(13, 36, 71));
}

int y = 180;
void rato(int x) {
  lcd.fillCircle(x - 15, y - 25, 10, RGBto565(255, 255, 255));
  lcd.fillCircle(x + 18, y - 20, 10, RGBto565(255, 255, 255));
  lcd.fillCircle(x - 15, y - 25, 9, RGBto565(233, 196, 185));
  lcd.fillCircle(x + 18, y - 20, 9, RGBto565(233, 196, 185));

  lcd.fillCircle(x, y, 20, RGBto565(158, 150, 158));
  lcd.fillCircle(x - 5, y + 3, 12, RGBto565(255, 169, 145));

  lcd.fillRect(x - 7, y + 20, 30, 20, RGBto565(158, 150, 158));
  lcd.fillRect(x - 7, y + 40, 5, 15, RGBto565(158, 150, 158));
  lcd.fillRect(x + 9, y + 40, 5, 15, RGBto565(158, 150, 158));

  lcd.fillRect(x - 12, y + 50, 5, 5, RGBto565(233, 196, 185));
  lcd.fillRect(x + 5, y + 50, 5, 5, RGBto565(233, 196, 185));

  lcd.fillCircle(x - 10, y, 2, RGBto565(0, 0, 0));
  lcd.fillCircle(x - 2, y, 2, RGBto565(0, 0, 0));

}


