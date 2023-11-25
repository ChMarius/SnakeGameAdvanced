#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstring>


bool gameOver;
int width=10, height=10;
int level=1;
int x,y,fruitX,fruitY,Score;
int tailX[100], tailY[100];
int bombX,bombY;
int nTail;
int pace=60;
enum eDirection {Stop=0,Left,Right,Up,Down};
eDirection dir;
bool isChanged=1, exitByChoice=0;
   


bool introToGame()
{
    char choice;
    // Introduces the player to the game
    if(!gameOver){
    std::cout<<"Welcome to the Snake Game!"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Would you like to play to play the game? Type Y for yes and N for no "<<std::endl;
    }
    else if (exitByChoice) {std::cout<<"You exited the game! Would you like to play again? Type Y for yes and N for no"<<std::endl;}
    else {std::cout<<"Oops, you lost. Would you like to play again? Type Y for yes and N for no"<<std::endl;}
    while(true){
        std::cout<<">"; std::cin>>choice;
        if(toupper(choice)=='Y') {return true;}
        if(toupper(choice)=='N'){ 
            std::cout<<"That's unfortunate :( Have a nice day! ";
            return false;}
        // If input is valid, it returns the player to the first line of the while loop to place an input again
        else {std::cout<<"Invalid Input. Please choose Y or N"<<std::endl; continue; }
        }
}




void Setup()
{
    // Everytime we press to start the game again after losing, we reset the tail
    nTail=0;
    tailX[0]=0;tailY[0]=0;
    // Placement of the bomb is done randomly
    width=10; 
    height=10;
    // Placement of the head is done in the center
    x=width/2;
    y=height/2;
    // Placement of bomb is done randomly
    bombX=rand()%width;
    bombY=rand()%height;
    gameOver=false;
    dir=Stop;
    // The placement of fruit is done randomly
    fruitX=rand()%width;
    fruitY=rand()%height;
    Score=0;// Everytime the snake catches the a fruit, we increment the score with 1
    level=2;
    exitByChoice=0;
}

void LevelCheck()
{
     /*else if(Score>10){level=3;}
    else if(Score>15){level=4;}*/
    if(level>2 && isChanged==0){
        height+=3;             //*=level-1;
        width+=6;             //*=level-1;
        Score=0;
        pace-=8;
        isChanged=1;
    }
}


// Building the terrain, the snake and printing the fruit
void Draw()
{ 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
    for(int j=0;j<=width+1;j++) {std::cout<<"#";}// The first # is 0 and the last # is 22, in order to have the width=20
    std::cout<<std::endl;
    for (int i=0;i<=height;i++){
        for(int j=0;j<=width;j++){
            if(j==0) {std::cout<<"#";} // Prints the left wall
            if(i==y && j==x) {std::cout<<"O";} // Prints the head of the snake
            else if(i==fruitY && j==fruitX) {std::cout<<"F";}
            else if(bombX==j && bombY==i) {std::cout<<"+";} 
            else 
            {
                bool print=false;
                for(int k=0;k<nTail;k++){
                    
                    if(tailX[k]==j && tailY[k]==i) {std::cout<<"o"; print=true;}// Prints the tail if coordinates of tailX and tailY (from logic function) are equal to i and j
                    
                }
                if(!print) {std::cout<<" ";}// Prints the whole 20x20 terrain
            }
            if(bombX==fruitX && bombY==fruitY){
                fruitX=rand()%width;
                fruitY=rand()%height;
            } 
             
            if(j==width-1) {std::cout<<"#";} // Prints the right wall
        }
        std::cout<<std::endl;
    }
    for(int j=0;j<=width+1;j++) {std::cout<<"#";} // Prints the bottom wall
    std::cout<<std::endl;
    std::cout<<"Score:"<<Score<<std::endl;
    std::cout<<"Level:"<<level-1<<std::endl;
}


// Assignemt of keys for their corresponding directions and the exit key
void Input()
{
   if(_kbhit()) // Returns a boolean value of true, when a keyboard key is pressed
   {
     switch (_getch())
     {
        // w-snake goes Up, s-snake goes Down, a-snake goes left, d-snake goes right
     case 'a': 
        dir=Left;
        break;
     case 'd':
        dir=Right;
        break;
     case 'w':
        dir=Up;
        break;
     case 's':
        dir=Down;
        break;
     // By pressing x, the game stops
     case 'x':
        exitByChoice=1;
        gameOver=true;
        break;
     }
   }
}
void Logic()
{
    int prevX=tailX[0]; int prevY=tailY[0]; 
    int prev2X, prev2Y;
    tailX[0]=x; tailY[0]=y;
    for(int i=1;i<nTail;i++){
        prev2X=tailX[i]; prev2Y=tailY[i];
        tailX[i]=prevX; tailY[i]=prevY;
        prevX=prev2X; prevY=prev2Y;
    }

    switch(dir)
    {
        // By each direction, we increment or decrement the x or the y (x=width, y=height)
        case Left:
         x--;
         break;
        case Right:
         x++;
         break;
        case Up:
         y--;
         break;
        case Down:
         y++;
         break;
        case Stop:
         dir=Stop;
         break;
    }
    if(x>=width || x<0 || y>=height+1 || y<0 || (y==bombY && x==bombX)) {gameOver=true;} // If the snake crosses the border, the game stops
    for(int i=0;i<nTail;++i){
        if(tailX[i]==x && tailY[i]==y) {gameOver=true;} //If the snake touches itself, the game stops
    }
    
    if(x==fruitX && y==fruitY ){ // Everytime the snake eats the fruit, the tail is incremented and the fruit is randomly placed
        nTail++;
        Score++;
        fruitX=rand()%width;
        fruitY=rand()%height;
        
        if(Score%5==0) {level++; isChanged=0;}

    } 
    

}
int main()
{
     //If "introToGame" returns true--> the game starts, if it returns false--> the terminal stops
     while(introToGame()){
      system("cls");
      Setup();
      while(!gameOver){
        LevelCheck();
        Draw();
        Input();
        Logic();
        Sleep(pace); // Slows the pace of the snake
    }
    if(gameOver){system("cls");}
  }
}
