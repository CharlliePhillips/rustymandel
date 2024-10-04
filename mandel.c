#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <SDL2/SDL.h>

int draw(SDL_Renderer *renderer, int res, double Xmax, double Xmin, double Ymax, double Ymin);
int pick(int i);
int main(){
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    int res = 1000;
    double Xmax = 2;
    double Xmin = -2;
    double Ymax = 2;
    double Ymin = -2;
    double centerX = -.4601222;
    double centerY = .570286;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(res,res,0,&window,&renderer);

    draw(renderer, res, Xmax, Xmin, Ymax, Ymin);
    SDL_RenderPresent(renderer);
    int mX;
    int mY;
    while (1) {
        if(event.type == SDL_MOUSEBUTTONDOWN){
            //printf("starting zoom");
            for(int i = 0; i < 6000; i++){
                Xmax = centerX + ((Xmax - centerX) * .9);
                Xmin = centerX + ((Xmin - centerX) * .9);
                Ymax = centerY + ((Ymax - centerY) * .9);
                Ymin = centerY + ((Ymin - centerY) * .9);
                SDL_RenderClear(renderer);
                draw(renderer, res, Xmax, Xmin, Ymax, Ymin);
                SDL_RenderPresent(renderer);
                SDL_Surface *sshot = SDL_CreateRGBSurface(0, res, res, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
                char text[] = "StringXXXX";
                text[6] = i + '0';
                SDL_SaveBMP(sshot, text);
                SDL_FreeSurface(sshot);
            }
        }
        if(event.type == SDL_KEYDOWN){
            printf("KEY\n");
            Xmax = 2;
            Xmin = -2;
            Ymax = 2;
            Ymin = -2;
            SDL_RenderClear(renderer);
            draw(renderer, res, Xmax, Xmin, Ymax, Ymin);
            SDL_RenderPresent(renderer);
        }
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT){
            break;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int draw(SDL_Renderer *renderer, int res, double Xmax, double Xmin, double Ymax, double Ymin){
    double complex z = 0;
    for(double x = Xmin; x <= Xmax; x += (Xmax-Xmin)/res){
        for(double y = Ymin; y <= Ymax; y += (Ymax-Ymin)/res){
            z = 0;
            int outX = (int)((x+(-1*Xmin))*(res/(Xmax-Xmin)));
            int outY = (int)((y+(-1*Ymin))*(res/(Ymax-Ymin)));
            if(outX > res){outX = res;}
            if(outY > res){outX = res;}
            if(outX < 0){outY = 0;}
            if(outY < 0){outY = 0;}
            int itr = 20000;
            for(int k = 0; k < itr; k++){
                z = z*z + x+y*I;
                if(cabs(z) > 2){
                    SDL_SetRenderDrawColor(renderer,pick(k%5),pick(k%4),pick(k%3),255);
                    SDL_RenderDrawPoint(renderer,outX,outY);
                    break;
                }
            }
            if(cabs(z) < 2){
                SDL_SetRenderDrawColor(renderer,0,0,0,0);
            }
        }
    }
}

int pick(int i){
    switch (i)
    {
    case 0:
        return 0;
    case 1:
        return 25;
    case 2:
        return 50;
    case 3:
        return 75;
    case 4:
        return 100;
    case 5:
        return 125;
    case 6:
        return 150;
    case 7:
        return 175;
    case 8:
        return 200;
    case 9:
        return 225;
    default:
        break;
    }
}