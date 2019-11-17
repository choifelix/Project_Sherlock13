#include <SDL2/SDL.h>        
#include <SDL2/SDL_image.h>        
#include <SDL2_ttf/SDL_ttf.h>        
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char ** argv){
	SDL_Event event;
	int quit = 0;
	int i;
	int H = 1280;
	int W = 750;
	int mx,my,connectEnabled,guiltSel,joueurSel,objetSel;
	int b[3];
	int guiltGuess[13];

	for (i=0;i<13;i++)
		guiltGuess[i]=0;


	b[0] = -1;
	b[1] = -1;
	b[2] = -1;

	guiltSel  = -1;
	joueurSel = -1;
	objetSel  = -1;
	connectEnabled = 1;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	char *nbobjets[]={"5","5","5","5","4","3","3","3"};
	char *nbnoms[]={"Sebastian Moran", "irene Adler", "inspector Lestrade",
  "inspector Gregson", "inspector Baynes", "inspector Bradstreet",
  "inspector Hopkins", "Sherlock Holmes", "John Watson", "Mycroft Holmes",
  "Mrs. Hudson", "Mary Morstan", "James Moriarty"};

	SDL_Window * window = SDL_CreateWindow("SDL2 SH13",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, H, W, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	

/*charge des images*/
	SDL_Surface *deck[13], *plaquette[13], *objets[13], *gobutton, *connectbutton, *fond;

	//SDL_Surface * fond; deja defini quelque part 
	fond = IMG_Load("decor/fond_standard.png"); //modif
	SDL_Rect rect_fond = {0, 0, 1280, 750};

	SDL_Texture * texture_fond;
	texture_fond = SDL_CreateTextureFromSurface(renderer, fond);

	SDL_RenderCopy(renderer, texture_fond, NULL, &rect_fond);
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
	SDL_Texture * texture_objets[13],*texture_plaquette[13], *texture_deck[13];

	for( i =0; i <13 ; i++){
		texture_objets[i] = SDL_CreateTextureFromSurface(renderer, objets[i]);
		texture_plaquette[i] = SDL_CreateTextureFromSurface(renderer, plaquette[i]);
		texture_deck[i] = SDL_CreateTextureFromSurface(renderer, deck[i]);
	}

	SDL_Rect rect_plaquette[13];
	SDL_Rect rect_objets[13];
	int plaquet_ecartY = W/15;
	int plaquet_ecartX = H/10;

	for( i =0; i <13 ; i++){ //dimension des plaquettes de personnage

		rect_plaquette[i].x = plaquet_ecartX/2 ;
		rect_plaquette[i].y = (i+1)*plaquet_ecartY;
		rect_plaquette[i].w = plaquet_ecartX;
		rect_plaquette[i].h = plaquet_ecartY ;

		rect_objets[i].x = plaquet_ecartX*3/2 ;
		rect_objets[i].y = (i+1)*plaquet_ecartY;
		rect_objets[i].w = plaquet_ecartX +10;
		rect_objets[i].h = plaquet_ecartY ;

	}	

	SDL_Rect dstrect1 = { H - (H/50+ H/4), W/8, H/4, W/4 };
	SDL_Rect dstrect2 = { H - (H/50+ H/4), W/4 + W/8, H/4, W/4 };
	SDL_Rect dstrect3 = { H - (H/50+ H/4), W/2 + W/8 , H/4, W/4 };

	for( i =0; i <13 ; i++){
		SDL_RenderCopy(renderer, texture_plaquette[i], NULL, &rect_plaquette[i]);
		SDL_RenderCopy(renderer, texture_objets[i], NULL, &rect_objets[i]);
	}

	

	

	if(connectEnabled == 1){

	}

	
	SDL_RenderPresent(renderer); //affiche le rendu
	while (!quit)
    {
		if (SDL_PollEvent(&event)){
		//printf("un event\n");
        	switch (event.type)
        	{
            		case SDL_QUIT:
                		quit = 1;
                		break;
				case  SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState( &mx, &my );
					printf("mx=%d my=%d\n",mx,my);
					if ((mx<200) && (my<50) && (connectEnabled==1))
					{
						//sprintf(sendBuffer,"C %s %d %s",gClientIpAddress,gClientPort,gName);
						//sprintf(sendBuffer,"C %s %d %s",gClientIpAddress,gClientPort,gName);
	                    //sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
	                    // RAJOUTER DU CODE ICI, fait ->sendMessageToServer
	                    printf("connect\n");

	                    connectEnabled=0;

						// RAJOUTER DU CODE ICI
					}
					else if ((mx>=0) && (mx<200) && (my>=90) && (my<330))
					{
						joueurSel=(my-90)/60;
						guiltSel=-1;
					}
					else if ((mx>=200) && (mx<680) && (my>=0) && (my<90))
					{
						objetSel=(mx-200)/60;
						guiltSel=-1;
					}
					else if ((mx>=100) && (mx<250) && (my>=350) && (my<740))
					{
						joueurSel=-1;
						objetSel=-1;
						guiltSel=(my-350)/30;
					}
					else if ((mx>=250) && (mx<300) && (my>=350) && (my<740))
					{
						int ind=(my-350)/30;
						guiltGuess[ind]=1-guiltGuess[ind];
					}
					else if ((mx>=500) && (mx<700) && (my>=350) && (my<450) )
					{
						printf("go! joueur=%d objet=%d guilt=%d\n",joueurSel, objetSel, guiltSel);
						if (guiltSel!=-1)
						{
								//sprintf(sendBuffer,"G %d %d",gId, guiltSel);
								//sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
								printf("guilt selected: %s",nbnoms[guiltSel]);
							// RAJOUTER DU CODE ICI,fait ->sendMessageToServer


							}
							else if ((objetSel!=-1) && (joueurSel==-1))
							{
								//sprintf(sendBuffer,"O %d %d",gId, objetSel);
								//sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
								printf("%d %d \n", objetSel, joueurSel);

							// RAJOUTER DU CODE ICI

							}
							else if ((objetSel!=-1) && (joueurSel!=-1))
							{
								//sprintf(sendBuffer,"S %d %d %d",gId, joueurSel,objetSel);
								//sendMessageToServer(gServerIpAddress, gServerPort, sendBuffer);
								printf("%d %d \n", objetSel, joueurSel);

							// RAJOUTER DU CODE ICI

							}
						}
					else
					{
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

        if (connectEnabled==0){
                //pthread_mutex_lock( &mutex );
                //printf("consomme |%s|\n",gbuffer);
        	if (b[0]==-1){
        		printf("connectEnabled \n");
				b[0] = 1;
				b[1] = 2;
				b[2] = 3;
			}	

        }
    }

    
	
    




	SDL_DestroyTexture(texture_fond);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	 	

	SDL_Quit();
	return 0;
}