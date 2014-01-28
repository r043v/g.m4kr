//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H

#include "stdio.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <jpeg.hpp>

#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <FileCtrl.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// Composants gérés par l'EDI
        TPanel *Panel1;
        TCheckBox *ratio;
        TCheckBox *stretch;
        TCheckBox *center;
        TCheckBox *CheckBox5;
        TButton *Button1;
        TScrollBar *alpha;
        TCheckBox *onTop;
        TStatusBar *StatusBar;
        TTimer *Timer300;
        TComboBox *outType;
        TImage *img;
        TCheckBox *createBin;
        TEdit *frmNb;
        TUpDown *UpDown1;
        TLabel *LabelSize;
        TCheckBox *CheckBox2;
        TCheckBox *flipWay;
        TComboBox *transType;
        TBevel *Bevel1;
        TBevel *Bevel2;

        void __fastcall ratioClick(TObject *Sender);
        void __fastcall stretchClick(TObject *Sender);
        void __fastcall centerClick(TObject *Sender);
        void __fastcall alphaChange(TObject *Sender);
        void __fastcall onTopClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Timer300Timer(TObject *Sender);
        void __fastcall imgClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall frmNbChange(TObject *Sender);
        void __fastcall flipWayClick(TObject *Sender);
        void __fastcall outTypeChange(TObject *Sender);
        void __fastcall transTypeChange(TObject *Sender);
private:	// Déclarations de l'utilisateur
        
        void virtual __fastcall WMDropFiles(TWMDropFiles &message);
public:		// Déclarations de l'utilisateur
        Graphics::TBitmap *bmp, *prev;
        int nb, fsx, fsy, vertical  ; // frameset info
        char msg[142] ; u32 msgTime ; // msg show in status bar
        AnsiString fname ;
        int __fastcall ask(const char * m) ;
          void __fastcall updatePreview(void);
        __fastcall TForm1(TComponent* Owner);
        //__fastcall TMainForm(TComponent* Owner);
  BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
  END_MESSAGE_MAP(TForm);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------

#endif
