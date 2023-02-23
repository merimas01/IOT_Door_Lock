#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

//#define Password_Length 5

const int Password_Length=5;

Servo myservo;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); //pinovi na arduinu za lcd

int pozicija = 0;

char Unos[15]; //neka korisnik kuca koliko hoce znakova
char IspravanPass[Password_Length] = "1234";
int unos_brojac = 0, ispravanpass_brojac = 0;

bool otvorena_vrata = false;


bool obrisanaSifra=false;
bool postavljenaNova=false;
char kopijaIspravneLozinke[Password_Length];
char adminPass[5]="5555";
char adminPassUnos[5];
bool adminPassTacan=false;
int brojac_admin=0;
int brojac_admin_unos=0;


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

void setup()
{
  pinMode(13,OUTPUT);
  myservo.attach(9);
  myservo.write(0); //pozicija
  lcd.begin(16, 2); 
}


void loop()
{
    /*
  if (vrata_otvorena == true)
  {
    customKey = customKeypad.getKey();
    if (customKey == 'A') //klikom na A zatvaramo vrata
    {
      lcd.clear();
      SpustiRampu ();
      lcd.print("Vrata zatvorena");
      //delay(3000);
      vrata_otvorena = false;
      lcd.begin(16,2);
    }
  }
  else
    OtvoriVrata();
  */
  
  if(otvorena_vrata==false) OtvoriVrata ();
}

void obrisiUnos()
{
  while (unos_brojac != 0)
  { 
    Unos[unos_brojac--] = 0;
  }
  return;
}

void obrisiIspravnuLozinku()
{
  while(ispravanpass_brojac!=0)
  {
    IspravanPass[ispravanpass_brojac --] = 0;
  }
  return;
}

void obrisiAdminUnos(){
  while (brojac_admin_unos != 0)
  { 
    adminPassUnos[brojac_admin_unos--] = 0;
  }
  return;
}

void SpustiRampu()
{
  //position=goes from 90 degrees to 0 degree
  for (pozicija = 90; pozicija >= 0; pozicija -= 1) { 
    myservo.write(pozicija);
  }
}

void PodigniRampu()
{
  for (pozicija = 0; pozicija <= 90; pozicija += 1) {
    myservo.write(pozicija);  
  }
}

void kopirajSifru(){
    for (int i=0; i<Password_Length; i++)
            kopijaIspravneLozinke[i] = IspravanPass[i];
}

void vratiStaruSifru(){
    for (int i=0; i<Password_Length; i++)
            IspravanPass[i] = kopijaIspravneLozinke[i];
}


void OtvoriVrata ()
{
 
  lcd.setCursor(0, 0);
  lcd.print("Unesi Pass");
  
  char customKey = customKeypad.getKey();
  
  if(customKey=='#'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Admin Pass:");
    obrisiUnos();
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
           if (strcmp(adminPassUnos, adminPass)==0){
             obrisiAdminUnos();
             adminPassTacan=true;
           }
            else{
               lcd.clear();
               lcd.print("Neispravan unos! ");
               delay(1000);
               obrisiAdminUnos();
               lcd.clear();
               lcd.print("Admin Pass:");
            }
         }
      else if(key=='C'){
        lcd.clear();
        lcd.print("Upisi Pass");
        delay(1000);
        obrisiAdminUnos();
        adminPassTacan=true; //samo da izadjemo iz petlje
        postavljenaNova=true; //da ne nastavi dalje
      }
       else if(key=='*'){  //za brisanje unosa ako smo pogrijesili
        obrisiAdminUnos();
        brojac_admin_unos=0;
        lcd.clear();
        lcd.print("Admin Pass:");        
      }
    }
    
    
    kopirajSifru();
    obrisiIspravnuLozinku();
    lcd.clear();
    lcd.setCursor(0,0);
    if(postavljenaNova==false){
      lcd.print("Upisi Novi Pass");
    }
    while(postavljenaNova==false)
    {
     char key=customKeypad.getKey();
      
      if(key>='0' && key<='9' && ispravanpass_brojac <Password_Length-1){
         IspravanPass[ispravanpass_brojac] = key;
         lcd.setCursor(ispravanpass_brojac, 1);
         lcd.print('*'); 
         ispravanpass_brojac ++;
      }
      else if(key=='D' && ispravanpass_brojac==4){
        lcd.clear();
       // obrisiUnos(); //.....
        lcd.print("Upisi Pass");
        postavljenaNova=true;
      }
      else if(key=='D' && ispravanpass_brojac!=4){
        lcd.setCursor(0,0);
        lcd.print("Ukucaj 4 broja! ");
        delay(2000);
        lcd.setCursor(0,0);
        lcd.print("Upisi Novi Pass");
      }
      else if(key=='C'){
       // obrisiUnos();
        vratiStaruSifru();
         lcd.clear();
        lcd.print("Upisi Pass");
        postavljenaNova=true;
      }
      else if(key=='*'){  //za brisanje unosa ako smo pogrijesili
        obrisiIspravnuLozinku();
        ispravanpass_brojac=0;
        lcd.clear();
        lcd.print("Upisi Novi Pass");        
      }
    }
    postavljenaNova=false;
    adminPassTacan=false;
  }
  
            
          
  
  if (customKey>='0' && customKey<='9')
  {
    Unos[unos_brojac] = customKey;
    lcd.setCursor(unos_brojac, 1);
    lcd.print('*'); //Unos[unos_brojac]
    unos_brojac++;
  }

  else if(customKey=='*'){  //za brisanje unosa ako smo pogrijesili
        obrisiUnos();
        unos_brojac=0;
        lcd.clear();
        lcd.print("Upisi Pass");        
  }
  
  else if (customKey=='D') //if (unos_brojac == Password_Length - 1)
  {
    if (strcmp(Unos, IspravanPass)==0)  //vraca nula ako su oba stringa ista
    {
      Serial.print(sizeof(Unos)/sizeof(Unos[0]));
      Serial.print(sizeof(IspravanPass)/sizeof(IspravanPass[0]));
      lcd.clear();
      PodigniRampu();
      lcd.print("Vrata otvorena.");
      otvorena_vrata = true;
      digitalWrite(13,HIGH);
      delay(6000);
      
      //sljedece linije izbrisati ako zelim da manuelno zakljucam vrata
      //a u loop dijelu odkomentarisati veliki komentar sa dva if-a
      lcd.clear();
      lcd.print("Isteklo vrijeme!");
      delay(2000);
      digitalWrite(13,LOW);
      lcd.clear();
      lcd.print("Vrata zatvorena.");
      delay(1000);
      SpustiRampu();
      otvorena_vrata = false;   
    }
    else
    {
      lcd.clear();
      lcd.print("Neispravan unos! ");
     // obrisiUnos(); //dodano
      otvorena_vrata = false;
    }
    delay(1000);
    lcd.clear();
    obrisiUnos();
  }
            
              
}
