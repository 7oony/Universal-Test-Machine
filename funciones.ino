int    pasos_motor = 200; //165
//int    pasos_motor = 26900;
int micro_segundos = 500;

const int dirPin  = 7; //D11;    // DIR
const int stepPin = 8; //D12;   // STEP
int        enable = 9;

char estado = 'e';
float Kgf=0;

int  boton_stop = 10;
int     final_1 = 11;
int     final_2 = 12;
//------------------------------------------
float kg_inicial=0;
float constante= 0.0074444444;
float deformacion=0;
//-------------------------------------------

void setup() 
       {
          Serial.begin(115200);
          configuracion_celda();                      
          pinMode(stepPin,OUTPUT); 
          pinMode(dirPin,OUTPUT);
          pinMode(enable,OUTPUT);
          pinMode(final_1,INPUT);
          pinMode(final_2,INPUT);
          pinMode(boton_stop,INPUT);
          
        }// fin de:  void setup()
   
void loop() 
   {
            if(estado=='t')
               {
                 baja();
                 while(1){digitalWrite(enable, HIGH);}
               }          

            if(estado=='c')
               {
                 corte();
               }
            
            if(estado=='s')
               {
                  sube();
               }
                   
            if(estado=='b')
                   {
                      baja();
                   }
                   
            if(estado=='e') {digitalWrite(enable, HIGH);} // apagado

            
          if (digitalRead(boton_stop)==HIGH)
             {       
              estado='e';          
             } 

        else{
              if(Serial.available())
                   {
                       switch(Serial.read())
                              {                      
                                 
                                  case '1':                //corte                
                                             estado='c';                                                                                                        
                                             break; 
                                  case '2':              
                                             estado='t';                              
                                             break; 
                                  case '3':               
                                             //estado='p';
                                             break;  
                                  case '4':              // sube
                                             estado='s';                                    
                                             break; 
                                  case '5':              // baja
                                             estado='b';                                     
                                             break;                                                                                                                 
                                  case '6':              
                                             estado='e';                                                                 
                                             break;                                                         
        
                             }// fin del switch                              
                   } // fin de serial habilitado                  
            }

   } // fin de:  void loop()


#include "HX711.h"
#define SCK  A4  
#define  DT  A5  
HX711 balanza;
float ESCALA = 130;



void configuracion_celda()
        {
          balanza.begin(DT, SCK);
          balanza.set_scale(ESCALA); 
          balanza.tare();
          delay(1000);  
       }
       





void medir_gramos()
            {
              Kgf = abs(balanza.get_units(10)/1000); //  si se quiere la medida en gramos, hacer una division dentro de 1 en vez de 1000.                                          
            }






void baja()
           {
              digitalWrite(enable, LOW);
              digitalWrite(dirPin, LOW);
              for(int x = 0; x < pasos_motor; x++) 
                   {
                    digitalWrite(stepPin, HIGH); 
                    delayMicroseconds(micro_segundos); 
                    digitalWrite(stepPin, LOW); 
                    delayMicroseconds(micro_segundos); 
                   }
            }


            

void sube()
        {
            digitalWrite(enable, LOW);
            digitalWrite(dirPin, HIGH); 
            for(int x = 0; x < pasos_motor; x++) 
                 {
                    digitalWrite(stepPin, HIGH); 
                    delayMicroseconds(micro_segundos); 
                    digitalWrite(stepPin, LOW); 
                    delayMicroseconds(micro_segundos); 
                 }    
         }







void corte()
   {

        if (digitalRead(boton_stop)==HIGH)
                                        {
                                        digitalWrite(enable, HIGH);
                                        estado='e';
                                        }
        sube();
        medir_gramos();   
        
         if(Kgf>=0.1)
                 { 
                  Serial.println("1");
                  while(1){
                            
                            if (digitalRead(boton_stop)==HIGH)
                                                               {
                                                                digitalWrite(enable, HIGH);
                                                                Kgf=22;
                                                                break;
                                                                }
                            if(Kgf>=23){
                                           Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));
                                           break;}
                            if(kg_inicial>=Kgf){
                                           Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));
                                           break;}                            
                            //sube(); 
                            baja();                                             
                            medir_gramos();
                            deformacion=deformacion+constante;
                            kg_inicial=Kgf;
                            //sube();
                            baja(); 
                            medir_gramos();
                            deformacion=deformacion+constante;
                            
                            Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));
                            //Serial.println(String(" kg_inicial:  ")+String(kg_inicial));
                                                                                                                                        
                           }// fin de while
                   
                   while(1)
                            {
                            if (digitalRead(boton_stop)==HIGH)
                                                               {
                                                                digitalWrite(enable, HIGH);
                                                                Kgf=22;
                                                                break;
                                                                }
                              
                              if(Kgf>=23){
                                           Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));
                                           break;}
                              if(Kgf<=kg_inicial/10){
                                           Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));
                                           break;}                                                              
                              if(Kgf>=kg_inicial){
                                           Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));
                                           break;}                                
                              
                              //sube();
                              baja();
                              medir_gramos();                                                                                         
                              deformacion=deformacion+constante;
                              Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));
                              //Serial.println(String(" kg_inicial:  ")+String(kg_inicial));                              
                            }

                   digitalWrite(enable, HIGH);
                   deformacion=0;
                   int dato_stop=111;
                   Kgf=0;
                   kg_inicial=0; 
                   Serial.println( String(dato_stop)+ String(" , ") + String(dato_stop));                  
                   estado='e';
                   
                   while(1)
                            {
                              if(Serial.read()=='6'){break;}
                            }
                 }

    }      














                   
