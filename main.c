unsigned short score[] = {0,0};
unsigned short minutes = 64;
unsigned short seconds = 0; 
unsigned short areaX = 0;
unsigned short areaY = 0;
unsigned short centerShip = 0;
unsigned short debris_turn = 0;


typedef struct players{//Pos inferior izquierda de la nave
    unsigned short x;
    unsigned short y;
}ships;

typedef struct debris{
    unsigned short x;
    unsigned short y;
    signed short dx;
	signed short dir;
}escombro;

ships ship[2];
escombro debris[14];

char info[9];

void draw_player(unsigned short x, unsigned short y,unsigned short color){
        Glcd_V_Line(y,y+6, x,color)
        Glcd_V_Line(y,y+6, x+1,color)
        Glcd_V_Line(y,y+6, x+2,color)
        Glcd_V_Line(y,y+6, x+3,color)
        Glcd_V_Line(y,y+6, x+4,color)
        Glcd_Dot(y, x-1, color);
        Glcd_Dot(y+1, x-1, color);
        Glcd_Dot(y, x-2, color);
        Glcd_Dot(y, x+5, color);
        Glcd_Dot(y+1, x+5, color);
        Glcd_Dot(y, x+6, color);
        Glcd_Dot(y+6, x+5, color);
}

void draw_time(){
	Glcd_V_Line(minutes,64,64,0);
	Glcd_V_Line(0,minutes,64,1);
}

void erase_player(unsigned short x, unsigned short y,unsigned short direction){
	if(direction == 1){//hacia arriba/ borrar la parte de abajo de la nave
		Glcd_H_Line(x,x+6,y,0);
		Glcd_Dot(y+6, x, 0);
		Glcd_Dot(y+6, x, 0);
	}
	else if(direction == 2){//hacia abajo/ borrar la parte superior de la nave
		Glcd_Dot(y+6, x+5, 0);
		
	}
}

void move_debris(posy,dir){
	unsigned short i; //variable para la colision
	unsigned short j;//variable para controlar los asteroides
	
	if(debris_turn == 20){
		for(j = 0; j <= 14; j++){
			if(debris[j].dir == 1){//asteroide moviendose de derecha a izquierda   <--
				debris[j].x -= debris[j].dx
				if(debri[j].x <= 0){
					debri[j].x = 128;
				}
			}
			else if(debris[j].dir == -1){//asteroide moviendose de izquierda a derecha -->
				debris[j].x += debris[j].dx
				if(debri[j].x >= 128){
					debri[j].x = 0;
				} 
			}
			
			//Si un asteriode choca con una nave
			for(i = 0; i < 2; i++){
				if((debris[j].y >= ship[i].y) && (debris[j].x <= ship[i].y + 6) && (debris[j].x >= ship[i].x) && (debris[j].x <= ship[i].x + 6)){
					draw_player(ship.x[i], ship[i].y, 0);
					if(i == 0{
						player[i].x = 40;
						player[i].y = 60;
					}
					else if(i == 1){
						player[i].x = 70;
						player[i].y = 60;
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
			debri[j].dir = -1;
		}
		else if(side = 1){
			debri[j].dir = 1;
		}
		//Dar ubicacion x y a los asteriodes
		debri[j].y = posY;
		debri[j].x = posX;
		posY += 3;
	}
}

void init(){
	player[0].x =  40;
    player[0].y = 60;

    player[1].x = 70;
    player[1].y = 60;

    debris.x = 64;
    debris.y = 32;
    debris.dx = 2;

    draw_player(ship[0].x, ship[0].y, 1);
    draw_player(ship[1].x, ship[1].y, 1);
    gen_debris();
}

void check_winner(){
	if(minutes == 0){
		if(score[0] < score[1]){
			Glcd_Fill();
			Glcd_Write_Text();
		}
		else if(score[0] > score[1]) {
			Glcd_Fill();
			Glcd_Write_Text();
		}
		else if(score[0] == score[1]){
			Glcd_Fill();
			Glcd_Write_Text();
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
							erase_player(ship[j].x, ship[j].y, 1)
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
					if((debris[j].y <= areaY) && (debris[j].y > ship[1].y){
						if(debris[j].y >= centerShip){
							//mover hacia arriba 
							ship[1].y += 2;
							erase_player(ship[j].x, ship[j].y, 1)
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
		else if(directon == 1){//hacia arriba
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
		Glcd_Write_Text('YOU WIN' ,,,)
		while(1){
            if(Button(&PORTC,1,1,1)){ //Quiero volver a la portada como si acabara de encender la consola
                Glcd_Fill(0x00);
                break;
            }
        }
	}
	else if(score[0] < score[1]){
		Glcd_Write_Text('PC WINS' ,,,)
		while(1){
            if(Button(&PORTC,1,1,1)){ //Quiero volver a la portada como si acabara de encender la consola
                Glcd_Fill(0x00);
                break;
            }
        }
	}
	else if(score[0] == score[1]){
		Glcd_Write_Text('IT\'S A TIE' ,,,)
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
	  for(i = 0; i <= 4; i++){
		if(i == 0){
			info[0] = ship[0].y + '0';
			info[1] = ship[1].y + '0';
	
			info[2] = score[0] + '0';
			info[3] = score[1] + '0';
	
			info[4] = debris[0].y + '0';  
			info[5] = debris[0].x + '0';  
	
			info[6] = debris[1].x + '0';
			info[7] = 'O';
			info[8] = 'K';	
		}
		else if(i == 1){
			info[0] = debris[1].y + '0';
			info[1] = debris[2].x + '0';
			info[2] = debris[2].y + '0';
			info[3] = debris[3].x + '0';
			info[4] = debris[3].y + '0';
			info[5] = debris[4].x + '0';
			info[6] = debris[4].y + '0';
			info[7] = 'O';
			info[8] = 'K';
		}
		else if(i == 2){
			info[0] = debris[5].y + '0';
			info[1] = debris[5].x + '0';
			info[2] = debris[6].y + '0';
			info[3] = debris[6].x + '0';
			info[4] = debris[7].y + '0';
			info[5] = debris[7].x + '0';
			info[6] = debris[8].y + '0';
			info[7] = 'O';
			info[8] = 'K';
		}
		else if(i == 3){
			info[0] = debris[8].x + '0';
			info[1] = debris[9].x + '0';
			info[2] = debris[9].y + '0';
			info[3] = debris[10].x + '0';
			info[4] = debris[10].y + '0';
			info[5] = debris[11].x + '0';
			info[6] = debris[11].y + '0';
			info[7] = 'O';
			info[8] = 'K';
		}
		else if(i == 4){
			info[0] = debris[12].y + '0';
			info[1] = debris[12].x + '0';
			info[2] = debris[13].y + '0';
			info[3] = debris[13].x + '0';
			info[4] = debris[14].y + '0';
			info[5] = debris[14].x + '0';
			info[6] = 'O';
			info[7] = 'K';
			info[8] = 0;
		}

	}

}
  
void desdata_pack(){   // Funcion para extraer datos del paquete recibido por esclavo
    unsigned short i = 0;
	for(i = 0; i <= 4; i++){
		if(i == 0){
			ship[0].y   =   info[0] - '0';
			ship[1].y   =   info[1] - '0';
			score[0]    =   info[2] - '0';
			score[1]    =   info[3] - '0';
			debris[0].y =   info[4] - '0';
			debris[0].x =   info[5] - '0';
			debris[1].x =   info[6] - '0';
		}
		else if(i == 1){
			debris[1].y =   info[0] - '0';
			debris[2].x =   info[1] - '0';
			debris[2].y =   info[2] - '0';
			debris[3].x =   info[3] - '0';
			debris[3].y =   info[4] - '0';
			debris[4].x =   info[5] - '0';
			debris[4].y =   info[6] - '0';	
		}
		else if(i == 2){
			debris[5].y =   info[0] - '0';
			debris[5].x =   info[1] - '0';
			debris[6].y =   info[2] - '0';
			debris[6].x =   info[3] - '0';
			debris[7].y =   info[4] - '0';
			debris[7].x =   info[5] - '0';
			debris[8].y =   info[6] - '0';	
		}
		else if(i == 3){
			debris[8].x  =   info[0] - '0';
			debris[9].x  =   info[1] - '0';
			debris[9].y  =   info[2] - '0';
			debris[10].x =   info[3] - '0';
			debris[10].y =   info[4] - '0';
			debris[11].x =   info[5] - '0';
			debris[11].y =   info[6] - '0';		
		}
		else if(i == 4){
			debris[12].y =   info[0] - '0';
			debris[12].x =   info[1] - '0';
			debris[13].y =   info[2] - '0';
			debris[13].x =   info[3] - '0';
			debris[14].y =   info[4] - '0';
			debris[14].x =   info[5] - '0';
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
     while(UART1_Data_Ready()==0);
     UART1_Read_Text(text_dir, "OK", 255);
}

void save_old_data(){
       send_paddles[0].y = paddle[0].y;
       send_paddles[1].y = paddle[1].y;

       send_balls.x = ball.x;
       send_balls.y = ball.y;
       send_balls.dx = ball.dx;
       send_balls.dy = ball.dy;
}

void main(){
	
	unsigned short Master = 0;
	unsigned short 
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
				y = ADC_Read(0);
                if(y <= 10){
					Glcd_Write_Text("one player",15,0,1);
                    delay_ms(1000);
                    while(1){
						y = ADC_Read(0);
                        //Glcd_Write_Text(y,15,0,1);
                        if(Button(&PORTC,1,1,0)){
                            flag = 1;
                            break;
                        }
                        else if(y >= 200){
							break;
                        }
                     }Glcd_Fill(0x00);
                                  //break;
                }
                else if(y >= 200){
                    Glcd_Write_Text("multiplayer",15,0,1);
                    delay_ms(1000);
                     //Glcd_Image(MULTIPLAYER);
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
                     //break;
                }
                else if(flag == 1 || flag == 2){
					break;
                }
                
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
                    if(turno_paleta > 10){
                       turno_paleta = 0;
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
					seconds += 1;
				}
				break;
			
			
			case 2:
		}
	}
	
	
	
}









