//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ratioClick(TObject *Sender)
{ img->Proportional ^= 1 ;  updatePreview();
  outTypeChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::stretchClick(TObject *Sender)
{  img->Stretch ^= 1 ;  updatePreview();
   outTypeChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::centerClick(TObject *Sender)
{  img->Center ^= 1 ;  updatePreview();
   outTypeChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::alphaChange(TObject *Sender)
{    if(alpha->Position == 255)
     {    Form1->AlphaBlend = 0 ;
          sprintf(msg,"alpha off") ;
     } else {
                Form1->AlphaBlend = 1 ;
                Form1->AlphaBlendValue = alpha->Position ;
                sprintf(msg,"alpha : %i",alpha->Position) ;
            }
     msgTime = GetTickCount() ;
     StatusBar->SimpleText = msg ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::onTopClick(TObject *Sender)
{  if(onTop->Checked){
     Form1->FormStyle = fsStayOnTop ;
     sprintf(msg,"on top") ;
   } else { Form1->FormStyle = fsNormal ;
            sprintf(msg,"on top off") ;
   }
   StatusBar->SimpleText = msg ;
   msgTime = GetTickCount() ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{       Form1->Height = 171 ;
        DragAcceptFiles(Handle, True);
        //outType->ItemIndex = 0 ;
        //outFormat->ItemIndex = 0 ;
        msgTime = GetTickCount() + 102400 ;
        bmp = new Graphics::TBitmap ;
        //preview->Visible = 0 ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::updatePreview(void)
{  if(!preview->img->Visible)
   {  sprintf(msg,"%i x %i",img->Picture->Graphic->Width,img->Picture->Graphic->Height) ;
      StatusBar->SimpleText = msg ;
     prev = new Graphics::TBitmap();
     prev->Width  = img->Picture->Graphic->Width + 15*(flipWay->Visible) ;
     prev->Height = img->Picture->Graphic->Height;
     preview->img->Picture->Graphic = prev;
     //for(int x=0 ; x<prev->Width ; x++)
     // for(int y=0 ; y<prev->Height ; y++)
     //  preview->img->Canvas->Pixels[x][y] = 0x00ff0000;
     preview->img->Visible = 1 ;
   }
   preview->img->Left = 15*(flipWay->Visible) ;
   if(preview->frmSelect->Position == 0) preview->img->Picture->Assign(img->Picture);
   preview->img->Proportional = img->Proportional ;
   preview->img->Stretch = img->Stretch ;
   preview->img->Center = img->Center ;
   preview->img->Height = preview->ClientHeight ;
   preview->img->Width = preview->ClientWidth - 15*(flipWay->Visible) ;

   return ;
/*        TColor color;
        byte r,g,b,grey;
   for (int x=bmp->Width;x>0;x--)
        for(int y=bmp->Height;y>0;y--)
        {
        color = img->Canvas->Pixels[x][y];
        r=byte(color);
        g=byte(color);
        b=byte(color);
        grey = (r+g+b)/3;
        preview->img->Canvas->Pixels[x][y]= (grey << 16) + (grey << 8) + grey;
        } */
}

void __fastcall TForm1::Timer300Timer(TObject *Sender)
{
     if(GetTickCount() > msgTime + 800)
     {  msgTime += 10240 ;
        StatusBar->SimpleText = "";
     }
}
//---------------------------------------------------------------------------

int __fastcall TForm1::ask(const char * m)
{ return Application->MessageBox(m,"?!",MB_YESNO) ; }

// fires an event when a file, or files are dropped onto the application.
void __fastcall TForm1::WMDropFiles(TWMDropFiles &message)
{ AnsiString FileName;
  FileName.SetLength(MAX_PATH);
  FileName.SetLength(DragQueryFile((HDROP)message.Drop, 0,FileName.c_str(), MAX_PATH));

  if(UpperCase(ExtractFileExt(FileName)) == ".BMP")
  {   bmp->LoadFromFile(FileName) ;
      img->Picture->Bitmap->Assign(bmp) ;
  } else if(UpperCase(ExtractFileExt(FileName)) == ".JPG")
         {   TJPEGImage * jpeg = new TJPEGImage();
             jpeg->LoadFromFile(FileName);
             jpeg->DIBNeeded();   
             bmp->Assign(jpeg);
             img->Picture->Bitmap->Assign(bmp) ;
             delete jpeg;
         }
  preview->frmSelect->Position = 0 ;
  outType->ItemIndex = 1 ;
  if(preview->Visible) updatePreview();
  int l = bmp->Width, h = bmp->Height ;
  transType->ItemIndex = 0 ;
  //preview->TransparentColor = 0 ;
  // auto compute frame number and frameset way
   if(l < 142 || h < 142)
    {  
       int s ; int yyop ; int *v=&h, *vh=&l ;
       if(l > h*2) { v=&l ; vh=&h ; vertical=0 ; } // horizontal frameset
       else vertical = 1 ;
       flipWay->Checked = !vertical ;
       s = *vh + 3 ;
       do { yyop = (*v)/s ; } while(s && yyop*s-- != *v) ; ++s ;
       if(s < 4)
       {  frmNb->Text = 1 ;
          sprintf(msg,"frames of %i*%i",l,h) ;
          LabelSize->Caption = msg ; fsx = l ; fsy = h ;
       }
       else {nb = yyop ; fsx = s ; fsy = *vh ;
             frmNb->Text = yyop ;
             sprintf(msg,"frames of %i*%i",s,*vh) ;
             LabelSize->Caption = msg ;
       }
    } else { frmNb->Text = 1 ;
             sprintf(msg,"frames of %i*%i",l,h) ;
             LabelSize->Caption = msg ;
             nb=1 ;
           }
  preview->ClientHeight = h ;
  preview->ClientWidth  = l + (nb > 1)*15 ;
  preview->Caption      = FileName + " (" + nb + ")" ;
  fname = FileName ;
  outTypeChange((TObject*)42) ;
  Form1->Button1->Enabled = 1 ;
  Form1->Height = 304 ;
  Form1->StatusBar->Visible = 1 ;
  // tell the OS that you're finished...
  DragFinish((HDROP) message.Drop);
}
void __fastcall TForm1::imgClick(TObject *Sender)
{  preview->Visible ^= 1 ;
   if(preview->Visible)
    { preview->frmSelect->Visible = flipWay->Visible ;
      outTypeChange(Sender);
    }
   updatePreview();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        delete bmp ;
        if(prev) delete prev ;        
}
//---------------------------------------------------------------------------
int* scanImg(int *img, int sx, int sy, int *trClr);
void mGfm2h(int **Gfm, int nb, const char *path, const char *name, int archive=1, int startFrm=-1);
int thereis16color(int *buffer, int size);

int *current2Gfm(int trColor = -42)
{ int sx = preview->img->Picture->Graphic->Width ;
  int sy = preview->img->Picture->Graphic->Height ;
  int *buffer = (int*)malloc(sx*sy*4), *ptr, *Gfm, n=0 ;
   if(trColor == -42)
   {   if(!preview->TransparentColor) trColor = 0x42434445 ;
        else trColor = (int)preview->TransparentColorValue  ;
   }
 switch(Form1->outType->ItemIndex)
 { case 0 : // Gfm 4 bit
   do { switch(n++)
        { case 1 : preview->img->Picture->Bitmap->PixelFormat = pf8bit ; break ;
          case 2 : preview->img->Picture->Bitmap->PixelFormat = pf4bit ; break ;
        };
     preview->img->Picture->Bitmap->PixelFormat = pf32bit ;
     for(int y=0;y<sy;y++) // fill buffer
      { ptr = (int*)(preview->img->Picture->Bitmap->ScanLine[y]) ;
        for(int x=0;x<sx;x++)
         buffer[y*sx+x] = ptr[x] ;
      };
   } while(!thereis16color(buffer, sx*sy)) ;
   if(n == 3) Form1->StatusBar->SimpleText = "down to 4b." ;
      Gfm = scanImg(buffer,sx,sy,&trColor) ;
   break ;
   case 1 : // Gfm 32 bit
    preview->img->Picture->Bitmap->PixelFormat = pf32bit ; // set deep to 32 bits
     for(int y=0;y<sy;y++) // fill buffer
      { ptr = (int*)(preview->img->Picture->Bitmap->ScanLine[y]) ;
        for(int x=0;x<sx;x++)
         buffer[y*sx+x] = ptr[x] ;
      };
      Gfm = scanImg(buffer,sx,sy,&trColor) ;
   break ;
 };
 free(buffer) ; return Gfm ;
}

void downTo(int deep) ;

void __fastcall TForm1::Button1Click(TObject *Sender)
{ preview->Visible = 1 ; int n=Form1->frmNb->Text.ToInt() ;
  int multiple = (!preview->frmSelect->Position) ;
  char *pth = (char *)malloc(1536) ;
  strcpy(pth,Form1->fname.c_str()); strcpy(pth+1024,pth);
  char *p = pth ; while(*p != '.') p++ ; *p=0 ;
  sprintf(pth+512 ,"%s.h",pth) ;
  char *np = p ; while(*np != '\\' && np>=pth) np-- ; np++ ;
  FILE *f = fopen(pth+512,"w") ;
  int bbp = 4+28*(outType->ItemIndex == 1) ;
  int zn = n ; if(!multiple) zn=1 ;
  fprintf(f,"\n// file %s (%ix%i) saved into %i %ix%i %ib Gfm ",pth+1024,
            img->Picture->Graphic->Width,img->Picture->Graphic->Height,zn,fsx,fsy,bbp) ;
  if(multiple) fprintf(f,"(%s0 to %s%i) ",np,np,zn-1) ;
    else       fprintf(f,"(frame %i/%i) ",preview->frmSelect->Position,n) ;
  fprintf(f,"| converted by Gdl² - Gm4kr.42\n\n") ; fclose(f) ;

  if(multiple) // convert all the frameset
  {  int **G = (int**)malloc(4*n), c ;
     int trColor ; preview->frmSelect->Position = 1 ; outTypeChange(0) ;
       if(!preview->TransparentColor) trColor = 0x42434445 ;
        else trColor = (int)preview->TransparentColorValue  ;
     for(c=0;c<n;c++)
      { preview->frmSelect->Position = c+1 ;
        outTypeChange(0) ; G[c] = current2Gfm(trColor) ;
      };
     mGfm2h(G,n,pth+512, np, (outType->ItemIndex == 0) ) ;
     for(c=0;c<n;c++) free(G[c]) ; free(G) ; free(pth) ;
     preview->frmSelect->Position = 0 ; outTypeChange(0) ;
     return ;
  }
      // else, save only one frame
  outTypeChange(0) ; int *Gfm = current2Gfm() ; // save preview as a Gfm

 switch(outType->ItemIndex)
 { case 0 : mGfm2h(&Gfm, 1, pth+512, np,1,preview->frmSelect->Position-1); break ; // Gfm 4 bit
   case 1 : mGfm2h(&Gfm, 1, pth+512, np,0,preview->frmSelect->Position-1); break ; // Gfm 32 bit



   case 7 :  break ; // gamepark sdk (8bit)
 };

  free(pth) ; free(Gfm) ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::frmNbChange(TObject *Sender)
{  static int lastGood = 42 ;//   int nb ;
   try { nb = frmNb->Text.ToInt() ; }
   catch( ... ) { nb = lastGood ;   }
   if(nb > 999) nb = 999 ; else if(!nb) nb=1 ;
   flipWay->Visible = (nb > 1) ;
   if(preview) { preview->frmSelect->Visible = flipWay->Visible ;
                 preview->img->Left = 15*(flipWay->Visible) ;
                 preview->img->Width = preview->ClientWidth - (flipWay->Visible)*15 ;
                 preview->frmSelect->Max = nb ;
               }
   if(nb)     { fsx = img->Picture->Bitmap->Width ; fsy = img->Picture->Bitmap->Height ;
                if(flipWay->Checked) // horizontal, fix width
                  fsx /= nb ;
                else fsy /= nb ;
                sprintf(msg,"frames of %i*%i",fsx,fsy) ;
                LabelSize->Caption = msg ;
                if(preview) preview->frmSelect->Position = 0 ;
              }
   //if((int)Sender != 42)
     if(preview) if(!preview->frmSelect->Position) preview->frmSelect->Position = 1 ;
   if(nb == lastGood) return ;
   frmNb->Text = lastGood = nb ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::flipWayClick(TObject *Sender)
{      Form1->vertical = flipWay->Checked ;
       nb = 1 ; fsx = img->Picture->Graphic->Width ;
                fsy = img->Picture->Graphic->Height;
       frmNbChange(Sender) ;
}
//---------------------------------------------------------------------------

void downTo(int deep)
{ if(deep == 8) { preview->img->Picture->Bitmap->PixelFormat = pf8bit  ;
                  preview->img->Picture->Bitmap->PixelFormat = pf32bit ;
                  return ;
                }
  if(deep == 15){ preview->img->Picture->Bitmap->PixelFormat = pf15bit ;
                  preview->img->Picture->Bitmap->PixelFormat = pf32bit ;
                  return ;
                }
  if(deep == 1){ preview->img->Picture->Bitmap->PixelFormat = pf1bit ;
                 preview->img->Picture->Bitmap->PixelFormat = pf32bit ;
                 return ;
               }
  if(deep == 24){ preview->img->Picture->Bitmap->PixelFormat = pf24bit ;
                  preview->img->Picture->Bitmap->PixelFormat = pf32bit ;
                  return ;
                }
  if(deep != 4) return ;
  int sx = preview->img->Picture->Graphic->Width ;
  int sy = preview->img->Picture->Graphic->Height;
  int *buffer = (int*)malloc(sx*sy*4), *ptr, n=0 ;
  do { switch(n)
        { case 1 : preview->img->Picture->Bitmap->PixelFormat = pf8bit ; break ;
          case 2 : preview->img->Picture->Bitmap->PixelFormat = pf4bit ; break ;
        };
     preview->img->Picture->Bitmap->PixelFormat = pf32bit ;
     for(int y=0;y<sy;y++) // fill buffer
      { ptr = (int*)(preview->img->Picture->Bitmap->ScanLine[y]) ;
        for(int x=0;x<sx;x++)
         buffer[y*sx+x] = ptr[x] ;
      };
   } while((!thereis16color(buffer, sx*sy)) && n++<2);
  free(buffer) ;
}

void applyTrans(void)
{ if(!Form1->transType->ItemIndex) preview->TransparentColor = 0 ;
  else {  switch(Form1->transType->ItemIndex)
           { case 1 : preview->TransparentColorValue = preview->img->Canvas->Pixels[0][0] ; break ;
             case 2 : preview->TransparentColorValue = preview->img->Canvas->Pixels[0][(preview->img->Picture->Graphic->Height )-1] ; break ;
             case 3 : preview->TransparentColorValue = (TColor)0xff00ff ; break ;
             case 4 : preview->TransparentColorValue = clBlack ; break ;
             case 5 : preview->TransparentColorValue = clWhite ; break ;
           };
       preview->TransparentColor = 1 ;
  }
}

int typeDeep[] = {4,32,8,15,8,15,15,8,15,8,15,32,8,15} ;
void __fastcall TForm1::outTypeChange(TObject *Sender)
{ preview->Visible = 1 ;
  preview->updatePreviewBitmap() ;
  downTo(typeDeep[Form1->outType->ItemIndex]);
  applyTrans() ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::transTypeChange(TObject *Sender)
{ outTypeChange(Sender);

}
//---------------------------------------------------------------------------

