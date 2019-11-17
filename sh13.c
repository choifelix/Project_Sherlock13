#include <SDL.h>        
#include <SDL_image.h>        
#include <SDL_ttf.h>        
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

pthread_t thread_serveur_tcp_id;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char gbuffer[256];
char gServerIpAddress[256];
int gServerPort;
char gClientIpAddress[256];
int gClientPort;
char gName[256];
char gNames[4][256];
int gId;
int joueurSel;
int objetSel;
int guiltSel;
int guiltGuess[13];
int tableCartes[4][8];
int b[3];
int goEnabled;
int connectEnabled;

char *nbobjets[]={"5","5","5","5","4","3","3","3"};
char *nbnoms[]={"Sebastian Moran", "irene Adler", "inspector Lestrade",
  "inspector Gregson", "inspector Baynes", "inspector Bradstreet",
  "inspector Hopkins", "Sherlock Holmes", "John Watson", "Mycroft Holmes",
  "Mrs. Hudson", "Mary Morstan", "James Moriarty"};

volatile int synchro;

void *fn_serveur_tcp(void *arg)
{
        int sockfd, newsockfd, portno;
        socklen_t clilen;
        struct sockaddr_in serv_addr, cli_addr;
        int n;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd<0)
        {
                printf("sockfd error\n");
                exit(1);
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = gClientPort;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
       if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        {
                printf("bind error\n");
                exit(1);
        }

        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        while (1)
        {
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0)
                {
                        printf("accept error\n");
                        exit(1);
                }

                bzero(gbuffer,256);
                n = read(newsockfd,gbuffer,255);
                if (n < 0)
                {
                        printf("read error\n");
                        exit(1);
                }
                //printf("%s",gbuffer);

                pthread_mutex_lock( &mutex );
                synchro=1;
                pthread_mutex_unlock( &mutex );

                while (synchro);

     }
}



void sendMessageToServer(char *ipAddress, int portno, char *mess)
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char sendbuffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(ipAddress);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
                printf("ERROR connecting\n");
                exit(1);
        }

        sprintf(sendbuffer,"%s\n",mess);
        n = write(sockfd,sendbuffer,strlen(sendbuffer));

    close(sockfd);
}

int estDans(int x, int y, SDL_Rect rect){
	if( (x > rect.x) && (x < rect.x + rect.w) && (y > rect.y) && (y < rect.y + rect.h) ){
		printf("%d,%d est dedans \n", x,y);
		return 1;
	}
	else{
		printf("%d,%d est pas dedans \n", x,y);
		return 0;
	}
}

int main(int argc, char ** argv)
{
	int ret;
	int i,j;

    int quit = 0;
    SDL_Event event;
	int mx,my;
	char sendBuffer[256];
	char lname[256];
	int Id;
	int a,c,d;
	int H = 1280;
	int W = 750;
	int ecart_buttonX = H/10; //mesure necessaire pour determiner la position des boutons connect et play
	int ecart_buttonY = W/20;

        if (argc<6)
        {
                printf("<app> <Main server ip address> <Main server port> <Client ip address> <Client port> <player name>\n");
                exit(1);
        }

        strcpy(gServerIpAddress,argv[1]);
        gServerPort=atoi(argv[2]);
        strcpy(gClientIpAddress,argv[3]);
        gClientPort=atoi(argv[4]);
        strcpy(gName,argv[5]);

    SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
 
    SDL_Window * window = SDL_CreateWindow(gName,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, H, W, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture *texture = NULL;
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); /* texture transparente */
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

	

/*charge des images*/
	SDL_Surface *deck[14], *plaquette[13], *objets[13], *gobutton, *connectbutton, *fond;

	//SDL_Surface * fond; deja defini quelque part 
	fond     = IMG_Load("decor/fond_standard.png"); //modif
	gobutton = IMG_Load("decor/play.png");
	connectbutton = IMG_Load("decor/start.png");
	SDL_Rect rect_fond = {0, 0, 1280, 750};

	SDL_Texture * texture_fond;
	texture_fond = SDL_CreateTextureFromSurface(renderer, fond);

		//###########################

	deck[0]  = IMG_Load("cartes/SH13_0.png");
	deck[1]  = IMG_Load("cartes/SH13_1.png");
	deck[2]  = IMG_Load("cartes/SH13_2.png");
	deck[3]  = IMG_Load("cartes/SH13_3.png");
	deck[4]  = IMG_Load("cartes/SH13_4.png");
	deck[5]  = IMG_Load("cartes/SH13_5.png");
	deck[6]  = IMG_Load("cartes/SH13_6.png");
	deck[7]  = IMG_Load("cartes/SH13_7.png");
	deck[8]  = IMG_Load("cartes/SH13_8.png");
	deck[9]  = IMG_Load("cartes/SH13_9.png");
	deck[10] = IMG_Load("cartes/SH13_10.png");
	deck[11] = IMG_Load("cartes/SH13_11.png");
	deck[12] = IMG_Load("cartes/SH13_12.png");
	deck[13] = IMG_Load("cartes/Sherlock13_DosSuspect.png");

	plaquette[0]  = IMG_Load("personnage/01.png");
	plaquette[1]  = IMG_Load("personnage/02.png");
	plaquette[2]  = IMG_Load("personnage/03.png");
	plaquette[3]  = IMG_Load("personnage/04.png");
	plaquette[4]  = IMG_Load("personnage/05.png");
	plaquette[5]  = IMG_Load("personnage/06.png");
	plaquette[6]  = IMG_Load("personnage/07.png");
	plaquette[7]  = IMG_Load("personnage/08.png");
	plaquette[8]  = IMG_Load("personnage/09.png");
	plaquette[9]  = IMG_Load("personnage/10.png");
	plaquette[10] = IMG_Load("personnage/11.png");
	plaquette[11] = IMG_Load("personnage/12.png");
	plaquette[12] = IMG_Load("personnage/13.png");

	objets[0]  = IMG_Load("object/object_moran.png");
	objets[1]  = IMG_Load("object/object_adler.png");
	objets[2]  = IMG_Load("object/object_lestrade.png");
	objets[3]  = IMG_Load("object/object_gregson.png");
	objets[4]  = IMG_Load("object/object_baynes.png");
	objets[5]  = IMG_Load("object/object_bradstreet.png");
	objets[6]  = IMG_Load("object/object_hopkins.png");
	objets[7]  = IMG_Load("object/object_holmes.png");
	objets[8]  = IMG_Load("object/object_watson.png");
	objets[9]  = IMG_Load("object/object_mycroft.png");
	objets[10] = IMG_Load("object/object_hudson.png");
	objets[11] = IMG_Load("object/object_mortan.png");
	objets[12] = IMG_Load("object/object_moriarty.png");

/*charge des textures*/
	SDL_Texture * texture_objets[13],*texture_plaquette[13], *texture_deck[14],*texture_gobutton,*texture_connectbutton;

	for( i =0; i <13 ; i++){
		texture_objets[i] = SDL_CreateTextureFromSurface(renderer, objets[i]);
		texture_plaquette[i] = SDL_CreateTextureFromSurface(renderer, plaquette[i]);
		texture_deck[i] = SDL_CreateTextureFromSurface(renderer, deck[i]);
	}
	texture_deck[13] = SDL_CreateTextureFromSurface(renderer, deck[13]);
	//SDL_Rect rect_plaquette[13];
	//SDL_Rect rect_objets[13];
	int plaquet_ecartY = W/15;
	int plaquet_ecartX = H/10;
	int table_ecartX = H/28;
	int table_ecartY = W/13;

	
	//definition des differents rectangles
	SDL_Rect button_rect = { ecart_buttonX*3.4, ecart_buttonY * 14, 400, 150 };

	SDL_Rect rect_plaquette[13];
	SDL_Rect rect_objets[13];
	for( i =0; i <13 ; i++){ //dimension des plaquettes de personnage
		rect_plaquette[i].x = plaquet_ecartX/2;
		rect_plaquette[i].y = (i+1)*plaquet_ecartY;
		rect_plaquette[i].w = plaquet_ecartX;
		rect_plaquette[i].h = plaquet_ecartY;

		rect_objets[i].x = plaquet_ecartX*3/2;
		rect_objets[i].y = (i+1)*plaquet_ecartY;
		rect_objets[i].w = plaquet_ecartX +10;
		rect_objets[i].h = plaquet_ecartY;
	}

	SDL_Rect rect_tableCartes[4][8];
	for(i = 0; i<4 ; i++){
		for(j=0; j<8 ; j++){
			rect_tableCartes[i][j].x = table_ecartX * (j) + plaquet_ecartX*3 + table_ecartX*2.2;
			rect_tableCartes[i][j].y = table_ecartY * (i) + 3*plaquet_ecartY ;
			rect_tableCartes[i][j].w = table_ecartX;
			rect_tableCartes[i][j].h = table_ecartY;
		}
	}

	SDL_Rect rect_player[4];

	for(i = 0; i<4 ; i++){
		rect_player[i].x = plaquet_ecartX*3;
		rect_player[i].y = table_ecartY * (i) + 3*plaquet_ecartY;
		rect_player[i].w = table_ecartX*2;
		rect_player[i].h = table_ecartY;
	}

	SDL_Rect rect_symbol[8];

	for(i=0; i<8 ;i++){
		rect_symbol[i].x = table_ecartX * i + plaquet_ecartX*3 + table_ecartX*2;
		rect_symbol[i].y = 2*plaquet_ecartY;
		rect_symbol[i].w = table_ecartX;
		rect_symbol[i].h = table_ecartY;
	}

	SDL_Rect rect_b0 = { H - (H/50+ H/4), W/8, H/4, W/4 };
	SDL_Rect rect_b1 = { H - (H/50+ H/4), W/4 + W/8, H/4, W/4 };
	SDL_Rect rect_b2 = { H - (H/50+ H/4), W/2 + W/8 , H/4, W/4 };










	

	strcpy(gNames[0],"-");
	strcpy(gNames[1],"-");
	strcpy(gNames[2],"-");
	strcpy(gNames[3],"-");

	joueurSel=-1;
	objetSel=-1;
	guiltSel=-1;

	b[0]=-1;
	b[1]=-1;
	b[2]=-1;

	for (i=0;i<13;i++)
		guiltGuess[i]=0;

	for (i=0;i<4;i++)
		for (j=0;j<8;j++)
			tableCartes[i][j]=-1;

	goEnabled=0;
	connectEnabled=1;

	for (i=0;i<13;i++)
		texture_deck[i] = SDL_CreateTextureFromSurface(renderer, deck[i]);
	

    texture_gobutton = SDL_CreateTextureFromSurface(renderer, gobutton);
    texture_connectbutton = SDL_CreateTextureFromSurface(renderer, connectbutton);

    TTF_Font* Sans = TTF_OpenFont("sans.ttf", 15); 
    printf("Sans=%p\n",Sans);

   /* Creation du thread serveur tcp. */
   printf ("Creation du thread serveur tcp !\n");
   synchro=0;
   ret = pthread_create ( & thread_serveur_tcp_id, NULL, fn_serveur_tcp, NULL);

    while (!quit){
		if (SDL_PollEvent(&event)){
			//printf("un event\n");
        	switch (event.type){
            	case SDL_QUIT:
                	quit = 1;
                	break;
				case  SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState( &mx, &my );
					//printf("mx=%d my=%d\n",mx,my);
					if (estDans(mx,my,button_rect) && (connectEnabled==1)){
						sprintf(sendBuffer,"C %s %d %s",gClientIpAddress,gClientPort,gName);
						sprintf(sendBuffer,"C %s %d %s",gClientIpAddress,gClientPort,gName);
	                    sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);

	                    connectEnabled=0;
					}
					else if ((mx>=plaquet_ecartX*3) && (mx<plaquet_ecartX*3 + table_ecartX*2) && (my>=3*plaquet_ecartY) && (my<table_ecartY * 4 + 3*plaquet_ecartY)){
						joueurSel=(my- 3*plaquet_ecartY)/table_ecartY;
						guiltSel=-1;
					}
					else if ((mx>=plaquet_ecartX*3 + table_ecartX*2) && (mx<table_ecartX * 8 + plaquet_ecartX*3 + table_ecartX*2) && (my>= 2*plaquet_ecartY ) && (my< 2*plaquet_ecartY + table_ecartY)){
						objetSel=(mx-plaquet_ecartX*3 - table_ecartX*2)/table_ecartX;
						guiltSel=-1;
					}
					/*else if ((mx>=100) && (mx<250) && (my>=350) && (my<740)){
						joueurSel=-1;
						objetSel=-1;
						guiltSel=(my-350)/30;
					}*/
					else if ((mx>=plaquet_ecartX/2) && (mx<plaquet_ecartX/2 + plaquet_ecartX) && (my>=plaquet_ecartY) && (my<14*plaquet_ecartY)){
						guiltSel=(my- plaquet_ecartY)/plaquet_ecartY;
						joueurSel=-1;
						objetSel=-1;
						guiltGuess[guiltSel]=1-guiltGuess[guiltSel];
					}
					else if ((estDans(mx,my,button_rect)) && (goEnabled==1)){
						printf("go! joueur=%d objet=%d guilt=%d\n",joueurSel, objetSel, guiltSel);
						if (guiltSel!=-1){
							sprintf(sendBuffer,"G %d",guiltSel);
							sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
						}
						else if ((objetSel!=-1) && (joueurSel==-1)){
							sprintf(sendBuffer,"O %d",objetSel);
							sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
						}
						else if ((objetSel!=-1) && (joueurSel!=-1)){
						sprintf(sendBuffer,"S %d %d", joueurSel,objetSel);
						sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
						}
						goEnabled = 0;
					}
					else{
						joueurSel=-1;
						objetSel=-1;
						guiltSel=-1;
					}
					break;
				case  SDL_MOUSEMOTION:
					SDL_GetMouseState( &mx, &my );
					break;
        	}
		}

        if (synchro==1){
                pthread_mutex_lock( &mutex );
                printf("consomme |%s|\n",gbuffer);
			switch (gbuffer[0]){
				// Message 'I' : le joueur recoit son Id
				case 'I':
					sscanf(gbuffer,"%*c %d",&gId);
					break;

				// Message 'L' : le joueur recoit la liste des joueurs
				case 'L':
					sscanf(gbuffer,"%*c %s %s %s %s",gNames[0],gNames[1],gNames[2],gNames[3]);
					break;

				// Message 'D' : le joueur recoit ses trois cartes
				case 'D':
					sscanf(gbuffer,"%*c %d %d %d",&b[0],&b[1],&b[2]);
					break;
				// Message 'M' : le joueur recoit le n° du joueur courant
				// Cela permet d'affecter goEnabled pour autoriser l'affichage du bouton go
				case 'M':
					// RAJOUTER DU CODE ICI
					sscanf(gbuffer,"%*c %d",&Id);
					printf("gID %d  ID %d GO %d\n", gId,Id,goEnabled);
					if(Id==gId)
						goEnabled =1;
					break;

				// Message 'V' : le joueur recoit une valeur de tableCartes
				case 'V':
					sscanf(gbuffer,"%*c %d %d %d",&a,&c,&d);
					tableCartes[a][c] = d;
					break;
			}
			synchro=0;
			pthread_mutex_unlock( &mutex );
        }

        /*SDL_Rect dstrect_grille = { 512-250, 10, 500, 350 };
        SDL_Rect dstrect_image = { 0, 0, 500, 330 };
        SDL_Rect dstrect_image1 = { 0, 340, 250, 330/2 };*/

		/*SDL_SetRenderDrawColor(renderer, 255, 230, 230, 230);
		SDL_Rect rect = {0, 0, 1024, 768}; 
		SDL_RenderFillRect(renderer, &rect);*/

	    SDL_RenderCopy(renderer, texture_fond, NULL, &rect_fond);

	    for( i =0; i <13 ; i++){ //dimension des plaquettes de personnage
			SDL_RenderCopy(renderer, texture_plaquette[i], NULL, &rect_plaquette[i]);
			SDL_RenderCopy(renderer, texture_objets[i], NULL, &rect_objets[i]);


		}	

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		if (joueurSel!=-1){
			SDL_SetRenderDrawColor(renderer, 255, 180, 180, 150);
			//SDL_Rect rect1 = {0, 90+joueurSel*60, 200 , 60}; 
			SDL_SetRenderTarget(renderer, texture);
			SDL_RenderFillRect(renderer, &rect_player[joueurSel]);
		}	

		if (objetSel!=-1){
			SDL_SetRenderDrawColor(renderer, 180, 255, 180, 150);
			//SDL_Rect rect1 = {200+objetSel*60, 0, 60 , 90};
			SDL_SetRenderTarget(renderer, texture); 
			SDL_RenderFillRect(renderer, &rect_symbol[objetSel]);
		}	

		if (guiltSel!=-1){
			SDL_SetRenderDrawColor(renderer, 180, 180, 255, 150);
			//SDL_Rect rect1 = {100, 350+guiltSel*30, 150 , 30}; 
			SDL_SetRenderTarget(renderer, texture);
			SDL_RenderFillRect(renderer, &rect_plaquette[guiltSel]);
		}	

		SDL_Color col1 = {0, 0, 0};

		for (i=0;i<4;i++)
	        	for (j=0;j<8;j++){
					if (tableCartes[i][j]!=-1){
						char mess[10];

						if (tableCartes[i][j]==100)
							sprintf(mess,"*");
						else
							sprintf(mess,"%d",tableCartes[i][j]);

	                	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, mess, col1);
	                	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);


	                	SDL_RenderCopy(renderer, Message, NULL, &rect_tableCartes[i][j]);
	                	SDL_DestroyTexture(Message);
	                	SDL_FreeSurface(surfaceMessage);
	                	
					}
	        	}
	    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);


		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		// Afficher les suppositions
		for (i=0;i<13;i++)
			if (guiltGuess[i])
			{
				SDL_RenderDrawLine(renderer, rect_plaquette[i].x,rect_plaquette[i].y,rect_plaquette[i].x + rect_plaquette[i].w,rect_plaquette[i].y + rect_plaquette[i].h);
				SDL_RenderDrawLine(renderer, rect_plaquette[i].x + rect_plaquette[i].w,rect_plaquette[i].y,rect_plaquette[i].x,rect_plaquette[i].y + rect_plaquette[i].h);
			}

		/*SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawLine(renderer, 0,30+60,680,30+60);
		SDL_RenderDrawLine(renderer, 0,30+120,680,30+120);
		SDL_RenderDrawLine(renderer, 0,30+180,680,30+180);
		SDL_RenderDrawLine(renderer, 0,30+240,680,30+240);
		SDL_RenderDrawLine(renderer, 0,30+300,680,30+300);

		SDL_RenderDrawLine(renderer, 200,0,200,330);
		SDL_RenderDrawLine(renderer, 260,0,260,330);
		SDL_RenderDrawLine(renderer, 320,0,320,330);
		SDL_RenderDrawLine(renderer, 380,0,380,330);
		SDL_RenderDrawLine(renderer, 440,0,440,330);
		SDL_RenderDrawLine(renderer, 500,0,500,330);
		SDL_RenderDrawLine(renderer, 560,0,560,330);
		SDL_RenderDrawLine(renderer, 620,0,620,330);
		SDL_RenderDrawLine(renderer, 680,0,680,330);

		for (i=0;i<14;i++)
			SDL_RenderDrawLine(renderer, 0,350+i*30,300,350+i*30);
		SDL_RenderDrawLine(renderer, 100,350,100,740);
		SDL_RenderDrawLine(renderer, 250,350,250,740);
		SDL_RenderDrawLine(renderer, 300,350,300,740);*/

	        //SDL_RenderCopy(renderer, texture_grille, NULL, &dstrect_grille);


		if(b[0]!= -1)
	        SDL_RenderCopy(renderer, texture_deck[b[0]], NULL, &rect_b0);
		else{
	        SDL_RenderCopy(renderer, texture_deck[13], NULL, &rect_b0);
		}


		if(b[1]!= -1)
	        SDL_RenderCopy(renderer, texture_deck[b[1]], NULL, &rect_b1);
		else{
	        SDL_RenderCopy(renderer, texture_deck[13], NULL, &rect_b1);
		}


		if(b[2]!= -1)
	        SDL_RenderCopy(renderer, texture_deck[b[2]], NULL, &rect_b2);
		else{
	        SDL_RenderCopy(renderer, texture_deck[13], NULL, &rect_b2);
		}


		// Le bouton go
		if (goEnabled==1)
	        SDL_RenderCopy(renderer, texture_gobutton, NULL, &button_rect);
		
		// Le bouton connect
		if (connectEnabled==1)
	        SDL_RenderCopy(renderer, texture_connectbutton, NULL, &button_rect);
		

	        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	        //SDL_RenderDrawLine(renderer, 0, 0, 200, 200);

		SDL_Color col = {0, 0, 0};
		for (i=0;i<4;i++)
			if (strlen(gNames[i])>0){
				SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, gNames[i], col);
				SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

				SDL_RenderCopy(renderer, Message, NULL, &rect_player[i]);
	    		SDL_DestroyTexture(Message);
	    		SDL_FreeSurface(surfaceMessage);
			}
        SDL_RenderPresent(renderer);
    }
 
    SDL_DestroyTexture(texture_deck[0]);
    SDL_DestroyTexture(texture_deck[1]);
    SDL_FreeSurface(deck[0]);
    SDL_FreeSurface(deck[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
 
    SDL_Quit();
 
    return 0;
}
