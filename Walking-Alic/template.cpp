// Includes, namespace and prototypes
#include "template.h"
using namespace AGK;
app App;

const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

//图像索引
const int GRASS  = 1;
const int PATH   = 2;
const int PATHNE = 3;
const int PATHNW = 4;
const int PATHSE = 5;
const int PATHSW = 6;
const int TREENW = 7;
const int TREENE = 8;
const int TREESW = 9;
const int TREESE = 10;
const int ROCK   = 11;

//图像宽、高
const int TILE_WIDTH  = 64;
const int TILE_HEIGHT = 48;
//行列
const int TILE_ROWS = 10;
const int TILE_COLS = 10;

//二维数组，存储图像索引
int g_tileMap[TILE_ROWS][TILE_COLS] =
{ {GRASS, GRASS, GRASS, GRASS, GRASS,  GRASS,  GRASS,  GRASS,  GRASS, GRASS},
  {GRASS, GRASS, GRASS, ROCK,  GRASS,  GRASS,  GRASS,  GRASS,  GRASS, GRASS},
  {GRASS, GRASS, GRASS, GRASS, GRASS,  GRASS,  GRASS,  PATHNW, PATH,  PATH },
  {GRASS, GRASS, GRASS, GRASS, GRASS,  TREENW, TREENE, PATH,   GRASS, GRASS},
  {PATH,  PATH,  PATH,  PATH,  PATHNE, TREESW, TREESE, PATH,   ROCK,  GRASS},
  {GRASS, GRASS, GRASS, GRASS, PATH,   GRASS,  GRASS,  PATH,   GRASS, GRASS},
  {GRASS, GRASS, GRASS, GRASS, PATHSW, PATH,   PATH,   PATHSE, GRASS, GRASS},
  {GRASS, GRASS, GRASS, GRASS, GRASS,  GRASS,  GRASS,  GRASS,  GRASS, GRASS},
  {GRASS, ROCK,  GRASS, GRASS, GRASS,  GRASS,  GRASS,  GRASS,  GRASS, GRASS},
  {GRASS, GRASS, GRASS, GRASS, GRASS,  GRASS,  GRASS,  GRASS,  GRASS, GRASS}
};

const int ALEC_IMAGE        = 12;  // Texture atlas image index
const int ALEC_SPRITE       = 100; // Alec精灵索引
const int ALEC_FRAME_WIDTH  = 40;  // Alec 帧宽
const int ALEC_FRAME_HEIGHT = 75;  // Alec 帧高
const int ALEC_FRAME_COUNT  = 16;  // Alec 帧数

const int EAST_START        = 1;   // 向东的第一帧
const int EAST_END          = 4;   // 向东的最后一帧
const int NORTH_START       = 5;   // 向北第一帧
const int NORTH_END         = 8;   // 向北最后一帧
const int SOUTH_START       = 9;   // 向南第一帧
const int SOUTH_END         = 12;  // 向南最后一帧
const int WEST_START        = 13;  // 向西第一帧
const int WEST_END          = 16;  // 向西最后一帧
const int ALEC_FPS          = 5;   // Alec每秒帧数
const int ANIMATION_LOOP    = 1;   //  让Alec 循环
const float ALEC_STARTING_X = 0;   // Alec的起始x位置
const float ALEC_STARTING_Y = 150; // Alec的起始y位置

//Alec的方向
const int NORTH   = 1;
const int SOUTH = 2;
const int EAST  = 3;
const int WEST  = 4;
int g_alecDirection = EAST;//起始方向


const int MAX_OBSTACLES = 7;//障碍物的最大数量
const int COLLISION_SOUND = 1;//碰到障碍物的声音
int g_obstacles[MAX_OBSTACLES];



// Begin app, called once at the start
void app::Begin( void )
{
	agk::SetWindowTitle("Walking Alec");
	agk::SetVirtualResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//加载Alec图像、创建精灵、设置位置和深度
    agk::LoadImage(ALEC_IMAGE, "Alec/Alec.png");
    agk::CreateSprite(ALEC_SPRITE, ALEC_IMAGE);
	agk::SetSpriteDepth(ALEC_SPRITE, 0);
	agk::SetSpritePosition(ALEC_SPRITE, ALEC_STARTING_X, 
                          ALEC_STARTING_Y);
	//设置动画帧
    agk::SetSpriteAnimation(ALEC_SPRITE, ALEC_FRAME_WIDTH, 
                           ALEC_FRAME_HEIGHT, ALEC_FRAME_COUNT);

	agk::LoadSound(COLLISION_SOUND, "Alec/boink.wav");//加载碰到障碍物的声音

	//加载背景图像
	loadTiles();

	//显示
	displayTiles();

}

// Main loop, called every frame
void app::Loop ( void )
{
	//获取方向键
	float directionX = agk::GetDirectionX();
    float directionY = agk::GetDirectionY();
	//如果左右键被按下，更新Alec位置
	
   if (directionX != 0)
   {
      updateAlecX(directionX);
	  checkForCollision();

   }

   if (directionY != 0)
   {
      updateAlecY(directionY);
	  checkForCollision();

   }

	//刷新屏幕
	agk::Sync();
}

// Called when the app ends
void app::End ( void )
{
}


//加载图像
void app::loadTiles()
{
   agk::LoadImage(GRASS,  "Alec/Grass.png");
   agk::LoadImage(PATH,   "Alec/Path.png");
   agk::LoadImage(PATHNE, "Alec/PathNE.png");
   agk::LoadImage(PATHNW, "Alec/PathNW.png");
   agk::LoadImage(PATHSE, "Alec/PathSE.png");
   agk::LoadImage(PATHSW, "Alec/PathSW.png");
   agk::LoadImage(TREENE, "Alec/TreeNE.png");
   agk::LoadImage(TREENW, "Alec/TreeNW.png");
   agk::LoadImage(TREESE, "Alec/TreeSE.png");
   agk::LoadImage(TREESW, "Alec/TreeSW.png");
   agk::LoadImage(ROCK,   "Alec/Rock.png");

}

//显示图像
void app::displayTiles()
{
	int spriteIndex;
	float x=0,y=0;
	int obstacleCounter = 0;
 
	for(int r=0;r<TILE_ROWS;r++)  //行
	{
		x=0;//每一行，x位置每次从0开始

		for(int c=0;c<TILE_COLS;c++)   //列
		{
			spriteIndex=agk::CreateSprite(g_tileMap[r][c]);
			
            if (g_tileMap[r][c] > PATHSW)//如果是一个障碍
            {
                g_obstacles[obstacleCounter] = spriteIndex;
                obstacleCounter++;
             }

			agk::SetSpritePosition(spriteIndex, x, y);	
             x += TILE_WIDTH;//下一个tile的位置
		}
		
      y += TILE_HEIGHT;//一行结束，y坐标会增加

	}
}

//更新Alec的X位置
void app::updateAlecX(float directionX)
{
	float alecX = agk::GetSpriteX(ALEC_SPRITE);//当前x位置
	float newX;//新的X位置

	//判断向左还是向右
   if (directionX > 0)
   {
	   agk::PlaySprite(ALEC_SPRITE, ALEC_FPS, //播放动画帧
                      ANIMATION_LOOP, 
                      EAST_START, EAST_END);

	   g_alecDirection = EAST;

	   newX = alecX + 1;
   }
   else if(directionX < 0)
   {
      agk::PlaySprite(ALEC_SPRITE, ALEC_FPS, 
                      ANIMATION_LOOP, 
                      WEST_START, WEST_END);

	  g_alecDirection = WEST;

      newX = alecX - 1;

   }

   agk::SetSpriteX(ALEC_SPRITE, newX);//设置x位置

}

//更新Alec的Y位置
void app::updateAlecY(float directionY)
{
	float alecY = agk::GetSpriteY(ALEC_SPRITE);//当前Y位置
	float newY;//新的Y位置

	if (directionY < 0)
	{
      agk::PlaySprite(ALEC_SPRITE, ALEC_FPS, 
                      ANIMATION_LOOP, 
                      NORTH_START, NORTH_END);

	  g_alecDirection = NORTH;

	  newY = alecY - 1;
	}
	else if(directionY > 0)
	{
		agk::PlaySprite(ALEC_SPRITE, ALEC_FPS, 
                      ANIMATION_LOOP,
                      SOUTH_START, SOUTH_END);

		g_alecDirection = SOUTH;

		newY = alecY + 1;

	}

	agk::SetSpriteY(ALEC_SPRITE, newY);
}


void app::checkForCollision()
{
	//获取Alic的位置
   float x = agk::GetSpriteX(ALEC_SPRITE);
   float y = agk::GetSpriteY(ALEC_SPRITE);

   for (int index = 0; index < MAX_OBSTACLES; index++)
   {   //检测碰撞
      if (agk::GetSpriteCollision(ALEC_SPRITE, g_obstacles[index]))
      {
         // 播放碰撞声音
         agk::PlaySound(COLLISION_SOUND);

         // 向Alic相反方向进行移动
         switch (g_alecDirection)
         {
         case NORTH:
            y++;
            break;

         case SOUTH:
            y--;
            break;

         case EAST:
            x--;
            break;

         case WEST:
            x++;
            break;
         }

         // Move Alec.
         agk::SetSpritePosition(ALEC_SPRITE, x, y);
      }
   }


}