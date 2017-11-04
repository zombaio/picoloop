// DrumSynth DS file renderer
// Copyright (c)1998-2000 Paul Kellett (mda-vst.com)
// Version 2.0 (5/10/00)

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "drumsynth_oop5.h"
//#include "getprivateprofilestring.h"
//#include "getpriv.h"
#include "profport.h"
#include <math.h>     //sin(), exp(), etc.
#include <stdio.h>    //sscanf(), sprintf()
#include <stdlib.h>   //RAND_MAX

//#define BLOCKSIZE 32

drumsynth::drumsynth()
{
  //wave=NULL;
}

drumsynth::~drumsynth()
{

}


void drumsynth::init()
{
  mem_t=1.0f;
  mem_o=1.0f; 
  mem_n=1.0f; 
  mem_b=1.0f; 
  mem_tune=1.0f; 
  mem_time=1.0f;
  wavemode=0;
  busy=0;

  progress=0;
  pos=0;
  tpos=0;
  x[0]=0.f;
  x[1]=0.f;
  x[2]=0.f;
  TDroop=0;
  b=0.f;
  c=0.f;
  TT=0.f;
  TTT=0.f;
  TphiStart=0.f;
  BdF=0.f;
  BdF2=0.f;
  OF1Sync=0;
  OF2Sync=0;
  Oc0=0.0f;
  Oc1=0.0f;
  Oc2=0.0f;
  MFin=0.f;
  MFout=0.f;

  prepare=0;
  commentLen=0;
  /* if (wave) */
  /*   free(wave); */
  /* wave=NULL; */



}


int drumsynth::LongestEnv(void)
{
  long e, eon, p;
  float l=0.f;  
  
  for(e=1; e<7; e++) //3
  {
    eon = e - 1; if(eon>2) eon=eon-1;
    p = 0;
    while (envpts[e][0][p + 1] >= 0.f) p++; 
    envData[e][MDA_MAX] = envpts[e][0][p] * timestretch;
    if(chkOn[eon]==1) if(envData[e][MDA_MAX]>l) l=envData[e][MDA_MAX];
  }
  //l *= timestretch;

  return 2400 + (1200 * (int)(l / 1200));
}


float drumsynth::LoudestEnv(void)
{  
  float loudest=0.f;
  int i=0;

  while (i<5) //2
  {
    if(chkOn[i]==1) if(sliLev[i]>loudest) loudest=(float)sliLev[i];
    i++;
  }
  return (loudest * loudest);
}


void drumsynth::UpdateEnv(int e, long t)
{  
  float endEnv, dT;
                                                             //0.2's added   
  envData[e][NEXTT] = envpts[e][0][(long)(envData[e][PNT] + 1.f)] * timestretch; //get next point
  if(envData[e][NEXTT] < 0) envData[e][NEXTT] = 442000 * timestretch; //if end point, hold
  envData[e][ENV] = envpts[e][1][(long)(envData[e][PNT] + 0.f)] * 0.01f; //this level
  endEnv = envpts[e][1][(long)(envData[e][PNT] + 1.f)] * 0.01f;          //next level
  dT = envData[e][NEXTT] - (float)t;
  if(dT < 1.0) dT = 1.0;
  envData[e][dENV] = (endEnv - envData[e][ENV]) / dT;
  envData[e][PNT] = envData[e][PNT] + 1.0f;
}


void drumsynth::getEnv(int env, LPCTSTR sec, LPCTSTR key, LPCTSTR ini)
{
  char en[256], s[8];
  int i=0, o=0, ep=0;
  char sec_table[256];
  char key_table[256];
  char ini_table[256];
  strncpy(sec_table,sec,256);
  strncpy(key_table,key,256);
  strncpy(ini_table,ini,256);
  //GetPrivateProfileString(sec, key, "0,0 100,0", en, sizeof(en), ini);
  GetPrivateProfileString(sec_table, key_table, "0,0 100,0", en, sizeof(en), ini_table);
  en[256]=0; //be safe!
    
  while(en[i]!=0)
  {
    if(en[i] == ',') 
    {
      if(sscanf(s, "%f", &envpts[env][0][ep])==0) envpts[env][0][ep] = 0.f;
      o=0;
    }  
    else if(en[i] == ' ')
    {
      if(sscanf(s, "%f", &envpts[env][1][ep])==0) envpts[env][1][ep] = 0.f;
      o=0; ep++;
    }
    else { s[o]=en[i]; o++; s[o]=0; }
    i++;
  }
  if(sscanf(s, "%f", &envpts[env][1][ep])==0) envpts[env][1][ep] = 0.f;
  envpts[env][0][ep + 1] = -1;

  envData[env][MDA_MAX] = envpts[env][0][ep];
}


float drumsynth::waveform(float ph, int form)
{
  float w;
   
  switch (form)
  {
     case 0: w = (float)sin(fmod(ph,TwoPi));                            break; //sine
     case 1: w = (float)fabs(2.0f*(float)sin(fmod(0.5f*ph,TwoPi)))-1.f; break; //sine^2
     case 2: while(ph<TwoPi) ph+=TwoPi;
             w = 0.6366197f * (float)fmod(ph,TwoPi) - 1.f;                     //tri
             if(w>1.f) w=2.f-w;                                         break;
     case 3: w = ph - TwoPi * (float)(int)(ph / TwoPi);                        //saw
             w = (0.3183098f * w) - 1.f;                                break;  
    default: w = (sin(fmod(ph,TwoPi))>0.0)? 1.f: -1.f;                  break; //square
  }         
   
  return w;
}


/* float GetPrivateProfileFloat(LPCTSTR sec, LPCTSTR key, float v, LPCTSTR ini) */
/* { */
/*   char tmp[16]; */
/*   float f=0.f; */

/*   GetPrivateProfileString(sec, key, "", tmp, sizeof(tmp), ini); */
/*   sscanf(tmp, "%f", &f); if(tmp[0]==0) f=v;  */
  
/*   return f;  */
/* } */


//int APIENTRY ds2mem(char *dsfile, short *buffer, float t, float o, float n, float b,
/* int drumsynth::ds2mem(char *dsfile, short *buffer, float t, float o, float n, float b,  */
/*                                                  float tune, float time) */
/* { */
/*   wavemode=1;  //don't make WAV file  */
  
/*   mem_t = t; */
/*   mem_o = o; */
/*   mem_n = n; */
/*   mem_b = b; */
/*   mem_tune = tune; */
/*   mem_time = time; */

/*   //if(ds2wav(dsfile, "", 0)!=0) return 0; */
/*   if(ds2wav(dsfile, "")!=0) return 0; */

/*   //MessageBox(NULL, "Ok!", "DrumSynth", MB_OK); ////////////////// */
/*   buffer = wave; */
/*   return wavewords; */
/* } */


//int APIENTRY ds2buf(long samples, short *buffer)
/* int drumsynth::ds2buf(long samples, short *buffer) */
/* { */
/*   long s = wavewords; */
/*   if(samples<wavewords) s = samples; */
  
/*   if(s>0) memcpy(buffer, wave, s * 2); */
/*   //MessageBox(NULL, "Copied!", "DrumSynth", MB_OK); */

/*   if(wave!=NULL)  */
/*   { */
/*     free(wave); */
/*     wave = NULL; */
/*   } */
/*   return s; */
/* } */



void drumsynth::set_tune(float t)
{
  mem_tune=t;
  //printf("set_tune:%f\n",t);
}

void drumsynth::set_time(float t)
{
  mem_t=t;
  mem_o=t;
  mem_n=t;
  mem_b=t;
  //printf("set_tune:%f\n",t);
}

 
int drumsynth::load_patch(char *dsfile)
{
  // init();
  // if(wavemode==0) //semi-real-time adjustments if working in memory!!
  // {
  //   mem_t = 1.0f;
  //   mem_o = 1.0f;
  //   mem_n = 1.0f;
  //   mem_b = 1.0f;
  //   mem_tune = 0.0f;
  //   mem_time = 1.0f;
  //   mem_time = 2.0f;
  // }

  //try to read version from input file
  //lstrcpy(sec, "General");
  strcpy(sec, "General"); 
  GetPrivateProfileString(sec,"Version","",ver,sizeof(ver),dsfile);
  ver[9]=0; 
  //if(lstrcmp(ver, "DrumSynth") != 0) {busy=0; return 2;} //input fail
  printf("%s\n",ver);
  if(strcmp(ver, "DrumSynth") != 0) {busy=0; return 2;} //input fail
  if(ver[11] != '1' && ver[11] != '2') {busy=0; return 1;} //version fail
  

  //read master parameters
  GetPrivateProfileString(sec,"Comment","",comment,sizeof(comment),dsfile);
  while((comment[commentLen]!=0) && (commentLen<254)) commentLen++;
  if(commentLen==0) { comment[0]=32; comment[1]=0; commentLen=1;}
  comment[commentLen+1]=0; commentLen++;
  if((commentLen % 2)==1) commentLen++; 
  
  timestretch = .01f * mem_time * GetPrivateProfileFloat(sec,"Stretch",100.0,dsfile);
  if(timestretch<0.2f) timestretch=0.2f; 
  if(timestretch>10.f) timestretch=10.f;

  DGain = 1.0f; //leave this here!
  DGain = (float)pow(10.0, 0.05 * GetPrivateProfileFloat(sec,"Level",0,dsfile));

  MasterTune = GetPrivateProfileFloat(sec,"Tuning",0.0,dsfile);
  MasterTune = (float)pow(1.0594631f, MasterTune + mem_tune);
  MainFilter = 2 * GetPrivateProfileInt(sec,"Filter",0,dsfile); 
  MFres = 0.0101f * GetPrivateProfileFloat(sec,"Resonance",0.0,dsfile);
  MFres = (float)pow(MFres, 0.5f);
  
  HighPass = GetPrivateProfileInt(sec,"HighPass",0,dsfile);
  getEnv(7, sec, "FilterEnv", dsfile);

 
  //read noise parameters
  //lstrcpy(sec, "Noise");
  strcpy(sec, "Noise");
  chkOn[1] = GetPrivateProfileInt(sec,"On",0,dsfile);
  sliLev[1] = GetPrivateProfileInt(sec,"Level",0,dsfile);    
  NT =  GetPrivateProfileInt(sec,"Slope",0,dsfile); 
  getEnv(2, sec, "Envelope", dsfile);
  NON = chkOn[1]; 
  NL = (float)(sliLev[1] * sliLev[1]) * mem_n;
  if(NT<0)
  { a = 1.f + (NT / 105.f); d = -NT / 105.f;
    g = (1.f + 0.0005f * NT * NT) * NL; }
  else
  { a = 1.f; b = -NT / 50.f; c = (float)fabs((float)NT) / 100.f; g = NL; }
  
  //if(GetPrivateProfileInt(sec,"FixedSeq",0,dsfile)!=0) 
    srand(1); //fixed random sequence
 
   //read tone parameters
    //lstrcpy(sec, "Tone");
  strcpy(sec, "Tone");
  chkOn[0] = GetPrivateProfileInt(sec,"On",0,dsfile); TON = chkOn[0];
  sliLev[0] = GetPrivateProfileInt(sec,"Level",128,dsfile); 
  TL = (float)(sliLev[0] * sliLev[0]) * mem_t;
  getEnv(1, sec, "Envelope", dsfile);
  F1 = MasterTune * TwoPi * GetPrivateProfileFloat(sec,"F1",200.0,dsfile) / Fs;
  if(fabs(F1)<0.001f) F1=0.001f; //to prevent overtone ratio div0
  F2 = MasterTune * TwoPi * GetPrivateProfileFloat(sec,"F2",120.0,dsfile) / Fs;
  Fsync = F2;
  TDroopRate = GetPrivateProfileFloat(sec,"Droop",0.f,dsfile);
  if(TDroopRate>0.f)
  {
    TDroopRate = (float)pow(10.0f, (TDroopRate - 20.0f) / 30.0f);
    TDroopRate = TDroopRate * -4.f / envData[1][MDA_MAX];
    TDroop = 1;
    F2 = F1+((F2-F1)/(1.f-(float)exp(TDroopRate * envData[1][MDA_MAX])));
    ddF = F1 - F2;
  }
  else ddF = F2-F1;
 
  Tphi = GetPrivateProfileFloat(sec,"Phase",90.f,dsfile) / 57.29578f; //degrees>radians

  //read overtone parameters
  //lstrcpy(sec, "Overtones");
  strcpy(sec, "Overtones");
  chkOn[2] = GetPrivateProfileInt(sec,"On",0,dsfile); OON = chkOn[2];
  sliLev[2] = GetPrivateProfileInt(sec,"Level",128,dsfile); 
  OL = (float)(sliLev[2] * sliLev[2]) * mem_o;
  getEnv(3, sec, "Envelope1", dsfile);
  getEnv(4, sec, "Envelope2", dsfile);
  OMode = GetPrivateProfileInt(sec,"Method",2,dsfile);
  OF1 = MasterTune * TwoPi * GetPrivateProfileFloat(sec,"F1",200.0,dsfile) / Fs;
  OF2 = MasterTune * TwoPi * GetPrivateProfileFloat(sec,"F2",120.0,dsfile) / Fs;
  OW1 = GetPrivateProfileInt(sec,"Wave1",0,dsfile);
  OW2 = GetPrivateProfileInt(sec,"Wave2",0,dsfile);
  OBal2 = (float)GetPrivateProfileInt(sec,"Param",50,dsfile);
  ODrive = (float)pow(OBal2, 3.0f) / (float)pow(50.0f, 3.0f);
  OBal2 *= 0.01f; 
  OBal1 = 1.f - OBal2;
  Ophi1 = Tphi; 
  Ophi2 = Tphi;
  if(MainFilter==0) 
    MainFilter = GetPrivateProfileInt(sec,"Filter",0,dsfile);
  if((GetPrivateProfileInt(sec,"Track1",0,dsfile)==1) && (TON==1))
  { OF1Sync = 1;  OF1 = OF1 / F1; }
  if((GetPrivateProfileInt(sec,"Track2",0,dsfile)==1) && (TON==1))
  { OF2Sync = 1;  OF2 = OF2 / F1; }

  OcA = 0.28f + OBal1 * OBal1;  //overtone cymbal mode
  OcQ = OcA * OcA;
  OcF = (1.8f - 0.7f * OcQ) * 0.92f; //multiply by env 2
  OcA *= 1.0f + 4.0f * OBal1;  //level is a compromise!
  Ocf1 = TwoPi / OF1;
  Ocf2 = TwoPi / OF2;
  for(i=0; i<6; i++) Oc[i][0] = Oc[i][1] = Ocf1 + (Ocf2 - Ocf1) * 0.2f * (float)i;

  //read noise band parameters
  //lstrcpy(sec, "NoiseBand");
  strcpy(sec, "NoiseBand");
  chkOn[3] = GetPrivateProfileInt(sec,"On",0,dsfile); BON = chkOn[3];
  sliLev[3] = GetPrivateProfileInt(sec,"Level",128,dsfile); 
  BL = (float)(sliLev[3] * sliLev[3]) * mem_b;
  BF = MasterTune * TwoPi * GetPrivateProfileFloat(sec,"F",1000.0,dsfile) / Fs;
  BPhi = TwoPi / 8.f;
  getEnv(5, sec, "Envelope", dsfile);
  BFStep = GetPrivateProfileInt(sec,"dF",50,dsfile); 
  BQ = (float)BFStep; 
  BQ = BQ * BQ / (10000.f-6600.f*((float)sqrt(BF)-0.19f));
  BFStep = 1 + (int)((40.f - (BFStep / 2.5f)) / (BQ + 1.f + (1.f * BF)));

  //lstrcpy(sec, "NoiseBand2");
  strcpy(sec, "NoiseBand2");
  chkOn[4] = GetPrivateProfileInt(sec,"On",0,dsfile); BON2 = chkOn[4];
  sliLev[4] = GetPrivateProfileInt(sec,"Level",128,dsfile); 
  BL2 = (float)(sliLev[4] * sliLev[4]) * mem_b;
  BF2 = MasterTune * TwoPi * GetPrivateProfileFloat(sec,"F",1000.0,dsfile) / Fs;
  BPhi2 = TwoPi / 8.f;
  getEnv(6, sec, "Envelope", dsfile);
  BFStep2 = GetPrivateProfileInt(sec,"dF",50,dsfile); 
  BQ2 = (float)BFStep2;
  BQ2 = BQ2 * BQ2 / (10000.f-6600.f*((float)sqrt(BF2)-0.19f));
  BFStep2 = 1 + (int)((40 - (BFStep2 / 2.5)) / (BQ2 + 1 + (1 * BF2)));
 
  //read distortion parameters
  //lstrcpy(sec, "Distortion");
  strcpy(sec, "Distortion");
  chkOn[5] = GetPrivateProfileInt(sec,"On",0,dsfile); DiON = chkOn[5];
  DStep = 1 + GetPrivateProfileInt(sec,"Rate",0,dsfile);
  if(DStep==7) DStep=20; if(DStep==6) DStep=10; if(DStep==5) DStep=8;

  clippoint = 32700;
  DAtten = 1.0f;

  if(DiON==1) 
  {
    DAtten = DGain * (short)LoudestEnv(); 
    if(DAtten>32700) clippoint=32700; else clippoint=(short)DAtten; 
    DAtten = (float)pow(2.0, 2.0 * GetPrivateProfileInt(sec,"Bits",0,dsfile));
    DGain = DAtten * DGain * (float)pow(10.0, 0.05 * GetPrivateProfileInt(sec,"Clipping",0,dsfile));
  }
  
  //prepare envelopes
  randmax = 1.f / RAND_MAX; randmax2 = 2.f * randmax;
  for (i=1;i<8;i++) { envData[i][NEXTT]=0; envData[i][PNT]=0; }
  Length = LongestEnv();

  /* if(wave!=NULL) free(wave); */
  /* wave = (short *)malloc(2 * Length + 1280); //wave memory buffer */
  if(wave==NULL) {busy=0; return 1;}
  wavewords = 0;
  prepare=1;
  return 0;
}

int drumsynth::generate(short * buffer,int len)
{
  //MessageBox(NULL, dsfile, "DrumSynth", MB_OK); //////////////////

  //input file

  int i=0;
  //output file 
  wavemode=1;
  wavewords=0;

  for (i=0;i<len;i++)
    buffer[i]=0;
  
  if (tpos>=Length)
    return -1;


  //sloppy programming? wait if DLL in use
  //while(busy==1) Sleep(0); busy=1;
  //while(busy==1) sleep(0); busy=1;

  prepare=1;
  if (prepare==0)
    {
      return -2;
    }

  for (i=0;i<len;i++)
    buffer[i]=0;


  //generate
  //tpos = 0;
  //while(tpos<Length)
  //{
    tplus = tpos + len-1;

    if(NON==1) //noise
    {
      for(t=tpos; t<=tplus; t++)
      {
        if(t < envData[2][NEXTT]) envData[2][ENV] = envData[2][ENV] + envData[2][dENV];
        else UpdateEnv(2, t);
        x[2] = x[1];
        x[1] = x[0];
        x[0] = (randmax2 * (float)rand()) - 1.f; 
        TT = a * x[0] + b * x[1] + c * x[2] + d * TT;
        DF[t - tpos] = TT * g * envData[2][ENV];
      }
      if(t>=envData[2][MDA_MAX]) NON=0;
    }
    else for(j=0; j<len; j++) DF[j]=0.f;
    
    if(TON==1) //tone
    {
      TphiStart = Tphi;
      if(TDroop==1)
      {
        for(t=tpos; t<=tplus; t++)
          phi[t - tpos] = F2 + (ddF * (float)exp(t * TDroopRate));
      }          
      else
      {
        for(t=tpos; t<=tplus; t++)
          phi[t - tpos] = F1 + (t / envData[1][MDA_MAX]) * ddF;
      }  
      for(t=tpos; t<=tplus; t++)
      {
        totmp = t - tpos;
        if(t < envData[1][NEXTT]) 
          envData[1][ENV] = envData[1][ENV] + envData[1][dENV];
        else UpdateEnv(1, t);
        Tphi = Tphi + phi[totmp];
        DF[totmp] += TL * envData[1][ENV] * (float)sin(fmod(Tphi,TwoPi));//overflow?
      }
      if(t>=envData[1][MDA_MAX]) TON=0;
    }
    else for(j=0; j<len; j++) phi[j]=F2; //for overtone sync
    
    if(BON==1) //noise band 1
    {
      for(t=tpos; t<=tplus; t++)
      {
        if(t < envData[5][NEXTT])
          envData[5][ENV] = envData[5][ENV] + envData[5][dENV];
        else UpdateEnv(5, t);
        if((t % BFStep) == 0) BdF = randmax * (float)rand() - 0.5f;
        BPhi = BPhi + BF + BQ * BdF;
        botmp = t - tpos;
        DF[botmp] = DF[botmp] + (float)cos(fmod(BPhi,TwoPi)) * envData[5][ENV] * BL;
      }
      if(t>=envData[5][MDA_MAX]) BON=0;
    }

    if(BON2==1) //noise band 2
    {
      for(t=tpos; t<=tplus; t++)
      {
        if(t < envData[6][NEXTT])
          envData[6][ENV] = envData[6][ENV] + envData[6][dENV];
        else UpdateEnv(6, t);
        if((t % BFStep2) == 0) BdF2 = randmax * (float)rand() - 0.5f;
        BPhi2 = BPhi2 + BF2 + BQ2 * BdF2;
        botmp = t - tpos;
        DF[botmp] = DF[botmp] + (float)cos(fmod(BPhi2,TwoPi)) * envData[6][ENV] * BL2;
      }
      if(t>=envData[6][MDA_MAX]) BON2=0;
    }

    for (t=tpos; t<=tplus; t++)
    {
      if(OON==1) //overtones
      {
        if(t<envData[3][NEXTT])
          envData[3][ENV] = envData[3][ENV] + envData[3][dENV];
        else
        {
          if(t>=envData[3][MDA_MAX]) //wait for OT2
          {
            envData[3][ENV] = 0; 
            envData[3][dENV] = 0; 
            envData[3][NEXTT] = 999999;
          }
          else UpdateEnv(3, t);
        }
        //
        if(t<envData[4][NEXTT])
          envData[4][ENV] = envData[4][ENV] + envData[4][dENV];
        else
        {
          if(t>=envData[4][MDA_MAX]) //wait for OT1
          {
            envData[4][ENV] = 0;
            envData[4][dENV] = 0;
            envData[4][NEXTT] = 999999;
          }
          else UpdateEnv(4, t);
        }
        //
        TphiStart = TphiStart + phi[t - tpos];
        if(OF1Sync==1) Ophi1 = TphiStart * OF1; else Ophi1 = Ophi1 + OF1;
        if(OF2Sync==1) Ophi2 = TphiStart * OF2; else Ophi2 = Ophi2 + OF2;
        Ot=0.0f;
        switch (OMode)
        {
          case 0: //add
            Ot = OBal1 * envData[3][ENV] * waveform(Ophi1, OW1);
            Ot = OL * (Ot + OBal2 * envData[4][ENV] * waveform(Ophi2, OW2));
            break;
        
          case 1: //FM
            Ot = ODrive * envData[4][ENV] * waveform(Ophi2, OW2);
            Ot = OL * envData[3][ENV] * waveform(Ophi1 + Ot, OW1);
            break;
        
          case 2: //RM
            Ot = (1 - ODrive / 8) + (((ODrive / 8) * envData[4][ENV]) * waveform(Ophi2, OW2));
            Ot = OL * envData[3][ENV] * waveform(Ophi1, OW1) * Ot;
            break;

          case 3: //808 Cymbal
            for(j=0; j<6; j++)
            {
              Oc[j][0] += 1.0f;
              
              if(Oc[j][0]>Oc[j][1])
              { 
                Oc[j][0] -= Oc[j][1]; 
                Ot = OL * envData[3][ENV];
              }
            }
            Ocf1 = envData[4][ENV] * OcF;  //filter freq
            Oc0 += Ocf1 * Oc1;
            Oc1 += Ocf1 * (Ot + Oc2 - OcQ * Oc1 - Oc0);  //bpf
            Oc2 = Ot;
            Ot = Oc1;
            break;
        }
      } 
          
      if(MainFilter==1) //filter overtones
      {
        if(t<envData[7][NEXTT])
          envData[7][ENV] = envData[7][ENV] + envData[7][dENV];
        else UpdateEnv(7, t);

        MFtmp = envData[7][ENV];
        if(MFtmp >0.2f)
          MFfb = 1.001f - (float)pow(10.0f, MFtmp - 1);
        else
          MFfb = 0.999f - 0.7824f * MFtmp; 
        
        MFtmp = Ot + MFres * (1.f + (1.f/MFfb)) * (MFin - MFout); 
        MFin = MFfb * (MFin - MFtmp) + MFtmp;
        MFout = MFfb * (MFout - MFin) + MFin;

        DF[t - tpos] = DF[t - tpos] + (MFout - (HighPass * Ot));
      }
      else if(MainFilter==2) //filter all
      {
        if(t<envData[7][NEXTT])
          envData[7][ENV] = envData[7][ENV] + envData[7][dENV];
        else UpdateEnv(7, t);

        MFtmp = envData[7][ENV];
        if(MFtmp >0.2f)
          MFfb = 1.001f - (float)pow(10.0f, MFtmp - 1);
        else
          MFfb = 0.999f - 0.7824f * MFtmp; 
        
        MFtmp = DF[t - tpos] + Ot + MFres * (1.f + (1.f/MFfb)) * (MFin - MFout); 
        MFin = MFfb * (MFin - MFtmp) + MFtmp;
        MFout = MFfb * (MFout - MFin) + MFin;
        
        DF[t - tpos] = MFout - (HighPass * (DF[t - tpos] + Ot));
      }
      else DF[t - tpos] = DF[t - tpos] + Ot; //no filter
    }
    
    if(DiON==1) //bit resolution
    {
      for(j=0; j<len; j++)
        DF[j] = DGain * (int)(DF[j] / DAtten);
      
      for(j=0; j<len; j+=DStep) //downsampling
      {
        DownAve = 0;
        DownStart = j;
        DownEnd = j + DStep - 1;
        for(jj = DownStart; jj<=DownEnd; jj++) 
          DownAve = DownAve + DF[jj];
        DownAve = DownAve / DStep;
        for(jj = DownStart; jj<=DownEnd; jj++) 
          DF[jj] = DownAve;
      }  
    }
    else for(j=0; j<len; j++) DF[j] *= DGain;
    
    for(j = 0; j<len; j++) //clipping + output
    {
      if(DF[j] > clippoint) 
        buffer[wavewords++] = clippoint;
      else
        if(DF[j] < -clippoint)
          buffer[wavewords++] = -clippoint;
        else 
          buffer[wavewords++] = (short)DF[j];
    } 

    if(((100*tpos) / Length) > (progress + 4))
    {
      progress = (100*tpos) / Length;
      sprintf(percent,"%i%%", progress);
      //if(hWnd>0) SetWindowText(hWnd, percent);
      
    }
    tpos = tpos + len;
    //}

  /* if(wavemode==0) */
  /* { */
  /*   fwrite(wave, 2, Length, fp);  //write data */
  /*   fwrite(&WI,  1, 44, fp); //write INFO chunk */
  /*   fwrite(&comment, 1, commentLen, fp); */
  /*   fclose(fp); */
  /* } */
  /* wavemode = 0; //force compatibility!! */
  /* busy = 0; //success!!  */

  return wavewords;
}

