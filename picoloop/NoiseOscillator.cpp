#include "NoiseOscillator.h"

using namespace std;


NoiseOscillator::NoiseOscillator()
{
  printf("NoiseOscillator::NoiseOscillator()");
  table_size=1024;
  table=NULL;
  index=0;
}

NoiseOscillator::~NoiseOscillator()
{
  printf("NoiseOscillator::~NoiseOscillator()\n");
  if (table!=NULL)
    free(table);
}

void NoiseOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16;
  srand(1<<(bitdepth-2));
  if (table==NULL)
    {
      table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      for (i=0;i<table_size;i++)
	{
	  table[i]=rand();
	  printf("NoiseOscillator::init() table[%d]=%d\n",i,table[i]);
	}
    }  
}


Sint16 NoiseOscillator::tick()
{

  index=index+(this->getFreq()*table_size)/44100;
  if (index>table_size)
    index=index-table_size;
  //printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];

}
