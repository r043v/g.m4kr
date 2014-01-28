//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tpreview *preview;
//---------------------------------------------------------------------------
__fastcall Tpreview::Tpreview(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tpreview::FormResize(TObject *Sender)
{    Form1->updatePreview();
     frmSelect->Height = preview->ClientHeight + 2 ;
     preview->img->Left = (frmSelect->Visible)*15 ;
     preview->img->Width = preview->ClientWidth - (frmSelect->Visible)*15 ;
     Form1->outTypeChange(0); Form1->transTypeChange(0);
}
//---------------------------------------------------------------------------

void __fastcall Tpreview::FormCloseQuery(TObject *Sender, bool &CanClose)
{
        CanClose = 0 ;
        preview->Visible = 0 ;        
}
//---------------------------------------------------------------------------

void __fastcall Tpreview::FormCreate(TObject *Sender)
{
        //TransparentColor = 1 ;
        //TransparentColorValue = 0x00FFFFFF ; // white
}
//---------------------------------------------------------------------------

void __fastcall Tpreview::updatePreviewBitmap(void)
{  int sx = Form1->fsx, sy = Form1->fsy, nb = Form1->nb, vertical = !Form1->flipWay->Checked ;
   int *buffer = (int*)malloc(sx*sy*4), *ptr ;
   int num = frmSelect->Position - 1 ;
   if(num < 0 || !frmSelect->Visible){ img->Picture->Bitmap->Assign(Form1->img->Picture->Bitmap) ;
                                       preview->Caption = Form1->fname + " (" + nb + ')' ;
                                       return ;
                                     } // all part draw

   preview->Caption = Form1->fname + " (frame " + (num+1) + ')' ;
          img->Picture->Bitmap->PixelFormat = pf32bit ;
   Form1->img->Picture->Bitmap->PixelFormat = pf32bit ;
   int py = vertical*num*sy, y ;
   img->Picture->Bitmap->Width  = sx ;
   img->Picture->Bitmap->Height = sy ;
    for(y=py ; y<py+sy ; y++)
     { ptr = (int*)(Form1->img->Picture->Bitmap->ScanLine[y]) ;
       if(vertical) memcpy(buffer+sx*(y-py),ptr,sx*4)  ;
        else   memcpy(buffer+sx*(y-py),ptr+num*sx,sx*4);
     };
    for(y=0 ; y<sy ; y++)
     { ptr = (int*)(img->Picture->Bitmap->ScanLine[y]) ;
       memcpy(ptr,buffer+sx*y,sx*4) ;
     };
    img->Refresh();
   free(buffer) ;
}

void downTo(int deep);

void __fastcall Tpreview::frmSelectChange(TObject *Sender)
{  Form1->outTypeChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tpreview::imgClick(TObject *Sender)
{ Form1->outTypeChange(Sender);
  /*int sx, sy ;
  if(frmSelect->Position) {sx = Form1->fsx ; sy = Form1->fsy ; }
  else { sx = img->Picture->Bitmap->Width ; sy = img->Picture->Bitmap->Height; }
  int small = ((sx < 256 || sy < 256)&&(Form1->stretch->Checked)) ;
  preview->ClientHeight = sy + small*(Form1->fsy) ;
  preview->ClientWidth  = sx + (Form1->nb > 1)*15 + small*(Form1->fsx + (Form1->nb > 1)*15) ;
  preview->img->Left = (frmSelect->Visible)*15 ;
  preview->img->Width = preview->ClientWidth - (frmSelect->Visible)*15 ;*/
}
//---------------------------------------------------------------------------

