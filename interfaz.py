import matplotlib.pyplot as plt
import matplotlib.animation as animation
from tkinter import*
import serial
import time
import sys
import itertools
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from tkinter import ttk


puerto  = serial.Serial(port='/dev/ttyUSB0', baudrate = 115200)

   
# borra los datos de la ultima prueba 
f = open('datos.txt', 'w')
f.close()   
esfuerzo=0
deformacion=0


#configuracion de la grafica a mostrar
fig = plt.figure(figsize=(5,10))
fig.facecolor=(.18, .31, .31)
fig.subplots_adjust(left=0.15, right=0.95)
ax1 = fig.add_subplot(1,1,1)



# fucion que grafica los datos recibidos 
    
def graficar():

    f = open('datos.txt', 'w')
    f.close()

    # a la hora de entrar aca, se matiene leyendo datos continuamente tipo un while
    # hasta que recibe el dato "111", con esto, sale y le pone stop a la grafica,
    # no la cierra para poder hacer otra prueba sin necesidad de cerrar el programa
    def animate(i):

        lectura= puerto.readline().decode()

        f = open('datos.txt', 'a')
        f.write(str(lectura))
        f.write("\n")
        f.close()

        pullData = open("datos.txt","r").read()
        dataArray = pullData.split('\n')
        xar = []
        yar = []
        for eachLine in dataArray:
            if len(eachLine)>1:
                x,y = eachLine.split(' , ')
                #----sale del ciclo---------------------------------------------------
                if float(y)==111 or float(x)==111:
                    puerto.write('6'.encode())                     
                    ani.event_source.stop()
                    print("lectura: ",lectura)
                    f = open('datos.txt', 'w')
                    f.close()
                    puerto.write('6'.encode()) 
                    break;
                #------------------------------------------------------                
                xar.append(float(x))
                yar.append(float(y))
                chat_1.delete(1)
                if float(x) <=0:
                    print("x: ",float(x))
                    x=1
                    young=float(y)/float(x)
                    print("x: ",float(x))
                
                chat_1.insert(END, '{:.2f}'.format(float(y)/float(x)))
                chat_2.delete(1)
                chat_2.insert(END, '{:.2f}'.format(float(y)))

             
        ax1.clear()
        ax1.plot(xar,yar, 'xkcd:black')
        ax1.plot(xar,yar,'ro-', color='red', linestyle='--')
        

        ax1.set_title('ESFUERZO  VRS  DEFORMACION', fontsize=13,color='green')
        ax1.set_ylabel("ESFUERZO (kgf)",fontsize=10, color='blue')
        ax1.set_xlabel("DEFORMACION (cm)",fontsize=10, color='blue')

        ax1.tick_params(labelcolor='tab:green')
        ax1.set_facecolor('#eafff5')



    try:
        # llama continuemente a la funcion que muestra la grafica
        ani = animation.FuncAnimation(fig, animate, interval=1,repeat = False)
        plt.show()
        
    except(UnicodeDecodeError, IndexError):
        pass
    except IOError:
        pass
        
        
    except ValueError:
        pass
        

    except ImportError:
        pass
        
        
    except EOFError:
        pass
        

    except KeyboardInterrupt:
        pass
         

    except:
        pass
            

#fuciones----------------------------------------
def up():
    puerto.write('4'.encode())
      
def down():
    puerto.write('5'.encode())

def pause():
    puerto.write('6'.encode())

def stop():
    ventana.quit()
    ventana.destroy()
    plt.close()

    puerto.write('6'.encode())    

def cort():
    
    puerto.write('1'.encode())
    while 1:
        lectura= puerto.read(1).decode()
        if lectura=='1':
            break
        
    print(lectura)
    graficar()
    puerto.write('6'.encode()) 

        
   
def tensi():
    puerto.write('t'.encode())
    graficar()            
            
def compre():
    puerto.write('c'.encode())
    graficar()


def reset():
    puerto.write('9'.encode())
    puerto.write('6'.encode()) 


# configuracion de la ventana pricipal---------------------------------------------------------------
# segun el boton presionado en la interfaz, como se muetra en la parte de abajo,
# llama a una funcion especifica de la que se han definido al principo
# cada una de las funciones solo manda una letra espeficica al arduino
# y ya en el archivo "principal" ya elege que funcion realizar, mediante la letra o numero enviado
# las partes llamadas chat, son los aparatados que muestran los datos numericos recibidos
    
ventana = Tk()
ventana.title("Esfuerzo Mecanico")
ventana.configure(bg="#662233") 
ventana.resizable(width=False, height=False)
ventana.geometry("250x440+545+0") 


texto_compresion = Label(ventana, text="CORTE", font=("Helvetica", 15), bg="blue"  ,fg="white")
texto_compresion.place(x=20, y= 150)

boton_compresion_on = Button(ventana ,text = "Iniciar", font=("Helvetica", 13), command=cort ,bg="blue",fg="white")
boton_compresion_on.place(x=25, y=183)




boton_stop= Button(ventana ,text = "Stop", font=("Helvetica", 15), command=stop ,bg="yellow",fg="black")
boton_stop.place(x=170, y=380)


texto_resultado = Label(ventana, text="Modulo de Young", font=("Helvetica", 14), bg="#662233" ,fg="white")
texto_resultado.place(x=20, y= 220)

chat_1 = Listbox(ventana, width=7, height=3,selectborderwidth=2, bg="white", fg="black",font='1000')
chat_1.place(x=50, y=250)
chat_1.insert(0, "")


texto_rotura = Label(ventana, text="Esfuerzo Rotura", font=("Helvetica", 14), bg="#662233" ,fg="white")
texto_rotura.place(x=20, y= 330)

chat_2 = Listbox(ventana, width=7, height=3,selectborderwidth=2, bg="white", fg="black",font='1000')
chat_2.place(x=50, y=360)
chat_2.insert(0, "")



texto_compresion = Label(ventana, text="AJUSTE", font=("Helvetica", 15), bg="#662233" ,fg="white")
texto_compresion.place(x=170, y= 5)

boton_s= Button(ventana ,text = "Subir" ,font=("Helvetica", 10), command=up ,bg="white",fg="blue")
boton_s.place(x=190, y=40)
boton_p= Button(ventana ,text = "Pausa", font=("Helvetica", 10), command=pause ,bg="white",fg="blue")
boton_p.place(x=187, y=75)
boton_b= Button(ventana ,text = "Bajar", font=("Helvetica", 10), command=down ,bg="white",fg="blue")
boton_b.place(x=190, y=110)

boton_r= Button(ventana ,text = "Reset", font=("Helvetica", 13), command=reset ,bg="black",fg="white")
boton_r.place(x=180, y=160)


ventana.mainloop()


f = open('datos.txt', 'w')
f.close()








