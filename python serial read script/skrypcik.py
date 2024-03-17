import serial
import time


start = time.time() #definicja timera
read_time = 10 # czas zbierania odczytów
 
f=open(r"C:\Users\mkkurkowicz\Desktop\knr\odczyty.csv","w") #adres pliku, rodzaj dostepu do pliku
#by nie usuwac wszystkich poprzednich pomiarow nalezy zmienić "w" na "a"
ser = serial.Serial(
    port='COM3',
    baudrate=115200,
    parity = serial.PARITY_NONE) #definicja portu

t = time.localtime()
current_time = time.strftime("%H:%M:%S", t)
f.write(current_time) #zapisanie aktualnej godziny do pliku z odczytami
f.write("\nX(G),Y(G),Z(G),X(A),Y(A),Z(A),T(ms)\n")





while time.time() < start + read_time: #TRUE: #odczyty będą prowadzone przez okreslony czas
    value = ser.readline()
    valueInString=str(value,'UTF-8')
    print(valueInString)
   
    f.write(valueInString) #zapisywanie odczytow do pliku
    
    
    
f.close()