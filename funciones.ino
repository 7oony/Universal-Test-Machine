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
        // Apaga la máquina si se presiona el boton de paro
        if (digitalRead(boton_stop)==HIGH)
                                        {
                                        digitalWrite(enable, HIGH);
                                        estado='e';
                                        }
        //sube la máquina hasta encontrar el valor indicado en el primer if, esto para tener seguridad de que la pieza ya esta sujetada
        sube();
        medir_gramos();   
        
         if(Kgf>=0.1)
                 {
                  // manda un 1 para indicar que la interfaz debe empezar  a imprimir los datos en pantalla
                  // sale cuando empieza a disminuir los valores de la celda de carga
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
                            sube();                                                                          
                            medir_gramos();
                            deformacion=deformacion+constante;
                            kg_inicial=Kgf;
                            sube();                            
                            medir_gramos();
                            deformacion=deformacion+constante;
                            
                            Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));                            
                                                                                                                                        
                           }// fin de while
                           
                   // sigur mandando datos hasta que se disminuye en un 10% del valor mas alto que alcanza en 
                   // el ciclo anterior
                   
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
                              
                              sube();                          
                              medir_gramos();                                                                                         
                              deformacion=deformacion+constante;
                              Serial.println( String(deformacion,9)+ String(" , ") + String(Kgf,9));
                                                           
                            }

                   // apaga el modulo del stepper y envía 111 para indicarle a python que termine de mostrar datos 
                    
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












                   
