//--------------------[LCD]----------------------//
//Importem llibreria pel lcd
#include <LiquidCrystal.h>
LiquidCrystal monitor (2,3,4,5,6,7); //Creem una variable de la llibreria pel nostre LCD/Monitor
//--------------------[Servo]--------------------//
//Importem llibreria pel Servo
#include <Servo.h> 
// Declarem la variable per a controlar el servo que ens fará de pany
Servo pany;
//--------------[Structura de control]-----------//
//Declarem STRUCT de control
struct control {
	int  inici;//controla inici programa
	int  pos;//posició del servo
	int  porta;//status de laporta 0 oberta , 1 tancada
	int  alarma;//status de la alarma 0 desactivada, 1 activada //Pendent sensor PIR
  	int  contrasenya[2];//contrasenya 
};//struct
typedef control Control;
Control status;
//--------------------[So]----------------------//
//Definim So del Buzzer/Altaveu i asignem freqüència
#define S_Do 261 
//Declarem variables i ennllaçem amb la poscició buzzer
const int Buzzer = 8;
//-------------------[Input / Outputs]---------------------//
//Declarem variables i ennllaçem amb la poscició dels botons
const int B_OK = 11;
//Declarem variables i ennllaçem amb la poscició dels LEDS
const int LED_R = 13;
const int LED_V = 12;
//Declarem variables i ennllaçem amb la poscició del potenciometre
const int I_selector = A0;
int I_value = 0;
void setup()
{
	Serial.begin(9600);
	digitalWrite(LED_R,HIGH);//ENCENEM LED TECLAT NO DISPONIBLE
	//--------------------[LCD]----------------------//
	monitor.begin(16, 2);//Inicialitzem el display onfigurant 16 columnes per 2 files
	monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
	monitor.print("     Sistema");//Imprimim string per el lcd
	monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
	monitor.print("    Seguretat");//Imprimim string per el lcd
	delay(3000); //Esperem 3 segons
	monitor.clear(); //Esborrem 
	monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
	monitor.print("     Ruben RS");
	monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
	monitor.print("  UOC 2022-2023");//Imprimim string per el lcd
	delay(5000); //Esperem 5 segons
	monitor.clear(); //Esborrem 
	//--------------------[BOTONS]----------------------//
	pinMode(LED_V, OUTPUT);//definir pin Led Verd com a sortida
	pinMode(LED_R, OUTPUT);	//definir pin LEd Vermell com a sortida
	digitalWrite(LED_R,HIGH);//encenem LED Vermell -TECLAT NO DISPONIBLE
	//Definim pins/botons com a entrada i els inicialitzem	
	pinMode(B_OK, INPUT); 
	digitalWrite(B_OK,HIGH);
	pinMode(I_selector, INPUT); 
	digitalWrite(I_selector,HIGH);
	//--------------------[Servo]----------------------//
	// Iniciem el servo perquè comenci a treballar amb el pin 9
	pany.attach(9, 500, 2500);
	//--------------------[STATUS]----------------------//
	// Iniciem variable senyal de control 
	status.inici = 0;
	status.pos = 0;
	status.alarma = 0;
	status.porta = 1;
	status.contrasenya[0] = 20;
	status.contrasenya[1] = 38;
	status.contrasenya[2] = 24;
	digitalWrite(LED_R,LOW);//APAGUEM LED Vermell TECLAT NO DISPONIBLE
}//setup
void *obrir_pany(struct control *temp_status)
{  
	if (temp_status->porta == 1){
		float pos = temp_status->pos;
		float percentatge = 0;
		delay(1000);
		while (pos <=180){
			pany.write(pos);
			pos++;
			monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
			monitor.print("  Obrint Porta");//Imprimim string per el lcd
			monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
			percentatge = (pos/180)*100;
			if (percentatge < 10){
				monitor.print("       0");
				}else{
				monitor.print("       ");
			}// if percentatge <10
			monitor.print((int)percentatge);
			monitor.print("%");
			delay(100);
			monitor.clear(); //Esborrem      
		}//while servo
		temp_status->pos = 180;
		monitor.clear(); //Esborrem
		}else{
		monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
		monitor.print("     ERROR!");//Imprimim string per el lcd
		monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
		monitor.print("Porta ja oberta");//Imprimim string per el lcd
		delay(3000); //Esperem 3 segons
		monitor.clear(); //Esborrem      
	}//if(porta == 0)  
	temp_status->porta = 0;//indiquem porta Oberta (0)
}//funció obrir_pany
void *tancar_pany(struct control *temp_status)
{  
	if (temp_status->porta == 0){
		float pos = temp_status->pos;
		float percentatge = 0;
		float num = 0;
		delay(1000);
		while (pos >= 0){
			pany.write(pos);
			pos--;
			num++;
			monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
			monitor.print("  Tancant Porta");//Imprimim string per el lcd
			monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
			percentatge = (num/180)*100;
			if (percentatge < 10){
				monitor.print("       0");
				}else{
				monitor.print("       ");
			}// if percentatge <10
			monitor.print((int)percentatge);
			monitor.print("%");
			delay(100);
			monitor.clear(); //Esborrem      
		}//while servo
		temp_status->pos = 0;
		monitor.clear(); //Esborrem
		}else{
		monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
		monitor.print("     ERROR!");//Imprimim string per el lcd
		monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
		monitor.print("Porta ja tancada");//Imprimim string per el lcd
		delay(3000); //Esperem 3 segons
		monitor.clear(); //Esborrem      
	}//if(porta == 0)  
	temp_status->porta = 1;//indiquem porta tancada _(1)
}//funció obrir_pany
void alarma_so(){
	float sin_val = 0;
	int to_val = 0;
	int i = 0;
	int cont = 0;
	monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
	monitor.print("     Trucant");//Imprimim string per el lcd
	monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
	monitor.print("     Policia");//Imprimim string per el lcd
	delay(3000);
	while (cont <= 18000){
		sin_val = (sin(i*(3.1412/180)));
		to_val = 2000+(int(sin_val*1000));
		tone(Buzzer,to_val);
		delay(4);
		i++;
		if(i > 180){i = 0;}
		cont++;  
		if (i % 30 == 0)
		{
			digitalWrite(LED_R,HIGH);//ENcenem LED 	
			}else{
			digitalWrite(LED_R,LOW);//Apaguem LED 	
		}
	}//while 
	delay(30000);
	monitor.clear(); //Esborrem  
	digitalWrite(LED_R,LOW);//Apaguem LED Verd	
} //alarma_so
void *des_bloqueig(struct control *temp_status, int bloqueig){
	int intents = 3; //máxim número intents
	bool correcte = false;
	int part_contrasenya = 1;
	while (intents > 0 && correcte == false && part_contrasenya < 4 )
	{
		monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
		monitor.print("Introdueix la ");//Imprimim string per el lcd
		monitor.print(part_contrasenya);//Imprimim string per el lcd
		monitor.print(" part");//Imprimim string per el lcd
		delay(3000); //Esperem 3 segons
		while(digitalRead(B_OK) != LOW){
			I_value = (analogRead( I_selector))/10;//llegeix el pin d'entrada analogic
			//Serial.println(I_value);
			monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
			monitor.print("      [");//Imprimim string per el lcd
			if (I_value < 10){
				monitor.print("0");
			}// if I_value <10
			monitor.print(I_value);//Imprimim string per el lcd
			monitor.print("]");//Imprimim string per el lcd
          delay(100);
		}//while mentres no es premi botó OK
		monitor.clear(); //Esborrem  
		if (I_value == status.contrasenya[part_contrasenya - 1]){
			monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
			monitor.print(" ");//Imprimim string per el lcd
			monitor.print(part_contrasenya);//Imprimim string per el lcd
			monitor.print(" Contrasenya");//Imprimim string per el lcd
			monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
			monitor.print("    Correcte");//Imprimim string per el lcd
			delay(3000); //Esperem 3 segons   //// ATENCIO  ATENCIO ATENCIO        				
			monitor.clear(); //Esborrem  
			if(part_contrasenya == 3){
				correcte = true;  
				break;
			}//si la 3ra part es correcte
			part_contrasenya++;
			}else{
			intents--;
			if(intents == 0){
				alarma_so();//Activem so
				break;              
			};//Error al introduir contrasenya
			monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
			monitor.print(" ");//Imprimim string per el lcd
			monitor.print(part_contrasenya);//Imprimim string per el lcd
			monitor.print(" Contrasenya");//Imprimim string per el lcd
			monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
			monitor.print("  Incorrecte");//Imprimim string per el lcd
			delay(3000); //Esperem 3 segons      				
			monitor.clear(); //Esborrem  
			monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
			monitor.print("Intents restants");//Imprimim string per el lcd
			monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
			monitor.print("       0");//Imprimim string per el lcd
			monitor.print(intents);//Imprimim string per el lcd
			delay(3000); //Esperem 3 segons  // ATENCIO  ATENCIO ATENCIO    				
			monitor.clear(); //Esborrem  
		}//If comparacio contrasenya
		delay(3000); //Esperem 3 segons
		monitor.clear(); //Esborrem  
	}//while 
	if(intents == 0){
		monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
		monitor.print("     Alarma");//Imprimim string per el lcd
		monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
		monitor.print("    Activada");//Imprimim string per el lcd
		delay(3000); //Esperem 3 segons
		//llamar a funcion de sonido de alarma y que parpade en rojo//-------------------------//
		monitor.clear(); //Esborrem      
	}//if tots els intents esgotats
	if(correcte == true){
		if(bloqueig == 0){
			obrir_pany(&status); 	
			}else{
			tancar_pany(&status); 	
		}//comprobem si Bloquegem o desbloquegem
		//llamar a funcion de sonido como de final fantasy y que parpade leden verde//-------------------------//
	}//if tots els intents esgotats
	monitor.clear(); //Esborrem 
}//funció desbloqueig
void *menu(struct control *temp_status){
	bool selection = false;
	int opcio = 99;
	monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
	monitor.print("___MENU PORTA___");//Imprimim string per el lcd
	monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
	monitor.print("[OBRIR | TANCAR]");//Imprimim string per el lcd
	delay(3000); //Esperem 3 segons
	monitor.clear(); //Esborrem 
	while (selection != true )	{
		monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
		monitor.print("[OBRIR | TANCAR]");//Imprimim string per el lcd
		while(digitalRead(B_OK) != LOW){
			I_value = (analogRead( I_selector))/10;//llegeix el pin d'entrada analogic
			//Serial.println(I_value);
			monitor.setCursor(0,1); //Posem el cursor en la segona fila a l'esquerra
			if (I_value <= 73){
				monitor.print("      <--       ");//Imprimim string per el lcd
				opcio = 0;//Obrir porta
				}else{
				monitor.print("      -->       ");//Imprimim string per el lcd
				opcio = 1;//Tancar porta
			}// if I_value <73				
		}//while mentres no es premi botó OK
		delay(3000); //Esperem 3 segons
		monitor.clear(); //Esborrem  
		if (opcio == 0){ 
			des_bloqueig(&status, 0); //0 per desbloquejar i 1 per bloquejar
			}else if (opcio == 1){ 
            des_bloqueig(&status, 1); //0 per desbloquejar i 1 per bloquejar   
		}//comprobem si Bloquegem o desbloquegem
		delay(3000); //Esperem 3 segons
		monitor.clear(); //Esborrem      
		selection = true;//control
	}//while selection
}//funció menu	
void loop(){ 
	if( status.inici == 0){
		pany.write(0);
		status.inici = 1;
	}//control del pany,sense es mou erraticament  al iniciar 	
	while(digitalRead(B_OK) == LOW){ 
		 menu(&status); //Menu Principal
        break;
	}//Buto OK/MEnu
	if(status.porta == 0){
		digitalWrite(LED_R,HIGH);//ENcenem LED 
		digitalWrite(LED_V,LOW);//Apaguem LED 	
        monitor.setCursor(0,0); //Posem el cursor en la primera fila a l'esquerra
		monitor.print("  PORTA OBERTA");//Imprimim string per el lcd
		}else{
		digitalWrite(LED_V,HIGH);//ENcenem LED 
		digitalWrite(LED_R,LOW);//Apaguem LED 
		monitor.print(" PORTA TANCADA");//Imprimim string per el lcd
	}//status de laporta 0 oberta , 1 tancada
	delay(1000); //Esperem 1 segons
	monitor.clear(); //Esborrem 
	}// Bucle/Funció Loop						