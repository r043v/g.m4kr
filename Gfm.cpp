// transparent blit routine for tinyPtc | Gdl²
// (C) 2k4/2k5 r043v

#include <windows.h>
#include <alloc.h>
#include <stdio.h>

#include "Unit1.h"

extern int WIDTH, HEIGHT, fSIZE, *pixel ;
extern unsigned char key[256] ;

int getSize(FILE *f)
{ int last = ftell(f) ;  fseek(f,0,SEEK_END) ;
  int size = ftell(f) ;
  fseek(f,last,SEEK_SET) ; return size ;
}

void toHeader(const char *file, const char * name, unsigned char *data=0, int sz=0)
{ FILE *f = fopen(file,"a") ;
  fprintf(f,"unsigned char %s[%i] = {\n",name,sz) ;
  for(int c=0;c<sz-1;c++){ fprintf(f,"\t%i,",data[c]) ; if(c>1) if(!((c+1)%8)) fprintf(f,"\n") ; }
  fprintf(f,"\t%i \n};\n",data[sz-1]) ; fclose(f) ;
}

void bin2h(const char *file, const char * name)
{ FILE *f = fopen(file,"rb") ;
  int sz = getSize(f) ;
  char * fl = (char*)malloc(sz) ;
  fread(fl,sz,1,f) ;    fclose(f) ;
  char path[42] ;       sprintf(path,"%s.h",file) ;
  f = fopen(path,"wb") ; fprintf(f,"unsigned char %s[%i] = {\n",name,sz) ;
  for(int c=0;c<sz-1;c++) { fprintf(f,"\t%i,",(unsigned char)fl[c]) ; if(c) if(!(c%8)) fprintf(f,"\n") ; }
  fprintf(f,"\t%i \n};\n",(unsigned char)fl[sz-1]) ;  fclose(f) ;
  free(fl);
}

void gfm2h(int *Gfm, const char * path, const char * name)
{ FILE *f = fopen(path,"a") ; // open in add
  fprintf(f,"int %s[%i]={",name,Gfm[1]/4) ;
  for(int c=0;c<(Gfm[1]/4)-1;c++) { fprintf(f,"\t0x%X,",Gfm[c]) ; if(Gfm[c] <= 0xFFFFF) fprintf(f,"\t"); if(c>1) if(!((c+1)%8)) fprintf(f,"\n\t") ; }
  fprintf(f,"\t0x%X \n};\n",Gfm[(Gfm[1]/4)-1]) ; fclose(f) ;
}

int* scanImg(int *img, int sx, int sy, int *trClr)
{   int *start, *lend;
    int tfull=0, ttr=0 ;  int line=0 ;
    int *dta  = (int*)malloc((sx*sy+sy*42*6)*4) ;        int size ;
    int * jmp = dta+sx*sy ; int * sze = jmp + sy*42*2 ; int * cln = sze + sy*42*2 ;
    int * jmpPtr = jmp ;    int * szePtr = sze ;    int * dtaPtr = dta ; int * clnPtr = cln ;
    int h=sy ;
    //*trClr = 0xC0C0BC02 ;
    img = img + sx*sy ; img -= sx ;
    lend = img + sx ;
    while(h--)
    {  *clnPtr = 0 ;
     while(img < lend)
     { (*clnPtr)++ ;
       start=img ;
       while(img < lend && *img == *trClr) ++img ;

       size = img-start ;
       ttr+=size ; start=img ; *jmpPtr++ = size ;

       while(img < lend && *img != *trClr) ++img ;

       size = img-start ; *szePtr++ = size ;
       if(size) { memcpy(dtaPtr,start,size*4) ; line++ ; dtaPtr+=size ; }
       else {  (*clnPtr)-- ; szePtr-- ; jmpPtr-- ; }
     }; clnPtr++ ;
         lend -= sx ; img = lend-sx ;
    };

    tfull = dtaPtr-dta ;
    int objectSize = 16 + (sy + line*2 + tfull)*4 ;
    int *out = (int *)malloc(objectSize) ; // alloc size for the Gfm data

    #ifdef VERBOSE
           printf("\nalloc %i bytes for the object.",objectSize) ;
    #endif

    int *o = out ;

    *o++ = 0x6d6647 ;            // put signature "Gfm\0"
    *o++ = objectSize ;            // put Gfm object size
    *o++ = sx<<16 | (sy & 65535) ; // put frame size x and y

    #ifdef VERBOSE
        printf("\n\nscan result\n") ;
    #endif

    int c=0 ; jmpPtr=jmp ; szePtr=sze ; dtaPtr=dta ; int cn ;
    while(c < sy)
    {   *o++ = cln[c] ;
        #ifdef VERBOSE
            printf("\nline %i\t %i full lines\t{ ",sy-c,cln[c]) ;
        #endif
        for(cn=0;cn < cln[c];cn++)
        {
          #ifdef VERBOSE
            if(*jmpPtr) printf("+%i ",*jmpPtr) ;
            printf("w%i ",*szePtr) ;
          #endif
          *o++ = *jmpPtr ; *o++ = *szePtr ;
          memcpy(o,dtaPtr,(*szePtr)*4) ; dtaPtr += *szePtr ; o += *szePtr ;
          jmpPtr++ ; szePtr++ ;
        };
        c++ ;
        #ifdef VERBOSE
           printf("}") ;
        #endif
    };      *o++ = 0x2a2a2a2a ; // end check with "****"
        #ifdef VERBOSE
               printf("\n\nobject size : %i bytes", (o-out)*4) ;
               printf("\n\n* total : \ntr   %i\nfull %i\nsum  %i\n%i lines\nsx * sy = %i * %i = %i\n",ttr,tfull,ttr+tfull,line,sx,sy,sx*sy) ;
        #endif
    free(dta) ; return out ;
}

void addFreeEntry(void * add) ;

int thereis16color(int *buffer, int size)    // 16 + transp -> 17 color
{ int pal[17], color, cpt, nb=1 ;  memset(pal,0,68) ; pal[0] = *buffer ;
  while(size)
  { color = buffer[--size] ;
    cpt=0 ; while(cpt<17) if(pal[cpt] == color) break ;
                          else                  cpt++ ;
    if(cpt==17) { pal[nb++]=color ;
                  if(nb == 17) return 0 ;
                }
  };
return (nb < 18) ;
}

char getId(int color, int *pal)
{ for(int c=0;c<16;c++)
   if(pal[c] == color) return c ;

  return 0 ;
}

// convert a 32 bit Gfm into a 16 color Gfm.
unsigned char* Gfm2data(int *Gfm, int *sze=0)
{ // first, count color number and create palette
  int clNum=0 ; int pal[16] ; memset(pal,0,16*4) ;
  int sy = Gfm[2]&0xffff ;
  int sx = Gfm[2]>>16 ;
  //int *max = Gfm ; max += Gfm[1] ;
  int cnt=0, c, tmp, tmp2, tmp3 ; int *gPtr = Gfm + 3 ; int jump, size ;
  while(cnt++ < sy)
  {    c = *gPtr++ ;
       while(c--) { *gPtr++ ; size = *gPtr++ ;
                    for(tmp=0;tmp<size;tmp++)
                    {   tmp3=0 ;
                        for(tmp2=0;tmp2<clNum;tmp2++)
                          if(*gPtr == pal[tmp2]) { tmp3=1 ; break ; }
                        if(!tmp3) pal[clNum++] = *gPtr ;
                        if(clNum > 16) Application->MessageBox("more than 16 color !", "", MB_OK);
                        gPtr++ ;
                    };
                  };
  };

  gPtr = Gfm+3 ; cnt=0 ;
  unsigned char *out = (unsigned char*)malloc(1024000) ; unsigned char *o = out ;
  int temp, temp2 ;

  while(cnt++ < sy)
  {    *o++ = c = *gPtr++ ; //printf("\nline %i, %i sublines",cnt,c) ;
       while(c--) { jump = *gPtr++ ; size = *gPtr++ ;
                    if(jump > 255 || size > 255){ sprintf(Form1->msg,"jump or size too big for 4b Gfm !\njump %i\nsize %i\nline %i",jump,size,cnt) ;
                                    Application->MessageBox(Form1->msg, "", MB_OK);
                                    free(out) ; return 0 ;
                    }
                    *o++ = jump ; *o++ = size ;
                    while(size > 1) {
                     //if(gPtr >= max) *o = 0 ;
                                      temp  = getId(*gPtr,pal)<<4 ; gPtr++ ;
                                      temp2 = getId(*gPtr,pal)&15 ; gPtr++ ;
                                      temp = temp | temp2 ;
                                      size-=2 ; *o++ = temp ;
                                    };
                    if(size){ *o++ = getId(*gPtr++,pal) ; }
                  };
  };

  unsigned char * agf = (unsigned char*)malloc(16 + (o-out) + clNum*4) ; unsigned char *a = agf ;
  //if(!agf) { printf("\nalloc error.\n") ; /*system("pause") ;*/ }
  *(int*)agf = 0x00666761 ; // put "agf"
  agf+=3 ;  *agf++ = clNum ; agf+=4 ;
  *(short*)agf = sx ; agf+=2 ; *(short*)agf = sy ; agf+=2 ;
  *(int*)agf = Gfm[1] ; agf+=4 ; // put Gfm size
  memcpy(agf,pal,clNum*4) ; agf += clNum*4 ;
  memcpy(agf,out,o-out)   ; agf += o-out   ;
  *(int*)(a+4) = agf-a ; // put agf size
  if(sze) *sze = agf-a ;
  free(out) ;
  //printf("\nconvert finish ...\noriginal size %i\nnew size %i",(gPtr-Gfm)*4,agf-a) ;
  return a ;
}

void saveGfm(void *Gfm, const char *path) // save a 4b or 32b gfm into a .gfm file
{ int *g = (int *)Gfm ;
  int chk = *g ; if(!(chk == 0x6d6647 || chk == 0x666761)) return ; // allow only "gfm" or "agf"
  FILE *f = fopen(path,"w") ;
  int size = g[1] ;
  fwrite(Gfm,size,1,f) ;
  fclose(f) ;
}

void mGfm2h(int **Gfm, int nb, const char *path, const char *name, int archive=1, int startFrm=-1)
{ char nm[42], pt[1024] ; int size ; unsigned char *Gf ;
  int bin = Form1->createBin->Checked ;
  if(bin) { if(nb>1) {  sprintf(Form1->msg,"do you really want create %i files ?",nb) ;
                        if(Form1->ask(Form1->msg)!=6) return ; // if no, return
                     }
            strcpy(pt+512,path) ;
            int n=512 ; while(pt[++n] != '.') ; pt[n]=0 ;  if(n>1000) return ;
  }
  for(int c=0;c<nb;c++)
  {  sprintf(nm,"%s%i",name,c+startFrm+(startFrm < 0)) ;
     if(bin){ if(nb>1) sprintf(pt,"%s%i.gfm",pt+512,c) ;
                 else  sprintf(pt,"%s.gfm",pt+512) ;
            }
     if(archive) { Gf = Gfm2data(Gfm[c],&size) ;
                   if(Gf){  if(bin) saveGfm(Gf,pt) ;
                            else toHeader(path, nm, Gf, size);
                            free(Gf) ;
                         }
     }    else { if(bin) saveGfm(Gfm[c],pt) ;
                  else gfm2h(Gfm[c], path, nm);
               } 
  };
 if(!bin && startFrm < 0) // if some frame converted, add array
 { FILE *f = fopen(path,"a") ;
   if(archive) fprintf(f,"\n\nint * data2Gfm(unsigned char *data) ;\n\nint* %s[%i] = { data2Gfm(%s0)",name,nb,name);
   else        fprintf(f,"\n\nint* %s[%i] = { %s0",name,nb,name);

   for(int c=1;c<nb;c++)
        if(archive) fprintf(f,", data2Gfm(%s%i)",name,c) ;
         else       fprintf(f,", %s%i",name,c) ;

   fprintf(f," };") ; fclose(f) ;
 }
}

void rotate90r(int *s, int *d, int sx, int sy)
{	int cptx=sx ; int * dd = d ; d+=(sx-1) ;
	while(cptx--)
	{	for(int cpty=0;cpty<sy;cpty++)
		{	*d = *s++ ; d+=sx ;	};
		d = dd+(cptx-1) ;
	};
}

void mGfm2Gph(int **Gfm, int nb, const char *path, const char *name, int deep)
{   if(deep==8)
     { u8 * rbf = (u8*)malloc(sx*sy*4) ;
       for(int c=0;c<nb;c++)
        { rotate90r(bf[c], int *d, int sx, int sy)



        };
     }
}





