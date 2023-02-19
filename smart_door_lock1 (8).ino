#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

//#define Password_Length 5

const int Password_Length=5;

Servo myservo;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

int pos = 0;

char Data[15]; //neka korisnik kuca koliko hoce znakova
char Master[Password_Length] = "1234";
int data_count = 0, master_count = 0;

bool door = false;


bool obrisanaSifra=false;
bool postavljenaNova=false;
char kopijaMastera[Password_Length];
char adminPass[5]="5555";
char adminPassUnos[5];
bool adminPassTacan=false;
int brojac_admin=0;
int brojac_admin_unos=0;

/*---preparing keypad---*/

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);


bool nijePritisnutoSlovo=true;


void setup()
{
  pinMode(13,OUTPUT);
  myservo.attach(9);
  myservo.write(0);
  lcd.begin(16, 2); 
}


void loop()
{
    /*
  if (door == true)
  {
    customKey = customKeypad.getKey();
    if (customKey == 'A') //kucajuci ovo slovo zatvaramo vrata
    {
      lcd.clear();
      ServoClose();
      lcd.print("Door is closed");
      //delay(3000);
      door = false;
      lcd.begin(16,2);
    }
  }
  else
    Open();
  */
  
  if(door==false) Open();
}

void clearData()
{
  while (data_count != 0)
  { 
    Data[data_count--] = 0;
  }
  return;
}

void clearMaster()
{
  while(master_count!=0)
  {
    Master[master_count--] = 0;
  }
  return;
}

void clearAdminUnos(){
  while (brojac_admin_unos != 0)
  { 
    adminPassUnos[brojac_admin_unos--] = 0;
  }
  return;
}

void ServoClose()
{
  //pos=position=goes from 90 degrees to 0 degree
  for (pos = 90; pos >= 0; pos -= 1) { 
    myservo.write(pos);
  }
}

void ServoOpen()
{
  for (pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);  
  }
}

void kopirajSifru(){
    for (int i=0; i<Password_Length; i++)
            kopijaMastera[i] = Master[i];
}

void vratiStaruSifru(){
    for (int i=0; i<Password_Length; i++)
            Master[i] = kopijaMastera[i];
}


void Open()
{
 
  lcd.setCursor(0, 0);
  lcd.print("Enter Pass");
  
  char customKey = customKeypad.getKey();
  
  if(customKey=='#'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Admin Pass:");
    while(adminPassTacan==false)
    {
       char key=customKeypad.getKey();
         if(key>='0' && key<='9' && brojac_admin_unos<4){
          adminPassUnos[brojac_admin_unos] = key;
          lcd.setCursor(brojac_admin_unos, 1);
          lcd.print('*');
          brojac_admin_unos++;
         }
         else if(key=='D'){
            if (strcmp(adminPassUnos, adminPass)==0)
                adminPassTacan=true;
            else{
               lcd.clear();
               lcd.print(" Wrong Password ");
               delay(1000);
               clearAdminUnos();
               lcd.clear();
               lcd.print("Admin Pass:");
            }
         }
      else if(key=='C'){
        lcd.clear();
        lcd.print("Enter Pass");
        delay(1000);
        clearAdminUnos();
        adminPassTacan=true; //samo da izadjemo iz petlje
        postavljenaNova=true; //da ne nastavi dalje
      }
    }
    
    
    kopirajSifru();
    clearMaster();
    lcd.clear();
    lcd.setCursor(0,0);
    if(postavljenaNova==false)
      lcd.print("Enter New Pass");
    while(postavljenaNova==false)
    {
     char key=customKeypad.getKey();
      
      if(key>='0' && key<='9' && master_count<Password_Length-1){
         Master[master_count] = key;
         lcd.setCursor(master_count, 1);
         lcd.print('*'); 
         master_count++;
      }
      else if(key=='D'){
        lcd.clear();
        lcd.print("Enter Pass");
        postavljenaNova=true;
      }
      else if(key=='C'){
        clearData();
        vratiStaruSifru();
         lcd.clear();
        lcd.print("Enter Pass");
        postavljenaNova=true; //da izadjemo iz petlje
      }
    }
    clearAdminUnos();
    postavljenaNova=false;
    adminPassTacan=false;
  }
  
            
          
  
  if (customKey>='0' && customKey<='9' && data_count<14) //jer se moze samo 14 znakova upisati
  {
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print('*'); //Data[data_count]
    data_count++;
  }

  else if (customKey=='D') //if (data_count == Password_Length - 1)
  {
    if (strcmp(Data, Master)==0)  //vraca nula ako su oba stringa ista
    {
      lcd.clear();
      ServoOpen();
      lcd.print(" Door is Open ");
      door = true;
      digitalWrite(13,HIGH);
      delay(6000);
      
      //sljedece linije izbrisati ako zelim da manuelno zakljucam vrata
      //a u loop dijelu odkomentarisati veliki komentar sa dva if-a
      lcd.clear();
      lcd.print(" Time is up! ");
      delay(1000);
      digitalWrite(13,LOW);
      ServoClose();
      door = false;   
    }
    else
    {
      lcd.clear();
      lcd.print(" Wrong Password ");
      door = false;
    }
    delay(1000);
    lcd.clear();
    clearData();
  }
            
              
}
