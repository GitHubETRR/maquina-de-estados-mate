#define LED1 4
#define LED2 5
#define BOTON 6

unsigned long time;

void reboteMEF_cargar();  // Debe cargar el estado inicial.
void reboteMEF_actualizar();  // Debe leer las entradas, resolver la lógica detransición de estados y actualizar las salidas.
void boton_presionado();   
void boton_liberado();    

typedef enum{
  BOTON_UP,    //0 BOTON LIBERADO (LOW)
  BOTON_FALLING, //1 OCURRIENDO FLANCO DESCENDENTE
  BOTON_DOWN,        //2 BOTON PRESIONADO (HIGH)
  BOTON_RISING,  //3 OCURRIENDO FLANCO ASCENDENTE
} rebote_t;

rebote_t rebote; 

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BOTON, INPUT);
  time=millis();
  reboteMEF_cargar();
}

void loop()
{
  reboteMEF_actualizar();
}

void reboteMEF_cargar(){
  rebote = BOTON_DOWN;  // BOTON INICIALIZADO SIN PRESIONAR
}

void reboteMEF_actualizar()
{
  switch (rebote){
    case BOTON_UP:     // Pasa a Fallling si se presiona
        time=millis();
        if(digitalRead(BOTON)== HIGH){
            rebote=BOTON_FALLING;
        }
      break;
    case BOTON_FALLING:
        if(millis()-time>40){  //En caso de que se quiera que sea mas tiempo, se cambia el 40. 
                time=millis();       //Por ejemplo, al cambiar el tiempo 40 por un segundo, solo cambiara si un segundo entero esta presionado el boton.
                if(digitalRead(BOTON)== HIGH){
                    rebote = BOTON_DOWN;
                    boton_presionado();
                }else{
                        rebote=BOTON_UP;
                }  
         }
      break;
    case BOTON_RISING:
        if(millis()-time>40){
                time=millis();
                if(digitalRead(BOTON)== LOW){
                    rebote = BOTON_UP;
                    boton_liberado();
                }else{
                        rebote=BOTON_DOWN;
                  }  
         }
      break;
    case BOTON_DOWN:     // Pasa a Rising si se suelta
        time=millis();
        if(digitalRead(BOTON)==LOW){
            rebote=BOTON_RISING;
        }
      break;
  }
}

void boton_presionado(){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
}

void boton_liberado(){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
}

//Si está en estado UP y Pesionan el pulsador pasa al estado FALLING.
//Si está en estado FALLING, espera un tiempo de 40 ms y vuelve a leer el pulsador, 
//si realmente estaba presionado pasa al estado DOWN y llama a la función boton_presionado(); si no estaba presionado vuelve al estado UP.
//Si está en estado DOWN y suelta el botón pasa al estado RISING.
//Si está en estado RISING, espera un tiempo de 40 ms y vuelve a leer el pulsador,
//si realmente estaba liberado pasa al estado UP y llama a la función boton_liberado(); si no estaba liberado vuelve al estado DOWN.
