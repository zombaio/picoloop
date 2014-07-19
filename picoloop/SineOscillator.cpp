#include "SineOscillator.h"

using namespace std;


SineOscillator::SineOscillator() : table(new Sint16[WAVETABLE_SIZE])
{
  printf("SineOscillator::SineOscillator()");
  table_size=WAVETABLE_SIZE;
  //table=NULL;
  //table=(Sint16*)malloc(sizeof(Sint16)*table_size);
  //table=new Sint16[table_size];
  index=0;
  //table = (Sint16*)malloc(sizeof(Sint16)*table_size);
}

SineOscillator::~SineOscillator()
{
  printf("SineOscillator::~SineOscillator() 0x%08.8X\n",table);
  //if (table!=NULL)
    //&& index!=0)
    //free(table);
    //delete(table);
  //table=NULL;
}

void SineOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  Sint16 bitdepth=16-1;
  //if (table==NULL)
  //{
  //  table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      printf("SineOscillator::init() 0x%08.8X\n",table);
      //table=new Sint16[table_size];
      //      if (table==NULL)
      //{
      //	  printf("ERROR\n");
      //	  exit(1);
      //	}
      //    }
      //  table=NULL;
      for (i=0;i<table_size;i++)
	{
	  //f=sin((2*3.14159*i*1)/table_size)*(1<<13);
	  s=sin((2*3.14159*i*1)/table_size)*(1<<bitdepth);
	  table[i]=s;

	  //printf("fvalue = %f ",f);
	  //printf("SineOscillator::init() table[%d]=%d\n",i,table[i]);
	}
      //}
  
}

Sint16 SineOscillator::tick()
{ 
  //index=index+(this->getFreq()*table_size)/44100;
  index=index+offset_next_index;
  if (index>table_size)
    //index=index-table_size;
    index=0;

  //printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];
}


