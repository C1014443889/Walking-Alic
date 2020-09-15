// Includes, namespace and prototypes
#include "template.h"
using namespace AGK;
app App;

const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

//ͼ������
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

//ͼ�����
const int TILE_WIDTH  = 64;
const int TILE_HEIGHT = 48;
//����
const int TILE_ROWS = 10;
const int TILE_COLS = 10;

//��ά���飬�洢ͼ������
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
const int ALEC_SPRITE       = 100; // Alec��������
const int ALEC_FRAME_WIDTH  = 40;  // Alec ֡��
const int ALEC_FRAME_HEIGHT = 75;  // Alec ֡��
const int ALEC_FRAME_COUNT  = 16;  // Alec ֡��

const int EAST_START        = 1;   // �򶫵ĵ�һ֡
const int EAST_END          = 4;   // �򶫵����һ֡
const int NORTH_START       = 5;   // �򱱵�һ֡
const int NORTH_END         = 8;   // �����һ֡
const int SOUTH_START       = 9;   // ���ϵ�һ֡
const int SOUTH_END         = 12;  // �������һ֡
const int WEST_START        = 13;  // ������һ֡
const int WEST_END          = 16;  // �������һ֡
const int ALEC_FPS          = 5;   // Alecÿ��֡��
const int ANIMATION_LOOP    = 1;   //  ��Alec ѭ��
const float ALEC_STARTING_X = 0;   // Alec����ʼxλ��
const float ALEC_STARTING_Y = 150; // Alec����ʼyλ��

//Alec�ķ���
const int NORTH   = 1;
const int SOUTH = 2;
const int EAST  = 3;
const int WEST  = 4;
int g_alecDirection = EAST;//��ʼ����


const int MAX_OBSTACLES = 7;//�ϰ�����������
const int COLLISION_SOUND = 1;//�����ϰ��������
int g_obstacles[MAX_OBSTACLES];



// Begin app, called once at the start
void app::Begin( void )
{
	agk::SetWindowTitle("Walking Alec");
	agk::SetVirtualResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//����Alecͼ�񡢴������顢����λ�ú����
    agk::LoadImage(ALEC_IMAGE, "Alec/Alec.png");
    agk::CreateSprite(ALEC_SPRITE, ALEC_IMAGE);
	agk::SetSpriteDepth(ALEC_SPRITE, 0);
	agk::SetSpritePosition(ALEC_SPRITE, ALEC_STARTING_X, 
                          ALEC_STARTING_Y);
	//���ö���֡
    agk::SetSpriteAnimation(ALEC_SPRITE, ALEC_FRAME_WIDTH, 
                           ALEC_FRAME_HEIGHT, ALEC_FRAME_COUNT);

	agk::LoadSound(COLLISION_SOUND, "Alec/boink.wav");//���������ϰ��������

	//���ر���ͼ��
	loadTiles();

	//��ʾ
	displayTiles();

}

// Main loop, called every frame
void app::Loop ( void )
{
	//��ȡ�����
	float directionX = agk::GetDirectionX();
    float directionY = agk::GetDirectionY();
	//������Ҽ������£�����Alecλ��
	
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

	//ˢ����Ļ
	agk::Sync();
}

// Called when the app ends
void app::End ( void )
{
}


//����ͼ��
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

//��ʾͼ��
void app::displayTiles()
{
	int spriteIndex;
	float x=0,y=0;
	int obstacleCounter = 0;
 
	for(int r=0;r<TILE_ROWS;r++)  //��
	{
		x=0;//ÿһ�У�xλ��ÿ�δ�0��ʼ

		for(int c=0;c<TILE_COLS;c++)   //��
		{
			spriteIndex=agk::CreateSprite(g_tileMap[r][c]);
			
            if (g_tileMap[r][c] > PATHSW)//�����һ���ϰ�
            {
                g_obstacles[obstacleCounter] = spriteIndex;
                obstacleCounter++;
             }

			agk::SetSpritePosition(spriteIndex, x, y);	
             x += TILE_WIDTH;//��һ��tile��λ��
		}
		
      y += TILE_HEIGHT;//һ�н�����y���������

	}
}

//����Alec��Xλ��
void app::updateAlecX(float directionX)
{
	float alecX = agk::GetSpriteX(ALEC_SPRITE);//��ǰxλ��
	float newX;//�µ�Xλ��

	//�ж�����������
   if (directionX > 0)
   {
	   agk::PlaySprite(ALEC_SPRITE, ALEC_FPS, //���Ŷ���֡
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

   agk::SetSpriteX(ALEC_SPRITE, newX);//����xλ��

}

//����Alec��Yλ��
void app::updateAlecY(float directionY)
{
	float alecY = agk::GetSpriteY(ALEC_SPRITE);//��ǰYλ��
	float newY;//�µ�Yλ��

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
	//��ȡAlic��λ��
   float x = agk::GetSpriteX(ALEC_SPRITE);
   float y = agk::GetSpriteY(ALEC_SPRITE);

   for (int index = 0; index < MAX_OBSTACLES; index++)
   {   //�����ײ
      if (agk::GetSpriteCollision(ALEC_SPRITE, g_obstacles[index]))
      {
         // ������ײ����
         agk::PlaySound(COLLISION_SOUND);

         // ��Alic�෴��������ƶ�
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