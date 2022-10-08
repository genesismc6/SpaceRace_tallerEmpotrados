unsigned short score[] = {0,0};
unsigned short minutes = 64;
unsigned short seconds = 0; 
unsigned short areaX = 0;
unsigned short areaY = 0;
unsigned short centerShip = 0;
unsigned short debris_turn = 0;
unsigned short turno = 0;
unsigned short flag = 0;
unsigned short y = 0;

typedef struct players{//Pos inferior izquierda de la nave
    unsigned short x;
    unsigned short y;
}ships;

typedef struct debris{
    unsigned short x;
    unsigned short y;
    signed short dx;
    signed short dir;
	unsigned short pos;
}escombro;

ships ship[2];
escombro debris[14];

ships old_ship[2];
escombro old_debris[14];

char info[9];
char GLCD_DataPort at PORTD;
sbit GLCD_CS1 at LATB0_bit;
sbit GLCD_CS2 at LATB1_bit;
sbit GLCD_RS  at LATB2_bit;
sbit GLCD_RW  at LATB3_bit;
sbit GLCD_EN  at LATB4_bit;
sbit GLCD_RST at LATB5_bit;

sbit GLCD_CS1_Direction at TRISB0_bit;
sbit GLCD_CS2_Direction at TRISB1_bit;
sbit GLCD_RS_Direction  at TRISB2_bit;
sbit GLCD_RW_Direction  at TRISB3_bit;
sbit GLCD_EN_Direction  at TRISB4_bit;
sbit GLCD_RST_Direction at TRISB5_bit;


void draw_player(unsigned short x, unsigned short y,unsigned short color){
	//print('|')
        Glcd_V_Line(y,y+6, x,color);
        Glcd_V_Line(y,y+6, x+1,color);
        Glcd_V_Line(y,y+6, x+2,color);
        Glcd_V_Line(y,y+6, x+3,color);
        Glcd_V_Line(y,y+6, x+4,color);
        Glcd_Dot(x-1,y, color);
        Glcd_Dot(x-1,y+1, color);
        Glcd_Dot(x-2,y, color);
        Glcd_Dot(x+5, y,color);
        Glcd_Dot(x+5,y+1, color);
        Glcd_Dot(x+6, y, color);
        Glcd_Dot(x+5,y+6, color);
}

void draw_time(){
        Glcd_V_Line(minutes,0,64,0);
        Glcd_V_Line(0,minutes,64,1);
}

void erase_player(unsigned short x, unsigned short y,unsigned short direction){
        if(direction == 1){//hacia arriba/ borrar la parte de abajo de la nave
                Glcd_H_Line(x-2,x+6,y,0);
        }
        else if(direction == 2){//hacia abajo/ borrar la parte superior de la nave
                Glcd_H_Line(x, x+4, 0);
                
        }
}

void move_debris(){
        unsigned short i; //variable para la colision
        unsigned short j;//variable para controlar los asteroides
        
        if(debris_turn == 20){
                for(j = 0; j <= 14; j++){
                        if(debris[j].dir == 1){//asteroide moviendose de derecha a izquierda   <--
                                Glcd_Dot(debris[j].x,debris[j].y, 0);
								debris[j].x -= debris[j].dx;
								Glcd_Dot( debris[j].x,debris[j].y, 1);
                                if(debris[j].x <= 0){
									Glcd_Dot(debris[j].x,debris[j].y, 0);
                                    debris[j].x = 128;
                                }
                        }
                        else if(debris[j].dir == -1){//asteroide moviendose de izquierda a derecha -->
                                Glcd_Dot( debris[j].x,debris[j].y, 0);
								debris[j].x += debris[j].dx;
								Glcd_Dot( debris[j].x,debris[j].y, 1);
                                if(debris[j].x >= 128){
									Glcd_Dot(debris[j].x,debris[j].y, 0);
                                    debris[j].x = 0;
                                } 
                        }
                        
                        //Si un asteriode choca con una nave
                        for(i = 0; i < 2; i++){
                                if((debris[j].y >= ship[i].y) && (debris[j].x <= ship[i].y + 6) && (debris[j].x >= ship[i].x) && (debris[j].x <= ship[i].x + 6)){
                                        draw_player(ship[i].x, ship[i].y, 0);
                                        if(i == 0){
                                                ship[i].x = 40;
                                                ship[i].y = 60;
                                        }
                                        else if(i == 1){
                                                ship[i].x = 70;
                                                ship[i].y = 60;
                                        }
                                }
                        }
                }
        }        
}

void gen_debris(){
        unsigned short j;
        unsigned short posY = 3;
        for(j = 0; j < 15; j++){
                int side = rand() % 2;
                int posX = rand() % 128;
                //Dar direccion a los asteroides
                if(side = 0){
                        debris[j].dir = -1;
                }
                else if(side = 1){
                        debris[j].dir = 1;
                }
                //Dar ubicacion x y a los asteriodes
                debris[j].y = posY;
                debris[j].x = posX;
                posY += 3;
        }
}

void init(){
    ship[0].x = 40;
    ship[0].y = 60;

    ship[1].x = 70;
    ship[1].y = 60;

    //debris.x = 64;
    //debris.y = 32;
    //debris[].dx = 2;

    draw_player(ship[0].x, ship[0].y, 1);
    draw_player(ship[1].x, ship[1].y, 1);
    //gen_debris();
}

void check_winner(){
        if(minutes == 0){
                if(score[0] < score[1]){
                        //Glcd_Fill();
                        //Glcd_Write_Text();
                }
                else if(score[0] > score[1]) {
                        //Glcd_Fill();
                        //Glcd_Write_Text();
                }
                else if(score[0] == score[1]){
                        //Glcd_Fill();
                        //Glcd_Write_Text();
                }
                
        }
}

void move_ia(){
        unsigned short j;
        unsigned short p;
        unsigned short q;
        areaX = ship[1].x;
        areaY = ship[1].y;
        centerShip = ship[1].y + 4;
        
        //La IA va a tener un rango en los que percibe los asteroides
        //Si detecta un asteroide entrar, if(asteroide.x >= rango)
        //va a ver que pos en y tiene, si esta es arriba de cierto punto va a esperar o a retroceder
        //si es menor que este punto va a moverse hacia adelante, tener en cuenta que debe tender a 
        //moverse hacia adelante para que pueda ganar
        for(q = 0; q <= 5; q++){
                ship[1].y += 2;
        }
        for (p = 0; p <= 2; p++){
                if(p == 0){
                        areaX = areaX - 10;
                        areaY = areaY - 1;
                        
                        for (j = 0; j <= 15; j++){
                                if((debris[j].x >= areaX) && (debris[j].x <= ship[1].x + 12)){        
                                        if((debris[j].y >= areaY) && (debris[j].y <= ship[1].y + 6)){
                                                if(debris[j].y >= centerShip){
                                                        //mover hacia arriba 
                                                        ship[1].y += 2;
                                                        erase_player(ship[j].x, ship[j].y, 1);
                                                }
                                                else if(debris[j].y < centerShip){
                                                        //mover hacia abajo
                                                        ship[1].y -= 2;
                                                        erase_player(ship[j].x, ship[j].y, 2);
                                                }
                                        } 
                                }
                        }
                }
                else if( p == 1){
                        areaX = areaX + 16;
                        areaY = areaY + 6;
                        
                        for (j = 0; j <= 15; j++){
                                if((debris[j].x <= areaX) && (debris[j].x >= ship[1].x - 10)){        
                                        if((debris[j].y <= areaY) && (debris[j].y > ship[1].y)){
                                                if(debris[j].y >= centerShip){
                                                        //mover hacia arriba 
                                                        ship[1].y += 2;
                                                        erase_player(ship[j].x, ship[j].y, 1);
                                                }
                                                else if(debris[j].y < centerShip){
                                                        //mover hacia abajo
                                                        ship[1].y -= 2;
                                                        erase_player(ship[j].x, ship[j].y, 2);
                                                }
                                        }
                                }
                        }
                }
        }
}

void move_player(unsigned short i, unsigned short direction){
        if(turno == 10){
                if(direction == 2){//hacia abajo
                        if(ship[i].y >= 63){
                                ship[i].y = 63;
                        }
                        else{
                                ship[i].y += 2;
                        }
                }
                else if(direction == 1){//hacia arriba
                        if(ship[i].y <= 0){
                                score[i] += 1;
                        }
                        else{
                                ship[i].y -= 2;
                        }
                }
        }
}

void game_over(){
        if(score[0] > score[1]){
            while(1){
				if(Button(&PORTC,1,1,1)){ //Quiero volver a la portada como si acabara de encender la consola
					Glcd_Fill(0x00);
					break;
				}
			}
        }
        else if(score[0] < score[1]){
                //Glcd_Write_Text('PC WINS' ,,,)
            while(1){
				if(Button(&PORTC,1,1,1)){ //Quiero volver a la portada como si acabara de encender la consola
					Glcd_Fill(0x00);
					break;
				}
			}
        }
        else if(score[0] == score[1]){
                //Glcd_Write_Text('IT\'S A TIE' ,,,)
            while(1){
				if(Button(&PORTC,1,1,1)){ //Quiero volver a la portada como si acabara de encender la consola
					Glcd_Fill(0x00);
					break;
				}
			}
        }        
}
void data_pack() {  //Funcion para empaquetar datos a enviar     ///serial_pack_data //Hay que ver como hago para enviar la ubicacion en y de cada asteroide
      // INFO DE BOLA //
          unsigned short i = 0;
          for(i = 0; i <= 2; i++){
                if(i == 0){
                        info[0] = ship[0].y + '0';
                        info[1] = ship[1].y + '0';
                        info[2] = score[0] + '0';
                        info[3] = score[1] + '0';
                        info[4] = debris[0].pos + '0';  
                        info[5] = debris[1].pos + '0';
						info[6] = debris[2].pos + '0';
                        info[7] = 'A';
                        info[8] = 'P';        
                }
                else if(i == 1){
                        info[0] = debris[3].pos + '0';
                        info[1] = debris[4].pos + '0';
                        info[2] = debris[5].pos + '0';
                        info[3] = debris[6].pos + '0';
                        info[4] = debris[7].pos + '0';
                        info[5] = debris[8].pos + '0';
                        info[6] = debris[9].pos + '0';
                        info[7] = 'B';
                        info[8] = 'P';
                }
                else if(i == 2){
                        info[0] = debris[10].pos + '0';
                        info[1] = debris[11].pos + '0';
                        info[2] = debris[12].pos + '0';
                        info[3] = debris[13].pos + '0';
                        info[4] = debris[14].pos + '0';
                        info[6] ='C';
                        info[7] = 'P';
                        info[8] = 0;
                        info[8] = 0;
                }
            }
        }
}
  
void desdata_pack(unsigned short packet_number){   // Funcion para extraer datos del paquete recibido por esclavo
                if(packet_number == 0){
                        ship[0].y   =   info[0] - '0';
                        ship[1].y   =   info[1] - '0';
                        score[0]    =   info[2] - '0';
                        score[1]    =   info[3] - '0';
                        debris[0].pos =   info[4] - '0';
                        debris[1].pos =   info[5] - '0';
                        debris[2].pos =   info[6] - '0';
                }
                else if(packet_number == 1){
                        debris[3].pos =   info[0] - '0';
                        debris[4].pos =   info[1] - '0';
                        debris[5].pos =   info[2] - '0';
                        debris[6].pos =   info[3] - '0';
                        debris[7].pos =   info[4] - '0';
                        debris[8].pos =   info[5] - '0';
                        debris[9].pos =   info[6] - '0';        
                }
                else if(packet_number == 2){
                        debris[10].pos =   info[0] - '0';
                        debris[11].pos =   info[1] - '0';
                        debris[12].pos =   info[2] - '0';
                        debris[13].pos =   info[3] - '0';
                        debris[14].pos =   info[4] - '0';
                }
                
        }
}

void output_character(char charValue){   
     while (UART1_Tx_Idle()!= 1);
       UART1_Write(charValue);
}

void input_character(char char_dir){    
      while (UART1_Data_Ready() == 0);
      char_dir = UART1_Read();
}

void output_data(char *serial_dir){     
     while (UART1_Tx_Idle()!= 1);
     UART1_Write_Text(serial_dir);
}

void input_data(char *text_dir){  
	unsigned short i = 0;
    while(UART1_Data_Ready()==0);
    UART1_Read_Text(text_dir, "P", 255);
	 
	for(i = 0; i < 10; i++){
		 if(info[i] == 'A'){desdata_pack(0);}
		 else if(info[i] == 'B'){desdata_pack(1);}
		 else if(info[i] == 'C'){desdata_pack(2);}
	}
}

void coordinate_pos(){
	unsigned short i = 0;
	unsigned short pos_y = 0;
	unsigned short pos_x = 0;
	
	for(i = 0; i < 15; i++){
		pos_y = debris[i].y * 127;
		pos_x = debris[i].x;
		if (debris[i].y == 0){
			debris[i].pos = debris[i].x;
		}
		else if(debris[i].y != 0){
			debris[i].pos = pos_y + pos_x + 1;
		}
	}
}

void pos_coordinate(){
	unsigned short i = 0;
	unsigned short j = 0;
	
	for(i = 0; i < 15; i++){
				debris[i].y = debri[i].pos / 127;
				debris[i].x = abs(debris[i].y * 127 - debris[i].pos) -1 //abs(debris[i].y * 127 - debris[i].pos));
	}
}

void save_old_data(){
		unsigned short i = 0;
        old_ship[0].y = ship[0].y;
        old_ship[1].y = ship[1].y;
		for(i = 0; i < 15; i++){
			old_debris[i].x = debris[i].x;
			old_debris[i].y = debris[i].y;
			old_debris[i].dx = debris[i].dx;
			old_debris[i].dir = debris[i].dir;
		}
}
     
void main(){
        
    unsigned short Master = 0;
	unsigned short counter = 0;
	unsigned short counter2 = 0;
    char Master_slave = '0';
    char move_other = 0;
		
    PORTC = 0;              //Establecemos las entradas en 0 para evitar conflictos
    TRISC.F0 = 1;   //Entrada para el eje y
    TRISC.F1 = 1;        // Entrada para el boton del joystick
        
    Glcd_Init();
    Glcd_Fill(0x00);
    
    UART1_Init(9600);
    Delay_ms(1000);
    
    ADC_Init();
    
    while(1){
        switch(flag){
            case 0:
				while(1){
					y = ADC_Read(0);
					if(y <= 10){
						Glcd_Write_Text("one player",15,0,1);
						delay_ms(1000);
						while(1){
							y = ADC_Read(0);
							if(Button(&PORTC,1,1,0)){
								flag = 1;
								break;
							}
							else if(y >= 200){
								break;
							}
						}Glcd_Fill(0x00);
					}	
					else if(y >= 200){
						Glcd_Write_Text("multiplayer",15,0,1);
						delay_ms(1000);
						while(1){
							y = ADC_Read(0);
							if(Button(&PORTC,1,1,0)){
								flag = 2;
								Glcd_Fill(0x00);
								break;
							}
							else if(y <= 20){
								Glcd_Fill(0x00);
								break;
							}
						}
						Glcd_Fill(0x00);
					}
					else if(flag == 1 || flag == 2){
						break;
					}
				}
				Master = 0;
				break;
				
			case 1:
				delay_ms(100);
				Glcd_Fill(0x00);
				init();
				gen_debris();
				while(1){
					y = ADC_Read(0);
					if(debris_turn > 10 ){
						debris_turn = 0;
					}
					if(turno > 10){
						turno = 0;
					}
					if( y >= 200){//HACIA ABAJO
						erase_player(ship[0].x, ship[0].y, 2);//erase_player(x,  y,direction) 1 arriba / 2 abajo
						move_player(0,2);//move_player(i, direction) 1 arriba / 2 abajo
						draw_player(ship[0].x, ship[0].y, 1);//draw_player(x, y, color)
					}
					else if( y <= 20){//HACIA ARRIBA        
						erase_player(ship[0].x, ship[0].y, 1);//erase_player(x,  y,direction) 1 arriba / 2 abajo
						move_player(0,1);//move_player(i, direction) 1 arriba / 2 abajo
						draw_player(ship[0].x, ship[0].y, 1);//draw_player(x, y, color)
					}
					
					move_ia;
					draw_player(ship[1].x, ship[1].y, 1);
					move_debris();
					draw_time();
					
					if(seconds == 50){
						seconds = 0;
						minutes -=1;
					}
					if(minutes == 0){
						 game_over();
					}
					seconds ++;
					debris_turn ++;
					turno ++;
				}
				break;
			
            case 2:
				init();
				Glcd_Fill(0x00);
                delay_ms(100);
				if(UART1_Data_Ready()==0){//Espera a que se conecte la otra consola
                    while(1){        //Manda constantemente un 1, siempre y cuando no detecte la otra consola
                        Glcd_Write_Text("Waiting for other player",0,0,1);
                        UART1_Write('1');
                        if(UART1_Data_Ready() ==1){
                            break;
                        }
                    }
                    Glcd_Fill(0x00);
                }
				else{
                    for (tiempo = 0; tiempo < 200; tiempo ++){//Ciclo for para evitar que el otro pic se quede en hold
                    UART1_Write('1');
                    }
                    Glcd_Write_Text("Press to start",0,0,1);//Una vez que se conecta,
											// pide que se presione el joystick
                    while(1){                     
                        Master_slave = UART1_Read();//Asi el primero que presione, sera el maestro
                        if(Button(&PORTC,1,1,0)){ //Y el que no presiono se le manda la senal para que
                            UART1_Write('3');
                            Master = 1;
                            break;
                        }
                        else if(Master_slave == '3'){
                            Master = 2;
                            break;
                        }
					}
                        Master_slave == '0';
                        Glcd_Fill(0x00);
                }
                delay_ms(500);
				
				if(Master == 1){
					while(1){
						y = ADC_Read(0);
						if(debris_turn > 5){
							turno = 0;
						}
						if(turno > 10){
							turno = 0;
						}
						if(y >= 200){
							erase_player(ship[0].x, ship[0].y,2); //2 hacia abajo
							move_player(0,2);
							draw_player(ship[0].x, ship[0].y,1);
						}
						else if(y <= 5){
							erase_player(ship[0].x, ship[0].y,1); //1 hacia arriba
							move_player(0,1);
							draw_player(ship[0].x, ship[0].y,1);
						}
						while(UART1_Data_Ready()==0);
						
						if(UART1_Read() == '1'){
							erase_player(ship[0].x, ship[0].y,2); //2 hacia abajo
							move_player(1,2);
							draw_player(ship[0].x, ship[0].y,1);
						}
						else if(UART1_Read == '2'){
							erase_player(ship[0].x, ship[0].y,2); //2 hacia abajo
							move_player(1,1);
							draw_player(ship[0].x, ship[0].y,1);
						}
						
						move_debris();
						draw_time();
						coordinate_pos();
						data_pack();
						output_data(info);
						
						while (UART1_Tx_Idle() != 1);
						
						if(seconds == 50){
							seconds = 0;
							minutes -=1;
						}
						if(minutes == 0){
							game_over();
						}
						seconds ++;
						debris_turn ++;
						turno ++;
					}
				}
				
				if(Master == 2){
					while(1){
						y = ADC_Read(0);
						save_old_data();
					
						if(y >= 200){        //Mover hacia abajo
							move_other = '1';
						}
						else if(y <= 5){         //Mover hacia arriba
							move_other = '2';
						}
						else{move_other = '0';}
						
						output_character(move_other);
						input_data(info);
						pos_coordinate();
						//desdata_pack ();
						
						for(counter = 0; counter < 15; counter ++){
							if(old_debris[counter].x != debris[counter].x || old_debris[counter].y != debris[counter].y){
								Glcd_Dot(old_debris[counter].x, old_debris[counter].y, 0);
							}
							Glcd_Dot(debris[counter].x, debris[counter].y, 1);
						}
						for(counter2 = 0; counter2 < 2; counter2 ++){
							if(old_ship[counter2].y != ship[counter2].y){
								draw_player(old_ship[counter2].x, old_ship[counter2].y,0);
							}
							draw_player(ship[counter2].x, ship[counter2].y,1);
						}
						draw_time();
					}
				}	
				break;
        }
    } 
}
